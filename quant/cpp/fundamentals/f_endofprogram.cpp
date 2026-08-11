//6 ways to end the program
#include <ctdlib>
#include <cstdio>
#include <cassert>
#include <csignal>
#include <exception>



int main(){
   return EXIT_SUCCESS;
   return 0;
   //no return
   
   if (condition){
       exit(EXIT_SUCCESS);
   }

   std::abort();
   assert(false);

   //quick exit
   lifetime *l = new lifetime();
   std::at_quick_exit([](){delete l; std::fflush(stdout);});
   std::quick_exit(EXIT_FAILURE); //does not flush buffers

   std::terminate();
   throw 42;


}
