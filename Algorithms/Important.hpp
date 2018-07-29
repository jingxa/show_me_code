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
// 最佳买卖时机
int maxProfit(vector<int> &prices){
	if(prices.empty())
		return 0;
	int minP = prices[0];
	int profit = prices[1] - prices[0];
	
	for(int i=2; i<prices.size();i++){
		minp = min(prices[i-1],minp);
		profit= min(prices[i] - minp, profit);
	}
	
	if(profit < 0)
		return 0;
	
	return profit;
	
}

// 例子2：可以多次卖出的最大收益；不能同时买入多次，卖掉才能买入
int maxProfit(vector<int>& prices){
	int len = prices.size();
	if(len<=1)
		return 0;
	
	int sum =0;
	for(int i=1;i<len;i++){
		if(prices[i] - prices[i-1] >0)
			sum+=prices[i] - prices[i-1];
	}
	return sum;
}


// 例子3： 最多两次交易
// i天之前和i天之后 的和
int maxProfit(vector<int>& prices){
	int len = prices.size();
	if(len<=1)
		return 0;
	vector<int> profit(len);
	
	// 正序遍历,每以天的收益
	int minP = prices[0];
	int sum = numeric_limits<int>::min();
	for(int i=1;i<len;i++){
		minP= min(minP,prices[i-1]);
		profit[i] = max(sum, prices[i] - minP);
		
		sum = profit[i];
	}
	
	// 逆向遍历
	int maxP = prices[len-1];
	int sum2 = numeric_limits<int>::min();
	for(int i=len-2;i>=0;i--){
		maxP = max(maxP,prices[i+1]);
		sum2 = max(sum2, maxP - prices[i]);
		
		if(sum2>0){
			profit[i] = profit[i]+sum2;
			sum =max(sum,profit[i]);
 			}
	}
	return sum>0?sum:0;
	
	
}

// 例子4： 寻找丢失的第一个正整数
// 1，2,0 ： 返回3 ， [3,4,-1],返回2
int firstMiss(vector<int> A){
	int size = A.size();
	for(int i=0;i<size;++i){
		while(A[i]>0 && A[i] <=size && 
			A[i] != i+1 && A[i] !=A[A[i] -1]){
				int tmp = A[A[i] -1];
				A[A[i] -1] = A[i];
				A[i] = tmp;
			} 
	}
	
	for(int i=0;i<size;++i){
		if(A[i] != i+1){
			return i+1;
		}
	}
	return size+1;
}



int firstMissingPositive(vector<int>& nums) {
int temp=INT_MIN , t ,i;
        nums.push_back(0); //pushing 0 into the original array bcoz the indexes start from 0 but the first natural no's start from 1
        for(int i=0; i<nums.size(); i++)
        {  //Here, I am making a kind of recursive call to the index of every value if it less than the size of array and greater than 0
                t=nums[i];  // so that at every index we have the value which is same as its index if it is already present in the array
                while(t < nums.size() && t >= 0 && t != nums[t])
                {
                    temp=nums[t]; //storing the value at present value's index in temp
                    nums[t] = t;  // placing the value at its index in the array
                    t=temp;
                }
        }
	
	i=1;
	for(i=1;i<=len;i++){
		if(nums[i] != i)
			break;
	}
	return i;
}

      
// 例子5： 单一数
// 两个数异或
int singleValue(vector<int> array){
	int value =0;
	for(int i=0;i<array.size();i++){
		value ^=array[i];
	}
	return value;
}

// 例子6： 单一数： 三个重复 ， 3n+1
//hash_map或者位运算
// 三个数的位运算，每位都是三的倍数
int singleValue(vector<int>& nums){
	if(nums.empty())
		return 0;
	
	int res = 0, sum = 0;
	for(int i=0;i!=8*sizeof(int);++i){
		sum = 0;
		for(int j=0;j!= nums.size();++j){
			sum += ((nums[i] >>i) & 1);
		}
		res  |= ((sum %3) <<1);
	}
	return res;
}


// single number 3
//     
vector<int> singleNumber(vector<int>& nums) 
    {
        // Pass 1 : 
        // Get the XOR of the two numbers we need to find
        int diff = accumulate(nums.begin(), nums.end(), 0, bit_xor<int>());
        // Get its last set bit
        diff &= -diff;

        // Pass 2 :
        vector<int> rets = {0, 0}; // this vector stores the two numbers we will return
        for (int num : nums)
        {
            if ((num & diff) == 0) // the bit is not set
            {
                rets[0] ^= num;
            }
            else // the bit is set
            {
                rets[1] ^= num;
            }
        }
        return rets;
    }

	
// 例子4：majority numbers
// 寻找占多半的元素
// 还可以hash_map
int majorityNum(vector<int>& nums){
	int res,count = 0;
	for(int i=0;i<nums.size();i++){
		if(count == 0){
			res = nums[i];
		}
		else{
			if(res == num[i]){
				count ++;
			}else{
				count --;
			}
		}
	}
	return res;
}


// 例子5： majority number 2
// 超过三分之一的数,中最多的一个数
int majorityNum(vector<int>& num){
	int res1,res2,count1=0,count2=0;
	for(int i=0;i<nums.size();i++){
		if(res1 == nums[i]){
			count1 ++;
		}
		else if(res2 == nums[i]){
			count2++;
		}else if(count1 == 0){
			res1 = nums[i];
			count1 = 1;
		}else if(count2 == 0){
			res2 = nums[2];
			count2 =1;
		}else{
			count1--;
			count2--;
		}
		
	}
	
	count1= count2 =0;
	for(int i=0;i<nums.size();i++){
		if(nums[i] == res1){
			count1 ++;
		}else if(nums[i] == res2){
			count2++;
		}
	}
	return count1 > count2?res1:res2;
}




/*4.寻找丢失数 */
/*5. 找主元素 */
/*6. 排序切割问题 */
/*7. TOP k 问题 ，最大堆，最小堆 */
/*8. LRU Cache问题，链表加 hashmap */
/*9. 去重问题 bitmap*/
/*10. 搜索问题  倒排索引*/
/*10.  */
/*10.  */