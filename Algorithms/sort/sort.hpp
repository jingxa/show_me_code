/**
	选择排序
	1. 找到数组中最小的元素
	2. 将它和数组中的第一个元素交换位置
	3. 重复，在于第二个位置交换
	
	分析： 需要 N^2 / 2次比较和N次交换
	算法复杂度：
*/ 

template <class T>
void selectsort(vector<T>& x){
	// 升序排列
	int N = x.size();
	for(int i=0;i<N;i++){
		int min = i;
		for(int j = i+1;j<N;j++){
			if(x[j] < x[min]) min = j; 
			swap(x[i],x[min]);
		}
	}
}


/**
	插入排序
	1. 将每一个元素插入到有序序列中
	
	插入排序 取决于输入元素的初始顺序；
	分析：
		- 最差情况： N^2/2 比较 和 N^2/2次交换
		- 最好情况：N-1 次比较 和 0次交换
		- 平均： N^2 / 4 次比较 和N^2 /4 次交换

*/
template<class T>
void Insertsort(vector<T>& x){
	int N = x.size();

	for(int i=0;i<N;i++){
		for(int j=i;j>0;j--){
			if(x[j-1] < x[j])
				swap(x[j] ,x[j-1]);
		}
	}
	
}


/**
	三、 希尔排序
		-  使数组中任意间隔为h的元素有序；
	1. 对于每个h，插入排序将h个子数组独立排序
	2.算法使用（3^k -1）/2 递减至1，这个序列为递增序列；逐渐将分组从大减小到1；
	
*/
template<class T>
void shellsort(vector<T>& x){
	
	int n = x.size();
	int h =1;
	while(h < n/3) h = 3*h +1;   // 每组间隔
	
	while(h>=1){
		// 将h数组有序
		for(int i=h;i<n;i++)
		{
			// 将x[i] 插入到x[i-h],x[i-2*h]..之中
			for(int j=i;j>=h && (x[j] < x[j-h]); j-=h){
				swap(x[j-h], x[j]);
			}
		}
		h = h/3;  // 缩小分组间隔
	}	
	
}

// 以2倍为递增序列
void shellsort2(vector<int>& n){
	int len = n.size();
	for(int gap = len/2;gap >0; gap /=2){  // 最小分组的元素为2，递增
		//对每一组进行排序
		for(int i = gap; i<len;i++){
			int j=i;
			while(j-gap>=0 && n[j] < n[j-gap]){
				swap(n[j],n[j-gap]);
				j -= gap;
			}
		}
	}
}






/**
	四、 归并排序
	-- 将两个有序的数组归并为一个数组；
	1. 首先递归的将数组分为两半排序，然后将结果归并起来；
	2. N长的数组： 时间 NlogN；
	3. 所需要的空间和N成正比；
*/

// 使用额外空间，将两个有序数组，原地归并算法
void merge(vector<int>& n, int low, int mid, int high,vector<int>& m){
	// 将[l,mid] 和 [mid +1 , h]归并
	int i= low, j = mid+1;
	for(int k = low; k<=high;k++){
		m[k] = n[k];       // 额外空间
	}

	for(int k= low; k<= high;k++){
		if		(i > mid) 		n[k] = m[j++];  // 左边区域用尽
		else if	(j >high) 		n[k] = m[i++];   // 右边空间用尽
		else if	(m[i] < m[j]) 	n[k] = m[i++];  // 左边小于右边元素
		else 					n[k] = m[j++];  // 右边元素比较小
	}


}


// 自顶向下的归并排序
void sort(vector<int>& n, int low,int high,vector<int>& m){
	if(high <= low )return;
	int mid = (low + high) / 2;
	sort(n,low,mid,m);
	sort(n, mid+1, high,m);
	merge(n,low,mid,high,m);

}


// 自底向上的归并排序,从两两归并开始
void sort(vector<int>& n){
	int len = n.size();
	vector<int> m(len,0);
	for(int size=1;size<len; size *= 2){ // 子数组的大小,2倍增大
		for(int low =0;low < len - size; low += 2 * size )
			merge(n,low,low+size -1,min(low+ 2*size -1,len -1), m);
	}
}




/**
 五、 快速排序
	1. 分治思想；
*/

// 切分
int partition(vector<int>& n, int low, int high){
	int i= low, j = high+1;
	int partial_ = n[low];
	while(true){
		while(n[++i] < partial_)if(i == high)break;  // 左边的元素小于切分点
		while(n[--j] > partial_)if(j == low)break;   // 右边的元素大于切分点
		if(i>=j)
			break;    // 一边小，一边大
		swap(n[i], n[j]);  // 交换
	}
	swap(n[low], n[j]);   // partial_ 放到正确位置
	return j;
}

void quicksort(vector<int>& n, int low ,int high){
	if(low >= high)return;
	int partial_ = partition(n, low , high);
	quicksort(n,low, partial_ -1);   // 左边部分排序
	quicksort(n, partial_ +1, high);  //右边部分排序
}


// 三向切分

void tripple_sort(vector<int>& n, int low ,int high){
	if(high <= low)return;
	int lt = low, i = lo+1, gt = high;
	int partial_ = n[low];   // 初始分界点
	while(i<=gt){
		if(n[i] < partial_) 		swap(n[lt++],n[i++]);   // 小的元素和分界点交换
		else if(n[i] > partial_) 	swap(n[i],n[gt--]);  // 大的元素交换到右边
		else						i++;				// 等于的话分界点不动
	}   // n[low...lt-1]< partial_=n[lt...gt] < n[gt+1...high] 成立
	
	tripple_sort(n,low,lt-1);   // 排序左边小于的元素
	tripple_sort(n,gt+1,high);  // 排序右边大于的元素
}


















