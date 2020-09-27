#include "cache.hpp"

typedef int data_type;

//template<>
//void cache_mem_t<data_type>::print_mem() const {
//
//    std::cerr << "Printing memory: " << std::endl;
//    for (auto const& i: list_) {
//        i.print_page();
//    }
//
//}
//
//template<>
//void cache_t<data_type>::add_page(cache_mem_t<data_type> mem, page_t<data_type> const page) {
//
//    mem.list_.push_front(page);
//    mem.list_.pop_back();
//}


void test_pages() {

    page_t<data_type> pages[] = {page_t<data_type>(0, 4), page_t<data_type>(1, 2), page_t<data_type>(2, 7), page_t<data_type>(3, 20), page_t<data_type>(4, 18)};

    for (int i = 0; i < 5; ++i) {
        assert(pages[i].pageOK());
        pages[i].print_page();
    }

}

//void test_mem() {
//
//    cache_mem_t<data_type> memory(5);
//
//    memory.print_mem();
//
//}
//
//void test_cache() {
//
//    cache_t<data_type> cache(5);
//}