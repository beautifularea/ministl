//
//  stl_iterator.h
//  ministl
//
//  Created by zhTian on 2017/6/7.
//  Copyright © 2017年 zhTian. All rights reserved.
//

#ifndef stl_iterator_h
#define stl_iterator_h

#include "stl_config.h"

__NAMESPACE__BEGIN

template <typename InputIterator>
inline typename ministl::iterator_traits<InputIterator>::difference_type
__distance(InputIterator __first, InputIterator __last, input_iterator_tag) {
    typename ministl::iterator_traits<InputIterator>::difference_type n = 0;
    while(__first != __last) {
        ++__first;
        ++n;
    }
    return n;
}

template <typename RandomAccessIterator>
inline typename ministl::iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator __first, RandomAccessIterator __last, random_access_iterator_tag) {
    return __last - __first;
}

template <typename InputIter>
inline typename ministl::iterator_traits<InputIter>::difference_type
distance(InputIter __first, InputIter __last) {
    typedef typename ministl::iterator_traits<InputIter>::iterator_category Category;
    return __distance(__first, __last, Category());
}

/////////////////////////// custom iterator ++ -- 重载
template <typename Iterator>
class custom_iterator {
public:
    typedef typename ministl::iterator_traits<Iterator>::iterator_category category;
    typedef typename ministl::iterator_traits<Iterator>::value_type value_type;
    
public:
    
    explicit custom_iterator() {};
    
public:
    custom_iterator<Iterator>& operator++() {
        //todo...
        return *this;
    }
    custom_iterator<Iterator> operator++(int) {
        custom_iterator<Iterator> t = *this;
        //todo...
        return t;
    }
    custom_iterator<Iterator>& operator--() {
        //todo
        return *this;
    }
    custom_iterator<Iterator> operator--(int) {
        custom_iterator<Iterator>  t = *this;
        //todo...
        return t;
    }
};


__NAMESPACE__END

#endif /* stl_iterator_h */
