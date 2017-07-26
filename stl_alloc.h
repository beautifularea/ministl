//
//  stl_alloc.h
//  ministl
//
//  Created by zhTian on 2017/6/7.
//  Copyright © 2017年 zhTian. All rights reserved.
//

#ifndef stl_alloc_h
#define stl_alloc_h

namespace {

    //第一级配置器开始
    template <int inst>
    class malloc_alloc_template {
    private:
        
        /*
         oom : out of memory ，用来处理内存不足的情况函数
         */
        static void *_s_oom_malloc(size_t __n) {
            void (* malloc_handler)();
            void *result;
            
            while(1) {
                malloc_handler = malloc_alloc_oom_handler;
                if(malloc_handler == 0) {
                    throw "bad alloc.";
                }
                (* malloc_handler)(); //调用
                result = malloc(__n);
                if(result) return result;
            }
        }
        static void *_s_oom_realloc(void *__p, size_t __n) {
            void (* realloc_handler)();
            void *result;
            
            while(1) {
                realloc_handler = malloc_alloc_oom_handler;
                if(realloc_handler == 0) {
                    throw "bad realloc.";
                }
                (*realloc_handler)();
                result = realloc(__p, __n);
                if(result) return result;
            }
        }
        //不能直接使用c++的new-handler机制，因为没有使用::operator new 来配置的内存，
        //而是，malloc，free
        static void (*malloc_alloc_oom_handler)(); //函数指针
        
    public:
        static void *allocate(size_t __n) {
            void *result = (void *)malloc(__n);
            if(0 == result) result = _s_oom_malloc(__n);
            return result;
        }
        static void *deallocate(void *__p, size_t) {
            free(__p);
        }
        static void *reallocate(void *__p, size_t, size_t __new_sz) {
            void *result = realloc(__p, __new_sz);
            if(0 == result) result = _s_oom_realloc(__p, __new_sz);
            return result;
        }
        
        //设置 out of memory handler
        static void (* __set_malloc_handler( void (*f)() ))() {
            void (* old)() = malloc_alloc_oom_handler;
            malloc_alloc_oom_handler = f;
            return old;
        }
    };
    typedef malloc_alloc_template<0> malloc_alloc;
    //第一级配置器完成

    //stl 标准的接口
    //简单的封装了第一和第二阶段的接口调用
    template <typename T, typename Alloc>
    class simple_alloc {
    public:
        static T *allocate(size_t __n) {
            if(__n > 0) {
                return (T *)Alloc::allocate(__n * sizeof( T ));
            }
            return 0;
        }
        static T *deallocate(T *__p, size_t __n) {
            if(__n > 0) {
                return (T *)Alloc::deallcate(__p, __n * sizeof(T));
            }
            return 0;
        }
        static void deallocate(T *__p) {
            Alloc::deallocate(__p, sizeof(T));
        }
        //没有reallocate的接口？？？
        static void *reallocate(T *__p, size_t, size_t __new_sz) {
            if(__new_sz > 0) {
                return (T *)Alloc::reallocate(__p, 0, __new_sz);
            }
            return 0;
        }
    };
    
    //默认使用第一配置器
#ifdef USE_MALLOC
    typedef malloc_alloc alloc;
#else
    
    //第二配置器
    /*
     小于等于128Bytes,使用第二配置器,大于则使用第一配置器
     维护16个free lists
     如果 memory pool不足，调用第一配置器
     8/16/24/32/40/48/56/64/72/80/88/96/104/112/120/128
     */

    template <int inst>
    class second_alloc_template {
    public:
        static void *allocate(size_t __n) {
            void *ret = 0;
            //大于128，使用malloc_alloc第一配置器
            if(__n > MAX_BYTES) {
                ret = malloc_alloc::allocate(__n);
            }
            else {
                obj ** target_free_list = _free_list + freelist_index(__n);
                obj *result;
                result = *target_free_list;
                if(result == 0) {
                    void *r = refill(round_up(__n));
                    return r;
                }
                else {
                    *target_free_list = result->_M_next;
                    ret = result;
                }
            }
            
            return ret;
        }
        static void deallocate(void *__p, size_t __n) {
            if(__n > MAX_BYTES) {
                malloc_alloc::deallocate(__p, __n);
                return;
            }
            else {
                obj ** target_free_list = _free_list + freelist_index(__n);
                obj *q = (obj *)__p;
                
                q->_M_next = *target_free_list;
                *target_free_list = q;
            }
        }
        
        
    private:
        enum {
            ALIGN = 8 //小区块上调
        };
        enum {
            MAX_BYTES = 128 //最大值
        };
        enum {
            PRELISTS = 16 //free lists个数
        };

        static size_t round_up(size_t __bytes) {
            return ( __bytes + ( ALIGN - 1 ) ) & ~(ALIGN - 1);
        }
        
        //free list node
        union obj {
            union obj *_M_next;
            char _M_data[1];
        };
        static obj * _free_list[PRELISTS];
        
        //bytes大小决定用哪个freelist 从0-15
        static size_t freelist_index(size_t __bytes) {
//            return (__bytes + ALIGN - 1) / (ALIGN  - 1);
            return __bytes / ALIGN;
        }
        static void *refill(size_t __n) {
            int nobjs = 20;
            char *chunk = chunk_alloc(__n, nobjs);
            obj **my_free_list;
            obj *result;
            obj *current_obj;
            obj *next_obj;
            int i;
            
            if(nobjs == 1) return chunk;
            my_free_list = _free_list + freelist_index(__n);
            
            //build free list
            result = (obj *)chunk;
            *my_free_list = next_obj = (obj *)(chunk + __n);
            for(i=1;;++i) {
                current_obj = next_obj;
                next_obj = (obj *)((char *)next_obj + __n);
                if(nobjs - 1 == i) {
                    current_obj -> _M_next = 0;
                    break;
                }
                else {
                    current_obj->_M_next = next_obj;
                }

            }
            
            return result;
        }
        
        static char *chunk_alloc(size_t __s, int& objs);
    
        static char *chunk_start;
        static char *chunk_end;
        static size_t heap_size;
        
        static void *reallocate(void *__p, size_t __old_sz, size_t __new_sz);
    };
    typedef second_alloc_template<0> alloc;
    
    //初始化成员变量
    template <int inst>
    char *second_alloc_template<inst>::chunk_end = 0;
    template <int inst>
    char *second_alloc_template<inst>::chunk_start = 0;
    template <int inst>
    size_t second_alloc_template<inst>::heap_size = 0;
    
    template <int inst>
    char *
    second_alloc_template<inst>::chunk_alloc(size_t __s, int &objs) {
        char *result;
        size_t total_bytes = __s * objs;
        size_t bytes_left = chunk_end - chunk_start;
        if(bytes_left >= total_bytes) {
            result = chunk_start;
            chunk_start += total_bytes;
            return result;
        }
        else if(bytes_left >= __s) {
            objs = (int)bytes_left/__s;
            total_bytes = __s * objs;
            result = chunk_start;
            chunk_start += total_bytes;
            return result;
        }
        else {
            //2倍于需求的大小
            size_t bytes_to_get = 2 * total_bytes + round_up(heap_size>>4);
            if(bytes_left > 0) {
                obj ** my_free_list = _free_list + freelist_index(bytes_left);
                ((obj *)chunk_start) -> _M_next = *my_free_list;
                *my_free_list = (obj *)chunk_start;
            }
            chunk_start  = (char *)malloc(bytes_to_get);
            if(chunk_start == 0) {
                size_t i;
                obj ** myfreelist;
                obj *__p;
                for(i=__s;
                    i<=(int)MAX_BYTES;
                    i += (size_t)ALIGN) {
                    myfreelist = _free_list + freelist_index(i);
                    __p = *myfreelist;
                    if(__p != 0) {
                        *myfreelist = __p->_M_next;
                        chunk_start = (char *)__p;
                        chunk_end = chunk_start + i;
                        return chunk_alloc(__s, objs);
                    }
                }
                
                //还有意外，就调用malloc_alloc 的 oom 机制，找下内存
                chunk_end = 0;
                chunk_start = (char *)malloc_alloc::allocate(bytes_to_get);
            }
            
            heap_size += bytes_to_get;
            chunk_end = chunk_start + bytes_to_get;
            return chunk_alloc(__s, objs);
        }
    }
#endif //使用第二配置器结束 USE_MALLOC
    
} //end namespace
#endif /* stl_alloc_h */
























