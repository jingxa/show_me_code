/**
	排序分类：
	1. 内排序是所有的数据读入内存
	2. 外排序：内存中无法保存全部数据，需要进行磁盘访问；
	
	内排序（部分）：
	1. Merge Sort
	2. Quick Sort
	3. Heap Sort
	4. Bucket Sort(桶排序)和 Radix Sort（基数排序）
*/

/*	1. Merge Sort */



/*	2. Quick Sort */


/*	3. Heap	Sort
	priority_queue
	
	-- make_heap : 生成大顶堆
	-- heap_sort: 层序递增的排序

 */
 
 // 堆排序：（最大堆，有序区）。从堆顶把根卸出来放在有序区之前，再恢复堆。

void max_heapify(int arr[], int start, int end) {
	//建立父節點指標和子節點指標
	int dad = start;
	int son = dad * 2 + 1;
	while (son <= end) { //若子節點指標在範圍內才做比較
		if (son + 1 <= end && arr[son] < arr[son + 1]) //先比較兩個子節點大小，選擇最大的
			son++;
		if (arr[dad] > arr[son]) //如果父節點大於子節點代表調整完畢，直接跳出函數
			return;
		else { //否則交換父子內容再繼續子節點和孫節點比較
			swap(arr[dad], arr[son]);
			dad = son;
			son = dad * 2 + 1;
		}
	}
}

void heap_sort(int arr[], int len) {
	//初始化，i從最後一個父節點開始調整
	for (int i = len / 2 - 1; i >= 0; i--)
		max_heapify(arr, i, len - 1);
	//先將第一個元素和已经排好的元素前一位做交換，再從新調整(刚调整的元素之前的元素)，直到排序完畢
	for (int i = len - 1; i > 0; i--) {
		swap(arr[0], arr[i]);
		max_heapify(arr, 0, i - 1);
	}
}
 
 
 
 
 
 
 


/*  4. Bucket Sort
	根据数据大小，放入合适的“桶（容器）”中，在依次从动桶中取出
	，形成有序序列；
	
	复杂度： O（max + n）
*/

// n: 序列长度， 容器中元素为[0,max]之间
void BucketSort(vector<int>& nums, int n, int max){
	// 复制
	vector<int> tmp(nums);

	vector<int> count(max,0);  // 建立桶
	
	int i=0;
	// 计算每个元素在序列中的个数
	for( i = 0; i<n;i++)
		count[nums[i]]++; 
	
	// count[i]存储着下一个元素(i+1)的开始位置
	for(i=1;i<max;i++)
		count[i] = count[i-1] + count[i]; 
	
	// 从后往前计算，不能从前往后，因为只知道起始位置
	// 即当前元素的下一个位置
	for(i=n-1;i>=0;i--){ 
		start = -- count[tmp[i]];  // 当前元素的最后一个位置
		nums[start] = tmp[i];
	}
	
}


/*
	5. 基数排序
	
	// https://www.cnblogs.com/skywang12345/p/3603669.html
	基数排序(Radix Sort)是桶排序的扩展，
	它的基本思想是：将整数按位数切割成不同的数字，然后按每个位数分别比较。
	具体做法是：将所有待比较数值统一为同样的数位长度，数位较短的数前面补零。
	然后，从最低位开始，依次进行一次排序。这样从最低位排序一直到最高位排序
	完成以后, 数列就变成一个有序序列.
	
	即：
	1. 按照个位数进行排序。
	2. 按照十位数进行排序。
	3. 按照百位数进行排序。
	
	--- 每次排序使用桶排序
	函数：
		1. get_max : 获取最大位数
		2. bucket_Sort: 桶排序
		3. radixSort: 基数排序
		
*/

int get_max(int a[], int n){
	int i, max = INT_MIN;
	for(i=0;i<n;i++){
		if(a[i] > max)
			max = a[i];
	}
	return max;
}

// 位数 桶排序
// exp:  1,10,100, ...（个，十，百）
void bucket_Sort(int a[], int n, int exp){
	int outputs[n];
	int i;
	int buckets[10]={0};	// 位数 0~9
	
	int j,last;
	
	// 存储到buckets中
	for(i=0;i<n;i++){
		j = (a[i] /exp)%10 ;
		buckets[j]++;
	}
	
	// bucket[i] 存储着 bucket[i+1] 的开始位置
	for(i=1;i<10;i++)
		buckets[i] = buckets[i-1]+buckets[i];
	
	for(i=n-1;i>=0;i--){
		j = (a[i] /exp)%10 ;
		last = -- buckets[a[j]];  // a[i] 元素的最后一个位置
		outputs[i] = a[i];
	}
	
	for(i=0;i<n;i++) // 拷贝回去
	a[i] = outputs[i];
	
}

// 基数排序
void radixSort(int a[],int n){
	int exp;
	int max = get_max(a,n);   // 最大值
	
	// 从个位 到 最高位进行排序
	for(exp = 1; max /exp > 0;exp *=10 )
		bucket_Sort(a, n , exp);
	
}









/*====================================================*/

/**
	二、外排序
	文件分成k块，
	1): 一次读入每个文件块，在内存中排序
	2): 在内存中间里一个最小值堆，读取每块文件的队列头
	3): 弹出堆顶元素，如果元素来自第i块，则从第i块文件中补充一个
		元素到小顶堆，弹出的元素存储到临时数组；
	4): 当临时数组满了，数组写到磁盘，清空数组内存；
	5): 重复3,4,直到文件快读取完毕；
*/



/*=================================================*/


/*例子

	当题目中出现“前k个”，“合并、排序k组数据”或者“数据流”时，
	可以考虑使用heap，动态地维护最大值，最小值信息

*/










