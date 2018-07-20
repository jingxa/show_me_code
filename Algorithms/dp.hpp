// 动态规划

/*
	1.递归和动态规划
		相同：
			都能分解成若干子问题
		不同：
			DP存储子问题结果
			
	
	2.动态规划：
		1. 建立子问题
		2. 递归的空间和时间成本
		3. 自底向上，和从上到下
	
	
	3. 四要素
		1. 状态
		2. 方程
		3. 初始化
		4. 答案
*/

// 1. fibonacci 数列

// 递归(自顶向下)
int F(int n){
	if(0 == n)
		return 0;
	if(1 == n)
		return 1;
	return F(n-1) + F(n-2);
}

// DP: 自底向上
int F(int n){
	if(n == 0)
		return 0;
	if(1==n)
		return 1;
	int a=0,b=1,sum;
	for(int i=0;i<n-1;i++)
	{
		sum = a+b;
		a= b;
		b= sum;
	}
	
	return sum;
}




/*
	例子：
	1.dp 自底向上解决收敛结构问题
	
	如果出现类似于“所有解”，“所有路径”使用top-down方法直接；
*/


// 1. 爬楼梯，一次1到2阶；计算方法多少种；

// count(n) = count(n-1) + count(n-2);
int climbstairs(int n){
	if(n<=1)return 1;
	if(n==2)return 2;
	
	int p=1,q =2, cur;
	for(int i=3;i<=n;i++){
		cur = p+q;
		p = q;
		q = cur;
	}
	return cur;
	
}
 
 
 // 2. 计算质数
 // 2,3,5,7 ...
 
// prime(n) = G(prime(n-1),prime(n-2),..prime(1));
// prime(1) = 2;
// G() 表示不能被整除关系

int getNthprime(int n){
	std::list<int> primes (1,2); // 长度为1， 值2
	int number = 3;
	while(primes.size() != n){
		bool isPrime = true;
		for(auto it = primes.begin();it != primes.end() && 
				(*it)*(*it)<= number;it++){
			if(number %(*it) == 0)
				isPrime = false;
		}
		if(isPrime)
			primes.push_back(number);
		
		number += 2; // 大于2的偶数不是质数
	}
	return *(primes.rbegin());  // 最后一个
}


// 2. word break 
// 字符串"applepie" ==> "apple pie"在字典中
// 考虑： 字典的不同，dont之类省略don't的，字典多大

bool wordbreak(string s, unordered_set<string>&dict){
	int begin =0, end = 0;
	string word;
	bool words[s.size()+1]={0};
	words[0] = true;
	
	for(int i=1;i<s.size()+1;i++){
		words[i] = false;
		for(end = 0; end< s.size();end++)
			for(begin =0;begin <=end;begin++){
				if(words[begin] &&
					dict.find(s.substr(begin,end-bgin+1)) !=dict.end() )
				{
					words[end+1] = true;
					break;
				}
			}
	}
}





// 3. 最小回文分割palindrome pation 
// 讲一个回文字符串分为每个字段都是回文 abba ,a是回文，ab不是,最小切分距离为1
// 分为两个部分：判断子字符串是否回文，如何切割源字符串
// isPa(i,j) = (value(i) == value(j)) AND (isPa(i+1,j-1) OR j-i<=1)

// minCut(i) = min(minCut(j+1)+1), for i<=j<n, and substr(i,j) ia pa;

int minCut(string s){
	if(s.empty())return 0;
	vector<vector<bool>> palin(s.size(),vector<bool>(s.size(),false));
	vector<int> minCut(s.size()+1,0);
	
	for(int i=0;i<=s.size();i++)
		minCut[i] = s.size() -1 -i;
	
	for(int i=s.size()-1;i>=0;i--){
		for(int j=i;j<s.size();j++){
			if(s[i] == s[j] && (j-i)<=1 || palin[i+1][j-1]){
				palin[i][j] = true;
				minCut[i] = min(minCut[j+1]+1, minCut[i]);
			}
		}
	}
	
}



/**
	2 . 聚合 性质
	求最值或者求和问题，
	如果仅仅在乎紧邻的一个的局部解，
*/

// 1. unique path
// (0,0) ==>(x,y)的路径，往右往下，计算路径和
// res[i][j] = res[i-1][j] + res[i][j-1]

int uniquepathBackTrack(int m,int n){
	if(m==1 || n==1)return 1;
	return uniquepaths(m-1,n) + uniquepaths(m,n-1);
} 

int uniquepaths(int m, int n){
	vector<vector<int>> table(m, vector<int>(n,1));
	for(int i=1;i<m;i++){
		for(int j=1;j<n;j++){
			table[i][j] = table[i-1][j] + table[i][j-1];
		}
	}
	return table[m-1][n-1];
	
}

// 方法2
int uniquepaths2(int m, int n){
	vector<int> table(n, 1);
	for(int i=1;i<m;i++){
		for(int j=1;j<n;j++){
			table[j] +=table[j-1];
		}
	}
	return table[n-1];
}

//2.  unique path 2
/*	多了障碍物，则设置当前位置为0
	dp[0][j] = grid[0][j]?0:dp[0,j-1]
	dp[i][0] = grid[i][0]?0:dp[i-1][0]
*/
int uniquepathWithObstacles(vector<vector<int>& grid){
	if(grid.empty() || grid[0].empty())
		return 0;
	int m = grid.size();
	int n = grid[0].size();
	
	int dp[m][n];
	// 初始dp根据grid
	dp[0][0] = grid[0][0] == 0?1:0;
	
	// 注意，第0行和第0列的初始化
	for(int i=1;i<m;i++) // 第0列
		dp[i][0] = (dp[i-1][0] == 1 && grid[i][0] == 0) ? 1: 0;
	
	for(int j=1;j<n;j++)
		dp[0][j] = (dp[0][j-1]== 1 && grid[0][j-1] == 0) ? 1: 0;
		
	// 其他的初始化
	for(int i=1;i<m;i++){
		for(int j=1;j<n;j++)
		{	if(grid[i][j] == 1)
				dp[i][j] = 0;
			else{
				dp[i][j] = dp[i-1][j] + dp[i][j-1];
			}
		}
	}
	return dp[m-1][n-1];
}


// 例子3.  最小路径和
/*
	在m*n中，充满非负数，寻找一个路径从做商检到右下的最小和路径
	dp[i][j] = min(dp[i-1][j], dp[i][j-1]) + grid[i][j]
*/
int minPathsum(vector<vector<int>> & grid){
	if(grid.empty() || grid[0].empty())
		return 0;
	
	int row = grid.size();
	int col = grid[0].size();
	int dp[row][col];
	
	dp[0][0] = grid[0][0];
	for(int i=1;i<row;i++)
		dp[i][0] = dp[i-1][0] + grid[i][0];
	
	for(int j=1;j<col;j++)
		dp[0][j] = dp[0][j-1] + grid[0][j];
	
	
	for(int i=0;i<row;i++){
		for(int j=1;j<col;j++){
			dp[i][j] = min(dp[i-1][j], dp[i][j-1]) + grid[i][j];
		}
	}
	return dp[row-1][col -1];
}



// 例子4： Jump game
/*
	一个非负数数组， 每个元素表示最大的跳跃格数，
	确定能否走到最后一个元素；
	如：
	[2,3,1,1,4] == > 能够到最后 返回true
	[3,2,1,0,4] -=> 失败 
*/

bool jump(vector<int> A, int n){
	if(n == 0)
		return true;
	int v = A[0];
	
	for(int i=1;i<n;i++){
		v--;   // 每次只走一步
		if(v < 0)
			return false;
		if(v<A[i])  // 更新步长，防止走完
			v = A[i];
	}
	return true;
	
}



// 例子4. Triangle
/**
	在一个三角形中，找到从顶到底的最小路径和；
	只能向下向右走；
	2
   3 4
  6 5 7
	dp[m+1][n] = min(dp[m][n],dp[m][n-1]) + triangle[m+1][n] if n > 0
	dp[m+1][0] = dp[m][0]+ triangle[m+1][0]
	
*/
int minimumTotal(vector<vector<int>>& tr){
	int row = tr.size();
	if(row == 0)
		return 0;
	
	// 初始化为最大值
	vector<int> total(row,INT_MAX);
	total[0] = tr[0][0];
	int minTotal = INT_MAX;
	for(int i=1;i<row;i++){
		for(int j=i;j>=0;j--){
			if(j==0){
				total[j] = total[j] + tr[i][j];
			}else{ // 从上面，从左面的到来
				total[j] = min(total[j-1],total[j]) + tr[i][j];
			}
		}
	}
	
	for(int i=0;i<row;i++){
		minTotal = min(minTotal, total[i]);
	}
	return minTotal;
	
}


// 例子6： unique binary search Tree

/**
	给定一个数，可以形成多少个unique BST树；
	例如3： 有五种：

	root: k
	左子树节点：  k (1,k) m种列
	右子树：	n-k-1  (k+1,n) n种排列
	dp[i] = sum(dp[k] * dp[i-k-1]), 0<=k < i
*/
int numTree(int n){
	vector<int> dp(n+1,0);
	// dp初始化
	dp[0] =0;
	dp[1] = 1;
		
	for(int i=2;i<=n;i++){
		for(int j=0;j<i;j++){
			dp[i] +=dp[j]*dp[i-j-1];
		}
	}
	return dp[n];
}











