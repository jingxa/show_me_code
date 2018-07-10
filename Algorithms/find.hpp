
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
		

*/





















