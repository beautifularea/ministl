//
//  type_traits.h
//  ministl
//
//  Created by zhTian on 2017/6/6.
//  Copyright © 2017年 zhTian. All rights reserved.
//

#ifndef type_traits_h
#define type_traits_h

#include "stl_config.h"

namespace ministl {
    struct __true_type {};
    struct __false_type {};
    
    template <typename T>
    struct __type_traits {
        typedef __true_type this_dummy_member_must_be_first;
        
        typedef __false_type has_trivial_destructor;
        typedef __false_type is_pod_type;
    };
    
    //specialization
    
    __STL_TEMPLATE_NULL
    struct __type_traits<bool> {
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_pod_type;
    };
    
    __STL_TEMPLATE_NULL
    struct __type_traits<char> {
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_pod_type;
    };
    
    //signed char
    
    //unsigned char
    
    //short
    
    //unsigned short
    
    //int
    
    //unsigned int
    
    //long
    
    //unsigned long
    
    //long long
    
    //unsigned long long
    
    //float
    
    //double
}

#endif /* type_traits_h */
