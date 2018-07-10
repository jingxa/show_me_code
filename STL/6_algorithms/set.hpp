/**
	set/ multiset相关算法
	1. set_union 并集
	2. set_intersection	交集
	3. set_difference 差集
	4. set_symmetric_difference  对称差集
*/


/**
	1. set_union
	-- 构造s1,s2的并集；
	-- 排序区间表示，返回一个迭代器，指向输出区间的尾端；
	
	-- 由于s1,s2的每个元素不需要唯一，一个值在两个区间分别出现 m,n次，
	   在输出区间取max(m,n)次，其中n次来自s1, 其余来自s2;
	   
	-- set_union 是一种稳定操作，（输入区间的每个元素的相对顺序不会改变）
	-- 第一个版本使用operator< 进行比较，第二个版本采用仿函数comp比较
*/


template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
						InputIterator2 first2,	InputIterator2 last2,
						OutputIterator result){
	// 两个区间都未到尾端：
	while(first1 != last1 && first2 != last2){
		// 分别移动迭代器，然后将元素较小的记录到目标
		// 然后移动迭代器，另一区间迭代器不动
		// 如果两个元素相等，取s1的元素记录到目标，两个迭代器同时移动；
		// 重复
		if(*first1 < * first2){
			*result = *first1;
			first1 ++;
		}else if( *first1 > *first2){
			*result = *first2;
			++first2;
		}else{ // *first1 == *first2
			*result = *first1;
			++first1;
			**first2;
		}
		++result;	
	}
	// 只要一个区域到达尾端，结束，然后直接拷贝剩下的区域//
	// 其中有一个区域是空白的
	return copy(first2,last2, copy(first1,last1,result));
}








/**

	2. set_intersection
	-- 构造s1,s2的交集
	-- 以排序区间输出， 返回值是一个迭代器，指向输出区间的尾端；
	
	-- 由于s1,s2的每个元素不需要唯一，一个值在两个区间分别出现 m,n次，
	   在输出区间取min(m,n)次，并且全部来自s1;
	   
	-- set_intersection 是一种稳定操作，（输入区间的每个元素的相对顺序不会改变）
	
	-- 第一个版本使用operator< 进行比较，第二个版本采用仿函数comp比较
*/



template<class InputIterator1, class InputIterator2,class OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
								InputIterator2 first2, InputIterator2 last2,
								OutputIterator result){
	// 两个区间都未到尾端：
	while(first1 != last1 && first2 != last2){
		// 两个区间分别移动，直到遇到相同元素暂停， 将该值放入目标区域，
		// 直到一个区域到达尾端
		if(*first1 < *first2)
			++first1;
		else if(*first2 < *first1)
			++first2;
		else{  // *first1 == *first2
			*result = *first1;
			++first1;
			++first2;
			++result;
		}
	}
	return result;
}






/**
	3. set_difference
	-- 构造s1，s2 的差集
	-- 以排序区间输出， 返回值是一个迭代器，指向输出区间的尾端；
	
	-- 由于s1,s2的每个元素不需要唯一，一个值在两个区间分别出现 m,n次，
	   在输出区间取max(n-m,0)次，其中n次来自s1, 其余来自s2;
	   
	-- set_difference 是一种稳定操作，（输入区间的每个元素的相对顺序不会改变）
	
	-- 第一个版本使用operator< 进行比较，第二个版本采用仿函数comp比较
*/


template<class InputIterator1, class InputIterator2,class OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
							InputIterator2 first2, InputIterator2 last2,
							OutputIterator result){
	// 两个区间都未到尾端：
	while(first1 != last1 && first2 != last2){
		// 当第一区间元素等于第二区间元素的时候，两区间同时前进，
		// 当第一区间大于第二区间元素的时候，就让第二区间前进；
		//  这两种处理，让第一区间元素小于第二区间元素时， 
		// 第一区间的元素只存在第一区间,不存在第二区间，然后记录到目标区域
		// 直到一个区域到达尾端
		if(*first1 < *first2){
			*result = *first1;
			++first1;
			++result;
		}else if(*first2 < *first1)
			++first2;
		else{  // *first1 == *first2
			++first1;
			++first2;
		}
	}
	return copy(first1,last1 ,result);								
}








/**
	4. set_symmetric_difference
	-- 构造s1，s2 的对称差集
	-- 以排序区间输出， 返回值是一个迭代器，指向输出区间的尾端；
	
	-- 由于s1,s2的每个元素不需要唯一，一个值在两个区间分别出现 m,n次，
	   在输出区间取|n-m|次，其中n次来自s1, 其余来自s2;
	   
	-- set_symmetric_difference 是一种稳定操作，（输入区间的每个元素的相对顺序不会改变）
	
	-- 第一个版本使用operator< 进行比较，第二个版本采用仿函数comp比较	
*/

template<class InputIterator1, class InputIterator2,class OutputIterator>
OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
							InputIterator2 first2, InputIterator2 last2,
							OutputIterator result){
	// 两个区间都未到尾端：
	while(first1 != last1 && first2 != last2){
		// 当第一区间元素等于第二区间元素的时候，两区间同时前进，
		// 当两个区间不等， 就记录较小值，然后较小值区间前进
	
		if(*first1 < *first2){
			*result = *first1;
			++first1;
			++result;
		}else if(*first2 < *first1){
			*result = *first2;
			++first2;
			++result;
			
		}else{  // *first1 == *first2
			++first1;
			++first2;
		}
	}
	return copy(first2,last2 ,copy(first1,last1,result));								
}










