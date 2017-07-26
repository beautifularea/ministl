//
//  stl_config.h
//  ministl
//
//  Created by zhTian on 2017/6/6.
//  Copyright © 2017年 zhTian. All rights reserved.
//

#ifndef stl_config_h
#define stl_config_h

#define __STL_TEMPLATE_NULL template<>

#define __VALUE_TYPE(__i) (ministl::__value_type(__i))
#define __DISTANCE_TYPE(i) (ministl::__distance_type(i))

#define __NAMESPACE__BEGIN namespace ministl {
#define __NAMESPACE__END   }

#endif /* stl_config_h */
