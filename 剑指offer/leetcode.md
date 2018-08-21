- 参考资料
- [leetcode](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/Leetcode%20%E9%A2%98%E8%A7%A3.md)

---

分类： 
- 双指针
- 排序
- 贪心算法
- 二分查找
- 分治
- 搜索
- 动态规划

- 数学
- 数据结构相关

---
# 1 双指针
## 1.1 167. Two Sum II - Input array is sorted
- [167. Two Sum II - Input array is sorted](https://leetcode.com/problems/two-sum-ii-input-array-is-sorted/description/)

<details><summary>code</summary>

```
class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        vector<int> res;
        if(numbers.empty())
            return res;
        // 双边搜索
        int left=0, right = numbers.size() -1;
        while(left <right){
            if(numbers[left] + numbers[right] < target)
                left++;
            else if(numbers[left] + numbers[right] > target)
                right --;
            else 
                break;
        }
        if(left >= right)
            return res;
        
        res.push_back(left+1);
        res.push_back(right+1);
        return res;
        
        
            
    }
};
```

</details>

## 1.2 633. Sum of Square Numbers[leetcode]


<details><summary>code</summary>

```
class Solution {
public:
    bool judgeSquareSum(int c) {
        if(c < 0)
            return false;
        int mid = static_cast<int>(sqrt(c));
        cout<<mid<<endl;
        int l=0, r = mid;
        while(l<=r){
            if(l*l + r*r < c)
                l++;
            else if(l*l + r*r > c)
                r--;
            else
                break;
        }
        if (l>r)
            return false;
        
        return true;
        
    }
};

```

</details>

## 1.3 345. Reverse Vowels of a String (Easy)
- 逆转两个元音字母

<details><summary>code</summary>

```

class Solution {
public:
    string reverseVowels(string s) {
        if(s.empty())
            return "";

       // cout<<"start.."<<endl;
        int l=0,r=s.size()-1;
        while(l<r){
            while(!res.count(s[l]))
                l++;
            while(!res.count(s[r]))
                r--;
            if(l<r)
                swap(s[l++],s[r--]);
            
        }
       // cout<<"s:"<<s<<endl;
        return s;
    }
public:
    set<char> res;
    Solution(){
        res.insert('a');
        res.insert('A');
        res.insert('e');
        res.insert('E');
        res.insert('i');
        res.insert('I');
        res.insert('o');
        res.insert('O');
        res.insert('U');
        res.insert('u'); 
    }
    
    
    
};

```

</details>


## 1.4 680. Valid Palindrome II

<details><summary>code</summary>

```
class Solution {
public:
    bool validPalindrome(string s) {
        if(s.empty() || s.size() ==1)
            return true;
        
        int i=0, j= s.size()-1;
        int cnt=0;
        bool t = ispalin(s,i,j,cnt);

        return t;

    }
    
    
    bool ispalin(string& s , int i, int j,int cnt){
        if (cnt > 1)
            return false;
            if(i>j)
            return true;

        if(s[i] != s[j]){
            cnt++;
            return  ispalin(s,i+1,j,cnt) || ispalin(s,i,j-1,cnt);
        }
        else 
            return ispalin(s,i+1,j-1,cnt);
    }
};
```

</details>

## 1.5 88. Merge Sorted Array (Easy)

<details><summary>code</summary>

```
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        if(m+n > nums1.size() || n==0)
            return ;
        int len = nums1.size()-1;
        
        if(m==0){
            for(int i=0;i<n;i++)
                nums1[i] = nums2[i];
        }


        int i=m-1,j=n-1;
        while((i>=0 || j>=0) && len>=0 ){
            if(i<0)
            {
                nums1[len] = nums2[j];
                j--;
            }else if(j<0)
                break;
            else if(nums1[i] >= nums2[j]){
                nums1[len]= nums1[i];
                i--;
            }else{
                nums1[len] = nums2[j];
                j--;
            }
            len--;
        }

    }
};

```

</details>

## 1.6 141. Linked List Cycle (Easy)

<details><summary>code</summary>

```
class Solution {
public:
    bool hasCycle(ListNode *head) {
        if(!head || head->next == nullptr)
            return false;
        ListNode* low , *fast;
        low = head;
        fast = low->next;
        while(low && fast && fast->next){
            if(low == fast)
                return true;
            low = low->next;
            fast = fast->next->next;
        }
        return false;
        
    }
};
```

</details>

## 1.7 524. Longest Word in Dictionary through Deleting (Medium)

<details><summary>code</summary>

```
class Solution {
public:
    string findLongestWord(string s, vector<string>& d) {
        if(s.size() == 0 || d.size() == 0)
            return "";
        int maxlen=0;
        int len1 = s.size();
        int tmp=0;
        int idx=-1;
        sort(d.begin(),d.end());
        for(int i=0;i<d.size();i++){
            int len2 = d[i].size();
            if(len1 < len2)
                continue;
            tmp = isstr(s, d[i]);
            if(tmp == len2 && tmp > maxlen)
            {
                maxlen = tmp;
                idx = i;
             }
        }
        return idx== -1 ? "" : d[idx];
        
    }
    
    int  isstr(string s1, string s2){
        int i=0,j=0;
        while(i<s1.size() && j < s2.size()){
            if(s1[i] == s2[j])
                j++;
            i++;
        }
        return j;
    }
    
};
```

</details>


# 2. 排序

## 2.1 215. Kth Largest Element in an Array (Medium)

<details><summary>code</summary>

```
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        if(nums.size() == 0)
            return 0;
        
        priority_queue<int> pq;
        for(int n : nums){
            pq.push(n);
        }
        
        while( k>1)
        {
            pq.pop();
            k--;
        }
        
        return pq.top();
        
    }
};

```

</details>


## 2.2 347. Top K Frequent Elements (Medium)

<details><summary>code</summary>

```
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int, int> freq;
        for (int i = 0; i < nums.size(); i++)
            freq[nums[i]]++;
        
        int range_min = 0;
        int range_max = 0;
        for (auto it = freq.begin(); it != freq.end(); it++)
        {
            range_min = min(range_min, it->second);
            range_max = max(range_max, it->second);
        }
        
        vector<vector<int>> buckets(range_max - range_min + 1);
        for (auto it = freq.begin(); it != freq.end(); it++)
        {
            buckets[it->second - range_min].push_back(it->first);
        }
        
        vector<int> result;
        for (int i = int(buckets.size())-1; i >= 0; i--)
        {
            for (int j = 0; j < buckets[i].size(); j++)
            {
                result.push_back(buckets[i][j]);
                if (result.size() == k)
                    return result;
            }
        }
        
        return result;
    }  
        
};

```

</details>

## 2.3 451. Sort Characters By Frequency (Medium)

<details><summary>code</summary>

```
class Solution {
public:
    string frequencySort(string s) {
        int len = s.size();
        map<char, int> mapping;
        int maxn = 0;
        for(char c : s){
            mapping[c] ++;
            if(maxn < mapping[c])
                maxn = mapping[c];
        }

        vector<vector<char>> buckets(maxn+1);
        
        for(auto it = mapping.begin();it != mapping.end(); it++){  // 桶排序
            buckets[it->second].push_back(it->first);
        }
        
        string res;
        for(int i= maxn; i>=0; i-- ){
            for(char c : buckets[i]){
                for(int j = 0;j<i;j++){
                    res += c;
                }
            }        
        }
        
        return res;

    }
};

```

</details>

## 2.4 75. Sort Colors (Medium)

> 荷兰国旗问题
> 荷兰国旗包含三种颜色：红、白、蓝。
> 有三种颜色的球，算法的目标是将这三种球按颜色顺序正确地排列。
> 它其实是三向切分快速排序的一种变种，在三向切分快速排序中，每次切分都将数组分成三个区间：小于切分> 元素、等于切分元素、大于切分元素，而该算法是将数组分成三个区间：等于红色、等于白色、等于蓝色。

<details><summary>code</summary>

```
class Solution {
public:
    void sortColors(vector<int>& nums) {
        if(nums.empty())
            return;
        int zero = 0, one = 0, two = nums.size()-1;
        while(one < two){
            if(nums[one] == 0){
                swap(nums[zero],nums[one]);

                zero++;
                one++;
            }
            else if(nums[one] == 2){
                swap(nums[one],nums[two]);

                two--;
            }
            else
                one++;
        }  

    }

};

```

</details>



# 3 贪心算法
- 保证每次操作都是局部最优的，并且最后得到的结果是全局最优的。

## 3.1 455. Assign Cookies (Easy)
<details><summary>code</summary>

```
class Solution {
public:
    int findContentChildren(vector<int>& g, vector<int>& s) {
        sort(g.begin(),g.end());
        sort(s.begin(),s.end());
        
        int i=0,j=0;
        while(i< g.size() && j < s.size()){
            if(g[i] <= s[j])
                i++;  // 满足一个孩子，下一个孩子
            j++;    // 下一块饼干
        }
        return i;
    }
};
```

</details>


## 3.2 435. Non-overlapping Intervals (Medium)

<details><summary>code</summary>

```
```

</details>

## 3.3


<details><summary>code</summary>

```

```

</details>

## 3.4


<details><summary>code</summary>

```
```

</details>


## 3.5

<details><summary>code</summary>

```
```

</details>


## 3.6

<details><summary>code</summary>

```
```

</details>

## 3.7

<details><summary>code</summary>

```
```

</details>

## 3.8
<details><summary>code</summary>

```
```

</details>







