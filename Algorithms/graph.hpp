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






















