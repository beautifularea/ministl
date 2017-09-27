//
//  stl_list.h
//  ministl
//
//  Created by zhTian on 2017/6/27.
//  Copyright © 2017年 zhTian. All rights reserved.
//

#ifndef stl_list_h
#define stl_list_h

//待实现 6.27
struct _List_node_base
{
	_List_node_base* _M_next;
	_List_node_base* _M_prev;
};

template <typename T>
struct _List_node : public _List_node_base
{
	T _M_data;
};

/** 迭代器基类  */
struct _List_iterator_base
{
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef bidirectional_iterator_tag iterator_category;

	_List_node_base* _M_node;

	_List_iterator_base(_List_node_base* _x) : _M_node(_x){}
	_List_iterator_base(){}

	void _M_incr()
	{
		_M_node = _M_node->_M_next;
	}
	void _M_decr()
	{
		_M_node = _M_node->_M_prev;
	}

	bool operator=(const _List_iterator_base& _x) const
	{
		return _M_node == _x._M_node;
	}

	bool operator!=(const _List_iterator_base& _x) const
	{
		return _M_node != _x._M_node;
	}
};

template <typename T, typename Ref, typename _Ptr>
struct _List_iterator : public _List_iterator_base
{
	typedef _List_iterator<T, T&, T*> iterator;
	typedef _List_iterator<T, const T&, const T*> const_iterator;
	typedef _List_iterator<T, Ref, _Ptr> _Self;

	typedef T value_type;
	typedef _Ptr pointer;
	typedef Ref reference;
	typedef _List_node<T> _Node;

	_List_iterator(_Node* _x) : _List_iterator_base(_x){}
	_List_iterator(){}
	_List_iterator(const iterator& _x) : _List_iterator_base(_x._M_node){}

	//取值运算符
	reference operator*() const
	{
		return ((_Node*)_M_node)->_M_data;
	}

	_Self& operator++()
	{
		this->_M_incr();
		return *this;
	}
	_Self operator++(int) //后缀运算符 带 int 参数
	{
		_Self __tmp = *this;
		this->_M_incr();
		return __tmp;
	}
	_Self& operator--()
	{
		this->_M_decr();
		return *this;
	}
	_Self& operator--(int)
	{
		_Self __tmp = *this;
		this->_M_decr();
		return __tmp;
	}
};

/** Use STD Allocators */
template <typename T, typename Alloc>
class _List_base
{
public:
	typedef Alloc allocator_type;
	allocator_type get_allocator() const { return allocator_type(); }

	_List_base(const allocator_type&)
	{
		_M_node = _M_get_node();
		_M_node->_M_next = _M_node;
		_M_node->_M_prev = _M_node;
	}

	~_List_base()
	{
		clear();
		_M_put_node(_M_node);
	}

	void clear();

protected:
	typedef simple_alloc<_List_node<T>, Alloc> Alloc_type;

	_List_node<T>* _M_get_node()
	{
		return Alloc_type::allocate(1);
	}

	void _M_put_node(_List_node<T>* __p)
	{
		Alloc_type::deallocate(__p, 1);
	}

private:
	_List_node<T>* _M_node;
};

template <typename T, class Alloc>
void
_List_base<T, Alloc>::clear()
{
	_List_node<T>* __cur = (_List_node<T>*)_M_node->_M_next;
	while(__cur != _M_node)
	{
		_List_node<T>* __tmp = __cur;

		__cur = (_List_node<T>*)__cur->_M_next;
		_Destroy(&__tmp->_M_data);
		_M_put_node(__tmp);
	}

	//结束条件
	_M_node->_M_next = _M_node;
	_M_node->_M_prev = _M_node;
}

template <typename T, typename Alloc = alloc>
class list : protected _List_base<T, Alloc>
{
	typedef _List_base<T, Alloc> Base;

protected:
	typedef void* Void_pointer;

public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef _List_node<T> _Node;

	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef typename Base::allocator_type allocator_type;
	allocator_type get_allocator() const
	{
		return Base::get_allocator();
	}

public:
	typedef _List_iterator<T, T&, T*> iterator;
	typedef _List_iterator<T, const T&, const T*> const_iterator;

protected:
	_Node* _M_create_node(const T& __x)
	{
		_Node* __p = _M_get_node();
		_Construct(&__p->_M_data, __x);
		return __p
	}
	_Node* _M_create_node()
	{
		_Node* __p = _M_get_node();
		_Construct(&__p->_M_node);
		return __p;
	}

public:
	/** 构造函数 */
	explicit list(const allocator_type& __a=allocator_type()) : Base(__a){}

	//这个很不解啊，为啥返回类型都不一样？？？
	/**
	Update: 2017-9-27
	Author: beautifularea
	Reason: 不理解就对了，你还是没有搞清楚内存布局而已......
	Explaination: 首先 _Node 类型是_List_node<_Tp>, _List_node_base是它的基类, 而iterator 类型为 _List_iterator<_Tp,_Tp&,_Tp*> , 而它的基类是
	_List_iterator_base ，而根据class/struct 在C++中的内存模型可知道，iteraotr的第一个且只有一个成员变量是_List_node_base* _M_node，即类型是 _List_node_base 
	so, iterator的首指针跟_Node的类型一致，所以，可以类型转换！！！
	
	My God 呀， 小小的一个类型转换涉及到这么多知识，还能不能好好编程了？！
	//所以，这样可以理解了吧，一周前的自己！
	*/
	iterator begin()
	{
		return (_Node*)(_M_node->_M_next);
	}
	iterator end() 
	{
		return (_M_node);
	}
	bool empty() const
	{
		return _M_node->_M_next == _M_node;
	}
	size_type size() const
	{
		size_type __result = 0;
		distance(begin(), end(), __result);
		return __result;
	}
	size_type max_size() const
	{
		return size_type(-1);
	}
	reference front()
	{
		return *begin();
	}
	reference back()
	{
		return *(--end());
	}

	iterator insert(iterator __position, const T& __x)
	{
		_Node* __tmp = _M_create_node(__x);

		__tmp->_M_next = __position._M_node;
		__tmp->_M_prev = __position._M_node->_M_prev;

		__position._M_node->_M_prev->_M_next = __tmp;
		__position._M_node->_M_prev = __tmp;

		return __tmp;
	}
}

#endif /* stl_list_h */
