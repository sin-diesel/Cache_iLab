#include <stdlib.h>
#include <unordered_map>
#include <list>
#include <iostream>

#define K1 0.5

//typedef int data_type;

template<typename data_t>
struct page_t {

    int _id; // page id
    unsigned _size; // page size
    data_t _data; // data

    page_t(int id, data_t data) {
        _id = id;
        _size = sizeof(data);
        _data = data;
    }

    page_t() { // default constructor
        _id = 0;
        _size = 0;
        _data = NULL;
    }

    void print_page() const;

    bool pageOK() const;
};

template<typename data_t>
struct cache_mem_t {

    std::list<page_t<data_t>> _list;
    unsigned _mem_size; // total size of cache buffer
    page_t<data_t>* _beginning;
    page_t<data_t>* _end;

    cache_mem_t() {
        _mem_size = 1;
        _list.insert(page_t<data_t>()); // creating single page by default
        _beginning = &_list.front();
        _end = &_list.back();
    }

    cache_mem_t(unsigned size) {
        _mem_size = size;

        for (int i = 0; i < size; ++i) {
            _list.insert(page_t<data_t>());
        }

        _beginning = &_list.front();
        _end = &_list.back();
    }

    void print_mem() const;

    void memOK() const;
};

void test_pages();

void test_mem();

void test_cache();

template<typename data_t>
struct cache_t {

    cache_mem_t<data_t> main_mem;

    unsigned _buffer_size;
    unsigned _main_size;

    void add_page(cache_mem_t<data_t> mem, const page_t<data_t> page);

    cache_t() {
        _buffer_size = main_mem._mem_size;
        _main_size = main_mem._mem_size;
    }

    cache_t(unsigned buffer_size) {
        _buffer_size = buffer_size;
        _main_size = K1 * buffer_size;

        main_mem = cache_mem_t<data_t>(_main_size);
    }
};