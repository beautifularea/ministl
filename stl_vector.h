//
//  stl_vector.h
//  ministl
//
//  Created by zhTian on 2017/6/7.
//  Copyright © 2017年 zhTian. All rights reserved.
//

#ifndef stl_vector_h
#define stl_vector_h

#include "stl_config.h"
#include "construct.h"

/*
 
 重点 ： insert 方法
 
 */

__NAMESPACE__BEGIN

template <typename T, typename Alloc>
class VectorBase {
public:
    typedef Alloc allocator_type;
    allocator_type get_allocator() const {
        return allocator_type();
    }
    
    VectorBase(const Alloc&) : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    VectorBase(size_t __n, const Alloc&) : _M_start(0), _M_finish(0), _M_end_of_storage(0) {
        _M_start = allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + __n;
    }
    virtual ~VectorBase() {
        deallocate(_M_start, _M_end_of_storage - _M_start);
    }
    
protected:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    
    typedef simple_alloc<T, Alloc> data_allocator;
    T *allocate(size_t __n) {
        return data_allocator::allocate(__n);
    }
    void deallocate(T *__p, size_t __n) {
        data_allocator::deallocate(__p,__n);
    }
};

template <typename T, typename Alloc = alloc>
class lvector : protected VectorBase<T, Alloc> {
public:
    typedef T value_type;
    typedef value_type& reference;
    typedef value_type* iterator;
    typedef size_t size_type;
    
private:
    typedef VectorBase<T, Alloc> Base;

    using Base::_M_start;
    using Base::_M_end_of_storage;
    using Base::_M_finish;
    
    using Base::allocate;

public:
    typedef typename Base::allocator_type allocator_type;
    allocator_type get_allocator() const {
        return Base::get_allocator();
    }

public:
    iterator begin() {
        return _M_start;
    }
    iterator end() {
        return _M_finish;
    }
    size_type size() {
        return size_type(_M_finish-_M_start);
    }
    size_type capacity() {
        return size_type(_M_end_of_storage-_M_start);
    }
    bool empty() {
        return _M_start == _M_finish;
    }
    reference operator[] (size_type __i) {
        return *(_M_start+__i);
    }
    reference at(size_type __i) {
        return (*this)[__i];
    }
    
    /*
     构造函数
     */
    explicit lvector(const allocator_type& a=allocator_type()) : Base(a) {}
    lvector(size_type __n, const T& __value, const allocator_type& __a = allocator_type()) : Base(__n, __a) {
        _M_finish = uninitialized_fill_n(_M_start, __n, __value);
    }
    explicit lvector(size_type __n) : Base(__n, allocator_type()){
        _M_finish = uninitialized_fill_n(_M_start, __n, T());
    }
    lvector(const lvector<T, Alloc>& __x) : Base(__x.size(), __x.get_allocator()) {
        _M_finish = uninitialized_copy(__x.begin(), __x.end(), _M_start);
    }
    template <typename InputIter>
    lvector(InputIter first, InputIter last, const allocator_type& type=allocator_type()) : Base(type) {
        //不对integer进行判断，默认为iterator = forward iterator
        size_type n = 0;
        distance(first, last);
        _M_start = allocate(n);
        _M_end_of_storage = _M_start + n;
        _M_finish = uninitialzed_copy(first, last, _M_start);
    }
    
    ~lvector() {
        destory(_M_start,_M_finish);
    }
    
    
    //其他方法
    reference front() {
        return *_M_start;
    }
    
    void push_back(const T& __x) {
        if(_M_finish != _M_end_of_storage) {
            construct(_M_finish, __x);
            ++_M_finish;
        }
        else {
            insert_aux(end(), __x);
        }
    }
    void pop_back() {
        -- _M_finish;
        destory(_M_finish);
    }
    
    iterator erase(iterator first, iterator last) {
        iterator i = copy(last, _M_finish, first);
        destory(i, _M_finish);
        _M_finish = _M_finish - (last - first);
        return first;
    }
    iterator resize(size_type new_size, const T& x) {
        if(new_size < size()) {
            erase(begin() + new_size, end());
        }
        else {
            insert(end(), new_size-size(), x);
        }
    }
    
    //对iterator进行了统一处理
    template <typename BidirectionalIter1, typename BidirectionalIter2>
    inline BidirectionalIter2
    copy_backward(BidirectionalIter1 first,
                  BidirectionalIter1 last,
                  BidirectionalIter2 result) {
        while(first != last) {
            *--result = *--last;
        }
        return result;
    }
    
    void insert_aux(iterator __p, const T& __x) {
        if(_M_finish != _M_end_of_storage) {
            construct(_M_finish, *(_M_finish-1));
            ++_M_finish;
            T copy_x = __x;
            copy_backward(__p, _M_finish - 2, _M_finish - 1);
        }
        else {
            size_type old_size = size();
            size_type len = old_size != 0 ? 2 * old_size : 1;
            iterator new_start = allocate(len);
            iterator new_finish = new_start;
            
            //没有实现 commit or rollback
            new_finish = uninitialized_copy(_M_start, __p, new_start);
            construct(new_finish, __x);
            ++new_finish;
            new_finish = uninitialzed_copy(__p, _M_finish, new_finish);
            
            destory(begin(), end());
            deallocate(_M_start, _M_end_of_storage - _M_start);
            _M_start = new_start;
            _M_finish = new_finish;
            _M_end_of_storage = new_start + len;
        }
    }
    
    iterator insert(iterator __p, const T& __x) {
        size_type n = __p - _M_start;
        if(_M_finish != _M_end_of_storage && __p == end()) {
            construct(_M_finish, __x);
            ++_M_finish;
        }
        else {
            insert_aux(__p, __x);
        }
        return begin() + n;
    }
};


__NAMESPACE__END

#endif /* stl_vector_h */
