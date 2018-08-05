# 3. 数组中重复的数字
# 4. 二维数组中的查找
5. 替换空格
6. 从尾到头打印链表
7. 重建二叉树
8. 二叉树的下一个结点
9. 用两个栈实现队列
10.1 斐波那契数列
10.2 跳台阶
10.3 变态跳台阶
10.4 矩形覆盖
11. 旋转数组的最小数字
12. 矩阵中的路径



# 13. 机器人的运动范围
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
# 14. 剪绳子

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
# 15. 二进制中 1 的个数

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

# 16. 数值的整数次方

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

---

17. 打印从 1 到最大的 n 位数

---
# 18.1 在 O(1) 时间内删除链表节点

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

---

# 18.2 删除链表中重复的结点

---

# 19. 正则表达式匹配
> 两个字符串都为空，返回true; 如果p为空，返回false
>(1)pattern 第二个字符不为‘*’
>  两种组合： 
> 1. aabc 和 aabc  : match(s+1, p+1) 
> 2. aabc 和a.bc   : match(s+1,p+1)
>  匹配成功都前进一步

>(2) pattern 第二个字符等于'*'
> 三种组合：
> 1. bc 和 a*bc  : a* 无用， s不动，p动，  match(s,p+2);
> 2. aabc 和 a*bc : a* 多个； p不动，s动， match(s+1, p);
> 3. aabc 和 .*abc   :  .* 可以匹配0到多个， match(s+1,p);


```
bool match(char* str, char* pattern)
{
	if (*str == '\0' && *pattern == '\0')
		return true;
	if (*str != '\0' && *pattern == '\0')
		return false;
	//if the next character in pattern is not '*'
	if (*(pattern+1) != '*')
	{
		if (*str == *pattern || (*str != '\0' && *pattern == '.'))
			return match(str+1, pattern+1);
		else
			return false;
	}
	//if the next character is '*'
	else
	{
		if (*str == *pattern || (*str != '\0' && *pattern == '.'))
			return match(str, pattern+2) || match(str+1, pattern);
		else
			return match(str, pattern+2);
	}
}

```

---
# 20. 表示数值的字符串


```
    bool isNumeric(char* s)
    {
        bool pre_ = false;   // 前面的正负号
        bool last_ = false;  // e后面正负号
        bool e_    = false;  // e
        
        bool dot1_ = false;
        int size = strlen(s);
        int i=0;
        
        for( i=0;i< size;i++){
            switch(s[i]){
                case '+': case '-':{
                            if( e_ && last_ ) // e后面符号位
                                return false;
                            else if(e_ && i+1 < size)
                                last_ = true;
                            else if(pre_ )
                                return false;
                            else if( i == 0 )
                                pre_ = true;
                            else
                                return false;
                            break;
                         }
                case '.':{
                            if( dot1_  )  // 前面多个小数点
                                return false;
                            else if( e_ )        // e_后出现 ‘.’.直接false
                                return false;  
                            else if(i < size -1 && i > 0) // 小数点出现在e前面
                                dot1_ = true;
                            else
                                return false;
                            break;
                         }
                    
                case 'E': case 'e' : {
                            if(e_)
                                return false;
                            else if(i< size -1)
                                e_ = true;
                            else
                                return false;
                            break;
                        }
                default:{
                           if( s[i] >= '0' && s[i] <= '9')
                               continue;
                            else
                                return false;
                            break;
                }
                    
            }
        }
        
   
        return true;

    }

};
```


---
# 21. 调整数组顺序使奇数位于偶数前面

```
class Solution {
public:
    void reOrderArray(vector<int> &array) {
        if(array.empty())
            return ;
        
        int cnt =0;
        for(int i=0;i<array.size();i++)
            if(array[i] % 2== 1)
                cnt++;
        
        vector<int> num(array);
        int j=0;
        for(int i=0;i< num.size();i++){
            if(num[i] % 2 == 1)  // 奇数
                array[j++] = num[i];
            else
                array[cnt++] = num[i];
        }
        
    }
};



void reOrderArray(vector<int> &array) {
	if(array.empty())
		return;
	int flag = false;
	for(int i=0;i<array.size();i++)
	{
		flag =true;
		for(int j= array.size() -1;j>i;j--)
		{
			if(array[j-1] % 2 == 0 && array[j] % 2 ==1){
				swap(array[j-1], array[j]);
				flag = false;
			}
		}
		if(flag)
			break;
	}
}
```



---

# 22. 链表中倒数第 K 个结点

```
    ListNode* FindKthToTail(ListNode* head, unsigned int k) {
    
        if(!head)
            return nullptr;
        ListNode* p = head;
        
        int cnt = 0;
        while(p){
            p = p->next;
            cnt++;
        }
        int rm = cnt -k;
        if(rm < 0)
            return nullptr;
        
        p = head;
        while(rm-- >0)
            p = p->next;
        
        return p;
 
    }
```

---
# 23. 链表中环的入口结点
> 假设x为环前面的路程
> a为环入口到相遇点的路程, c为环的长度
> 当快慢指针相遇的时候：
> 此时慢指针走的路程为Sslow = x + m * c + a
> 快指针走的路程为Sfast = x + n * c + a
> 2 Sslow = Sfast
> 2 * ( x + m*c + a ) = (x + n *c + a)
> 从而可以推导出：
> x = (n - 2 * m )*c - a
> = (n - 2 *m -1 )*c + c - a
> 即环前面的路程 = 数个环的长度（为可能为0） + c - a



```
class Solution {
public:
    ListNode* EntryNodeOfLoop(ListNode* head)
    {
        if( ! head || head->next == nullptr)
            return nullptr;
        
        ListNode* fast, *slow;
        
        slow = head->next;
        fast = head->next->next;
        
        while(fast && fast->next  && fast != slow){
            slow = slow->next;
            fast = fast->next->next;
        }
        
        if(fast == nullptr)  // 无环
            return nullptr;
        
        fast = head;
        while(fast != slow)
        {
            fast = fast->next;
            slow = slow->next;
        }
        return fast;
    }
};
```


---
# 24. 反转链表

```
class Solution {
public:
    ListNode* ReverseList(ListNode* head) {
        if( head == nullptr || head->next == nullptr)
            return head;
        
        ListNode* new_head = new ListNode(0);
        ListNode* cur ;
        while(head){
            cur = head->next;
            head->next = new_head->next;
            new_head->next = head;
            head = cur;
        }
        head = new_head->next;
        
        delete new_head;
        return head;
        
    }
};

// 递归

```

---
# 25. 合并两个排序的链表

```
class Solution {
public:
    ListNode* Merge(ListNode* head1, ListNode* head2)
    {
        if(head1 == nullptr)
            return  head2;
        else if(head2 == nullptr)
            return head1;
        
        if(head1->val < head2->val){
            head1->next = Merge(head1->next, head2);
            return head1;
        }
        else{
            head2->next = Merge(head1, head2->next);
            return head2;
        }

    }
};
```
---
# 26. 树的子结构
```
class Solution {
public:
    bool HasSubtree(TreeNode* root1, TreeNode* root2)
    {
        if(! root1  || !root2 )
            return false;
        return isequal(root1,root2)|| isequal(root1->left,root2) || isequal(root1->right, root2);

    }
    
    bool isequal(TreeNode* root1, TreeNode* root2){
        if (root2 == nullptr)
            return true;
        if (root1 == nullptr)
            return false;
        if(root1->val != root2->val)
            return false;
        
        return isequal(root1->left, root2->left) && isequal(root1->right, root2->right);

    }
};
```

---
# 27. 二叉树的镜像
- 先序遍历或者后序遍历

```
class Solution {
public:
    void Mirror(TreeNode *root) {
        if(!root)
            return;
        if(root->left)
            Mirror(root->left);
        if(root->right)
            Mirror(root->right);
        
        TreeNode* tmp = root->left;
        root->left = root->right;
        root->right = tmp;
    }
};
```

---
# 28 对称的二叉树

```
class Solution {
public:
    bool isSymmetrical(TreeNode* root)
    {
        if(!root)
            return true;
        
        return issymmetric(root->left, root->right);
        
        
    
    }
    
   bool issymmetric(TreeNode* left, TreeNode* right){
       if(!left && ! right)
           return true;
       if(!left || !right)
           return false;
       if(left->val != right->val)
           return false;
        
       return issymmetric(left->left, right->right) && issymmetric(left->right, right->left);
   }

};
```
---
# 29. 顺时针打印矩阵

- 考虑最后两个循环的单行单列问题
```
class Solution {
public:
    vector<int> printMatrix(vector<vector<int> > matrix) {
        vector<int> res;
        if(matrix.empty())
            return res;
        int r2 = matrix.size()-1;  
        int c2 = matrix[0].size()-1;
        int r1=0;
        int c1=0;
        int i;
        
       while(r1<=r2 && c1<= c2){
           for(i=c1;i<=c2;i++){  // 右行
               res.push_back(matrix[r1][i]);
           }
     
           for(i = r1+1; i<= r2;i++){  // 下行
               res.push_back(matrix[i][c2]);
           }
            
           // 后两种防止单列情况
           if(r1!=r2)  // 单行
               for(i = c2-1; i>=c1; i--){  // 左行
                   res.push_back(matrix[r2][i]);
               }
            if(c1!=c2)  // 单列
               for(i=r2-1; i>r1;i--){ // 上行
                   res.push_back(matrix[i][c1]);
               }
           r1++;c1++;r2--;c2--;

       }
        return res;
        
    }
};
```



---
# 30. 包含 min 函数的栈

```
class Solution {
public:
    void push(int value) {
           if(minstack.empty())
               minstack.push(value);
            else if(value < minstack.top()){
                minstack.push(value);
            }
        datastack.push(value);
        
        
    }
    void pop() {

        if(datastack.top() == minstack.top())
            minstack.pop();
        datastack.pop();
        
    }
    int top() {
        return datastack.top();
        
    }
    int min() {
        return minstack.top();
    }
private:
    stack<int> datastack;
    stack<int> minstack;
};
```

---
# 31. 栈的压入、弹出序列

- 使用一个栈模拟

```
class Solution {
public:
    bool IsPopOrder(vector<int> in,vector<int> out) {
        if(in.empty() && !out.empty() || ( !in.empty() && out.empty() ))
            return false;
        stack<int> sin;
        int index = 0;
        int len = out.size();
        for(int i : in){
            sin.push(i);
            while(index < len && sin.top() == out[index])
            {
                sin.pop();
                index++;
            }
        }
        return sin.empty();
        
    }
};
```

---
# 32.1 从上往下打印二叉树
- 使用队列
```
class Solution {
public:
    vector<int> PrintFromTopToBottom(TreeNode* root) {
        vector<int> res;
        if(!root)
            return res;
        deque<TreeNode*> d;
        d.push_back(root);
        TreeNode* cur;
        while(!d.empty()){
            cur = d.front();
            res.push_back(cur->val);
            if(cur->left)
                d.push_back(cur->left);
            if(cur->right)
                d.push_back(cur->right);
            d.pop_front();
        }
        return res;
        
    }
};
```
---
32.2 把二叉树打印成多行
- 及时读取队列长度

```
class Solution {
public:
        vector<vector<int> > Print(TreeNode* root) {
            vector<vector<int>> res;
            if(!root)
                return res;
           deque<TreeNode*> d;
            d.push_back(root);
           TreeNode* cur;
           int len =0;
           while(! d.empty()){
               vector<int> tmp;
               len = d.size();
               for(int i=0;i<len;i++){
                    cur = d.front();
                    tmp.push_back(cur->val);
                    if(cur->left)
                        d.push_back(cur->left);
                    if(cur->right)
                        d.push_back(cur->right);
                    d.pop_front();  
               }
               res.push_back(tmp);
           }
            return res;
        }
    
};
```

---
# 32.3 按之字形顺序打印二叉树

- 使用两个栈模拟左右顺序
- 记录下行号，逆序就将vector,逆序一下

```
class Solution {
public:
    vector<vector<int> > Print(TreeNode* root) {
        vector<vector<int>> res;
        if(!root)
            return res;
        stack<TreeNode*> left;
        stack<TreeNode*> right;
        bool flag = false;
        TreeNode* cur;
        
        left.push(root);
        
        while(!left.empty() || !right.empty()){ // 有一个不为空
            vector<int> tmp;
            while(! left.empty()){
                cur = left.top();
                tmp.push_back(cur->val);
                if(cur->left)
                    right.push(cur->left);
                if(cur->right)
                    right.push(cur->right);  // 后进先出
                left.pop();
            }
            res.push_back(tmp);
            
            vector<int> tmp2;
            while(!right.empty()){
                 cur = right.top();
                 tmp2.push_back(cur->val);
                if(cur->right)
                    left.push(cur->right);
                if(cur->left)
                    left.push(cur->left);  // 后进先出
                right.pop();
            }
            
            if(!tmp2.empty())
                res.push_back(tmp2);

        }
        
        return res;
  
        
    }
    
};
```

---
# 33. 二叉搜索树的后序遍历序列
- 二叉搜索树：left < root < right

```
class Solution {
public:
    bool VerifySquenceOfBST(vector<int> sequence) {
        if(sequence.empty())
            return false;
        
        return isBST(sequence,0,sequence.size()-1);
    }
    
    
    bool isBST(vector<int> s, int start, int end){
        if(end - start <=1)  // 一个节点或者含一个子节点
            return true;
        int root = s[end];  //后序
        int cur = start;
        
        // 查找左子树
        while(cur < end && s[cur] < root)
            cur++;
        for(int i= cur;i<end;i++){  // 右子树存在小于root的值
            if(s[i]< root)
                return false;
        }
        return isBST(s,start,cur-1) && isBST(s,cur,end-1);
    }
};
```



---
# 34. 二叉树中和为某一值的路径

```
class Solution {
public:
    vector<vector<int> > FindPath(TreeNode* root,int expectNumber) {
       vector<int> tmp;
        tracking(root, expectNumber,tmp);
        return res;
        
    }
    vector<vector<int>> res;
    
    void tracking(TreeNode* root, int sum , vector<int> tmp){    // 使用赋值传值
        if(! root || sum<0) return;
        sum -= root->val;
        tmp.push_back(root->val);
        if(sum == 0 && root->left == NULL && root->right == NULL)
            res.push_back(tmp);
        else{
            tracking(root->left, sum,tmp);
            tracking(root->right, sum, tmp);
        }
    }
};
```

---

# 35. 复杂链表的复制
- 在每一个节点后面复制一个节点，指针同样指向复制节点

```
class Solution {
public:
    RandomListNode* Clone(RandomListNode* pHead)
    {
        if(!pHead)
            return NULL;
        // 对每个节点进行复制
        RandomListNode* cur = pHead;
        while(cur){
            RandomListNode* tmp = new RandomListNode(cur->label);    // 新建节点
            tmp->next = cur->next;
            cur->next = tmp;
            cur = tmp->next;
        }
        
        // random节点
        cur = pHead;
        while(cur){
            RandomListNode* tmp = cur->next;
            if(cur->random)
                tmp->random = cur->random->next;  // 指向复制节点
            cur = tmp->next;
        }
        
        // 分开
        cur = pHead;
        RandomListNode* head = cur->next; //复制节点的头结点
        while (cur->next != NULL) {
        RandomListNode* next = cur->next;
        cur->next = next->next;
        cur = next;
        }
        
        return head;
    }
};
```

---
36. 二叉搜索树与双向链表
37. 序列化二叉树
38. 字符串的排列
39. 数组中出现次数超过一半的数字
40. 最小的 K 个数
41.1 数据流中的中位数
41.2 字符流中第一个不重复的字符
42. 连续子数组的最大和
43. 从 1 到 n 整数中 1 出现的次数
44. 数字序列中的某一位数字
45. 把数组排成最小的数
46. 把数字翻译成字符串
47. 礼物的最大价值
48. 最长不含重复字符的子字符串
49. 丑数
50. 第一个只出现一次的字符位置
51. 数组中的逆序对
52. 两个链表的第一个公共结点
53 数字在排序数组中出现的次数
54. 二叉搜索树的第 K 个结点
55.1 二叉树的深度
55.2 平衡二叉树
56. 数组中只出现一次的数字
57.1 和为 S 的两个数字
57.2 和为 S 的连续正数序列
58.1 翻转单词顺序列
58.2 左旋转字符串
59. 滑动窗口的最大值
60. n 个骰子的点数
61. 扑克牌顺子
62. 圆圈中最后剩下的数
63. 股票的最大利润
64. 求 1+2+3+...+n
65. 不用加减乘除做加法
66. 构建乘积数组
67. 把字符串转换成整数
68. 树中两个节点的最低公共祖先













