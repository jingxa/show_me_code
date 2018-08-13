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
## 1.2 633. Sum of Square Numbers[leetcode]

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

## 1.3 345. Reverse Vowels of a String (Easy)

```

```
