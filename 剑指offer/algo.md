# 目录

* [1.动态规划](#动态规划)

* [2.查找](#2-查找)



---
# 动态规划






---
# DFS

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

# 2. 查找



