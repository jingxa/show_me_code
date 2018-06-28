// SGI STL <stl_iterator.h>

// 五中迭代器类型
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag:public input_iterator_tag{};
struct bidirectional_iterator_tag: public forward_iterator_tag{};
struct random_access_iterator_tag: public bidirectional_iterator_tag{};

// 继承 std::iterator
template<class category, class T, class Distance = ptrdiff_t,
	class Pointer = T*, class  Reference = T&>
	struct iterator{
		typedef Category 	iterator_category;
		typedef T 			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
	};

	
// traits
template<class Iterator >
struct iterator_traits{
	typedef typename	Iterator::iterator_category		iterator_category;
	typedef typename 	Iterator::value_type			value_type;
	typedef	typename	Iterator::difference_type		difference_type;
	typedef typename	Iterator::pointer				pointer;
	typedef typename	Iterator::reference				reference;
	
};

// 原生指针的偏特化
template<class T>
struct iterator_traits<T*>{
	typedef random_access_iterator_tag iterator_category;
	typedef T							value_type;
	typedef ptrdiff_t					difference_type;
	typedef T*							pointer;
	typedef T&							reference;
};

//原生指针 的pointer to const  的偏特化
template<class T>
struct iterator_traits<const T*>{
	typedef random_access_iterator_tag iterator_category;
	typedef T 							value_type;
	typedef ptrdiff_t					difference_type;
	typedef	const T* 					pointer;
	typedef const T&					reference;
};


// iterator_category 判断
template<class Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&){
	typedef typename iterator_traits<Iterator>::iterator_category category;
	return category();
}

// distance  判断
template<class Iterator>
inline typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&){
	return statatic_cast<typename iterator_traits<Iterator>::difference_type* >(0);	
}

// value type 判断
template<class Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&){
	return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}



// distance 函数: 举例了两种类型的迭代器
template<class InputIterator>
inline iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, input_iterator_tag){
	while(first != last){
		++first;
		++n;
	}
	return n;
}

template <class RandomAccessIterator>
inline iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag){
	return last - first;
}

template<class InputIterator>
inline iterator_traits<InputIterator>:difference_type
distance(InputIterator first , InputIterator last){
	typedef typename iterator_traits<InputIterator>::iterator_category category;
	return __distance(first, last, category());
}


// advance 函数
template<class InputIterator, class Distance>
inline void __advance(InputIterator& i, Distance n, input_iterator_tag){
	while(n--) ++i;
}

template<class BidirectionalIterator , class Distance>
inline void __advance(BidirectionalIterator& i , Distance n, bidirectional_iterator_tag){
	if(n>=0)
		while(n--) ++i;
	else
		while(n++) --i;
}


template < class RandomAccessIterator, class Distance>
inline void __advance(RandomAccessIterator& i , Distance n, random_access_iterator_tag){
	i += n;
}

template<class InputIterator, class Distance>
inline void advance(InputIterator& i , Distance n){

	return __advance(i, n, iterator_category(i));
}








/**
	SGI 内部 
	iterator_traits: 萃取迭代器的特性
	__type_traits : 萃取 型别(type)的特性
	__type_traits<T>::has_trivial_deefault_constructor  具有无用默认构造函数
	__type_traits<T>::has_trivial_copy_constructor 		具有无用复制构造函数
	__type_traits<T>::has_trivial_assignment_operator	具有无用赋值符
	__type_traits<T>::is_POD_type   					plain old Date
	返回值：
	struct __true_type{};
	struct __false_type();
*/


struct __true_type{};
struct __false_type();



/* SGI 首先定义出最报上的值，然后对每一个标量设计一个特化版本*/
template<class type>
struct __type_traits{
	typedef __true_type 		this_dummy_member_must_be_first; /*通知编译器将__type_traits 是特殊的*/
	typedef __false_type 		has_trivial_deefault_constructor;
	typedef __false_type		has_trivial_copy_constructor;
	typedef __false_type		has_trivial_assignment_operator;
	typedef	__false_type		has_trivial_destructor;
	typedef	__false_type		is_POD_type; 

};

/**
	c++基本类系列：char, 			signed char, 		unsigned char, 		short, 
				   unsigned short	int , 				unsigned int ,      long,
				   unsigned long,	float, 				double, 			long double
	每个都提供特化版本，每一个成员的值都是__true_type,
	表示这些型别都可以采用最快速方式(如memcpy)来拷贝或者赋值；
*/


// SGI STL <stl_config.h>将 __STL_TEMPLATE_NULL定义为template<>

__STL_TEMPLATE_NULL struct __type_traits<char>{
	
	typedef __true_type 		has_trivial_deefault_constructor;
	typedef __true_type		has_trivial_copy_constructor;
	typedef __true_type		has_trivial_assignment_operator;
	typedef	__true_type		has_trivial_destructor;
	typedef	__true_type		is_POD_type; 
	
};


// 原生指针的偏特化
template<class T>
struct __type_traits<T*>{
	typedef __true_type 		has_trivial_deefault_constructor;
	typedef __true_type		has_trivial_copy_constructor;
	typedef __true_type		has_trivial_assignment_operator;
	typedef	__true_type		has_trivial_destructor;
	typedef	__true_type		is_POD_type; 
};


// __type_traits 的应用



/*    函数参数为原生指针的情况  */
// copy函数

template<class T>
inline void copy(T* source, T* destination, int n){
	copy(source ,destination,n,typename __type_traits<T>::has_trivial_copy_constructor());
}



// 拷贝一个数组，类型有non-trivial copy 构造函数
template<class T>
void copy(T* source ,T* destination, int n, __false_type){
	// ...
}


// 拷贝一个数组，类型有trivial copy 构造函数
template<class T>
void copy(T* source, T* destination, int n, __true_type){
	//...
}

























