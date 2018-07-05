/**
	heap： 底层使用vector
	1. 使用完全二叉树作为存储方式
	2. 元素排列： max-heap
	3. 隐式表述法 ： 将vector的0位置保留，从1开始存储
		- 节点i的左子节点为 2i,右子节点为2i+1
*/

// push_heap
template<class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last){
	// 此函数调用的时候 新元素已经置于容器底部
	__push_heap_aux(first,last,distance_type(first),value_type(first));
}


template<class RandomAccessIterator, class Distance, class T>
inline void __push_heap_aux(RandomAccessIterator first,
				RandomAccessIterator last, Distance*, T*){
	// 新值在容器最尾端，位置： （last -first）-1
	// 隐式表述法 ,根节点编号从0开始
	__push_heap(first, Distance(last - first)-1, Distance(0), T(*(last -1)));
}


// 一下这组push_back()不允许指定'大小比较标准'
template<class RandomAccessIterator, class Distance, class T>
void __push_heap(RandomAccessIterator first, Distance holeIndex, 
			Distance topIndex, T value){
	
	Distance parent = (holeIndex -1) /2; // 找出父节点
	while(holeIndex > topIndex && *(first + parent) < value){  // 大根堆调整
		// 尚未到达顶端并且父节点小于新值
		*(first + holeIndex) = *(first + parent); // 令洞位置为父节点
		holeIndex = parent; // 调整洞号， 向上提升
		parent = (holeIndex - 1) /2;   // 新父节点	
	}   // 调整完成
	*(first + holeIndex) = value;  // 赋值
}


/**
	pop_heap
	1. 取走根节点
	2. 满足完全二叉树的条件，将最下一层的最右边的叶节点拿掉
*/


template<class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last){
	
	__pop_heap_aux(first, last, value_type(first));
	
}



// 根据隐式表述法， pop操作的结果为底部容器的第一个元素
// 首先将首值和尾节点互换，然后重新调整[first, last -1)
template<class RandomAccessIterator , class T>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last,
							T*){
	__pop_heap(first,last-1, last-1, T(*(last - 1), distance_type(first));							
}

template<class RandomAccessIterator, class T, class Distance>
inline void __pop_heap(RandomAccessIterator first, 
						RandomAccessIterator last,
						RandomAccessIterator result, 
						T value, Distance*)
{
	*result = * first;   // 设定尾值为首值 
					     //然后后面可以由容器pop_back()取出
	// 重新调整heap
	__adjust_heap(first,Distance(0), Distance(last -first), value);
	
}


template<class RandomAccessIterator, class Distance, class T>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, 
					Distance len, T value){
	
	Distance topIndex = holeIndex; 
	Distance secondChild = 2*holeIndex +2;  // 洞节点的右子节点
	while(secondChild < len){
		// 比较洞节点的两个子节点， 然后secondChild代替较大子节点
		if(*(first + secondChild) < *(first(secondChild -1))){
			secondChild --;
		}
		*(first + holeIndex ) = *(first + secondChild);  // 替换父节点和子节点
		holeIndex = secondChild;
		secondChild = 2*(secondChild +1);  // 下一层
	}
	if(secondChild == len){  // 没有右子节点 ，只有左节点
		// 令左子值为洞值，在令洞号移动左子节点处
		*(first + holeIndex ) = *(first +(secondChild -1));
		holeIndex = secondChild -1;
	}
	// 将要调整的洞号填入目前的洞号，
	// 此时已经满足次序性
	// 可直接为*（first + holeIndex) = value;
	
	__push_heap(first, holeIndex, topIndex,value);
	
	
}



/**
	sort_heap
	1. pop_heap 每次取出一个最大值，持续pop，可得逆序序列；
*/


template<class RandomAccessIterator>
void sort_heap(RandomAccessIterator first, 
				RandomAccessIterator last){
	// 没执行一次pop_heap(),极值放在尾端
	while(last - first >1){
		pop_heap(first,last--); // 执行一次，操作范围缩小一点
	}
}






/**
	make_heap: 将一段数据转化为堆
*/
template<clss RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last){
	__make_heap(first,last, value_type(first),distance_type(first));
}

template<class RandomAccessIterator, class T, class Distance>
void __make_heap(RandomAccessIterator first,
				RandomAccessIterator	last, T*, Distance*)
{
	if(last - first < 2) return ; // 长度0或1 不用重新排列
	Distance len = last - first;
	
	// 找出第一个需要重新排列的子树头部，以parent标出，
	Distance parent = (len -2 ) /2;
	
	while(true){
		// 重新排列parent为首的子树，len为了让__adjust_heap()判断操作范围
		__adjust_heap(first,parent,len,T(*(first+parent)));
		if(parent == 0) return ; // 走完根节点就结束；
		parent --; // 重排子树的头部向前一个节点；
	}
	
}












