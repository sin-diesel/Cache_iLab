#include <iostream>
#include "cache.hpp"


int main() {

    test_pages();
    test_mem();
    test_cache();

    int buffer_size = 10;

    cache_t<int> two_q_cache(buffer_size, 0.3, 0.25); // coefficients for splitting memory

    tests_t<int> tests(1000);

    //tests.fill_cache(two_q_cache);

    two_q_cache.print_cache();

    tests.test_cache(two_q_cache);


    return 0;
}