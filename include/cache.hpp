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

template<typename data_t>
struct cache_mem_t {

    std::list<page_t<data_t>> list_;
    unsigned mem_size_ = 0; // total size of cache buffer
    page_t<data_t> *beginning_ = NULL;
    page_t<data_t> *end_ = NULL;

    cache_mem_t() {
        mem_size_ = 1;
        list_.push_back(page_t<data_t>()); // creating single page by default
        beginning_ = &list_.front();
        end_ = &list_.back();
    }

    cache_mem_t(unsigned size) {
        mem_size_ = size;

        for (int i = 0; i < size; ++i) {
            list_.push_back(page_t<data_t>());
        }

        beginning_ = &list_.front();
        end_ = &list_.back();
    }

    void print_mem() const {
        std::cerr << "Printing memory: " << std::endl;
        for (auto const &i: list_) {
            i.print_page();
        }
    }

    void add_page(page_t<data_t> const page) {

        list_.push_front(page);
        beginning_ = &list_.front();
        list_.pop_back();
        end_ = &list_.back();

    }

    bool memOK() {
        if (mem_size_ < 0) {
            return false;
        }

        return true;
    }
};

template<typename data_t>
struct cache_t {

    cache_mem_t<data_t> main_mem;

    unsigned buffer_size_ = 0;
    unsigned main_size_ = 0;


    cache_t() {
        buffer_size_ = main_mem.mem_size_;
        main_size_ = main_mem.mem_size_;
    }

    cache_t(unsigned buffer_size) {
        buffer_size_ = buffer_size;
        main_size_ = K1 * buffer_size;

        main_mem = cache_mem_t<data_t>(main_size_);
    }
};

void test_pages();

void test_mem();

void test_cache();
