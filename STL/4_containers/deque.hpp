template<class T, class Alloc = alloc, size_t BufSiz = 0>
class deque{	
public:
	typedef T 						value_type;
	typedef value_type* 			pointer;
	typedef size_t					size_type;
		
protected:
	// 元素的指针的指针
	typedef pointer* 				map_pointer;
	
public:
	typedef __deque_iterator<T, T&,T*,BufSiz> iterator;
	
protected:
	//  data  member
	iterator 	start;    // 表示第一个节点
	iterator    finish;   // 表示最后一个节点
	
	map_pointer map;    // 指向map, map是连续空间，每个元素是指针，指向一块缓冲区
	size_type map_size;	// map空间大小
	
protected:
	// 专属空间配置器  ,每次配置一个元素大小
	typedef simple_alloc<value_type, Alloc> data_allocator;
	// 每次配置一个指针大小
	typedef simple_alloc<pointer, Alloc> map_allocator;

	

public:
	// 构造函数
	deque(int n, const value_type& value)
		: start(),finish(), map(0), map_size(0){
			fill_initialize(n, value);
		}
		
	
protected:
	void fill_initialize(size_type n, const value_type& value){
		create_map_and_nodes(n);   // 吧deque的结构都产生冰球安排好
		map_pointer cur;
		__STL_TRY{
			// 为每个节点的缓冲区设定初始值
			for(cur = start.node;cur< finish.node; ++cur){
				uninitialized_fill(*cur, *cur+buffer_size(),value);
			}
			// 最后一个节点的设定不同（尾部备用空间不用设定初始值）
			uninitialized_fill(finish.first,finish.cur,value);
		}catch(...){
			...
		}
	}
	
	
	
protected:
	// map 重新配置
	void reserve_map_at_back()size_type node_to_add =1){
		if(node_to_add +1 > map_size - (finish.node - map)){
			// 如果map尾端的节点空间不足，重新配置更大的map空间
			reallocate_map(node_to_add,false);
		}
	}
	
	void reserve_map_at_front(size_type node_to_add =1){
		if(node_to_add > start.node - map){
			reallocate_map(node_to_add, true);
		}
	}
	
	
	// 重新配置map
	void reallocate_map(size_type node_to_add, bool add_to_front){
		size_type old_num_nodes = finish.node - start.node;
		size_type new_num_nodes = old_num_nodes + node_to_add;
		
		map_pointer new_nstart;
		if(map_size > 2*new_num_nodes){ // 原来空间足够大，中间平移
			new_nstart = map + (map_size - new_num_nodes) /2
						+ (add_to_front ? node_to_add  : 0); // 
			if(new_nstart <start.node){
				copy(start.node, finish.node + 1,new_nstart);
			}else{
				copy_backward(start.node,finish.node, new_nstart + old_num_nodes);
			}
		}else{
			size_type new_map_size = map_size + max(map_size, node_to_add) + 2);
			// 配置一块新的空间
			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_nstart = new_map + (new_map_size - new_num_nodes) /2
						+ (add_to_front? node_to_add : 0);
			// 把原来map内容拷贝过来
			copy(start.node, finish.node+1, new_nstart);
			
			// 释放原来map
			map_allocator::deallocate(map, map_size);
			map = new_map;
			map_size = new_map_size;
			
		}
		// 重新设定迭代器
		start.set_node(new_nstart);
		finish.set_node(new_nstart + old_num_nodes -1);
		
	}
	
	
	
	
protected:
	
	//负责产生并且安排好deque的结构
	void create_map_and_nodes(size_type num_elements){
		// 需要节点数= （元素个数 / 每个缓冲区的大小） + 
		// 刚好整除，多陪一个节点
		size_type num_nodes = num_elements / buffer_size() +1;
		
		// 一个map管理的节点，最少为8个，最多是“所需节点+2”
		// 前后预留一个，扩充时使用
		map_size = max(initial_map_size(),num_nodes +2);
		map = map_allocator::allocate(map_size);
		
		// 令nstart和nfinish指向map拥有的全部节点的中间区段
		// 保持在中央，可是头尾两端的扩充能量一样大，
		map_pointer nstart = map + (map_size - num_nodes) /2;
		map_pointer nfinish = nstart + num_nodes -1;
		
		map_pointer cur;
		__STL_TRY{
			// 为map内的每个节点配置缓冲区，
			for(cur = nstart; cur <=nfinish; ++cur){
				*cur = allocate_node();
			}
		}catch(...){
			// "commit of rollback"
			...
		}
		start.set_node(nstart);
		finish.set_node(nfinish);
		start.cur  = start.first;
		finish.cur = finish.first + num_elements % buffer_size();

	}
	
	
	// push_bac_aux 先配置一整块新的缓冲区，然后设置新元素，更改finish
	void push_bac_aux(const value_type& t){
		value_type t_copy=t;
		reserve_map_at_back();  // 若符合条件重现换一个map
		*(finish.node + 1) = allocate_node(); // 新配置一个节点
		__STL_TRY{
			construct(finish.cur,t_copy);
			finish.set_node(finish.node + 1);  // 改变finish,指向新节点
			finish.cur = finish.first;
		}
		__STL_UNWIND(deallocate_node(*(finish.node + 1)));
	}
	
	// push_front_aux 第一缓冲区无备用空间
	void push_front_aux(const value_type& t){
		value_type t_copy = t;
		reserve_map_at_front();   // 若符合条件重现换一个map
		*（start.node -1) = allocate_node();  // 配置新节点
		__STL_TRY{
			start.set_node(start.node - 1);
			start.cur = start.last - 1; // 设定start的状态
			construct(start.cur, t_copy);
		}catch(...){
			// "commit or rollback"
			start.set_node(start.node + 1);
			start.cur = start.first;
			deallocate_node(*(start.node - 1);
		}
	}
	
	
	// pop_back_aux
	void pop_back_aux(){
		deallocate_node(finish.first); // 释放最后一个缓冲区
		finish.set_node(finish.node -1); // 调整finish的指向
		finish.cur = finish.last -1;
		destroy(finish.cur);   // 析构该元素
	}
	
	//pop_front_aux
	void pop_front_aux(){
		destroy(start.first);  // 第一缓冲区第一个元素析构
		deallocate_node(start.first) ;  // 释放第一缓冲区
		start.set_node(start.node + 1);
		start.cur = start.first;  // 下一个缓冲区第一个元素
	}
	
		
	// insert_aux
	iterator insert_aux(iterator pos, const value_type& x){
		difference_type index = pos - start; // 插入之间的元素个数
		value_type x_copy = x;
		if(index < size() /2 ){  // 插入之前的元素个数较少
			push_front(front());   // 在最前端加入与第一元素同值的元素
			iterator front1 = start;
			++front1;
			iterator front2 = front1;
			++front2;
			pos = start + index;
			iterator pos1 = pos;
			++pos1;
			copy(front2, pos1, front1);  // 元素移动
 		}else{  // 插入点之后元素比较少
			push_back(back());   // 尾端插入与最后一个元素一样的元素
			iterator back1 = finish;
			-- back1;
			iterator back2 = back1;
			-- back2;
 			pos = start + index;
			copy(pos, back2, back1);  // 元素移动
		}
		*pos = x_copy;
		return pos;
	}
	
	
public:

	void push_back(const value_type& t){
		if(finish.cur != finish.last-1){
			// 最后缓冲有一个以上的备用空间
			construct(finish.cur,t);
			++finish.cur;	// 调整最后缓冲区的状态
		}else{  // 最后缓冲区只剩一个备用空间
			push_back_aux(t);
		}
	}
	
	void push_front(const value_type& t){
		if(start.cur != start.first){
			construct(start.cur -1, t);  // 直接在备用空间构建元素
			-- start.cur;
		}else{
			push_front_aux(t);
		}
	}
	
	// pop 
	void pop_back(){
		if(finish.cur != finish.start){ // 最后缓冲区还有一个以上的元素
			-- finish.cur;  // 调整指针；
			destroy(finish.cur);// 最后元素析构
		}else{
			 // 最后缓冲区没有元素
			pop_back_aux(); //  释放缓冲区
		}
	}
	
	
	void pop_front(){
		if(start.cur != start.last -1){// 第一缓冲区还有多个元素
			destroy(start.cur);
			++ start.cur;
		}else{  // 第一缓冲区只有一个元素
			pop_front_aux();  // 释放缓冲区
		}
	}
	
// 清除deque，保留一个缓冲区
	void clear(){
		// 针对头和尾部中间的缓冲区，都是饱满的
		for(map_pointer node = start.node +1; node<finish.node ; ++node){
			// 析构所有元素
			destroy(*node, *node+buffer_size());
			// 释放缓冲区
			data_allocator::deallocate(*node,buffer_size());
		}
		
		if(start.node !=finish.node){  // 还有头和尾部两个缓冲区
			destroy(start.cur, start.last);   // 将头部缓冲区所有元素析构
			destroy(finish.first, finish.cur); // 尾部缓冲区析构
			// 释放掉尾部缓冲区，头部缓冲区保留
			data_allocator::deallocate(finish.start, buffer_size());
		}else{  // 只有一个缓冲区
			destroy(start.cur, finish.cur); // 所有元素析构
			finish = start; // 调整状态
		}
		
	}
	
	
	// erase 操作
	iterator erase(iterator pos){
		iterator next = pos;
		++next;
		difference_type index = pos - next;  // 清除点之前的元素个数
		if(index <(size() >>1 )){
			copy_backward(start,pos, next); // 移动清除之前的元素
			pop_front();  // 消除最前的一个元素
		}else{
			copy(next, finish, pos);  // 移动清除之后的元素
			pop_back();
		}
		return start+ index;
	}
	
	
	// erase  [first, last)
	void erase(iterator first, iterator last){
		if(first == start && last == finish){  // 如果是清除整个deque
			clear();
			return finish;
		}else{
			difference_type n = last - first;
			difference_type elems_before = first - start;  // 清除区间前方的元素个数
			if(elems_before < (size() -n) /2){ // 前方元素比较少
				copy_backward(start, first, last);
				iterator new_start = start +n;   // 新起点
				destroy(start, new_start);       // 移动完毕，冗余元素析构
				for(map_pointer cur = start.node; cur<new_start.node; ++cur){
					data_allocator::deallocate(*cur, buffer_size());
				}
				start = new_start; // 设定deque的新起点
			}else{
				copy(last,finish, first);  // 向前移动后方元素
				iterator new_finish = finish - n; 
				destroy(new_finish , finish);   // 移动完成冗余析构
				
				for(map_pointer cur = new_finish.node +1;cur <=finish.node; ++cur){
					data_allocator::deallocate(*cur, buffer_size());
				}
				finish = new_finish ; // deque 新尾部
				
			}
			return start + elems_before;
			
		}
	}
	
	
	// insert 	
	iterator isnert(iterator position, const value_type& x){
		if(position.cur == start.cur){// 插入最前端
			push_front(x);
			return start;
		}else if(position.cur = finish.cur){  // 冲入最尾部
			push_back(x);
			iterator tmp = finish;
			--tmp;
			return tmp;
			
		}else{
			return insert_aux(position, x);
		}
	}
	


public:
	// 基本访问
	iterator begin(){return start;}
	iterator end(){return finish;}
	
	reference operator[](size_type n){
		return start[difference_type(n)];  // 调用__deque_iterator<>::operator[]
	}
	
	reference front(){return *start;}
	reference back() {
		iterator tmp = finish;
		--tmp;		// 调用 __deque_iterator<>::operator--
		return *tmp;
	}
	
	size_type size() const{return finish - start;}
	size_type max_size() const{return size_type(-1);} // ??
	bool empty() const{return finish == start; }
		
	
};



/**
	deque 的迭代器
	
*/

template<class T, class Ref, class Ptr, size_t BufSiz>
struct __deque_iterator{
	typedef __deque_iterator<T, T&,T*, BufSiz>				iterator;
	typedef	__deque_iterator<T, const T&, const T*, BufSiz> const_iterator;
	static	size_t buffer_size() {return __deque_buf_size(BufSiz, sizeof(T));}
	
	// 未继承std::iterator, 必须自行添加iterator_category
	typedef random_access_iterator_tag  			iterator_category;
	typedef T										value_type;
	typedef Ptr										pointer;
	typedef	Ref										reference;
	typedef	size_t									size_type;
	typedef	ptrdiff_t								difference_type;
	typedef T**										map_pointer;
	
	typedef	__deque_iterator						self;
	
	// 保持与容器的联结
	T*	cur;  // 迭代器指向缓冲区的current 元素
	T*  first;	// 指向缓冲区的头；
	T*  last; 	// 指向缓冲区的尾巴（含备用空间）
	map_pointer node;   // 指向管理中心
	
	
public:
	void set_node(map_pointer new_node){
		node = new_node;
		first =*new_node;	// 缓冲区开头
		last = first + difference_type(buffer_size());
	}

	// 运算符重载
	reference operator*() const{return *cur;}
	
	pointer operator->() const{return &(operator*());}
	
	difference_type operator-(const self& x) const{
		return difference_type(buffer_size()) * (node - x.node -1) + 
			(cur - first) + ( x.last - x.cur);
	}
	
	self& operator++(){
		++cur;
		if(cur == last){ // 到达尾部， 切换到下一节点
			set_node(node+1);
			cur = first;
		}
		return *this;
	}
	
	self operator++(int){
		self tmp  = *this;
		++*this;
		return tmp;
	}
	
	self operator--(){
		if(cur == first){
			set_node(node -1);
			cur = last;
		}
		-- cur;
		return *this;
	}

	
	self operator--(int){
		self tmp = *this;
		-- *this;
		return tmp;
	}
	
	// 随机存取
	self& operator+= (difference_type n){
		difference_type offset = n +(cur - first);
		//  first< cur -n <= offset <= cur +n < last 
		// 目标位置在同一缓冲区
		if(offset >= && offset < difference_type(buffer_size())){ 
			cur += n;
		}else{
		// 目标的位置不在同一缓冲区
			difference_type node_offset = 
				offset > 0 ? offset / difference_type(buffer_size())
					: difference_type((-offset - 1) / buffer_size()) -1;
		
			// 切换到正确的节点
			set_node(node + node_offset);
			// 切换到正确的元素
			cur = first + (offset - node_offset * difference_type(buffer_size()))
		}
		return *this;
	}
	
	self operator+(difference_type n) const{
		self  tmp = *this;
		return tmp += n;
	}
	
	self& operator -=(difference_type n){
		return *this += -n;
	}
	
	self operator-(difference_type n ){
		self tmp = *this;
		return tmp -= n;
	}
	
	// 随机存取[]
	reference operator[] (difference_type n) const {
		return *(*this + n);
	}
		
	bool operator== (const self& x)const{
		return cur == x.cur;
	}
	bool operator!=(const self& x)const{
		return !(*this == x);
	}
	boll operator<(const self& x)const{
		return (node == x.node) ? (cur < x.cur) :(node < x.node);
	}
		
		
	
	
	
};


/** 
	全局函数  __deque_buf_size
	1. 如果n 不为0， 传回n， bcui buffer size 有用户自定义
	2. 如果n 为0， 表示buffer size 使用默认值
		- 如果sz（元素大小，sizeof(value_type)）小于512， 传回512/sz
		- 如果sz 不小于 512 ， 传回1
*/

inline  size_t  __deque_buf_size(size_t n, size_t sz){
	return n!=0 ? n : (sz < 512 ? size_t(512/sz) : size_t(1));
}






/**

	数据结构
	
*/





/**
	构造和内存管理
*/










