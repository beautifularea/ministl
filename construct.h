//
//  construct.h
//  ministl
//
//  Created by zhTian on 2017/6/5.
//  Copyright © 2017年 zhTian. All rights reserved.
//

#ifndef construct_h
#define construct_h

/*
 本文件包含【五大全局函数】，用来处理内存的：
 1、对象的构建和销毁
 construct/destory
 2、初始化
 没有实现异常处理和 commit or rollback操作
 uninitialized_copy
 uninitialized_fill
 uninitialized_fill_n
 
 */

#include "type_traits.h"
#include "iterator_traits.h"

/*
 包含全局函数：construct()/destory() 
 负责对象的构建和析构操作.
 
 内存的 开辟 和销毁，见 allocate/deallocate 方法
 */
namespace lpstl {
    
    template <typename T>
    inline void _Construct(T *__p) {
        new ((void *)__p) T(); //placement new
    }
    
    template <typename T1, typename T2>
    inline void _Construct(T1 *__p, const T2& __value) {
        new ((void *)__p) T1(__value);
    }
    
    template<typename T>
    inline void _Destory(T *__pointer) {
        __pointer->~T();
    }
    
    template <typename ForwardIterator>
    inline void __destory_aux(ForwardIterator, ForwardIterator, ministl::__true_type){}
    
    template <typename ForwardIterator>
    void __destory_aux(ForwardIterator __first, ForwardIterator __last, ministl::__false_type) {
        for(; __first != __last ; ++ __first)
            destory(&*__first);
    }
    
    template <typename ForwardIterator, typename T>
    inline void __destory(ForwardIterator __first, ForwardIterator __last, T*) {
        typedef typename ministl::__type_traits<T>::has_trivial_destructor Trivial_destructor;
        __destory_aux(__first, __last, Trivial_destructor());
    }
    
    template <typename ForwardIterator>
    inline void _Destory(ForwardIterator __first, ForwardIterator __last) {
        __destory(__first, __last, __VALUE_TYPE(__first)); //value_type : 是iterator_traits 萃取的内容
    }
    
    //接口
    template <typename T>
    inline void construct(T *__p) {
        _Construct(__p);
    }
    
    template <typename T1, typename T2>
    inline void construct(T1 *__p, const T2& __value) {
        _Construct(__p, __value);
    }
    
    template <typename T>
    inline void destory(T *__pointer) {
        _Destory(__pointer);
    }
    
    template <typename ForwardIterator>
    inline void destory(ForwardIterator __first, ForwardIterator __last) {
        _Destory(__first, __last);
    }
    
    //////////////////////////////////////////// uninitialized_copy
    template <typename InputIter, typename ForwardIter>
    inline ForwardIter
    __uninitialized_copy_aux(InputIter __first, InputIter __last, ForwardIter __result, ministl::__true_type) {
        return copy(__first, __last, __result); //POD 使用 系统 copy
    }
    
    template <typename InputIter, typename ForwardIter>
    inline ForwardIter
    __uninitialized_copy_aux(InputIter __first, InputIter __last, ForwardIter __result, ministl::__false_type) {
        ForwardIter cur = __result;
        for(;__first != __last; ++__first, ++cur) {
            construct(&*cur, *__first);
        }
        
        return cur;
    }
    
    template <typename InputIter, typename ForwardIter, typename T>
    inline ForwardIter
    __uninitialized_copy(InputIter __first, InputIter __last, ForwardIter __result, T *) {
        typedef typename ministl::__type_traits<T>::is_pod_type is_pod;
        return __uninitialized_copy_aux(__first, __last, __result, is_pod());
    }
    
    template <typename InputIterator, typename ForwardIterator>
    ForwardIterator uninitialized_copy(InputIterator __first, InputIterator __last, ForwardIterator __result) {
        return __uninitialized_copy(__first, __last, __result, __VALUE_TYPE(__first));
    }
    //////////////////////////////////////////// uninitialized_fill
    template <typename ForwardIter, typename T>
    inline void
    __uninitialized_fill_aux(ForwardIter __first, ForwardIter __last, const T& __x, ministl::__true_type) {
        fill(__first, __last, __x); //pod 使用 系统 fill
    }
    
    template <typename ForwardIter, typename T>
    inline void
    __uninitialized_fill_aux(ForwardIter __first, ForwardIter __last, const T& __x, ministl::__false_type) {
        ForwardIter cur = __first;
        for(;cur != __last; ++cur) {
            construct(&*cur, __x);
        }
        //没有加异常处理
    }
    
    template <typename ForwardIter, typename T, typename TP>
    inline void
    __uninitialized_fill(ForwardIter __first, ForwardIter __last, const T& __x, TP*) {
        typedef typename ministl::__type_traits<TP>::is_pod_type is_pod;
        __uninitialized_fill_aux(__first, __last, __x, is_pod());
    }
    
    template <typename ForwardIter, typename T>
    inline void uninitialized_fill(ForwardIter __first, ForwardIter __last, const T& __x) {
        __uninitialized_fill(__first, __last, __x, __VALUE_TYPE(__first));
    }
    //////////////////////////////////////////// uninitialized_fill_n
    template <typename ForwardIter, typename size, typename T>
    inline ForwardIter
    __uninitialized_fill_n_aux(ForwardIter __first, size n, const T& __x, ministl::__true_type) {
        return fill_n(__first, n, __x); //pod
    }
    template <typename ForwardIter, typename size, typename T>
    inline ForwardIter
    __uninitialized_fill_n_aux(ForwardIter __first, size n, const T& __x, ministl::__false_type) {
        ForwardIter cur = __first;
        
        while(n) {
            construct(&*cur, __x);
            --n;
            ++cur;
        }
        
        
        return cur;
    }
    
    template <typename ForwardIter, typename size, typename T, typename TP>
    inline ForwardIter
    __uninitialized_fill_n(ForwardIter __first, size n, const T& __x, TP*) {
        typedef typename ministl::__type_traits<TP>::is_pod_type is_pod;
        return __uninitialized_fill_n_aux(__first, n, __x, is_pod());
    }
    
    
    template <typename ForwardIter, typename size, typename T>
    inline ForwardIter
    uninitialized_fill_n(ForwardIter __first, size __n, const T& __x) {
        return __uninitialized_fill_n(__first, __n, __x, __VALUE_TYPE(__first));
    }
}

#endif /* construct_h */





















