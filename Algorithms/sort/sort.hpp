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













