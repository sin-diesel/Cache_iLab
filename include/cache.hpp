#include <stdlib.h>
#include <unordered_map>
#include <list>
#include <iostream>
#include <vector>
#include <assert.h>

#define DEBUG true

#define DUB(a) if (DEBUG) {a}  

#define Kin 0.25
#define Kout 0.5


struct input_t {
    unsigned buffer_size_;
    unsigned n_tests_;

    input_t(unsigned buffer_size, unsigned n_tests) {
        buffer_size_ = buffer_size;
        n_tests_ = n_tests;
    }
};

template<typename data_t>
struct page_t {

    int id_ = 0; // page id
    unsigned size_ = 0; // page size
    data_t data_ = 0; // data

    bool operator == (const page_t& page) const {
        return id_ == page.id_;
    }

    page_t(int id = 0, data_t data = 0): id_(id), size_(sizeof(data)), data_(data) {
        if (data == 0) {
            size_ = 0;
        }
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

template<typename elem_t>
struct cache_mem_t { // cache mem structure for easier interpretation of am and secondary_mem in 2q algorithm

    std::list<elem_t> list_;
    std::unordered_map<int, elem_t*> hash_table;

    unsigned mem_size_; // total size of cache buffer
    unsigned cur_size_;
    elem_t* beginning_;
    elem_t* end_;

    cache_mem_t() {
        mem_size_ = 0;
        cur_size_ = 0;
        beginning_ = NULL;
        end_ = NULL;
    }

    cache_mem_t(unsigned size) {
        mem_size_ = size;
        cur_size_ = 0;

        for (int i = 0; i < size; ++i) {
            list_.push_back(elem_t());
        }

        beginning_ = &list_.front();
        end_ = &list_.back();
    }

    void print_mem() const {
        std::cerr << "Printing memory: " << this << std::endl;
        std::cerr << "Memory size: " << mem_size_ << std::endl;
        std::cerr << "Memory current size: " << mem_size_ << std::endl;
        for (auto v : list_) {
            v.print_page();
        }
    }

    void add_page(elem_t page) {

        list_.push_front(page);
        hash_table.insert({page.id_, &page});
        beginning_ = &list_.front();

        
        elem_t* deleted = &list_.back();
        hash_table.erase(deleted->id_);
        list_.pop_back();
        end_ = &list_.back();

        cur_size_++;

    }

    void remove_page(elem_t page) {
        hash_table.erase(page.id_);
        list_.remove(page);  // actually I do not want to remove page completely, I want to leave the space that it was occupying (buffer has fixed sized and we do not want to change it)
        list_.push_back(elem_t());
        cur_size_--;
    }

    bool page_at(int id) const {
        try { // can be done better?
            hash_table.at(id);
        } catch(std::out_of_range) { // how to do?
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
    
    cache_mem_t<page_t<data_t>> am; // main buffer Am
    cache_mem_t<page_t<data_t>> a1; // secondary memory which Ain
    cache_mem_t<page_t<data_t>> aout; // memory to hold only references to pages Aout (handle pointer later, for now storing simply pages)

    cache_t() = delete; // can not delete :(


    cache_t(unsigned n_pages): am(), a1(), aout() {
        unsigned aout_size = Kout * n_pages * (sizeof(page_t<data_t>) / sizeof(page_t<data_t>*));
        if (aout_size == 0) {
            aout_size = 1;
        }
        unsigned a1_size = Kin * (n_pages - aout_size);
        if (a1_size == 0) {
            a1_size = 1;
        }
        unsigned am_size = n_pages - aout_size - a1_size;

        am = cache_mem_t<page_t<data_t>>(am_size);
        a1 = cache_mem_t<page_t<data_t>>(a1_size);
        aout = cache_mem_t<page_t<data_t>>(aout_size);
    }

    void print_cache() const {
        std::cerr << "\n\n\n";
        std::cerr << "Printing 2q cache at: " << this << std::endl;
        std::cerr << "\tprinting am memory: " << std::endl;
        am.print_mem();
        std::cerr << "\t printing a1 memory: " << std::endl;
        a1.print_mem();
        std::cerr << "\t printing aout memory: " << std::endl;
        aout.print_mem();
        std::cerr << "Cache " << this << " printed.";
        std::cerr << "\n\n\n";
    }
    
    void reclaimfor(page_t<data_t> const page) {
        if (am.cur_size_ < am.mem_size_) {
            am.add_page(page);
        } else if (a1.cur_size_ < a1.mem_size_) {
            a1.add_page(page);
        } else if (a1.cur_size_ == a1.mem_size_) {

            page_t<data_t> tail = a1.list_.back();
            aout.add_page(tail); // do not use list
            a1.remove_page(tail);

            // if (aout.cur_size_ == aout.mem_size_) {
            //     page_t<data_t> tail = aout.list_.back();
            //     aout.remove_page(tail);
            // }
            a1.add_page(page); 
        } else {
            page_t<data_t> tail = am.list_.back();
            am.remove_page(tail);
            am.add_page(page);
        }
    }

    int handle_page(page_t<data_t> const page) { // main algorithm

        if (am.page_at(page.id_)) { // if page is in Am queue, move page to the head
            am.remove_page(page); // add swap method later, use pointers to page instead
            am.add_page(page);
            return 1; // hit
        } else if (aout.page_at(page.id_)) {
            reclaimfor(page); // deciding where to put the page
            aout.remove_page(page);
            return 0; // miss
        } else if (a1.page_at(page.id_)) {
            return 1; // hit
        } else { // page is in no queue
            reclaimfor(page);
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
            cache.am.add_page(pages[i]);
        }
    }

    void test_cache(cache_t<data_t>& cache) const {
        int hits = 0;
        for (int i = 0; i < num_of_pages_; ++i) {
            std::cerr << "Accessing page with id: " << pages[i].id_ << "\n";
            hits += cache.handle_page(pages[i]);
            cache.print_cache();
        }
        std::cerr << "Hits: " << hits << std::endl;
    }
};

void test_pages();

void test_mem();

void test_cache();

void acquire_input();

///TODO: 1) page_ok, test_ok, mem_ok
/// 2) debugging defines
/// 3) cmd line parameters to launch program in different mods
/// 4) introduce input system
/// 5) fix makefile

