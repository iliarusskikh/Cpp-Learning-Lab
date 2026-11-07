#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>


int main()
{
    //seed the random number generator
    srand(time(nullptr));
    
    //generate random numbers between 1 and 100
    int random1 = rand()%100 +1;
    int random2 = rand()%100 +1;
    //limited range control
    
    //random engines
    /*
     std::mt19937 Mersenne Twister
     std::default_rangom_engine
     std::random_device true random source
     std::minstd_rand - linear congruential
     std;:ranlux24 - RANLIX algorithm
     
     //distribution types
     std::uniform_int_distribution
     std::uniform_real_distribution
     std::normal_distribution
     std::bernoulli_distribution
     std::discrete_distribution
     */
    
    
    //uniform integer distribution
    //create a random generator
    std::random_device rd; //used to obtain a seed
    std::mt19937 gen(rd());//standard engine
    
    //define distribution (1 to 100);
    std::uniform_int_distribution<int> dist(1,100);
    
    //gen nums
    int random_numbers = dist(gen);//1 number
    
    
    //generating floating point random numbers
    std::uniform_real_distribution<double> dist2(0.0,1.0);
    std::cout <<std::fixed<<std::setprecesion(6);
    std::cout <<dist2(gen);
    
    
    
    
    return 0;
}
