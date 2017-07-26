//
//  stl_priorityqueue.h
//  ministl
//
//  Created by zhTian on 2017/6/9.
//  Copyright © 2017年 zhTian. All rights reserved.
//

#ifndef stl_priorityqueue_h
#define stl_priorityqueue_h

#include "stl_config.h"

/*
    binary heap 是一种 完全二叉树(complete binary tree)
 特性：
 lvector来存储节点：保存#0(级值，最大或者最小)，之后对于每个节点index有：
 左子节点：2 * index
 右子节点：2 * index + 1
 
 max-heap :
 每个节点的key 【大于等于】子节点 key
 */
__NAMESPACE__BEGIN

//上溯算法 : holeindex 初始为最后加入的value的index
template <typename RandomAccessIter, typename Distance, typename T>
void
__push_heap(RandomAccessIter first, Distance holeindex, Distance topindex, T value) {
    Distance p = (holeindex - 1)/2;
    while(holeindex > topindex && *(first+p) < value) {
        *(first+holeindex) = *(first+p);
        holeindex = p;
        p = (holeindex-1)/2;
    }
    *(first+holeindex) = value;
}

template <typename RandomAccessIter, typename Distance, typename T>
void
__push_heap_aux(RandomAccessIter first, RandomAccessIter last, Distance*, T*) {
    __push_heap(first, Distance(last - first - 1), Distance(0), T(*(last-1)));
}

template <typename RandomAccessIter>
inline void
push_heap(RandomAccessIter  first, RandomAccessIter last) {
    __push_heap_aux(first, last, __DISTANCE_TYPE(first), __VALUE_TYPE(first));
}

template <typename RandomAccessIterator, typename Distance, typename T>
void __adjust_heap(RandomAccessIterator first, Distance holeindex, Distance len, T value) {
    Distance topindex = holeindex;
    Distance secondchild = 2 * holeindex + 2;
    
    while(secondchild < len) {
        if(*(first + secondchild) < *(first + (secondchild - 1))) {
            secondchild --;
        }
        *(first+holeindex) = *(first + secondchild);
        holeindex = secondchild;
        secondchild = 2 * (secondchild + 1);
    }
    
    if(secondchild == len) {
        *(first+holeindex) = *(first+ (secondchild-1));
        holeindex = secondchild - 1;
    }
    
    __push_heap(first, holeindex, topindex, value);
}


template <typename RandomAccessIterator, typename Distance, typename T>
void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance *) {
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last-first), value);
}

template <typename RandomAccessIter, typename T>
inline void __pop_aux(RandomAccessIter first, RandomAccessIter last, T *) {
    __pop_heap(first, last-1, last-1, T(*(last-1)), __DISTANCE_TYPE(first));
}

template <typename RandomAccessIter>
inline void
pop(RandomAccessIter first, RandomAccessIter last) {
    __pop_aux(first, last, __VALUE_TYPE(first));
}

template <typename RandomAccessIter>
void
sort_heap(RandomAccessIter first, RandomAccessIter last) {
    while(last - first > 1) {
        pop(first, last--);
    }
}


/*
 priority queue :
  底端加入元素，顶端取出元素。
 vector和heap算法实现
 */
template <typename T, typename Sequence=std::vector<T>, typename Compare=std::less<typename Sequence::value_type>>
class priority_queue {
public:
    typedef typename Sequence::value_type value_type;
protected:
    Sequence c;
    Compare comp;
    
public:
    priority_queue() : c() {
        
    }
    explicit priority_queue(const Compare& x) : c() , comp(x) {}
    
    bool push(const value_type& x) {
        c.push_back(x);
        push_heap(c.begin(), c.end(), comp);
    }
    void pop() {
        pop(c.begin(), c.end());
        c.pop_back();
    }
};






























__NAMESPACE__END

#endif /* stl_priorityqueue_h */
