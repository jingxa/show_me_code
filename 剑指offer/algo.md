# 目录


---
# 位运算
---

## 二进制中 1 的个数
>输入一个整数，输出该数二进制表示中 1 的个数。
>n       : 10110100
>n-1     : 10110011
>n&(n-1) : 10110000


```
class Solution {
public:
     int  NumberOf1(int n) {
         int cnt =0;
         while(n!=0){
             cnt++;
             n = n&(n-1);
         }
         return cnt;
     }
};



    int  NumberOf1(int n) {
         int count = 0;
         while(n != 0){
             count += n & 1;
             n = n >>> 1;   // 无符号右移动， 不需要判断正负
         }
         return count;
     }

```




---
# 动态规划
---

## 剪绳子

> 把一根绳子剪成多段，并且使得每段的长度乘积最大
---

```
    int integerBreak(int n) {
        if(n<2)
            return 0;
        if(n==2)
            return 1;
        if(n==3)
            return 2;
// 方法1 ： 贪心 ，至少支取2的长度
//         int timeof3 = n/3;
//         if((n - timeof3*3) == 1) 
//             timeof3 --;
//         int timeof2 = (n-timeof3*3)/2;
//        // cout<<"timeof3:"<<timeof3<<" timeof2："<<timeof2<<endl;
        
//         return pow(3,timeof3)*pow(2,timeof2);
        
// 方法2 ： dp[i] = max(dp[i], j*(i-j), j* dp[i-j]) // 分成两部分，或者取一部分，对剩下的再分
        vector<int> dp(n+1, 0);
        dp[1] = 1;
        for(int i=2;i<=n;i++){
            for(int j=1;j<i;j++){
                dp[i] = max(dp[i],max(j*(i-j),j * dp[i - j] ));
            }
        }
        return dp[n];
        
        
    }

```


---
# DFS
---
## 数值的整数次方
>给定一个 double 类型的浮点数 base 和 int 类型的整数 exponent。
>求 base 的 exponent 次方。
![](https://camo.githubusercontent.com/a9c60dcae8e57f42cb7f60473d695dddb3dd6221/68747470733a2f2f6c617465782e636f6465636f67732e636f6d2f6769662e6c617465783f785e6e3d5c6c6566745c7b5c626567696e7b61727261797d7b72636c7d28782a78295e7b6e2f327d26267b6e5c25323d307d5c5c782a28782a78295e7b6e2f327d26267b6e5c25323d317d5c656e647b61727261797d5c72696768742e)

```
class Solution {
public:
    double Power(double b, int e) {
        if(e == 0)
            return 1;
        if(e==1)
            return b;
        int isN = false;
        if(e<0)   // 负指数
        {
            e = -e;
            isN = true;
        }
        double p = Power(b*b, e/2);
        if(e%2 == 1)
            p = b*p;
        
        return isN ?1/ p : p; 
        
    }
};

```


## 机器人的运动范围

- 题目描述

>地上有一个 m 行和 n 列的方格。一个机器人从坐标 (0, 0) 的格子开始移动，
>每一次只能向左右上下四个方向移动一格，但是不能进入行坐标和列坐标的数位之和大于 k 的格子。
>例如，当 k 为 18 时，机器人能够进入方格（35, 37），因为 3+5+3+7=18。
>但是，它不能进入方格（35, 38），因为 3+5+3+8=19。请问该机器人能够达到多少个格子？

- 思路：
1. 坐标和的计算： 使用数组保存
2. DFS遍历，保存最大的移动数



```
class Solution {
public:
    int movingCount(int threshold, int rows, int cols)
    {
        if(threshold < 0 || rows <=0 || cols <=0 )
            return 0;
        vector<vector<int>> sums = calcSum(rows, cols);
        vector<vector<bool>> marked (rows, vector<bool> (cols, false));
        
        int cnt = 0;
        dfs(threshold, 0,0, rows,cols, cnt, marked, sums);
        return cnt;
        
    }
    
    // 坐标和的计算
    vector<vector<int>> calcSum(int rows, int cols){
        int len = rows>cols ? rows : cols;
        vector<int> maxLen(len,0);
        for(int i=0;i<len; i++){
            int j=i;
            while(j>0){
                maxLen[i] += j % 10;
                j = j/10;
            }
        }
        
        vector<vector<int>> res(rows, vector<int>(cols,0));
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++)
                res[i][j] = maxLen[i] + maxLen[j];
        }
        return res;     
    }
    
    
    // dfs
    void dfs(int threshold, int i, int j, int r, int c, int& cnt,
             vector<vector<bool>>& marked, vector<vector<int>>& sums){
        if(i<0 || i>=r || j<0 || j>= c || marked[i][j])
            return;
        marked[i][j] = true;
        if(sums[i][j]  > threshold)
            return;
        cnt ++;

        dfs(threshold, i,j+1, r,c, cnt, marked, sums);
        dfs(threshold, i,j-1, r,c, cnt, marked, sums);
        dfs(threshold, i+1,j, r,c, cnt, marked, sums);
        dfs(threshold, i-1,j, r,c, cnt, marked, sums);
   
    }
 
};
```


---
# 链表
---
## 1. 单链表删除节点
- 中间节点：将下一个节点的值赋值给当前节点，删除下一节点
- 尾巴节点：遍历到尾部


```
ListNode* delnode(ListNode* head, ListNode* node){
	if(head == nullptr || head->next == nullptr || node == nullptr)
		return nullptr;
	if(node->next != nullptr){
		ListNode* next = node->next;
		node->val = next->val;
		node->next = next->next;
	}else{
		ListNode* cur = head;
		while(cur->next != node)
			cur = cur->next;
		cur->next = nullptr;
	}
	return head;

}

```

## 2. 删除链表中重复的节点
- 当前节点和下一个节点比较,如果值相等，删除当前节点

```

```

