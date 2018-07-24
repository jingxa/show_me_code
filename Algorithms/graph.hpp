/* graph 
	1. 有向图
	2. 无向图
	
	表示
	1. 领接表
	2. 邻接矩阵
		1. 有向图： 
		2. 无向图： 对称矩阵
*/


/*
	一、 搜索
	1.BFS:由近到远搜索，最小路径
	2.DFS: 
*/





/* 单元最短路径问题 
	1. Bellman-Ford
	2. Dijkstra
*/

// Dijkstra


/*
	回溯问题
	1. 
*/
// 回溯模板

void backtracking(P node , vector<P>&path, vector<vector<P>> & paths){
	if(!node)  // 无效节点
		return ;
	path.push_back(node);
	
	bool success = ..; // 成功条件
	if(success){
		paths.push_back(vector<P>(path.begin(), path.end()));// 不要返回
	}
	
	for(P next: all directions){
		backtracking(next, path ,paths);
	}
	path.pop_back();
	return;
}





// 例子：1 parentheses
/*
	n对数，生成有效的联合
	
*/

void parenthesesCombination(int leftRem, int rightRem, 
	string &path, vector<string>&paths){
	if(leftRem < 0 || rightRem <0)
		return;
	
	if(leftRem > 0){
		// 选择
		path.push_back('(');
		parenthesesCombination(leftRem-1 ,rightRem,path, paths);
		
		path.pop_back();
	}
	
	if(leftRem < rightRem){
		path.push_back(')');
		rightRem -= 1;
		if(rightRem == 0)
			paths.push_back(path);
		
		parenthesesCombination(leftRem,rightRem,path, paths);
		path.pop_back();
	}
}


vector<string> generateP(int n){
	vector<string> res;
	if(n<0)
		return res;
	string path;
	parenthesesCombination(n,n,path,res);
	return res;
}


// N queen

/* N*N , 每行或每列只能一个Queen */
bool checkvalid(int row1, int col1, int *rowcol){
	for(int row2=row1 -1;row2>=0;row2--){   // 行
		if(rowcol[row2] == col1) // 目标列被占据
			return false;
		if(abs(row1 -row2) == abs(rowcol[row2] - col1))
			return false;
	}
	return true;
}

const int GRID_SIZE;
void placeQ(int row, int rowcol[], vector<int*>& res){
	if(row == GRID_SIZE){
		int p[GRID_SIZE];
		for(int i=0;i<GRID_SIZE;i++){
			p[i] = rowcol[i];
		}
		res.push_back(p);
		return;
	}
	
	int col = 0; 
	for(col = 0;col<GRID_SIZE;col++){
	if(checkvalid(row,col, rowcol)){
		rowcol[row] = col;
		placeQ(row+1,rowcol,res);
	}
}

vector<int*>& placeQ(int n){
	GRID_SIZE = N;
	int[] rowcol = new int[n];
	vector<int*>& res;
	placeQ(0,rowcol,res);
	return res;
}



// 例子2： 部分排列
// 从0-9选3个打印
void Permutation(char* pstr, char* pbegin){
	assert(pstr && pbegin);
	
	if(*pbegin == '\0'){
		printf("%s\n",pstr);
	}else{
		for(char *pch = pbegin;*pch != '\0';pch++){
			swap(*pbegin,*pch);
			Permutation(pstr,pbegin+1);
			swap(*pbegin,*pch);
		}
	}
}

void swap(int& a, int& b){
	int temp = a;
	a = b;
	b = temp;
}

void Permutation(char str[], int index, int m ,int size){
	if(index ==m ){
		for(int i=0;i<m;i++)
			cout<<str[i];
		cout<<'\t';
	}else{
		for(int i=index; i< size ; i++){
			swap(str[index], str[i]);
			Permutation(str, index +1, m,size);
			swap(str[index],str[i]);
		}
	}
}




// 唯一二叉搜索树
/*
	
*/
vector<TreeNode*> generateTree(int n){
	return helper(1,n);
}

vector<TreeNode*> helper(int start ,int end){
	vector<TreeNode*> result;
	if(start > end){
		result.push_back(nullptr);
		return result
	}
	
	for(int i=0;i<=end; ++i){  // 每个点作为根节点
		vector<TreeNode*> leftTree = helper(start, i-1);  // 左边的为左子树
		vector<TreeNode*> rightTree = helper(i+1, end);  // 右边的为右子树
		for(int j=0;j<leftTree.size();++j){
			for(int k=0;k<rightTree.size();++k){
				TreeNode* root = new TreeNode(i);
				root->left = leftTree[j];
				root->right = rightTree[k]'
				result.push_back(root);
			}
		}
	}
	return result
}


// combination sum
// 两个整数n,k,返回所有的组合大小为k的组合从1,2，，n
vector<vector<int>> combin(int n, int k){
	vector<vector<int> res;
	if(n<=0)
		return res;
	
	vector<int > curr;
	DFS(res, curr, n, k,1);
	return res;
}

void DFS(vector<vector<int>>& res, vector<int> curr, 
	int n, int k, int level){
	if(curr.size() == k){
		res.push_back(curr);
		return ;
	}
	
	if(curr.size() > k)
		return ;
	
	for(int i=level; i<=n;i++){
		curr.push_back(i);
		DFS(res,curr,n,k,i+1);
		curr.pop_back();
	}
}


// 组合和
vector<vector<int> combinationSum(vector<int>& cand, int t){
	vector<vector<int>> res;
	if(cand.size() == 0 || t< 0)
		return res;
	vector<int> curr;
	sort(cand.begin(), can.end());
		
	BackTracking(res, curr, cand, t, 0);
	return res;
	
}

void BackTracking(vector<vector<int>>& res, vector<int> curr, 
	vector<int> cand, int t, int level){
	
	if(t == 0){
		res.push_back(curr);
		return ;
	}else if(t < 0)
		return;
	
	for(int i=level;i<cand.size(); ++i){ // 对每一个元素开始
		t -= cand[i];
		curr.push_back(cand[i]);
		BackTracking(res,curr, cand, t, i);
		curr.pop_back();
		t += cand[i];
	}
	
}


// 组合和， 但是每个元素只能一次
vector<vector<int> combinationSum2(vector<int>& cand, int t){
	vector<vector<int>> res;
	if(cand.size() == 0 || t< 0)
		return res;
	vector<int> curr;
	sort(cand.begin(), can.end());
		
	BackTracking(res, curr, cand, t, 0);
	return res;
	
}

void BackTracking(vector<vector<int>>& res, vector<int> curr, 
	vector<int> cand, int t, int level){
	
	if(t == 0){
		res.push_back(curr);
		return ;
	}else if(t < 0)
		return;
	
	for(int i=level;i<cand.size(); ++i){ // 对每一个元素开始
		t -= cand[i];
		curr.push_back(cand[i]);
		BackTracking(res,curr, cand, t, i);
		curr.pop_back();
		t += cand[i];
		while(i < cand.size() - 1 && cand[i] == cand[i+1] ) // 跳过重复值
			++i;
	}

	
}



// 电话号码
// 9键盘上对应数字的字符串组合

vector<string> letterCombinations(string digits){
	vector<string> res;
	vector<string dic;
	string curr; 
	dic.push_back(" ");
	dic.push_back(" ");
	dic.push_back("abc");
	dic.push_back("def");
	dic.push_back("ghi");
	dic.push_back("jki");
	dic.push_back("mno");
	dic.push_back("pqrs");
	dic.push_back("tuv");
	dic.push_back("wxyz");
	combinations(res, curr, digits, dic,0);
	return  res;
}

void combination(vector<string>& res, string curr, 
	string digits, vector<string> dic, int level)
{
	if(level == digits.size())
	{
		res.push_back(curr);
		return ;
	}
	int index = digits[level] -'\0';  // char to int
	for(int i=0;i<dic[index].size();++i){ // 每个数字符串解析，如“23”
		curr.push_back(dic[index][i]);
		combination(res,curr,digits,dic, level +1);
		curr.pop_back();
	}
	
	
}


// word Ladder
// 一个word组合中，给两个单词，每次单词中改变一个字母，
// 然后寻找一个最短路径从头到尾；
// 1. BFS
int ladderlength(string start, string end, unordered_set<string>&dict){
	int count = 1;
	unordered_set<string> check;
	queue<string> myque;
	
	myque.push(start);
	myque.push("");  // 保证myque不为空
	string word;
	string mid;
	
	while(!myque.empty()){
		word = myque.front();
		myque.pop();
		if(word == end){ // 到达节点
			return count;
		}
		
		if(word.size() == 0 && !myque.empty()){ // 空节点，判断下一个节点
			count++;
			myque.push("");
		}else if(!myque.empty()){
			for(int i=0;i<word.size() ;i++){
				mid = word;
				for(int j='a'; j< 'z';j++){
					mid[i] = (char)j; // 第i位变换
					if(check.find(mid) == check.end()
						&& dic.find(mid) != dic.end()){ // 字典中存在并且未使用过
							myque.push(mid);
							check.insert(mid);
						}
				}
			}
		}
	}
}
































