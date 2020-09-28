#include <stdlib.h>
#include <unordered_map>
#include <list>
#include <iostream>
#include <vector>

#define K1 0.5
#define K2 0.5

//typedef int data_type;

template<typename data_t>
struct page_t {

    int id_ = 0; // page id
    unsigned size_ = 0; // page size
    data_t data_ = NULL; // data

    bool operator == (const page_t& page) const {
        return id_ == page.id_;
    }

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
struct cache_mem_t { // cache mem structure for easier interpretation of main_mem and secondary_mem in 2q algorithm

    std::list<page_t<data_t>> list_;
    std::unordered_map<int, page_t<data_t>*> hash_table;

    unsigned mem_size_ = 0; // total size of cache buffer
    unsigned cur_size_ = 0;
    page_t<data_t> *beginning_ = NULL;
    page_t<data_t> *end_ = NULL;


    cache_mem_t() {
        mem_size_ = 1;
        cur_size_ = 1;
        list_.push_back(page_t<data_t>()); // creating single page by default
        beginning_ = &list_.front();
        end_ = &list_.back();
    }

    cache_mem_t(unsigned size) {
        mem_size_ = size;
        cur_size_ = 0;

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

    void add_page(page_t<data_t> page) {

        list_.push_front(page);
        hash_table.insert({page.id_, &page});
        beginning_ = &list_.front();
        cur_size_ += 1;

        page_t<data_t>* deleted = &list_.back();
        hash_table.erase(deleted->id_);
        list_.pop_back();
        end_ = &list_.back();

    }

    void remove_page(page_t<data_t> page) {

        hash_table.erase(page.id_);
        list_.remove(page);  // actually I do not want to remove page completely, I want to leave the space that it was occupying (buffer has fixed sized and we do not want to change it)
        list_.push_back(page_t<data_t>());
        cur_size_ -= 1;
    }

    bool page_at(int id) const {
        try { // can be done better?
            hash_table.at(id);
        } catch(std::out_of_range) {
            return false;
        }
        return true;
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
    cache_mem_t<data_t> a1_mem;
    cache_mem_t<data_t> aout_mem;

    unsigned buffer_size_ = 0;
    unsigned main_size_ = 0;
    unsigned a1_size = 0;
    unsigned aout_size = 0;


    cache_t() {
        buffer_size_ = main_mem.mem_size_;
        main_size_ = main_mem.mem_size_;
        a1_mem = a1_mem.mem_size_;
        aout_mem = aout_mem->mem_size_;
    }

    cache_t(unsigned buffer_size) {

        assert(buffer_size > 0);
        buffer_size_ = buffer_size;
        main_size_ = buffer_size;
        a1_size = buffer_size;
        aout_size = buffer_size;

        main_mem = cache_mem_t<data_t>(main_size_);
        a1_mem = cache_mem_t<data_t>(a1_size);
        aout_mem = cache_mem_t<data_t>(aout_size);
    }

    cache_t(unsigned buffer_size, float am, float a1) {

        assert(buffer_size > 0);
        buffer_size_ = buffer_size;
        main_size_ = am * buffer_size;
        a1_size = buffer_size_ * a1;
        aout_size = buffer_size_ - main_size_ - a1_size;

        assert(main_size_ >= 0);
        assert(a1_size >= 0);
        assert(aout_size >= 0);

        main_mem = cache_mem_t<data_t>(main_size_);
        a1_mem = cache_mem_t<data_t>(a1_size);
        aout_mem = cache_mem_t<data_t>(aout_size);
    }

    void print_cache() const {
        std::cerr << "\n\n\n";
        std::cerr << "Printing 2q cache: " << std::endl;
        std::cerr << "buffer size: " << buffer_size_ << std::endl;
        std::cerr << "main size: " << main_size_ << std::endl;
        std::cerr << "a1 size: " << a1_size << std::endl;
        std:: cerr << "aout size: " << aout_size << std::endl;
        std::cerr << "\tprinting main memory: " << std::endl;
        main_mem.print_mem();
        std::cerr << "\t printing a1 memory: " << std::endl;
        a1_mem.print_mem();
        std::cerr << "\t printing aout memory: " << std::endl;
        aout_mem.print_mem();
        std::cerr << "\n\n\n";
    }

    int handle_page(page_t<data_t> page) { // main algorithm

        if (main_mem.page_at(page.id_)) { // if page is in Am queue, move page to the head
            main_mem.remove_page(page);
            main_mem.add_page(page);
            return 1; // hit
        } else if (aout_mem.page_at(page.id_)) {
            aout_mem.remove_page(page);
            main_mem.add_page(page);
            return 0; // miss
        } else if (a1_mem.page_at(page.id_)) {
            a1_mem.remove_page(page); // moving to the head
            a1_mem.add_page(page);
            return 1; // hit
        } else { // page is in no queue
            if (a1_mem.cur_size_ < a1_mem.mem_size_) {
                a1_mem.add_page(page);
            } else {
                aout_mem.add_page(a1_mem.list_.back());
                a1_mem.add_page(page);
            }
            return 0; // miss
        }

    }



};

template<typename data_t>
struct tests_t {

    unsigned num_of_pages_;
    std::vector<page_t<data_t>> pages;

    tests_t(unsigned num_of_pages) {

        assert(num_of_pages > 0);
        num_of_pages_ = num_of_pages;

        srand(time(NULL));

        for (int i = 0; i < num_of_pages_; ++i) {
            pages.push_back(page_t<data_t>(rand() % 100, rand() % 100)); // generating random data in page with random id, need some ideas for interesting tests
        }
    }

    void print_test_data() const {
        std::cerr << "Printing test data: \n";
        std::cerr << "Number of pages: " << num_of_pages_ << "\n";
        std::cerr << "Printing generated pages: \n" << "\n";

        for (int i = 0; i < num_of_pages_; ++i) {
            pages[i].print_page();
        }
    }

    void fill_cache(cache_t<data_t>& cache) const {

        for (int i = 0; i < num_of_pages_; ++i) {
            cache.main_mem.add_page(pages[i]);
        }
    }

    void test_cache(cache_t<data_t>& cache) const {

        int hits = 0;

        for (int i = 0; i < num_of_pages_; ++i) {
            std::cerr << "Accessing page with id: " << pages[i].id_ << "\n";
            hits += cache.handle_page(pages[i]);
            //cache.print_cache();
        }
        std::cout << "Hits: " << hits << std::endl;
    }
};

void test_pages();

void test_mem();

void test_cache();

