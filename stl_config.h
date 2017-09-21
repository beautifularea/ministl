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

//  本配置文件功能表:  
//  (1)  如果不编译器没有定义bool, true, false则定义  
//  (2)  如果编译器不支持drand48()函数则定义__STL_NO_DRAND48  
//       注: drand48产生双精度的伪随机数, 因为采用了48bit计算, 故名drand48  
//  (3)  如果编译器不支持static members of template classes(模板类静态成员),  
//       则定义__STL_STATIC_TEMPLATE_MEMBER_BUG  
//  (4)  如果编译器不支持'typename'关键字, 则将'typename'定义为空(null macro)  
//  (5)  如果编译器支持partial specialization of class templates(模板类偏特化),  
//       则定义__STL_CLASS_PARTIAL_SPECIALIZATION  
//       参考文献: http://msdn.microsoft.com/en-us/library/9w7t3kf1(v=VS.71).aspx  
//  (6)  如果编译器支持partial ordering of function templates(模板函数特化优先级),  
//       则定义__STL_FUNCTION_TMPL_PARTIAL_ORDER  
//       参考资料: http://msdn.microsoft.com/zh-cn/library/zaycz069.aspx  
//  (7)  如果编译器支持calling a function template by providing its template  
//       arguments explicitly(显式指定调用模板函数的模板参数)  
//       则定义__STL_EXPLICIT_FUNCTION_TMPL_ARGS  
//  (8)  如果编译器支持template members of classes(类模板成员),  
//       则定义__STL_MEMBER_TEMPLATES  
//  (9)  如果编译器不支持'explicit'关键字, 则将'explicit'定义为空(null macro)  
//  (10) 如果编译器不能根据前一个模板参数设定后面的默认模板参数,  
//       则定义__STL_LIMITED_DEFAULT_TEMPLATES  
//  (11) 如果编译器处理模板函数的non-type模板参数类型推断有困难,  
//       则定义__STL_NON_TYPE_TMPL_PARAM_BUG  
//  (12) 如果编译器不支持迭代器使用'->'操作符,  
//       则定义__SGI_STL_NO_ARROW_OPERATOR  
//  (13) 如果编译器(在当前编译模式下)支持异常,  
//       则定义__STL_USE_EXCEPTIONS  
//  (14) 如果我们将STL放进命名空间中,  
//       则定义__STL_USE_NAMESPACES  
//  (15) 如果本STL在SGI的编译器上编译, 并且用户没有选择pthreads或者no threads,  
//       则默认使用__STL_SGI_THREADS  
//       注: POSIX thread 简称为pthread, Posix线程是一个POSIX标准线程.  
//  (16) 如果本STL在Win32平台的编译器上使用多线程模式编译,  
//       则定义__STL_WIN32THREADS  
//  (17) 适当的定义命名空间相关的宏(__STD, __STL_BEGIN_NAMESPACE, 等)  
//  (18) 适当的定义异常相关的宏(__STL_TRY, __STL_UNWIND, 等)  
//  (19) 根据是否定义__STL_ASSERTIONS, 将__stl_assert定义为断言或者空(null macro)  

#endif /* stl_config_h */
