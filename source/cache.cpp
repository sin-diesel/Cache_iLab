#include "cache.h"

typedef int data_type;

template<>
void page_t<data_type>::print_page() const {
    std::cerr << "Page ID: " << _id << " " << "Page size: " << _size << " " << "Page data " << _data << std::endl;
}

template<>
bool page_t<data_type>::pageOK() const {

    if (_size < 0) {
        std::cerr << "Page with invalid size: " << std::endl;
        print_page();
        return false;
    }

    return true;
}

template<>
void cache_mem_t<data_type>::print_mem() const {

    std::cerr << "Printing memory: " << std::endl;
    for (auto const& i: _list) {
        i.print_page();
    }

}

template<>
void cache_t<data_type>::add_page(cache_mem_t<data_type> mem, page_t<data_type> const page) {

    mem._list.push_front(page);
    mem._list.pop_back();
}


void test_pages() {

    page_t<data_type> pages[] = {page_t<data_type>(0, 4), page_t<data_type>(1, 2), page_t<data_type>(2, 7), page_t<data_type>(3, 20), page_t<data_type>(4, 18)};

    for (int i = 0; i < 5; ++i) {
        assert(pages[i].pageOK());
        pages[i].print_page();
    }

}

void test_mem() {

    cache_mem_t<data_type> memory(5);

    memory.print_mem();

}

void test_cache() {

    cache_t<data_type> cache(5);
}