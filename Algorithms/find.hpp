
// 参考资料：http://www.cnblogs.com/maybe2030/p/4715035.html


/**
	1. 线性查找
	
*/

//顺序查找
int SequenceSearch(vector<int>& n, int key)
{
	int len = n.size();
    int i;
    for(i=0; i<len; i++)
        if(a[i]==key)
            return i;
    return -1;
}



/**
	1. 二分查找
　　--- 基本思想：也称为是折半查找，属于有序查找算法。
		用给定值k先与中间结点的关键字比较，中间结点把线形表分成两个子表，
		若相等则查找成功；若不相等，再根据k与该中间结点关键字的比较结果，
		确定下一步查找哪个子表，
		这样递归进行，直到查找到或查找结束发现表中没有这样的结点。

　　--- 复杂度分析：最坏情况下，关键词比较次数为log2(n+1)，且期望时间复杂度为O(log2n)；
	
*/

// 找到返回下标，否则返回-1
int binary_find(vector<int>& n, int key){
	int len = n.size();
	int left = 0;
	int right = len -1;
	
	while(left <=right){
		int mid = (left + right) /2;
		if(key < n[mid] )
			right = mid -1;
		else if(key > n[mid])
			left = mid +1;
		else
			return mid;
	}
	return -1;
	
}


// 迭代二分查找
int binary_find(vector<int>& n,int key,int low, int high){
	if(high <= low)return low;
	int mid = (low +high) /2;
	if(key < n[mid])
		return binary_find(n,key, low,mid -1);
	else if(key > n[mid])
		return binary_find(n,key,mid +1,high);
	else
		return mid;
}


/**

	3. 插值查找
	--- 二分查找中查找点计算如下：
　　	mid=(low+high)/2, 即mid=low+1/2*(high-low);
　　	通过类比，我们可以将查找的点改进为如下：
　　	mid=low+(key-a[low])/(a[high]-a[low])*(high-low)，
　　	也就是将上述的比例参数1/2改进为自适应的，根据关键字在整个有序表中所处的位置，
		让mid值的变化更靠近关键字key，这样也就间接地减少了比较次数。
	
　　--- 基本思想：基于二分查找算法，将查找点的选择改进为自适应选择，
		可以提高查找效率。当然，差值查找也属于有序查找。
　　	
	--- 注：对于表长较大，而关键字分布又比较均匀的查找表来说，
		插值查找算法的平均性能比折半查找要好的多。
		反之，数组中如果分布非常不均匀，那么插值查找未必是很合适的选择。
		
　　--- 复杂度分析：查找成功或者失败的时间复杂度均为O(log2(log2n))。
*/


int InsertSearch(vector<int>& n, int key, int low ,int high){
	int mid = low + (key - n[low])/(n[high] - n[low]) *(high - low);
	if( key < n[mid])
		return InsertSearch(n, key, low,mid -1);
	else if(key > n[mid])
		return InsertSearch(n, key, mid +1 ,high);
	else
		return mid;
}




/**
	4. 斐波那契查找
	
	--- 基本思想：也是二分查找的一种提升算法，
		通过运用黄金比例的概念在数列中选择查找点进行查找，提高查找效率。
		同样地，斐波那契查找也属于一种有序查找算法。
　　	相对于折半查找，一般将待比较的key值与第mid=（low+high）/2位置的元素比较，

	--- 斐波那契查找与折半查找很相似，他是根据斐波那契序列的特点对有序表进行分割的。
		他要求开始表中记录的个数为某个斐波那契数小1，就是 n=F(k)-1;
		
	 开始将k值与第F(k-1)位置的记录进行比较(即 mid=low+F(k-1)-1)
	 比较结果分为三种：
		1) < : high = mid -1，k-=1
		--- 说明：low=mid+1说明待查找的元素在[low,mid-1]范围内，k-=1 
			说明范围[low,mid-1]内的元素个数为F(k-1)-1个，所以可以递归 的应用斐波那契查找。
		2) > : low = mid +1, k -=2
		--- 说明：low=mid+1说明待查找的元素在[mid+1,high]范围内，
			k-=2 说明范围[mid+1,high]内的元素个数为
			n-(F(k-1))= Fk-1-F(k-1)=Fk-F(k-1)-1=F(k-2)-1个，所以可以递归的应用斐波那契查找。
		3) == : mid 
		说明：low=mid+1说明待查找的元素在[low,mid-1]范围内，
		k-=1 说明范围[low,mid-1]内的元素个数为F(k-1)-1个，所以可以递归 的应用斐波那契查找。

　　复杂度分析：最坏情况下，时间复杂度为O(log2n)，且其期望复杂度也为O(log2n)。

*/



// 构造一个斐波那契数组
void Fibonacci(vector<int>& F,int n){
	F[0] = 0;
	F[1] = 1;
	for(int i=2; i< n;++i){
		F[i] = F[i-1] + F[i-2];
	}
}

// 斐波那契查找
static const int max_size = 20; // 定义一个最大长度
int FibonacciSearch(vector<int>& a,int n, int k){
	// a： 查找数组； n:数组长度， k:关键字
	int low = 0; 
	int high = 0;
	vector<int> F(max_size);
	Fibonacci(F,max_size);  // 构造一个斐波那契数组
	
	int k = 0;
	while(n > F[k] -1)
		++k;    // 计算n位于斐波那契的位置
	
	vector<int> tmp(F[k] -1);  // 定义一个新数组
	copy(a.begin(),a.end(),tmp.begin());
	
	// tmp多余的空间全部设为a最后一个
	for(int i=n;i< tmp.size();i++)
		tmp[i] = a[n-1];
	
	while(low <= high){
		int mid = low + F[k] -1;
		if(key < tmp[mid]){
			high = mid -1;
			k-=1;
		}
		else if(key >tmp[mid]){
			low = mid +1;
			k-=2;
		}
		else{
			if(mid < n)
				return mid ; // 若mid<n，说明mid即为查找位置
			else
				return n-1;// 说明mid >=n, 在扩展的位置，返回n-1
		}
	}
	return -1;  // 未找到
	
}

/**
	5. 树表查找
	5.1 二叉树查找算法
	- 复杂度分析：它和二分查找一样，插入和查找的时间复杂度均为O(logn)，
	但是在最坏的情况下仍然会有O(n)的时间复杂度。
*/


template <class T>
BSTNode<T>* BSTSearch(BSTNode<T>* x, T key) const
{
    while ((x!=NULL) && (x->key!=key))
    {
        if (key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    return x;
}



/**
	5.2 平衡查找树2-3查找树（2-3 Tree）
	-- 构成：
	1. 由2或3节点构成：
	2. 2-node:left-node < key < right-node
	3. 3-node : left-node< key1< 中间节点 key2 < right-node
	
	-- 复杂度分析：
　　2-3树的查找效率与树的高度是息息相关的。
	在最坏的情况下，也就是所有的节点都是2-node节点，查找效率为lgN
	在最好的情况下，所有的节点都是3-node节点，查找效率为log3N约等于0.631lgN

	
*/

/**
	5.3 红黑树查找
	红黑树的定义：红黑树是一种具有红色和黑色链接的平衡查找树，同时满足：
	- 红色节点向左倾斜
	- 一个节点不可能有两个红色链接
	- 整个树完全黑色平衡，即从根节点到所以叶子结点的路径上，黑色链接的个数都相同。
*/



/**
	5.4 B树 和B+树
　　B树定义：
　　-- B树可以看作是对2-3查找树的一种扩展，即他允许每个节点有M-1个子节点。
	-- 根节点至少有两个子节点
	-- 每个节点有M-1个key，并且以升序排列
	-- 位于M-1和M key的子节点的值位于M-1 和M key对应的Value之间
	-- 其它节点至少有M/2个子节点
	
	
	B+树定义：
　　B+树是对B树的一种变形树，它与B树的差异在于：
	--有k个子结点的结点必然有k个关键码；
	--非叶结点仅具有索引作用，跟记录有关的信息均存放在叶结点中。
	--树的所有叶结点构成一个有序链表，可以按照关键码排序的次序遍历全部记录。
	
*/


/**
	6. 分块查找
	　　分块查找又称索引顺序查找，它是顺序查找的一种改进方法。
　　算法思想：将n个数据元素"按块有序"划分为m块（m ≤ n）。
			  每一块中的结点不必有序，但块与块之间必须"按块有序"；
			  即第1块中任一元素的关键字都必须小于第2块中任一元素的关键字；
			  而第2块中任一元素又都必须小于第3块中的任一元素，……
　　算法流程：
　　step1 先选取各块中的最大关键字构成一个索引表；
　　step2 查找分两个部分：先对索引表进行二分查找或顺序查找，以确定待查记录在哪一块中；
			然后，在已确定的块中用顺序法进行查找。

*/




/**
	7. 哈希查找
*/


