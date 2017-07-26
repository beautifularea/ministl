//
//  iterator_traits.h
//  ministl
//
//  Created by zhTian on 2017/6/7.
//  Copyright © 2017年 zhTian. All rights reserved.
//

#ifndef iterator_traits_h
#define iterator_traits_h

namespace ministl {
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};
    
    template <typename T, typename Distance>
    struct input_iterator {
        typedef input_iterator_tag iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef T* pointer;
        typedef T& reference;
    };
    
    struct out_iterator {
        typedef output_iterator_tag iterator_category;
        typedef void value_type;
        typedef void difference_type;
        typedef void pointer;
        typedef void reference;
    };
    
    template <typename T, typename Distance>
    struct forward_iterator {
        typedef forward_iterator_tag iterator_category;
        typedef T value_type;
    };
    
    template <typename T, typename Distance>
    struct bidirectional_iterator {
        typedef bidirectional_iterator_tag iterator_category;
        typedef T value_type;
    };
    
    template <typename T, typename Distance>
    struct random_access_iterator {
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
    };
    
    //iterator_traits
    template <typename Category, typename T, typename Distance=ptrdiff_t,
        typename Pointer=T *, typename Reference = T&>
    struct iterator {
        typedef Category iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef Pointer pointer;
        typedef Reference reference;
    };
    template <typename Iterator>
    struct iterator_traits {
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::difference_type difference_type;
    };
    
    //function
    template <typename Iter>
    inline typename iterator_traits<Iter>::value_type*
    __value_type(const Iter&) {
        return static_cast<typename iterator_traits<Iter>::value_type*>(0);
    }
    
    template <typename Iter>
    inline typename iterator_traits<Iter>::difference_type*
    __distance_type(const Iter&) {
        return static_cast<typename iterator_traits<Iter>::difference_type *>(0);
    }

    
    
    
    
    
}

#endif /* iterator_traits_h */
