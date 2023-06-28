#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <cassert>
#include <string.h>
#include <exception>
#include <cstdarg>
#include <utility>
#include <cmath>
#include <array>

class fvException : public std::runtime_error {
public:
    template<typename ... Args>
    explicit fvException(const char *fmt, Args &&... args) : std::runtime_error(MakeError(fmt, std::forward<Args>(args) ...)) {}
    explicit fvException(const std::string &msg) : std::runtime_error(msg) { }
private:
    static std::string MakeError(const char *fmt, ...) {
        constexpr int bufsize = 1024;
        char buf[bufsize + 1];
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(buf, bufsize, fmt, ap);
        va_end(ap);
        return buf;
    }
};

class BoolExpr
{
public:
    explicit BoolExpr(const char *expr, const std::vector<std::string> &inPins) : m_expr(expr), m_inPins(inPins)
    {
        for (size_t i = 0; i < m_inPins.size(); i++)
            m_inPinIds[m_inPins[i]] = i;
        buildExpr();
    }

private:
    // arg1 op arg2
    class Node
    {
    public:
        enum OP
        {
            NOT,
            AND,
            OR,
            XOR,
            UNKNOWN
        };
        OP op = UNKNOWN;
        // index to argument index, arguments start with input pins
        // after input pins, the arguments are the nodes one by one
        static constexpr int NO_ARG = -1;
        int arg1 = NO_ARG;
        int arg2 = NO_ARG;

        std::string getOpName() const
        {
            return op == NOT ? "NOT" :
                   op == AND ? "AND" :
                   op == OR ? "OR" :
                   op == XOR ? "XOR" : "UNKNOWN";
        }

        std::string getOpSym() const
        {
            return op == NOT ? "!" :
                   op == AND ? "&" :
                   op == OR ? "|" :
                   op == XOR ? "^" : "UNKNOWN";
        }

        static int getOpPri(char op)
        {
            return op == '!' ? 3 :
                   op == '^' ? 2 :
                   op == '&' ? 1 :
                   op == '|' ? 0 : 0;
        }
    };

private:
    void buildExpr()
    {
        assert(!m_expr.empty());

        // the buf may be modified during build
        std::vector<char> buf(m_expr.size() + 1);
        char *curPos = strcpy(buf.data(), m_expr.c_str());

        std::vector<char> opStack;
        std::vector<int> argStack;
        std::vector<char> varBuf(m_expr.size() + 1);
        char *var = varBuf.data();
        while (char c = nextToken(curPos, var))
        {
            if (c == 'v')
            {
                if (m_inPinIds.find(var) == m_inPinIds.end())
                    throw fvException("Syntax error in expression {%s}, invalid pin name '%s'", var);
                int i = m_inPinIds.at(var);
                argStack.push_back(i);
                if (!opStack.empty() && opStack.back() == '!')
                    createNode(opStack, argStack); // "!A" node
            }
            else if (c == '(')
            {
                opStack.push_back(c);
                argStack.push_back(Node::NO_ARG);
            }
            else if (c == ')')
            {
                while (!opStack.empty() && opStack.back() != '(')
                    createNode(opStack, argStack); // nodes in "()"
                if (opStack.empty() || opStack.back() != '(')
                    throw fvException("Syntax error in expression {%s}, extra ')'", m_expr.c_str());
                opStack.pop_back(); // pop '('
                int saveArg = argStack.back(); // current node's index
                argStack.pop_back();
                if (argStack.back() != Node::NO_ARG) // '(' invalid argument
                    throw fvException("Syntax error in expression {%s}: invalid '('", m_expr.c_str());
                argStack.back() = saveArg; // replace '(' invalid argument
            }
            else if (c == '\'')
            {
                opStack.push_back('!'); // convert ' to !, because createNode only support !
                createNode(opStack, argStack); // "A'" node
            }
            else
            {
                while (!opStack.empty() && opStack.back() != '(' && Node::getOpPri(opStack.back()) >= Node::getOpPri(c))
                    createNode(opStack, argStack); // nodes in the stack that have higer priority
                opStack.push_back(c); // save next op
            }
        }

        while (!opStack.empty() && opStack.back() != '(')
            createNode(opStack, argStack);

        if (!opStack.empty() || argStack.size() != 1)
            throw fvException("Syntax error in expression {%s}: remaining ops(%zu) or args(%zu)", m_expr.c_str(), opStack.size(), argStack.size()-1);
    }

    // helper function for Expression::build
    //  return next token
    //  return values are:
    //    'v' (for variable), '!', '&', '|', '^', '(', ')'
    //  p is updated to point to next char after returned token
    //  variable is returned through 'var'
    char nextToken(char *&p, char *var)
    {
        // skip leading space
        while (isspace(*p) && *(p + 1) != '.')
            p++;

        auto replaceSpaceWithAnd = [](char *s) {
            // judge if following space is AND, if so, replace it with '&' character
            while (*s == ')') s++;
            if (isspace(*s))
            {
                char *h = s;
                while (isspace(*s)) s++;
                if (*s == '(' || isalpha(*s) || *s == '!' || *s == '\\')
                    *h = '&'; // after the space it is a valid node, so the space is AND
            }
        };

        // return operators
        switch (*p++)
        {
        case '\'':
        {
            replaceSpaceWithAnd(p);
            return '\''; // invert previous expression
        }
        case '!':
            return '!'; // invert following expression
        case '*':
            return '&';
        case '&':
            return '&';
        case '+':
            return '|';
        case '|':
            return '|';
        case '(':
            return '(';
        case ')':
            return ')';
        case '^':
            return '^';
        case 0:
            return 0;
        }

        // get variable
        p--;
        if (!isalpha(*p))
            throw fvException("Invalid character '%c' in the expression", *p);
        int i;
        bool inEscape = false;
        for (i = 0; isalnum(*p) || *p == '[' || *p == ']' || *p == '_' || *p == '.' || *p == '\\' || (inEscape && *p == ' '); p++, i++)
        {
            if (*p == '\\')
                inEscape = true;
            else if (*p == ' ')
                inEscape = false;
            var[i] = *p;
        }
        var[i] = 0;

        replaceSpaceWithAnd(p);

        return 'v';
    }

    void createNode(std::vector<char> &opStack, std::vector<int> &argStack)
    {
        assert(!opStack.empty() && !argStack.empty() && argStack.back() != Node::NO_ARG);

        Node n;

        char opcode = opStack.back();
        opStack.pop_back();
        n.arg1 = argStack.back();
        argStack.pop_back();

        switch (opcode)
        {
        case '!':
            n.op = Node::NOT;
            n.arg2 = Node::NO_ARG;
            break;
        case '|':
        case '&':
        case '^':
            assert(!argStack.empty() && argStack.back() != Node::NO_ARG);
            n.arg2 = argStack.back();
            argStack.pop_back();
            n.op = opcode == '|' ? Node::OR :
                   opcode == '&' ? Node::AND : Node::XOR;
            break;
        default:
            assert(false);
        }

        m_nodes.push_back(n);
        int nodeId = m_inPinIds.size() + m_nodes.size() - 1;
        argStack.push_back(nodeId);
    }

public:
    void dump() const
    {
        printf("Expression: %s\n", m_expr.c_str());
        for (const auto &pin : m_inPins)
            printf("%10d: %s\n", m_inPinIds.at(pin), pin.c_str());

        int nodeId = m_inPinIds.size();
        for (const auto &node: m_nodes)
            printf("%10d: op %3s, arg1 %4d, arg2 %4d\n", nodeId++, node.getOpName().c_str(), node.arg1, node.arg2);
    }

    std::string expr() const
    {
        return m_expr;
    }

    // pin0: bit0, pin1: bit1, ..., max 64 pins
    bool eval(uint64_t inVal) const
    {
        std::vector<bool> nodesVal;
        nodesVal.reserve(m_inPins.size() + m_nodes.size());
        for (size_t i = 0; i < m_inPins.size(); i++)
            nodesVal.push_back((inVal & (1 << i)) ? 1 : 0);

        assert(!m_nodes.empty());
        for (const auto &node : m_nodes)
        {
            switch(node.op)
            {
            case Node::NOT:
            {
                assert(node.arg1 < nodesVal.size());
                nodesVal.push_back(!nodesVal[node.arg1]);
                break;
            }
            case Node::AND:
            {
                assert(node.arg1 < nodesVal.size());
                assert(node.arg2 < nodesVal.size());
                nodesVal.push_back(nodesVal[node.arg1] && nodesVal[node.arg2]);
                break;
            }
            case Node::OR:
            {
                assert(node.arg1 < nodesVal.size());
                assert(node.arg2 < nodesVal.size());
                nodesVal.push_back(nodesVal[node.arg1] || nodesVal[node.arg2]);
                break;
            }
            case Node::XOR:
            {
                assert(node.arg1 < nodesVal.size());
                assert(node.arg2 < nodesVal.size());
                nodesVal.push_back(nodesVal[node.arg1] != nodesVal[node.arg2]);
                break;
            }
            default:
                assert(false);
            }
        }

        return nodesVal.back();
    }

private:
    std::string m_expr;
    const std::vector<std::string> &m_inPins;
    std::unordered_map<std::string, uint16_t> m_inPinIds;
    std::vector<Node> m_nodes;
};

class GlitchGateModel
{
public:
    GlitchGateModel(const char *expr, const std::vector<std::string> &inPins) : m_expr(expr, inPins), m_inPins(inPins)
    {
        setTruthTable();
        createGlitchModel();
    }

    void dump() const
    {
        printf("Expression: %s\n", m_expr.expr().c_str());
        for (const auto &[togglePinIds, glitchCoeff] : m_glitchCoeff)
        {
            printf("\tToggle pins: %s, %s\n", m_inPins[togglePinIds & 0xFFFF].c_str(), m_inPins[togglePinIds >> 16].c_str());
            for (const auto &[highLowPinsVal, coeff] : glitchCoeff)
            {
                printf("\t\tOther pinVal: 0x%04x, coeff: %f\n", highLowPinsVal, coeff);
            }
        }
    }

private:
    void setTruthTable()
    {
        const uint64_t maxVal = pow(2, m_inPins.size());
        uint64_t inVal = 0;
        while (inVal < maxVal)
        {
            m_truthTable[inVal] = m_expr.eval(inVal);
            inVal++;
        }
    }

    void createGlitchModel()
    {
        for (uint16_t pinAId = 0; pinAId < m_inPins.size(); pinAId++)
        {
            for (uint16_t pinBId = pinAId + 1; pinBId < m_inPins.size(); pinBId++)
            {
                // Pickup two input pins for glitch toggle trigger
                const uint32_t pinABKey = (pinBId << 16) | pinAId;
                const uint64_t pinABMask = (1ULL << pinAId) | (1ULL << pinBId);
                const uint64_t pinABVal00 = (0ULL << pinAId) | (0ULL << pinBId);
                const uint64_t pinABVal11 = (1ULL << pinAId) | (1ULL << pinBId);
                const uint64_t pinABVal01 = (0ULL << pinAId) | (1ULL << pinBId);
                for (const auto &[in, out]: m_truthTable)
                {
                    const uint64_t key2 = (in & ~pinABMask);
                    if ((in & pinABMask) == pinABVal00)
                        m_glitchTables[pinABKey][key2][0] = out;
                    else if ((in & pinABMask) == pinABVal11)
                        m_glitchTables[pinABKey][key2][1] = out;
                    else if ((in & pinABMask) == pinABVal01)
                        m_glitchTables[pinABKey][key2][2] = out;
                    else // pinABVal10
                        m_glitchTables[pinABKey][key2][3] = out;
                }
            }
        }

        for (const auto &[togglePinIds, glitchTables] : m_glitchTables)
        {
            for (const auto &[highLowPinsVal, outVal] : glitchTables)
            {
                double coeff = 0.0;
                if (outVal[0] == outVal[1] && outVal[1] == outVal[2] && outVal[2] == outVal[3])
                {
                    // case1: output does change when the pinA and pinB toggles, so no glitch
                    // example: BUF gate
                    coeff = 0.0;
                }
                else if ((outVal[0] == outVal[1]) && (outVal[2] == outVal[3]))
                {
                    // case2: when pinA (0<->1) and pinB (0<->1) toggle in this way, output does change,
                    //        when pinA (0<->1) and pinB (1<->0) toggle in this way, output does change,
                    //        but the ouput value is different when the pinA and pinB toggle in different way,
                    //        so it always have glitch when pinA and pinB toggle, and no toggle delay time is required
                    // example: XOR gate
                    coeff = 2.0;
                }
                else if ((outVal[0] == outVal[1]) || (outVal[2] == outVal[3]))
                {
                    // case3: only one toggle way keep output unchanged,
                    //        so when pinA toggles, it decides pinB toggle way, and it also requires the toggle delay time
                    // example: AND gate
                    coeff = 0.5;
                }
                else
                {
                    assert((outVal[0] != outVal[1]) && (outVal[2] != outVal[3]));
                    // case4: output always changes when pinA and pinB toggle, so no glitch
                    coeff = 0.0;
                }
                m_glitchCoeff[togglePinIds][highLowPinsVal] = coeff;
            }
        }
    }

private:
    const BoolExpr m_expr;
    std::vector<std::string> m_inPins;
    // key: input value; value: expression value
    std::map<uint64_t, bool> m_truthTable;
    // key1: two toggle pins' index, (pinB_Id << 16) | (pinA_Id)
    // key2: other pins value, pin0 is bit0, pinA and pinB's value should be zero here
    // value: four the expression values for when pinA and pinB's value is (0, 0), (1, 1), (0, 1), (1, 0),
    //        other pins' value is key2
    std::map<uint32_t, std::map<uint64_t, std::array<bool, 4>>> m_glitchTables;
    // key1 and key2 is same as m_glitchTables, the value is the glitch coefficient
    // when pinA and pinB is toggle and other pin's high/low value is key2
    std::map<uint32_t, std::map<uint64_t, double>> m_glitchCoeff;
};

int main(int argc, char **argv)
{
    std::cout << "-- Boolean Expression Evaluator --" << std::endl;

    GlitchGateModel gAnd2("(A * B)", {"A", "B"});
    gAnd2.dump();

    GlitchGateModel gAnd3("(A * B * C)", {"A", "B", "C"});
    gAnd3.dump();

    GlitchGateModel gAnd4("(A  B  C  D)", {"A", "B", "C", "D"});
    gAnd4.dump();

    GlitchGateModel gor2("(A | B)", {"A", "B"});
    gor2.dump();

    GlitchGateModel gor3("(A | B | C)", {"A", "B", "C"});
    gor3.dump();

    GlitchGateModel gor4("(A | B | C | D)", {"A", "B", "C", "D"});
    gor4.dump();

    GlitchGateModel gxor2("(A ^ B)", {"A", "B"});
    gxor2.dump();

    GlitchGateModel gxor3("(A ^ B ^ C)", {"A", "B", "C"});
    gxor3.dump();


    return 0;
}
