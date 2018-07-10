/**

	基本算法
	sgi的算法定义：
	1，一些常用算法定义 <stl_algobase.h>
	2. 其他算法定义<stl_algo.h>
*/
/* equal ：两个序列区间是否相等，第二个序列元素较多，多出的不考虑*/

// 在比较之前，应该先比较两个序列的元素个数，
// 如果第二序列元素比第一序列元素个数少，越界

template<class InputIterator1, class InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1,
					InputIterator2 first2){
	// 	序列一过一遍
	for(;first1!= last1; ++first1, ++first2){
		if(*first1 != *first2)
			return false;
	}
	return true;
}

template<class InputIterator1, class InputIterator2, class BinaryPredicate>
inline bool equal(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, BinaryPredicate binary_pred){
	for(; first1 != last1; ++first1, ++first2){
		if(!binary_pred(*first1, *first2)
			return false;
		
	}
	return true;
}


// fill  [first, last)内的元素填写新值
template<class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last,const T& value){
	for(;first != last; ++first)
		*first = value;
}

/* fill_n : 前 n 个元素填写新值 ，返回迭代器指向最后一个元素的下一个位置*/
//fill_n是一种赋值操作，是覆写操作， 如果n大于边界，造成不可预期结果
// 利用inserter() 插入器，
// vector<int>iv = {0,1,2}
// fill_n(inserter(iv, iv.begin()), 5,7) // 7 7 7 7 7 0 1 2

template< class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value){
	for(; n>0; --n, ++first){
		*first = value;
	}
	return first;
}


/* iter_swap : 两个 ForwardIterator 所指向的对象对调*/
template<class ForwardIterator1, class ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b){
	__iter_swap(a,b, value_type(a)); // 注意第三项的型别
}
template<class ForwardIterator1, class ForwardIterator2, T*>
inline void __iter_swap(ForwardIterator1 a, ForwardIterator2 b, T*){
	// T: typename iterator_traits<ForwardIterator1>::value_type
	T tmp = *a ;
	*a = *b;
	*b = tmp;
}





/* lexicographical_compare : “字典排列方式”对两个序列
	[first1, last1)和 [first2, last2)进行比较

	1. 某一组对应元素彼此不相等
	2. 同时达到last1和last2（序列大小相同）
	3. 到达last1或last2(大小不相同)
		
*/

// 版本1
template< class InputIterator1 ,class InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
							InputIterator2 first2, InputIterator2 last2){
	for(; first!=last1 && first2 != last2; ++first1, ++first2){
		if(*first1 <*first2) // 第一序列小于第二序列
			return true;
		if(*first1 > *first2)
			return false;
	}
	// 到达至少一个序列的尾端
	// 如果序列一达到尾端，序列二有元素，
	return first1 == last1 && first2 != last2;
}



//版本2
template< class InputIterator1 ,class InputIterator2, class Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
							InputIterator2 first2, InputIterator2 last2,Compare comp){
	for(; first!=last1 && first2 != last2; ++first1, ++first2){
		if(comp(*first1, *first2)) // 第一序列小于第二序列
			return true;
		if(comp(*first2, *first1))
			return false;
	}
	// 到达至少一个序列的尾端
	// 如果序列一达到尾端，序列二有元素，
	return first1 == last1 && first2 != last2;
}


// const char * 特化版本

inline bool lexicographical_compare(const unsigned char* first1,
									const unsigned char* last1，
									const unsigned char* first2,
									const unsigned char* last2)
{
	const size_t len1= last1 - first1;
	const size_t len2 = last2 - first2;
	const int result = memcmp(first1, first2, min(len1,len2));
	return result !=0 ? result < 0: len1 < len2;;
}




// max
template< class T>
inline const T& max(const T& a, const T& b){
	return a<b?b:a;
}


template< class T, class Compare>
inline const T& max(const T& a, const T& b, Compare comp){
	return comp(a,b)?b:a;
}


// min

template< class T>
inline const T& min(const T& a, const T& b){
	return b<a?b:a;
}


template< class T, class Compare>
inline const T& min(const T& a, const T& b, Compare comp){
	return comp(b,a)?b:a;
}


// mismatch
// 用来平行比较两个序列，指出两个之间的第一个不匹配点，
// 返回一对跌电器，分别指向两个序列中的不匹配点
template<class InputIterator1, class InputIterator2>
pair<InputIterator1,InputIterator2> mismatch(InputIterator1 first1,
			InputIterator1 last1, InputIterator2 first2){
	while(first1 != last1 && *first1 == *first2){
		++first1;
		++first2;
	}
	return pair<InputIterator1, InputIterator2> (first1,first2);
}




template<class InputIterator1, class InputIterator2, class BinaryPredicate>
pair<InputIterator1,InputIterator2> mismatch(InputIterator1 first1,
			InputIterator1 last1, InputIterator2 first2,BinaryPredicate binary_pred){
	while(first1 != last1 &&binary_pred(*first1, *first2)){
		++first1;
		++first2;
	}
	return pair<InputIterator1, InputIterator2> (first1,first2);
}



// swap 

template< class T>
inline void swap(T& a, T& b){
	T tmp = a; 
	a= b;
	b = tmp;
}


/**
	copy  实现
  
*/

// 完全泛化版本
template< class InputIterator, class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last,
						OutputIterator result){
	return __copy_dispatch<InputIterator,OutputIterator>()(first,last,resut);						
}


// 特殊版本1
inline char* copy(const char* first, const char* last, char* result){
	memmove(result, first,last - first);
	return result + (last - first);
}


// 特殊版本2
inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result){
	memmove(result, first, sizeof(wchar_t) * (last - first));
	return result + (last - first);
}


// 完全泛化版本
template<class InputIterator, class OutputIterator>
struct __copy_dispatch{
	OutputIterator operator()(InputIterator first,InputIterator last,
					OutputIterator result){
		return __copy(first,last, result, iterator_category(first));				
	}
};


// 特化版本1:两参数都是T*
template<class T>
struct __copy_dispatch<T*, T*>{
	T* operator()(T* first,T* last, T* result){
		typedef typename __type_traits<T>::has_trivial_aasignment_operator t;
		return __copy_t(first, last,result,t());
	}
};

// 特化版本2:一个为const T*,一个为T*

template<class T>
struct __copy_dispatch<const T*, T*>{
	T* operator()(const T* first,const T* last, T* result){
		typedef typename __type_traits<T>::has_trivial_aasignment_operator t;
		return __copy_t(first, last,result,t());
	}
};



// InputIterator 版本
template<class InputIterator, class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last,
							OutputIterator result, input_iterator_tag){
	// 以迭代器等同判断，决定循环，速度慢
	for(;first != last; ++result, ++first){
		*result = *first;  // 赋值符
	}
	return result;
}

// RandomAccessIterator 版本
template<class RandomAccessIterator , class OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last,
							OutputIterator result, random_access_iterator_tag){
	// 划分一个新函数，
	return __copy_d(first,last,result,distance_type(first));
}



template<class RandomAccessIterator, class OutputIterator,class Distance>
inline OutputIterator __copy_d(RandomAccessIterator first,
		RandomAccessIterator last , OutputIterator result, Distance*){
	// 以n决定循环次数， 速度快
	for(Distance n = last - first; n>0; --n, ++result, ++first){
		*result = *first;
		
	}
	return result;
}


// 指针所指向的对象具备"trivial assignment operator"
template<class T>
inline T* __copy_t(const T* first, const T* last, T* result,__true_type){
	memmove(result, first, sizeof(T)*(last - first));
	return result + (last - first);
}


// 指针所指向的对象具备“non-trivial assignment operator”
template<class T>
inline T* __copy_t(const T* first, const T* last, T* result, __false_type){
	// 原生指针是一种 RandomAccessIterator , 所以交给 __copy_d()完成
	return __copy_d(first, last, result,(ptrdiff_t*)0);
}







/**

	copy_backward
	1. 逆行的方向复制，从后往前复制
*/





















