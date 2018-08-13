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

    class const_iterator;
    // Bidirectional iterator for list
    class iterator : public std::iterator<std::bidirectional_iterator_tag, value_type>
    {
        node<value_type> *n;

      public:
        iterator() : n(nullptr){};
        iterator(node<value_type> *node) : n(node){};
        iterator(const iterator &it) : n(it.n){};
        value_type& operator*() const { return n->value; }

        iterator &operator++()
        {
            n = (node<value_type> *)(n->mn.mln_Succ);
            return *this;
        }
        iterator &operator--()
        {
            n = (node<value_type> *)(n->mn.mln_Pred);
            return *this;
        }
        iterator operator++(int)
        {
            iterator tmp(*this);
            operator++();
            return tmp;
        }
        iterator operator--(int)
        {
            iterator tmp(*this);
            operator--();
            return tmp;
        }

        bool operator==(const iterator &rhs) const { return n == rhs.n; }
        bool operator!=(const iterator &rhs) const { return n != rhs.n; }

        friend class string::const_iterator;
    };

    class const_iterator : public std::iterator<std::bidirectional_iterator_tag, value_type>
    {
        const node<value_type> *n;

      public:
        const_iterator() : n(nullptr){};
        const_iterator(node<value_type> *node) : n(node){};
        const_iterator(const const_iterator &it) : n(it.n){};
        const_iterator(const list<value_type>::iterator &it) : n(it.n) {};
        value_type &operator*() const { return n->value; }

        const_iterator &operator++()
        {
            n = (node<value_type> *)(n->mn.mln_Succ);
            return *this;
        }
        const_iterator &operator--()
        {
            n = (node<value_type> *)(n->mn.mln_Pred);
            return *this;
        }
        const_iterator operator++(int)
        {
            const_iterator tmp(*this);
            operator++();
            return tmp;
        }
        const_iterator operator--(int)
        {
            const_iterator tmp(*this);
            operator--();
            return tmp;
        }

        bool operator==(const const_iterator &rhs) const { return n == rhs.n; }
        bool operator!=(const const_iterator &rhs) const { return n != rhs.n; }
    };

    class reverse_iterator : public std::iterator<std::bidirectional_iterator_tag, value_type>
    {
        node<value_type> *n;

      public:
        reverse_iterator() : n(nullptr){};
        reverse_iterator(node<value_type> *node) : n(node){};
        reverse_iterator(const reverse_iterator &it) : n(it.n){};
        value_type &operator*() const { return n->value; }

        reverse_iterator &operator++()
        {
            n = (node<value_type> *)(n->mn.mln_Pred);
            return *this;
        }
        reverse_iterator &operator--()
        {
            n = (node<value_type> *)(n->mn.mln_Succ);
            return *this;
        }
        reverse_iterator operator++(int)
        {
            reverse_iterator tmp(*this);
            operator++();
            return tmp;
        }
        reverse_iterator operator--(int)
        {
            reverse_iterator tmp(*this);
            operator--();
            return tmp;
        }

        bool operator==(const reverse_iterator &rhs) const { return n == rhs.n; }
        bool operator!=(const reverse_iterator &rhs) const { return n != rhs.n; }
    };

    // Constructors
    explicit list() : count(0) { NEWLIST(&_list); }
    explicit list(size_type n, const value_type& val = value_type()) : list() { while(n--) push_front(val); }
    ~list() { clear(); }
    list& operator= (const list& x) { clear(); node<value_type> *n; ForeachNode(&x._list, n) push_back(n->value); return *this; }
    
    // Iterators
    iterator begin() { if(count > 0) return iterator((node<value_type> *)_list.mlh_Head); else return end(); }
    iterator end() { return iterator((node<value_type> *)&_list.mlh_Tail); }
    reverse_iterator rbegin() { if(count > 0) return reverse_iterator((node<value_type> *)_list.mlh_TailPred); else return rend(); }
    reverse_iterator rend() { return reverse_iterator((node<value_type> *)&_list.mlh_Head); }

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
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last) {
        clear(); for (auto it=first; it != last; ++it) {
            push_back(*it);
        }
    }
    void assign(size_type n, const value_type &val) { clear(); while(n--) push_front(val); }
    void push_front(const value_type& val) {
        node<value_type> *n = (node<value_type> *)AllocMem(sizeof(node<value_type>), MEMF_CLEAR);
        new(n) node<value_type>(val);
        ADDHEAD(&_list, n); count++;
    }
    void pop_front() {
        if (count > 0) {
            node<value_type> *n = (node<value_type> *)REMHEAD(&_list);
            n->~node<value_type>();
            FreeMem(n, sizeof(node<value_type>));
            count--;
        }
    }
    void push_back(const value_type& val) {
        node<value_type> *n = (node<value_type> *)AllocMem(sizeof(node<value_type>), MEMF_CLEAR);
        new(n) node<value_type>(val);
        ADDTAIL(&_list, n); count++;
    }
    void pop_back() {
        if (count > 0) {
            node<value_type> *n = (node<value_type> *)REMTAIL(&_list);
            n->~node<value_type>();
            FreeMem(n, sizeof(node<value_type>));
            count--;
        }
    }
    // insert
    // erase
    void swap(list& x) { size_type sz = count; count = x.count; x.count = sz; MinList ml = _list; _list = x._list; x._list = ml; }
    void resize(size_type n, value_type val = value_type()) {
        if (count < n) {
            int cnt = n - count;
            while(cnt--)
                push_back(val);
        } else {
            int cnt = count - n;
            while(cnt--)
                pop_back();
        }
    }
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
                count--;
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
    void test() {
        node<value_type> *n = (node<value_type> *)GetHead(&_list);
        printf("list=%p, list->head=%p, list->tail=%p, list->tailpred=%p\n",
               (void *)&_list, (void *)(_list.mlh_Head), (void *)(_list.mlh_Tail), (void *)(_list.mlh_TailPred));
        printf("HEAD @ %p. Head->succ=%p Head->pred=%p\n", (void *)n, (void *)(n->mn.mln_Succ), (void *)(n->mn.mln_Pred));
        n = (node<value_type> *)GetTail(&_list);
        printf("TAIL @ %p. Head->succ=%p Head->pred=%p\n", (void *)n, (void *)(n->mn.mln_Succ), (void *)(n->mn.mln_Pred));
    }
private:
    MinList             _list;
    size_type           count;


};

}