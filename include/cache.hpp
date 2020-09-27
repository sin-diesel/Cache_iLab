#include <stdlib.h>
#include <unordered_map>
#include <list>
#include <iostream>

#define K1 0.5

//typedef int data_type;

template<typename data_t>
struct page_t {

    int id_ = 0; // page id
    unsigned size_ = 0; // page size
    data_t data_ = NULL; // data

    page_t(int id, data_t data) {
        id_ = id;
        size_ = sizeof(data);
        data_ = data;
    }

    page_t() { // default constructor worse then in - class initialization?
        id_ = 0;
        size_ = 0;
        data_ = NULL;
    }

    void print_page() const { // is it ok to write function definition in class, even for .hpp file?
        std::cerr << "Page ID: " << id_ << " " << "Page size: " << size_ << " " << "Page data " << data_ << std::endl;
    }

    bool pageOK() const {
        if (size_ < 0) {
            std::cerr << "Page with invalid size: " << std::endl;
            print_page();
            return false;
        }

        return true;
    }
};

//template<typename data_t>
//struct cache_mem_t {
//
//    std::list<page_t<data_t>> list_;
//    unsigned mem_size_; // total size of cache buffer
//    page_t<data_t>* beginning_;
//    page_t<data_t>* end_;
//
//    cache_mem_t() {
//        mem_size_ = 1;
//        list_.insert(page_t<data_t>()); // creating single page by default
//        beginning_ = &list_.front();
//        end_ = &list_.back();
//    }
//
//    cache_mem_t(unsigned size) {
//        mem_size_ = size;
//
//        for (int i = 0; i < size; ++i) {
//            list_.insert(page_t<data_t>());
//        }
//
//        beginning_ = &list_.front();
//        end_ = &list_.back();
//    }
//
//    void print_mem() const;
//
//    void memOK() const;
//};

void test_pages();

void test_mem();

void test_cache();

//template<typename data_t>
//struct cache_t {
//
//    cache_mem_t<data_t> main_mem;
//
//    unsigned _buffer_size;
//    unsigned _main_size;
//
//    void add_page(cache_mem_t<data_t> mem, const page_t<data_t> page);
//
//    cache_t() {
//        _buffer_size = main_mem._mem_size;
//        _main_size = main_mem._mem_size;
//    }
//
//    cache_t(unsigned buffer_size) {
//        _buffer_size = buffer_size;
//        _main_size = K1 * buffer_size;
//
//        main_mem = cache_mem_t<data_t>(_main_size);
//    }
//};