#include <assert.h>
#include "cache.hpp"
    

typedef int data_type;

void test_pages() { // a good way to test?

    std::cerr << "<<<<<<<ENTERING PAGES UNIT TEST>>>>>>>" << "\n";
    page_t<data_type> pages[] = {page_t<data_type>(0, 4), page_t<data_type>(1, 2), page_t<data_type>(2, 7), page_t<data_type>(3, 20), page_t<data_type>(4, 18)};

    std::cerr << "Printing pages:\n\n\n";
    for (int i = 0; i < 5; ++i) {
        assert(pages[i].pageOK());
        pages[i].print_page();
    }

    std::cerr << "<<<<<<PAGES_OK>>>>>>" << "\n";

    std::cerr << "\n\n\n";


}

void test_mem() { // is it a good implementation of manual tests?

    std::cerr << "<<<<<<<ENTERING MEMORY UNIT TEST>>>>>>>" << "\n";
    int mem_size = 4;
    cache_mem_t<page_t<data_type>> memory(mem_size);

    std::cerr << "Printing test mem: \n\n\n";

    memory.print_mem();

    page_t<data_type> pages[] = {page_t<data_type>(0, 4), page_t<data_type>(1, 2), page_t<data_type>(2, 7), page_t<data_type>(3, 20), page_t<data_type>(4, 18)};

    memory.add_page(pages[0]);
    memory.add_page(pages[1]);
    memory.add_page(pages[2]);

    memory.print_mem();

    assert(memory.beginning_->id_ == 2);
    assert(memory.beginning_->data_ == 7);
    assert(memory.mem_size_ == mem_size);
    assert(memory.cur_size_ == 3);

    std::cerr << "<<<<<<MEMORY_OK>>>>>>" << "\n";

    std::cerr << "\n\n\n";
}

void test_cache() { // same question

    std::cerr << "<<<<<<<ENTERING CACHE UNIT TEST>>>>>>>" << "\n";
    unsigned n_pages = 10;
    unsigned aout_size = Kout * n_pages * (sizeof(page_t<data_type>) / sizeof(page_t<data_type>*));
    unsigned a1_size = Kin * (n_pages - aout_size);
    unsigned am_size = n_pages - aout_size - a1_size;

    cache_t<data_type> cache(n_pages);

    cache.print_cache();
    assert(cache.aout.mem_size_ == aout_size);
    assert(cache.am.mem_size_ == am_size);
    assert(cache.a1.mem_size_ == a1_size);

    DUB(cache.print_cache();)
    page_t<data_type> page_1(3, 4);
    page_t<data_type> page_2(4, 2);
    page_t<data_type> page_3(5, 4);
    page_t<data_type> page_4(6, 8);
    page_t<data_type> page_5(9, 4);
    page_t<data_type> page_6(15, 10);

    cache.am.add_page(page_1);
    cache.am.add_page(page_2);
    cache.am.add_page(page_3);
    cache.am.add_page(page_4);
    cache.am.add_page(page_5);

    cache.print_cache();
    assert(cache.am.page_at(3) == false);

    cache.am.remove_page(page_2);
    cache.am.remove_page(page_3);
    cache.am.remove_page(page_1)
    ;
    assert(cache.am.page_at(4) == false);
    assert(cache.am.page_at(5) == false);

    cache.print_cache();
    
    cache.a1.add_page(page_1);
    cache.a1.add_page(page_2);
    cache.a1.add_page(page_3);

    cache.print_cache();

    cache.am.add_page(page_6);

    assert(cache.am.page_at(3) == false);
    assert(cache.am.page_at(4) == false);
    assert(cache.am.page_at(5) == false);
    assert(cache.am.page_at(6) == true);
    assert(cache.am.page_at(9) == true);
    assert(cache.am.page_at(15) == true);

    cache_t<int> two_q_cache(15);

    for (int i = 0; i < 4; ++i) {
         two_q_cache.am.add_page(page_t<int>(i,i * 3));
    }

    two_q_cache.print_cache();

    std::cerr << "Now generating data for cache and printing it: \n";

    tests_t<data_type> tests(40);
    tests.print_test_data();

    std::cerr << "After filling with data: \n";
    tests.fill_cache(two_q_cache);
    two_q_cache.print_cache();

    std::cerr << "Creating 2q cache with double data type\n";
    cache_t<double> two_q_cache_double(20);
    tests_t<double> dbl_tests(100);
    dbl_tests.print_test_data();
    dbl_tests.fill_cache(two_q_cache_double);
    two_q_cache_double.print_cache();

    std::cerr << "<<<<<<CACHE_OK>>>>>>" << "\n";
    std::cerr << "\n\n\n";
}

