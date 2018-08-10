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
    typedef intptr_t            size_type;

    // Constructors
    explicit list() : count(0) { NEWLIST(&_list); }
    explicit list(size_type n, const value_type& val = value_type()) : list() { while(n--) push_front(val); }
    ~list() { clear(); }
    list& operator= (const list& x) { clear(); node<value_type> *n; ForeachNode(&x._list, n) push_back(n->value); return *this; }
    
    // Iterators

    // Capacity
    size_type size() { return count; }
    bool empty() { return 0 == count; }
    size_type max_size() { return (uintptr_t)-1 / sizeof(node<value_type>); }

    // Element access
    reference front() { node<value_type> *n = (node<value_type> *)GetHead(&_list); return n->value; }
    const_reference front() const { node<value_type> *n = (node<value_type> *)GetHead(&_list); return n->value; }
    reference tail() { node<value_type> *n = (node<value_type> *)GetTail(&_list); return n->value; }
    const_reference tail() const { node<value_type> *n = (node<value_type> *)GetTail(&_list); return n->value; }

    // Modifiers
    //template <class InputIterator>
    //void assign(InputIterator first, InputIterator last);
    void assign(size_type n, const value_type &val) { clear(); while(n--) push_front(val); }
    void push_front(const value_type& val) {
        node<value_type> *n = (node<value_type> *)AllocMem(sizeof(node<value_type>), MEMF_CLEAR);
        new(n) node<value_type>(val);
        ADDHEAD(&_list, n); count++;
    }
    void pop_front() {
        if (count > 0) {
            REMHEAD(&_list);
            count--;
        }
    }
    void push_back(const value_type& val) {
        node<value_type> *n = AllocMem(sizeof(node<value_type>), MEMF_CLEAR);
        new(&n) node<value_type>(val);
        ADDTAIL(&_list, n); count++;
    }
    void pop_back() {
        if (count > 0) {
            REMTAIL(&_list);
            count--;
        }
    }
    // insert
    // erase
    void swap(list& x) { size_type sz = count; count = x.count; x.count = sz; MinList ml = _list; _list = x._list; x._list = ml; }
    // resize
    void clear() { node<value_type> *n; while((n = (node<value_type> *)REMHEAD(&_list)) != nullptr) { n->~node<value_type>(); FreeMem(n, sizeof(node<value_type>)); }; count = 0; }

    // Operations
    // splice
    void remove(const value_type& val) {
        node<value_type> *n, *next;
        ForeachNodeSafe(&_list, n, next) {
            if (n->value == val) {
                REMOVE(n);
                n->~node<value_type>();
                FreeMem(n, sizeof(node<value_type>));
            }
        }
    }
    // remove_if
    void unique() {
        node<value_type> *n;
        ForeachNode(&_list, n) {
            node<value_type> *n1, *next;
            n1 = (node<value_type> *)GetSucc(n);
            while(n1) {
                next = (node<value_type> *)GetSucc(n1);
                if (n1->value == n->value) {
                    REMOVE(n1);
                    count--;
                    n1->~node<value_type>();
                    FreeMem(n1, sizeof(node<value_type>));
                }
                n1 = next;
            }
        }
    }
    void merge(list &x) {
        node<value_type> *n, *next;
        ForeachNodeSafe(&x._list, n, next) {
            REMOVE(x);
            ADDTAIL(&_list, x);
            count++;
        }
        x.count = 0;
    }
    // sort
    // reverse
private:
    MinList             _list;
    size_type           count;


};

}