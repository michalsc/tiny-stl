// 

#include <stdio.h>
#include <iterator>
#include "support.h"

namespace t_std {

namespace {
    
    template <class T>
    struct node {
        MinNode mn;
        T       value;

        node(const T& val) : mn({nullptr, nullptr}), value(val) {}
    };

}

template <class T>
class list {
public:
    typedef T                   value_type;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef unsigned int        size_type;

    explicit list() : count(0) { NEWLIST(&_list); }
    explicit list(size_type n, const value_type& val = value_type()) : list() { while(n--) push_front(val); }

    void push_front(const value_type& val) {
        node<value_type> *n = (node<value_type> *)AllocMem(sizeof(node<value_type>), MEMF_CLEAR);
        new(n) node<value_type>(val);
        ADDHEAD(&_list, n);
        count++;
    }

    void push_back(const value_type& val) {
        node<value_type> *n = AllocMem(sizeof(node<value_type>), MEMF_CLEAR);
        new(&n) node<value_type>(val);
        ADDTAIL(&_list, n);
        count++;
    }

    void pop_front() {
        if (count > 0) {
            REMHEAD(&_list);
            count--;
        }
    }

    void pop_back() {
        if (count > 0) {
            REMTAIL(&_list);
            count--;
        }
    }

    size_type size() { return count; }
    bool empty() { return 0 == count; }

    void clear() { node<value_type> *n; while((n = (node<value_type> *)REMHEAD(&_list)) != nullptr) { n->~node<value_type>(); FreeMem(n, sizeof(node<value_type>)); }; count = 0; }

private:
    MinList             _list;
    size_type           count;


};

}