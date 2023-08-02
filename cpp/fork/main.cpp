#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <string.h>

int main(int argc, char *argv[])
{
   int    wstatus;
   pid_t  cpid, w;

   try
   {
      cpid = fork();
      if (cpid == -1) {
         perror("fork");
         exit(EXIT_FAILURE);
      }

      if (cpid == 0) {            /* Code executed by child */
         printf("Child PID is %jd\n", (intmax_t) getpid());
         if (argc == 1) sleep(1);
            // pause();                    /* Wait for signals */
         //_exit(atoi(argv[1]));
         // throw std::runtime_error("abc");
         if (chdir("abc") < 0)
            throw std::runtime_error("abc");
         exit(EXIT_SUCCESS);

      } else {                    /* Code executed by parent */
         do {
               w = waitpid(cpid, &wstatus, WUNTRACED | WCONTINUED);
               if (w == -1) {
                  perror("waitpid");
                  exit(EXIT_FAILURE);
               }
               if (WEXITSTATUS(wstatus) != EXIT_SUCCESS)
                  printf("child process error with exit code %d: %s\n", WEXITSTATUS(wstatus), strerror(errno));
               if (WIFEXITED(wstatus)) {
                  printf("w=%d, cpid=%d, exited, status=%d\n", w, cpid, WEXITSTATUS(wstatus));
               } else if (WIFSIGNALED(wstatus)) {
                  printf("killed by signal %d\n", WTERMSIG(wstatus));
               } else if (WIFSTOPPED(wstatus)) {
                  printf("stopped by signal %d\n", WSTOPSIG(wstatus));
               } else if (WIFCONTINUED(wstatus)) {
                  printf("continued\n");
               }
         } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
         exit(EXIT_SUCCESS);
      }
   }
   catch(const std::exception &e)
   {
      std::cout << "exception " << e.what() << std::endl;
   }
   catch(...)
   {
      std::exception_ptr p = std::current_exception();
      std::cout << "Unexpected error happens when reading SPEF files: " << (p ? p.__cxa_exception_type()->name() : "null") << "\n";
   }
}