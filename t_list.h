// 
#ifndef _T_LIST_H
#define _T_LIST_H

#include <stdio.h>
#include <iterator>
#include "support.h"

#include "t_allocator.h"

namespace t_std {

namespace {
    
    template <class T>
    struct node {
        MinNode mn;
        T       value;

        node(const T& val) : mn({nullptr, nullptr}), value(val) {}
    };

}

template < class T, class Alloc = allocator<T> >
class list {
public:
    typedef T                       value_type;
    typedef value_type&             reference;
    typedef const value_type&       const_reference;
    typedef value_type*             pointer;
    typedef const value_type*       const_pointer;
    typedef intptr_t                size_type;
    typedef allocator<node<value_type> >            allocator_type;

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
            if (n->mn.mln_Succ)
                n = (node<value_type> *)(n->mn.mln_Succ);
            return *this;
        }
        iterator &operator--()
        {
            if (n->mn.mln_Pred)
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

        friend class list::const_iterator;
        friend class list;
    };

    class const_iterator : public std::iterator<std::bidirectional_iterator_tag, value_type>
    {
        const node<value_type> *n;

      public:
        const_iterator() : n(nullptr){};
        const_iterator(node<value_type> *node) : n(node){};
        const_iterator(const const_iterator &it) : n(it.n){};
        const_iterator(const typename list<value_type>::iterator &it) : n(it.n) {};
        value_type &operator*() const { return n->value; }

        const_iterator &operator++()
        {
            if (n->mn.mln_Succ)
                n = (node<value_type> *)(n->mn.mln_Succ);
            return *this;
        }
        const_iterator &operator--()
        {
            if (n->mn.mln_Pred)
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

        friend class list;
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
            if (n->mn.mln_Pred)
                n = (node<value_type> *)(n->mn.mln_Pred);
            return *this;
        }
        reverse_iterator &operator--()
        {
            if (n->mn.mln_Succ)
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

        friend class list;
    };

    // Constructors
    explicit list(const allocator_type& alloc = allocator_type()) : count(0), alloc(alloc) { NEWLIST(&_list); }
    explicit list(size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type()) : list(alloc) { while(n--) push_front(val); }
    explicit list(int n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type()) : list((size_type)n, val, alloc) {}
    template <class InputIterator>
    list(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type()) : list(alloc) { for (auto it=first; it != last; ++it) push_back(*it); }
    ~list() { clear(); }
    list(const list& x) : count(0) { NEWLIST(&_list); alloc = allocator_type(x.alloc); void *i; ForeachNode(&x._list, i) { node<value_type> *n = (node<value_type> *)i; push_back(n->value); } }
    list& operator= (const list& x) { clear(); void *i; ForeachNode(&x._list, i) { node<value_type> *n = (node<value_type> *)i; push_back(n->value); } return *this; }
    
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
        node<value_type> *n = alloc.allocate(1);
        alloc.construct(n, node<value_type>(val));
        ADDHEAD(&_list, n); count++;
    }
    void pop_front() {
        if (count > 0) {
            node<value_type> *n = (node<value_type> *)REMHEAD(&_list);
            alloc.destroy(n);
            alloc.deallocate(n, 1);
            count--;
        }
    }
    void push_back(const value_type& val) {
        node<value_type> *n = alloc.allocate(1);
        alloc.construct(n, val);
        ADDTAIL(&_list, n); count++;
    }
    void pop_back() {
        if (count > 0) {
            node<value_type> *n = (node<value_type> *)REMTAIL(&_list);
            alloc.destroy(n);
            alloc.deallocate(n, 1);
            count--;
        }
    }
    iterator insert(iterator position, const value_type& val) {
        node<value_type> *n = alloc.allocate(1);
        alloc.construct(n, node<value_type>(val));
        
        n->mn.mln_Succ = position.n->mn.mln_Succ;
        n->mn.mln_Pred = (struct MinNode *)position.n;

        n->mn.mln_Succ->mln_Pred = (struct MinNode *)n;
        n->mn.mln_Pred->mln_Succ = (struct MinNode *)n;
        
        count++;

        return iterator(n);
    }
    void insert(iterator position, int n, const value_type& val) {
        insert(position, (size_type)n, val);
    }
    void insert(iterator position, size_type n, const value_type& val) {
        if (n > 0)
        {
            MinList l;
            NEWLIST(&l);

            count += n;

            while(n--)
            {
                node<value_type> *no = alloc.allocate(1);
                alloc.construct(no, node<value_type>(val));
                ADDTAIL(&l, (MinNode *)no);
            }

            node<value_type> *first = (node<value_type> *)GetHead(&l);
            node<value_type> *last = (node<value_type> *)GetTail(&l);

            first->mn.mln_Pred = (struct MinNode *)position.n;
            last->mn.mln_Succ = position.n->mn.mln_Succ;

            first->mn.mln_Pred->mln_Succ = (struct MinNode *)first;
            last->mn.mln_Succ->mln_Pred = (struct MinNode *)last;
        }
    }
    template <class InputIterator>
    void insert (iterator position, InputIterator first, InputIterator last) {
        MinList l;
        NEWLIST(&l);

        for (auto it=first; it != last; ++it)
        {
            node<value_type> *no = alloc.allocate(1);
            alloc.construct(no, node<value_type>(*it));
            ADDTAIL(&l, (MinNode *)no);
            count++;
        }

        node<value_type> *fi = (node<value_type> *)GetHead(&l);
        node<value_type> *la = (node<value_type> *)GetTail(&l);
        if (fi)
        {
            fi->mn.mln_Pred = (struct MinNode *)position.n;
            la->mn.mln_Succ = position.n->mn.mln_Succ;

            fi->mn.mln_Pred->mln_Succ = (struct MinNode *)fi;
            la->mn.mln_Succ->mln_Pred = (struct MinNode *)la;
        }
    }
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
    void clear() { node<value_type> *n; while((n = (node<value_type> *)REMHEAD(&_list)) != nullptr) { alloc.destroy(n); alloc.deallocate(n, 1); }; count = 0; }

    // Operations
    // splice
    void remove(const value_type& val) {
        node<value_type> *n, *next;
        ForeachNodeSafe(&_list, n, next) {
            if (n->value == val) {
                REMOVE(n);
                alloc.destroy(n);
                alloc.deallocate(n, 1);
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
                    alloc.destroy(n1);
                    alloc.deallocate(n1, 1);
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
    allocator_type      alloc;
};

}

#endif // _T_LIST_H