typedef	bool __rb_tree_color_type;
const __rb_tree_color_type	__rb_tree_red = false;  // red 为0
const __rb_tree_color_type	__rb_tree_black = true;  // black 1

struct __rb_tree_node_base{
	typedef	__rb_tree_color_type	color_type;
	typedef	__rb_tree_node_base*	base_ptr;
	
	color_type		color;  // 节点颜色
	base_ptr		parent;  // 父节点
	base_ptr		left;	// 左节点
	base_ptr		right;	// 右节点
	
	
	static base_ptr	minimum(base_ptr x){
		while(x->left !=9) x= x->left;   // 一直向左走，找到最小值
		return x;
	}
	
	static base_ptr	maxmum(base_ptr	x){
		while(x->right !=0) x = x->right;   // 一直向右走，就会找到最大值
		return x;
	}
	
};


template<class Value>
struct __rb_tree_node : public _rb_tree_node_base{
	typedef  __rb_tree_node<Value>*  link_type;
	Value value_field;  // 节点值
	
};








/**

	RB_tree 的迭代器：双向迭代器，不具备随机定位能力
	1. 类别
	2. 前进，后退
	3. 提领(dereference)
	4. 成员访问

*/

// 基层迭代器

struct __rb_tree_base_iterator{
	
	typedef __rb_tree_node_base::base_ptr			base_ptr;
	typedef bidirectional_iterator_tag  			iterator_category;
	typedef	ptrdiff_t								difference_type;
	
	base_ptr	node;   // 和容器产生联系
	
	
	
	//迭代器操作
	
	
	/**
		迭代器增加： 二乘搜索树
		1. 有右子节点，进入右子节点，然后向左，一直到底部
		2. 没有右节点， 向上找父节点,找到此子树为父节点左子树的父节点
		3、如果节点为最大节点，那么久一直向上回溯，直到header，
	*/
	void increment(){
		if(node->right !=0){ // 右节点存在，就向右走，然后一直往左子树走到底
			node = node->right;
			while(node->left != 0){
				node = node ->left;
			}
		}else{     // 没有右子节点 找到父节点，
			//如果现结点为父节点的右子节点，就一直上溯，直到“不为右子节点”为止
			base_ptr	y = node->parent; 
			while(node == y->right){
				node = y;
				y = y->parent;
			}
			if(node ->right != y)   // 此时右子节点不等于此时的父节点
				node = y;			// node即为父节点
		}
	}
	
	
	/**
		迭代器减操作
		1. 如果是红色节点，且祖父节点等于自己(即此节点为header节点)，则右子节点为答案
	*/
	void decrement(){
		// 如果父节点为红， 并且父节点的父节点等于自己
		if(node->color == __rb_tree_red &&
			node->parent->parent == node)
			node = node->right;	右子节点为上一个节点
		// 以上情况为node为header的时候(node 为end()的时候)
		// header的右节点为mostright，指向最大节点
		else if(node->left != 0){		// 如果有左子节点
			base_ptr	 y = node->left;	 // 令y指向左子节点
			while(y->right != 0){			// 一直往右走
				y = y->right;
			}
			node = y;						// 最后为答案
		}else{								// 既非根节点，也无左子节点
			base_ptr y= node->parent;		// 上溯
			while(node == y->left){			// 当此节点为父节点的左子节点
				node = y;					// 一直往上
				y = y->parent;
			}
			node = y;						// 父节点为答案
		}
	}
	
};




template<class Value, class Ref, class Ptr>
struct __rb_tree_iterator: public __rb_tree_base_iterator{
	typedef Value 										value_type;
	typedef	Ref											reference;
	typedef	Ptr											pointer;
	typedef	__rb_tree_iterator<Value,Value&,Value*>		iterator;
	typedef	__rb_tree_iterator<Value,const Value&, const Value*> 	const_iterator;
	typedef	__rb_tree_iterator<Value,Ref,Ptr>			self;
	typedef	__rb_tree_node*								link_type;
	
	__rb_tree_iterator(){}
	__rb_tree_iterator(link_type x){node = x;}
	__rb_tree_iterator(const link_type& it){node = it.node; }
	
	
	reference  operator*(){return link_type(node)->value_field;}
	#ifdef __SGI_STL_NO_ARROW_OPERATOR
	pointer	operator->() const {return &(*operator*());}	
	#endif
	
	self& operator++(){return increment(); return *this;}
	self  operator++(int){
		self tmp = *this;
		++ *this;
		return tmp;
	}
	
	self& operator--(){decrement(); return *this;}
	self  operator--(int){
		self tmp = *this;
		decrement();
		return tmp;
	}
	
};



/**
	RB_tree 的数据结构
*/

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc = alloc>
class rb_tree{
protected:
	typedef void* void_pointer;
	typedef	__rb_tree_node_base* base_ptr;
	
	// 内存管理
	typedef	__rb_tree_node<Value> rb_tree_node;
	typedef	simple_alloc<rb_tree_node, Alloc>	rb_tree_node_allocator;
	
	
	typedef	__rb_tree_color_type	color_type;
	
public:
	typedef	Key							key_type;
	typedef	Value						value_type;
	typedef	value_type*					pointer;
	typedef	const value_type*			const_pointer;
	typedef	value_type&					reference;
	typedef	const value_type&			const_reference;
	typedef	rb_tree_node*				link_type;
	typedef	size_t						size_type;
	typedef	ptrdiff_t					difference_type;
	
protected:
	link_type get_node(){return rb_tree_node_allocator::allocate();}
	void push_node(link_type p){rb_tree_node_allocator::deallocate(p);}
	
	link_type create_node(const value_type& x){
		link_type tmp = get_node();		//配置空间
		__STL_TRY{
			construct(&tmp->value_field,x);
		}
		
		__STL_UNWIND(put_node(tmp));
		return tmp;
	}
	
	link_type clone_node(link_type x){
		link_type tmp = create_node(x->value_field);
		tmp->color = x->color;
		tmp->left = 0;
		tmp->right = 0;
		return tmp;
	}
	
	void destroy_node(link_type	p){
		destroy(&p->value_field);   // 析构内容
		put_node(p);  //释放内存
	}
	
	
protected:

	size_type  node_count ;  // 追踪记录树的大小
	link_type	header;		// 实现上的技巧
	Compare		key_compare;		// 节点比较的大小准则， 应该是该function object；
	
	//以下三个函数方便取得header的成员
	link_type& root() const {return (link_type&) header->parent;}
	link_type& leftmost() const{return (link_type&)header->left;}
	link_type& rightmost() const{return (link_type&)header->right;}
	
	// 以下六个函数方便取x的成员
	static link_type& left(link_type x){
		return (link_type&)(x->left);
	}
	
	static	link_type& right(link_type x){
		return (link_type&)(x->right);
	}
	
	static link_type& parent(link_type x){
		return (link_type&)(x->parent);
	}
	
	static reference value(link_type x){
		return  x->value_field;
	}
	
	static const Key&(link_type x){
		return KeyOfValue()(Value(x));
	}
	
	static color_type& color(link_type x){
		return (color_type&)(x->color);
	}
	
	// 以下六个函数取得节点x 的成员
	
		static link_type& left(base_ptr x){
		return (link_type&)(x->left);
	}
	
	static	link_type& right(base_ptr x){
		return (link_type&)(x->right);
	}
	
	static link_type& parent(base_ptr x){
		return (link_type&)(x->parent);
	}
	
	static reference value(base_ptr x){
		return  ((link_type)x)->value_field;
	}
	
	static const Key&(base_ptr x){
		return KeyOfValue()(Value((link_type)x)));
	}
	
	static color_type& color(base_ptr x){
		return (color_type&)(link_type(x)->color);
	}
	
	
	// 求极大值和极小值
	static link_type	minimum(link_type x){
		return (link_type) __rb_tree_node_base::minimum(x);
	}
	
	static link_type  maximum(link_type x){
		return (link_type) __rb_tree_node_base::maximum(x);
	}
	
private:
	void init(){
		header = get_node();		// 产生一个新节点令header指向它
		color(header) = __rb_tree_red;  //令header为红色，用来区分header
										// 用来区分header和root,在iterator.iterator++
		root() =0;
		leftmost() = header;  // 令header的左子节点 为自己
		rightmost() = header; // 令header的右子节点为自己
		
	}
	
	
public:
	rb_tree(const Compare comp = Compare())
		:node_count(0), key_compare(comp){init();}
	
	~rb_tree(){
		clear();
		put_node(header);
	}
	
	rb_tree<Key,Value,KeyOfValue, Compare, Alloc>&
		operator=(const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& x);
		
public:

	Compare key_comp() const {return key_compare;}
	iterator begin() {return leftmost();}   // RB开始为最小节点处
	iterator end()	{return header;}    // RB的终点为 header所指处
	bool empty() const{return node_count == 0;}
	size_type size() const{return  node_count;}
	size_type	max_size()	const {return size_type(-1);}
	
	

private:
	// 真正的插入操作
	iterator __insert(base_ptr x_,base_ptr y_, const Value v){
		// 参数x_为新节点 ,参数y_为插入点的父节点，v为新值
		link_type x = (link_type) x_;
		link_type y = (link_type) y_;
		link_type z;
		
		// key_compare 键值大小比较准则
		if(y == header || x!= 0 || key_compare(KeyOfValue()(v), key(y)))
		{
			z = create_node(v);  // 产生一个新节点
			left(y) = z;  // 使得y为header的时候，leftmost = z;
			if(y == header){
				root() = z;
				rightmost() = z;
			}else if(y == leftmost()){ // 如果y为最左节点
				leftmost() = z;
			}else{
				z = create_node(v);
				right(y) =z;
				if(y == rightmost()){
					rightmost() = z;  // 维护rightmost(), 使他永远指向最右节点
				}
			}
			parent(z) = y;  // 设定新节点的父节点
			left(z) = 0; // 设定新节点的左子节点
			right(z) = 0; // 设定新节点的右子节点
			
			
			
		}
	}
	
public:
	// 将x插入到RB_tree之中(保持节点独一无二)
	pair<iterator, bool> insert_unique(const value_type& x){
		link_type y = header;
		link_type x = root();
		bool comp = true;
		while( x!= 0){
			y = 0;
			comp = key_compare(KeyOfValue()(v),key(x));
			x = comp?left(x) : right(x); 
			// 以上， 遇到大往左，“小于等于”往右
			
		} // 离开while, y所指向的即插入点的父节点（此时的它为叶节点）
		
		
		iterator j = iterator(y);  // 迭代器j指向插入点的父节点y
		if(comp) // 如果 离开while时comp为真 ，（表示大，将插到左侧）
		{
			if(j == begin()){  // 如果插入点的父节点为最左节点
				return	pair<iterator,bool>(__insert(x,y,v), true);
				// 以上， x为插入点，y为插入点的父节点，v为新值
			}else{   // 否则（插入点父节点不为最左节点）
				--j;   // 调整j,回头
			}
		}
		if(key_compare(key(j.node),KeyOfValue()(v))))
			// 小于新值（表示遇到小，插入右侧）
			return pair<iterator, bool>(__insert(x,y,v), true);
		
		// 进行到此，表示新值和数中的键值重复，那么就不插入
		return pair<iterator, bool>(j, false);
	}
	// 将节点插入到RB_tree中，允许重复
	iterator inset_equal(const value_type& x){
		link_type y = header;
		link_type x = root();   // 从根节点开始，往下开始寻找
		while( x != 0){
			y =x; 
			x = key_compare(KeyOfValue()(v), key(x))? left(x): right(x);
			// 以上， 遇到大往左，“小于等于”往右
		}
		return __insert(x,y,v);
	}
	

	
	
public:

		
	
	
};



/**

	RB_tree 调整
	
*/


inline void __rb_tree_rebalance(__rb_tree_node_base* x, 
	__rb_tree_node_base*& root){
	x->color = __rb_tree_red;
	while(x!= root && x->parent->parent->color == __rb_tree_red){
		// 父节点为红
		if(x->parent == x->parent->parent->left){
			// 父节点为祖父节点的左子节点
			__rb_tree_node_base* y =x->parent->parent->right; // 令y为伯父节点
			if(y && y->color == __rb_tree_red){ // 伯父节点存在，且为红色
				x->parent->color = __rb_tree_black; // 修改伯父节点为黑
				y->color == __rb_tree_black;  // 父节点为黑
				x->parent->parent->color = __rb_tree_red; // 祖父节点为红
				x= x->parent->parent;
				
			}else{ // 伯父节点不存在,或者为黑
				if(x == x->parent->right){  // 新节点为父节点的右子节点
					x = x->parent;
					__rb_tree_rotate_left(x, root);  // 左旋，第一参数为左旋					
				}
				x->parent->color = __rb_tree_black;   // 改变颜色
				x->parent->parent->color = __rb_tree_red;
				
				__rb_tree_rotate_right(x->parent->parent,root); //第一参数为右旋点
			}
			
		}else{  // 父节点为祖父节点的右子节点
			__rb_tree_node_base* y = x->parent->parent->left; // 令y为伯父节点
			if(y && y->color == __rb_tree_red){ // 伯父节点为红
			
				x->parent->color = __rb_tree_black; // 父亲颜色改为黑
				y->color = __rb_tree_black;			// 伯父节点为黑
				x->parent->parent->color = __rb_tree_red;  // 祖父节点为红
				x = x->parent->parent;
			}else{  // 无伯父节点。或者为黑
				if(x == x->parent->left){  // 新节点为父节点的左子节点
				x = x->parent;
				__rb_tree_rotate_right(x, root);
				}
				x->parent->color = __rb_tree_black;  // 改变颜色
				x->parent ->parent->color = __rb_tree_red;
				__rb_tree_rotate_left(x->parent->parent, root);  
			}
		}
	} // while结束
	root->color = __rb_tree_black;  // 根节点永远为黑
}



// 全局函数

inline __rb_tree_rotate_left(__rb_tree_node_base* x, 
								__rb_tree_node_base*& root){
	// x为旋转点
	__rb_tree_node_base* y = x->right;  // 令y为旋转点的右子节点
	x->right = y->left;
	if(y->left != 0){
		y->left->parent = x;  // 回马枪设定父节点
	}
	y->parent = x->parent;
	
	// 令y 完全顶替x的地位
	if(x == root){  // x为根节点
		root = y;  
	}else if(x == x->parent->left){ // x 为父节点的左子节点
		x->parent->left = y; 
	}else{  						// x为父节点的右子节点
		x->parent->right = y;
	}
	y->left = x;
	x->parent y;
}



// 全局函数
// 新节点必为红节点，如果插入处父节点为红色节点必须旋转

inline  void __rb_tree_rotate_right(__rb_tree_node_base* x,
									__rb_tree_node_base*& root){
	// x为旋转点
	__rb_tree_node_base* y = x->left;  // y 为旋转点的左节点
	x->left = y->right;
	if(y->right != 0){
		y->right ->parent = x;  // 回马枪设定父节点
	}
	y->parent = x->parent;
	
	// 令y完全顶替x的地位
	if(x ==root){
		root = y;
	}else if(x == x->parent->right){    // x为父节点的右子节点
		x->parent->right  = y;   
	}else{								// x为父节点的左子节点
		x->parent->left = y;
	}
	y->right = x;
	x->parent = y;
										
}







