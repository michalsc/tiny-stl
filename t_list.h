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
        node    *succ;
        node    *pred;
        T       value;

        node(const T& val) : succ(nullptr), pred(nullptr), value(val) {}
        void remove() { pred->succ = succ; succ->pred = pred; pred = nullptr; succ = nullptr;}
        node *getSucc() { if (succ && succ->succ) return succ; else return nullptr; }
        node *getPred() { if (pred && pred->pred) return pred; else return nullptr; }
    };

    template <class T>
    struct minlist {
        node<T> *head;
        node<T> *tail;
        node<T> *tailPred;

        minlist() : head((node<T> *)&tail), tail(nullptr), tailPred((node<T> *)&head) { }
        void reset() { head = (node<T> *)&tail; tail = nullptr; tailPred = (node<T> *)&head; }
        bool isEmpty() { return tailPred == (node<T> *)&head; }
        void addHead(node<T> *n) { n->succ = head; n->pred = (node<T> *)&head; head->pred = n; head = n; }
        void addTail(node<T> *n) { n->succ = (node<T> *)&tail; n->pred = tailPred; tailPred->succ = n; tailPred = n; }
        node<T> *getHead() const { return head->succ ? head : nullptr; }
        node<T> *getTail() const { return tailPred->pred ? tailPred : nullptr; }
        node<T> *remHead() { node<T> *head = getHead(); if (head) head->remove(); return head; }
        node<T> *remTail() { node<T> *tail = getTail(); if (tail) tail->remove(); return tail; }
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
            if (n->succ)
                n = n->succ;

            return *this;
        }
        iterator &operator--()
        {
            if (n->pred)
                n = n->pred;
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
            if (n->succ)
                n = n->succ;
            return *this;
        }
        const_iterator &operator--()
        {
            if (n->pred)
                n = n->pred;
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
            if (n->pred)
                n = n->pred;
            return *this;
        }
        reverse_iterator &operator--()
        {
            if (n->succ)
                n = n->succ;
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
    explicit list(const allocator_type& alloc = allocator_type()) : count(0), alloc(alloc) { }
    explicit list(size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type()) : list(alloc) { while(n--) push_front(val); }
    explicit list(int n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type()) : list((size_type)n, val, alloc) {}
    template <class InputIterator>
    list(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type()) : list(alloc) { for (auto it=first; it != last; ++it) push_back(*it); }
    ~list() { clear(); }
    list(const list& x) : count(0) { alloc = allocator_type(x.alloc); node<value_type> *n = x._list.getHead(); while(n) { push_back(n->value); n = n->getSucc(); } }
    list& operator= (const list& x) { clear(); node<value_type> *n = x._list.getHead(); while(n) { push_back(n->value); n = n->getSucc(); } return *this; }
    
    // Iterators
    iterator begin() { return iterator(_list.head); }
    iterator end() { return iterator((node<value_type> *)&_list.tail); }
    reverse_iterator rbegin() { return reverse_iterator(_list.tailPred); }
    reverse_iterator rend() { return reverse_iterator((node<value_type> *)&_list.head); }

    // Capacity
    size_type size() { return count; }
    bool empty() { return 0 == count; }
    size_type max_size() { return (uintptr_t)-1 / sizeof(node<value_type>); }

    // Element access
    reference front() { return _list.getHead()->value; }
    const_reference front() const { return _list.getHead()->value; }
    reference back() { return _list.getTail()->value; }
    const_reference back() const { return _list.getTail()->value; }

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
        _list.addHead(n); count++;
    }
    void pop_front() {
        if (count > 0) {
            node<value_type> *n = _list.remHead();
            if (n)
            {
                alloc.destroy(n);
                alloc.deallocate(n, 1);
                count--;
            }
        }
    }
    void push_back(const value_type& val) {
        node<value_type> *n = alloc.allocate(1);
        alloc.construct(n, val);
        _list.addTail(n); count++;
    }
    void pop_back() {
        if (count > 0) {
            node<value_type> *n = _list.remTail();
            if (n)
            {
                alloc.destroy(n);
                alloc.deallocate(n, 1);
                count--;
            }
        }
    }
    iterator insert(iterator position, const value_type& val) {
        node<value_type> *n = alloc.allocate(1);
        alloc.construct(n, node<value_type>(val));
        
        n->succ = position.n->succ;
        n->pred = position.n;

        n->succ->pred = n;
        n->pred->succ = n;
        
        count++;

        return iterator(n);
    }
    void insert(iterator position, int n, const value_type& val) {
        insert(position, (size_type)n, val);
    }
    void insert(iterator position, size_type n, const value_type& val) {
        if (n > 0)
        {
            minlist<value_type> l;

            count += n;

            while(n--)
            {
                node<value_type> *no = alloc.allocate(1);
                alloc.construct(no, node<value_type>(val));
                l.addTail(no);
            }

            node<value_type> *first = l.getHead();
            node<value_type> *last = l.getTail();

            first->pred = position.n;
            last->succ = position.n->succ;

            first->pred->succ = first;
            last->succ->pred = last;
        }
    }
    template <class InputIterator>
    void insert (iterator position, InputIterator first, InputIterator last) {
        minlist<value_type> l;

        for (auto it=first; it != last; ++it)
        {
            node<value_type> *no = alloc.allocate(1);
            alloc.construct(no, node<value_type>(*it));
            l.addTail(no);
            count++;
        }

        node<value_type> *fi = l.getHead();
        node<value_type> *la = l.getTail();
        if (fi)
        {
            fi->pred = position.n;
            la->succ = position.n->succ;

            fi->pred->succ = fi;
            la->succ->pred = la;
        }
    }
    iterator erase(iterator position) {
        node<value_type> *n = position.n;
        if (n->succ)
        {
            iterator it(n->succ);
            n->remove();
            alloc.destroy(n);
            alloc.deallocate(n, 1);
            count--;
            return it;
        }
        else return position;
    }
    iterator erase(iterator first, iterator last) {
        iterator it(first);
        while(it != last) {
            node<value_type> *n = it.n;
            ++it;
            n->remove();
            alloc.destroy(n);
            alloc.deallocate(n, 1);
            count--;
        }
        return last;
    }
    void swap(list& x) { size_type sz = count; count = x.count; x.count = sz; minlist<value_type> ml = _list; _list = x._list; x._list = ml; }
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
    void clear() { node<value_type> *n; while((n = _list.remHead()) != nullptr) { alloc.destroy(n); alloc.deallocate(n, 1); }; count = 0; }

    // Operations
    void splice(iterator position, list& x) {
        node<value_type> *first = x._list.getHead();
        node<value_type> *last = x._list.getTail();
        if (first)
        {
            if (position.n == _list.head)
            {
                first->pred = (node<T> *)&_list.head;
                last->succ = _list.head;
                
                _list.head->pred = last;
                _list.head = first;
            }
            else
            {
                first->pred = position.n->pred;
                last->succ = position.n;

                first->pred->succ = first;
                last->succ->pred = last;
            }
            x._list.reset();
            count += x.count;
            x.count = 0;
        }
    }
    void splice (iterator position, list& x, iterator i) {
        if (i != x.end())
        {
            node<value_type> *n = i.n;
            n->remove();
            x.count--;

            if (position.n == _list.head)
            {
                n->pred = (node<T> *)&_list.head;
                n->succ = _list.head;
                
                _list.head->pred = n;
                _list.head = n;
            }
            else
            {
                n->pred = position.n->pred;
                n->succ = position.n;

                n->succ->pred = n;
                n->pred->succ = n;
            }

            count++;
        }
    }
    void splice (iterator position, list& x, iterator first, iterator last) {
        minlist<value_type> l;
        iterator it(first);
        while (it != last)
        {
            node<value_type> *no = it.n;
            ++it;
            no->remove();
            x.count--;
            l.addTail(no);
            count++;
        }

        node<value_type> *fi = l.getHead();
        node<value_type> *la = l.getTail();

        if (fi)
        {
            if (position.n == _list.head)
            {
                fi->pred = (node<T> *)&_list.head;
                la->succ = _list.head;
                
                _list.head->pred = la;
                _list.head = fi;
            }
            else
            {
                fi->pred = position.n->pred;
                la->succ = position.n;

                fi->pred->succ = fi;
                la->succ->pred = la;
            }
        }
                //test();
    }
    void remove(const value_type& val) {
        node<value_type> *n = _list.getHead(), *next;
        while(n) {
            next = n->getSucc();
            if (n->value == val) {
                n->remove();
                alloc.destroy(n);
                alloc.deallocate(n, 1);
                count--;
            }
            n = next;
        }
    }
    // remove_if
    void unique() {
        node<value_type> *n;
        for(n = _list.getHead(); n; n = n->getSucc()) {
            node<value_type> *n1, *next;
            n1 = n->getSucc();
            while(n1) {
                next = n1->getSucc();
                if (n1->value == n->value) {
                    n1->remove();
                    count--;
                    alloc.destroy(n1);
                    alloc.deallocate(n1, 1);
                }
                n1 = next;
            }
        }
    }
    void merge(list &x) {
        node<value_type> *n;
        while ((n = x._list.remHead()) != nullptr) {
            _list.addTail(x);
            count++;
        }
        x.count = 0;
    }
    // sort
    // reverse
    void test() {
        node<value_type> *n = _list.getHead();
        printf("list=%p, list->head=%p, list->tail=%p, list->tailpred=%p\n",
               (void *)&_list, (void *)(_list.head), (void *)(_list.tail), (void *)(_list.tailPred));
        printf("  HEAD @ %p. Head->succ=%p Head->pred=%p\n", (void *)n, (void *)(n->succ), (void *)(n->pred));

        n = n->getSucc();
        while(n != _list.getTail())
        {
            printf("  node @ %p. node->succ=%p node->pred=%p\n", (void *)n, (void *)(n->succ), (void *)(n->pred));
            n = n->getSucc();
        }

        n = _list.getTail();
        printf("  TAIL @ %p. Tail->succ=%p Tail->pred=%p\n", (void *)n, (void *)(n->succ), (void *)(n->pred));
    }
private:
    minlist<T>          _list;
    size_type           count;
    allocator_type      alloc;
};

}

#endif // _T_LIST_H