#include <assert.h>
#include "cache.hpp"

typedef int data_type;


void test_pages() { // a good way to test?

    page_t<data_type> pages[] = {page_t<data_type>(0, 4), page_t<data_type>(1, 2), page_t<data_type>(2, 7), page_t<data_type>(3, 20), page_t<data_type>(4, 18)};

    std::cerr << "Printing pages:\n\n\n";
    for (int i = 0; i < 5; ++i) {
        assert(pages[i].pageOK());
        pages[i].print_page();
    }

    std::cerr << "\n\n\n";


}

void test_mem() { // is it a good implementation of manual tests?


    int mem_size = 4;
    cache_mem_t<data_type> memory(mem_size);

    std::cerr << "Printing test mem: \n\n\n";

    memory.print_mem();

    page_t<data_type> pages[] = {page_t<data_type>(0, 4), page_t<data_type>(1, 2), page_t<data_type>(2, 7), page_t<data_type>(3, 20), page_t<data_type>(4, 18)};

    memory.add_page(pages[0]);
    memory.add_page(pages[1]);
    memory.add_page(pages[2]);

    memory.print_mem();

    std::cerr << "\n\n\n";

    assert(memory.beginning_->id_ == 2);
    assert(memory.beginning_->data_ == 7);
    assert(memory.mem_size_ == mem_size);
    assert(memory.cur_size_ == 3);
}

void test_cache() { // same question

    cache_t<data_type> cache(5);

    page_t<data_type> page_1(3, 4);
    page_t<data_type> page_2(4, 2);
    page_t<data_type> page_3(5, 4);
    page_t<data_type> page_4(6, 8);
    page_t<data_type> page_5(9, 4);
    page_t<data_type> page_6(15, 10);



    //std:: cerr << "Printing cache main memory: \n\n\n";
    assert(cache.main_mem.memOK());
    //cache.main_mem.print_mem();
    //std::cerr << "\n\n\n";

    cache.main_mem.add_page(page_1);
    cache.main_mem.add_page(page_2);
    cache.main_mem.add_page(page_3);
    cache.main_mem.add_page(page_4);
    cache.main_mem.add_page(page_5);

    cache.print_cache();
    assert(cache.main_mem.page_at(3) == true);

    cache.main_mem.remove_page(page_2);
    cache.main_mem.remove_page(page_3);
    assert(cache.main_mem.page_at(4) == false);
    assert(cache.main_mem.page_at(5) == false);

    cache.print_cache();

    cache.main_mem.add_page(page_2);
    cache.main_mem.add_page(page_3);

    cache.print_cache();

    //std::cerr << "Printing cache main memory: \n\n\n";
    //cache.main_mem.print_mem();
    //std::cerr << "\n\n\n";

    cache.main_mem.add_page(page_6);

    //std::cerr << "Printing cache main memory: \n\n\n";
    //cache.main_mem.print_mem();
    //std::cerr << "\n\n\n";

    assert(cache.main_mem.page_at(3) == false);
    assert(cache.main_mem.page_at(4) == true);
    assert(cache.main_mem.page_at(5) == true);
    assert(cache.main_mem.page_at(6) == true);
    assert(cache.main_mem.page_at(9) == true);
    assert(cache.main_mem.page_at(15) == true);

    cache_t<int> two_q_cache(15, 0.5, 0.4);

    for (int i = 0; i < 4; ++i) {
        two_q_cache.main_mem.add_page(page_t<int>(i,i * 3));
    }


    //two_q_cache.print_cache();

    //std::cerr << "Now generating data for cache and printing it: \n";

    tests_t<int> tests(40);
    //tests.print_test_data();

    cache_t<int> cache_2(20, 1, 0);
    //cache_2.print_cache();

    //std::cerr << "After filling with data: \n";
    tests.fill_cache(cache_2);

    //cache_2.print_cache();

    std::cerr << "\n\n\n";
}

