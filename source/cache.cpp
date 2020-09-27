#include "cache.hpp"

typedef int data_type;


void test_pages() {

    page_t<data_type> pages[] = {page_t<data_type>(0, 4), page_t<data_type>(1, 2), page_t<data_type>(2, 7), page_t<data_type>(3, 20), page_t<data_type>(4, 18)};

    for (int i = 0; i < 5; ++i) {
        assert(pages[i].pageOK());
        pages[i].print_page();
    }

}

void test_mem() { // is it a good implementation of manual tests?


    int mem_size = 4;
    cache_mem_t<data_type> memory(mem_size);

    memory.print_mem();

    page_t<data_type> pages[] = {page_t<data_type>(0, 4), page_t<data_type>(1, 2), page_t<data_type>(2, 7), page_t<data_type>(3, 20), page_t<data_type>(4, 18)};

    memory.add_page(pages[0]);
    memory.add_page(pages[1]);
    memory.add_page(pages[2]);

    memory.print_mem();

    assert(memory.beginning_->id_ == 2);
    assert(memory.beginning_->data_ == 7);
    assert(memory.mem_size_ == mem_size);
}

void test_cache() {

    cache_t<data_type> cache(5);
}