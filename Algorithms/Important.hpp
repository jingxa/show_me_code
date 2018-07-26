/* 1. 数组求和问题 */
// 例子1： 寻找数组中的两个元素值为target的索引；
// hashmap
vector<int> addsToTarget(vector<int>& numbers, int target){
	unordered_map<int,int> numtoindex;
	vector<int> vi(2);
	for(autoit = numbers.begin(); it != numbers.end();it++){
		if(numtoindex.count(target - *it)){
			vi[0] = numtoindex[target -*it] +1;
			vi[1] = (int)(it - numbers.begin()) +1;
			return vi;
		}
		numtoindex[*it] = (int)(it - numbers.begin()); // 存储索引
	}
	return vi;
}

// 方法2： 快排
bool hasArrayTwoCandidates(int A[], int arr_size, int sum){
	int l,r;
	// 排列元素
	quickSort(A,0,arr_size-1);
	
	l =0;
	r = arr_size-1;
	while(l<r){
		if(A[l]+A[r] == sum){
			rturn 1;
		}else if(A[l]+A[r] <sum)
			l++;
		else
			r--;
	}
	return 0;
}


// 例子2： 寻找三个数的和等于0，并且三个数递增排序
// 先排序，
set<vector<int>> find_triplets(vector<int> arr){
	sort(arr.begin(), arr.end());
	set<vector<int> triplets;
	vector<int> triplet(3);
	int n =arr.size();
	for(int i=0;i<n;i++){
		int j=i+1;
		int k = n-1;
		
		while(j<k){	
			int sum_two = arr[i]+arr[j];
			if(sum_two +arr[k] < 0){
				j++;
			}else if(sum_two + arr[k] >0)
				k--;
			else{  // 三项和为0
				triplet[0] = arr[i];
				triplet[1] = arr[j];
				triplet[2] = arr[k];
				triplets.insert(triplet);
				j++;
				k--; // 继续找
			}
		}
	
	}
	return triplets;
}

// 方法2：不允许排序，use hash
bool three_sum(int nums[], int n , int target)
{
	hash_map<int,int>arr_hash;
	for(int i=0;i<n;i++){
		arr_hash[nums[i]] = i;
	}
	
	hash_map<int,int>::iterator iter;
	for(int i=0;i<n-1;i++){
		for(int j=i+1;j<n;j++){
			int sum = num[i] + num[j];
			iter = arr_hash.find(target- sum);
			if(iter != arr_hash.end()){
				if(iter->second > j){
					cout<<nums[i]<<" "<<nums[j]<<" "<<iter->first<<endl;
					return true;
				}
			}
		}
	}
	return false;
}


// 例子3：三个元素 sum的和最近于一个target
int threeSumClosest(vector<int> &num, int target){
	if(num.size() <=3)return accumulate(num.begin(),num.end(),0);
	sort(num.begin(),num.end());
	
	int result =0, n= num.size(),temp;
	result = num[0] + num[1] + num[2];
	for(int i=0;i<n-2;i++){
		int j=i+1,k =n-1;
		while(j<k){
			temp = num[i] + num[j] + num[k];
			if(abs(target - result) > abs(target-temp))
				result= temp;
			if(result == target)
				return result;
			(temp > result)?k--:j++;
		}
	}
	return result;
}



/*******************************************/
/* 2. 子数组问题系列 */
// 例子1： 连续子数组最大和
// dp[i+1] = max(dp[i],dp[i]+a[i+1])
int maxSubarray(int A[],int n){
	int sum =0;
	int m= INT_MIN;
	
	for(int i=0;i<n;i++){
		sum +=A[i];
		m = max(m, sum);
		if(sum < 0){
			sum =0;
		}
	}
	return m;
}

// 例子2： 寻找子串和为0
vector<int> subarraysum(vector<int> nums){
	vector<int> result;
	map<int,int> hash;
	hash[0] = 0;  // 起点
	
	int curr_sum =0;
	for(int i=0;i!=nums.size();++i){
		cur_sum += nums[i];
		if(hash.find(cur_sum) != hash.end()){  // 和为0
			result.push_back(hash[cur_sum]);// 起始点
			result.push_back(i);  // 终止点
			return result;
		}else{
			hash[curr_sum] = i+1;  // 新节点
		}
	}
	return result;
}


// sum 为k
vector<int> subarraysum(vector<int> nums){
	vector<int> result;
	map<int,int> hash;
	hash[0] = 0;  // 起点
	
	int curr_sum =0;
	for(int i=0;i!=nums.size();++i){
		cur_sum += nums[i];
		if(hash.find(cur_sum - k) != hash.end()){  // 和为0
			result.push_back(hash[cur_sum]);// 起始点
			result.push_back(i);  // 终止点
			return result;
		}else{
			hash[curr_sum] = i+1;  // 新节点
		}
	}
	return result;
}


// 例子2： 连续子串和最近于0
vector<int> subarraysumClosest(vector<int> nums){
	vector<int> result;
	if(nums.empty()){
		return result;
	}
	
	const int num_size = nums.size();
	vector<int ,int > sum_index(num_size +1);
	for(int i=0;i<num_size;i++){
		sum_index[i+1].first = sum_index[i].first +nums[i];
		sum_index[i+1].second = i+1;
	}
	
	sort(sum_index.begin(), sum_index.end());
	
	int min_diff = INT_MIN;
	int closest_index  =1;
	for(int i=1;i<num_size +1;i++){
		int sum_diff = abs(sum_index[i].first - sum_index[i-1].first);
		if(min_diff > sum_diff){
			min_diff  = sum_diff;
			closest_index i;
			
		}
	}
	
	int left_index = min(sum_index[closest_index -1].second,
			sum_index[closest_index].second);
		
	int right_index = -1 + max(sum_index[closest_index -1].second,
			sum_index[closest_index].second);
			
	result.push_back(left_index);
	result.push_back(right_index);
	
	return result;
	
	
}


// 例子3： 连续子串最大乘积
/*
	maxDP[i+1] = max(maxDP[i] * A[i+1],A[i+1],minDP[i]*A[i+1])   // 正数
	minDP[i+1] = min(minDP[i] * A[i+1],A[i+1],maxDP[i]*A[i+1])  // 负数
	dp[i+1] = max(dp[i],maxDP[i+1])
*/

int maxProduct(int A[], int n){
	if(n==0)
		return 0;
	else if(n==1)
		return A[0];
	
	int p = A[0];
	int maxP=A[0];
	int minP = A[0];
	for(int i=1;i<n;i++){
		int t = maxP;
		maxP = max(max(maxP * A[i],A[i]), minP*A[i]);
		minP = min(min(t*A[i],A[i]), minP*A[i]);
		p = max(maxP, p);
	}
	return p;
	
}



/* 3. 买卖股票 */
// 





/*4.寻找丢失数 */
/*5. 找主元素 */
/*6. 排序切割问题 */
/*7. TOP k 问题 ，最大堆，最小堆 */
/*8. LRU Cache问题，链表加 hashmap */
/*9. 去重问题 bitmap*/
/*10. 搜索问题  倒排索引*/
/*10.  */
/*10.  */