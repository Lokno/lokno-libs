#include "LokLibStats.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <cassert> 
#include <cmath>

// https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
double generateGaussianNoise(double mu, double sigma)
{
    static const double epsilon = std::numeric_limits<double>::min();
    static const double two_pi = 2.0*3.14159265358979323846;

    thread_local double z1;
    thread_local bool generate;
    generate = !generate;

    if (!generate)
       return z1 * sigma + mu;

    double u1, u2;
    do
     {
       u1 = rand() * (1.0 / RAND_MAX);
       u2 = rand() * (1.0 / RAND_MAX);
     }
    while ( u1 <= epsilon );

    double z0;
    z0 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2);
    z1 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);
    return z0 * sigma + mu;
}

int main()
{
    LokLibStats stats;
    const size_t count = 1000;

    srand(time(NULL));

    for( size_t i = 0; i < count; ++i )
    {
        stats.add(generateGaussianNoise(1.0,3.0));
    }
    
    assert( stats.get_count() == count );

    std::cout << "Min:     " << stats.get_minimum() << std::endl;
    std::cout << "Max:     " << stats.get_maximum() << std::endl;
    std::cout << "Mean:    " << stats.get_mean()    << std::endl;
    std::cout << "Medium:  " << stats.get_medium()  << std::endl;
    std::cout << "Std Dev: " << stats.calculate_std_deviation()  << std::endl;

    return 0;
}