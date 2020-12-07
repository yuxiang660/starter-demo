#include <iostream>
#include <string>
#include <exception>

#define INFO(msg) \
   fprintf(stderr, "info: %s:%d: ", __FILE__, __LINE__); \
   fprintf(stderr, "%s", msg);

#define THROW_MSG(code, msg) \
   throw EtDevException(code, __FILE__, __LINE__, msg);

#define THROW(code) \
   throw EtDevException(code, __FILE__, __LINE__);

std::string get_msg(const std::string& msg)
{
   return std::string(__FILE__) + ":" + std::to_string(__LINE__) + " - " + msg;
}


class EtDevException : public std::exception
{
public:
   EtDevException(int error_code, const char* file, int line, const char* msg = ""):
      m_code(error_code),
      m_msg
      (
         "Error report from " + std::string(file) + ":" + std::to_string(line) +
         (std::string(msg).empty() ? "" : ("\nError Description: " + std::string(msg)))
      )
   {
      
   }

   const char* what () const throw()
   {
      return m_msg.c_str();
   }

   int code() const throw()
   {
      return m_code;
   }

private:
   const int m_code;
   const std::string m_msg;
};

void throw_fun()
{
   THROW_MSG(-1, "error_msg");
   //THROW(-1);
}

int main(int argc ,char **argv)
{
   std::cout << "Print filename and line number" << std::endl;
   std::string msg = "TEST MSG";

   try
   {
      throw_fun();
   } catch(EtDevException& e) {
      std::cout << "EtDevException caught" << std::endl;
      std::cout << e.what() << std::endl;
      std::cout << e.code() << std::endl;
   } catch(std::exception& e) {
      std::cout << "other" << std::endl;
   }

   return 0;
}
