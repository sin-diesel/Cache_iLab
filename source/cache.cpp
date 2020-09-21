#include "cache.h"

template<>
void page_t<int>::print_page() const {
    std::cout << "Page ID: " << _id << " " << "Page size: " << _size << " " << "Page data " << _data << std::endl;
}


void test_pages() {

    page_t<int> pages[] = {page_t<int>(0, 4), page_t<int>(1, 2), page_t<int>(2, 7), page_t<int>(3, 20), page_t<int>(4, 18)};

    for (int i = 0; i < 5; ++i) {
        pages[i].print_page();
    }

}