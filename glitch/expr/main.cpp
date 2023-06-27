#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>
#include <string.h>
#include <exception>
#include <cstdarg>

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
                    throw fvException("Invalid pin name '%s'", var);
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
                if (argStack.empty() || argStack.back() == Node::NO_ARG)
                    throw fvException("Syntax error in expression {%s}: \' does not have arg", m_expr.c_str());
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
                if (*s == '(' || isalpha(*s) || *s == '!' || *s == '\'')
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

private:
    std::string m_expr;
    std::vector<std::string> m_inPins;
    std::unordered_map<std::string, int> m_inPinIds;
    std::vector<Node> m_nodes;
};

int main(int argc, char **argv)
{
    std::cout << "-- Boolean Expression Evaluator --" << std::endl;

    BoolExpr expr1("(A & B | A)", {"A", "C", "B"});
    expr1.dump();
    BoolExpr expr2("A'", {"A"});
    expr2.dump();
    BoolExpr expr3("A' B + A B", {"A", "B"});
    expr3.dump();

    return 0;
}
