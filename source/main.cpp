#include <iostream>
#include "cache.hpp"


int main(int argc, char* argv[]) {

    test_pages();
    test_mem();
    test_cache();

    int buffer_size = 10;

    cache_t<int> two_q_cache(buffer_size); 

    tests_t<int> tests(50);
    
    tests.test_cache(two_q_cache);


    return 0;
}