/**
	数值算法：
	1. 头文件：<numeric>

*/

// accumulate: 数字统计算法

//版本1
template<class InputIterator , class T>
T accumulate<InputIterator first, InputIterator last ,T init){
	for(;first != last; ++first)
		init = init + *first;  // 将每个元素累加到init身上
	return init;
}

// 版本2
template<class InputIterator, class T, class BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, 
				BinaryOperation binary_op){
	for(;first != last; ++first)
		init = binary_op(init, *first);  // 对每一个元素执行二元操作
	return init;
}



/* adjacent_difference : 相邻元素的差额*/
// 版本1
template<class InputIterator, class OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
									OutputIterator result){
	if(first == last) return result;
	*result = * first;  // 首先记录第一个元素
	return __adjacent_difference(first,last, result, value_type(first));
	// 不用传递调用，可直接改写为：
	// if(first == last) return result;
	// *result = *first;
	// iterator_traits<InputIterator>::value_type value = *first;
	// while(++first != last){
	//	... 一下对应于 __adjacent_difference的内部
	//}
}

template<class InputIterator, class OutputIterator, class T>
OutputIterator __adjacent_difference(InputIterator first, InputIterator last,
									OutputIterator result ,T*){
	T value = *first;
	while(++first != last){
		T tmp = *first;
		*++result = tmp - value; // 将相邻量元素的差异， 赋值给目的端
	}
	return ++result;
}

// 版本2
template<class InputIterator , class OutputIterator, class BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
								OutputIterator result BinaryOperation binary_op){
	if(first == last) return result;
	*result = * first;
	return __adjacent_difference(first,last,result,value_type(first),binary_op);
}



template<class InputIterator, class OutputIterator, class T, class BinaryOperation>
OutputIterator __adjacent_difference(InputIterator first, InputIterator last,
									OutputIterator result ,T*
									BinaryOperation binary_op){
	T value = *first;
	while(++first != last){
		T tmp = *first;
		*++result = binary_op(tmp, value); // 将相邻量元素的运算结果， 赋值给目的端
		value = tmp;
	}
	return ++result;
}

/* inner product*/

// 版本1 
template<class InputIterator1, class InputIterator2, class T>
T inner_product(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, T init){
	for(; first1 != last1; ++first1, ++first2){
		init = init + (*first1 + *first2);
	}			
	return init;
}


// 版本2 
template<class InputIterator1, class InputIterator2, class T,
			class BinaryOperation1, class BinaryOperation2>
T inner_product(InputIterator1 first1, InputIterator1 last1,
				InputIterator2 first2, T init, 
				BinaryOperation1 binary_op1,BinaryOperation2 binary_op2)
{
	for(; first1 != last1; ++first1, ++first2){
		init = binary_op1(init, binary_op2(*first1,*first2));
	}
	return init;
}



/* partial_sum */
// 版本1 
template<class InputIterator, class OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last,
							OutputIterator result)
{
	if(first == last) return result;
	*result = *first;
	return __partial_sum(first, last,result value_type(first));
}

template<class InputIterator, class OutputIterator, class T>
OutputIterator __partial_sum(InputIterator first, InputIterator last,
		OutputIterator result, T*){
	T value = *first;
	while(++first != last){
		value = value + *first;
		* ++result = value;
	}
	return ++result;
}


// 版本2


template<class InputIterator, class OutputIterator, class BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last,
							OutputIterator result,BinaryOperation binary_op)
{
	if(first == last) return result;
	*result = *first;
	return __partial_sum(first, last,result value_type(first),binary_op);
}

template<class InputIterator, class OutputIterator, class T,class BinaryOperation>
OutputIterator __partial_sum(InputIterator first, InputIterator last,
		OutputIterator result, T*,BinaryOperation binary_op){
	T value = *first;
	while(++first != last){
		value = binary_op(value , *first);
		*++result = value;
	}
	return ++result;
}



/* power    (SGI专属，不在STL标准)*/

// 版本1 ， 乘幂
template<class T,class Integer>
inline T power(T x, Integer n){
	return power(x,n,multiplies<T>());  // 指定运算式为乘法
}

// 版本2 ： 幂次方
// 如果n >=0, 返回x^n
template<class T, class Integer, class MonioidOperation>
T power(T x, Integer, MonioidOperation op){
	if(n == 0)
		return identity_element(op);  // “证同元素”
	else{
		while((n&1) ==0 ){
			n>>= 1;
			x = op(x,x);
		}
		T result = x;
		n>>= 1;
		while(n!=0){
			x = op(x,x);
			if((n & 1) != 0)
				result = op(result, x);
			n>>= 1;
		}
		return result;
	}
}































