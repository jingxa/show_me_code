template<class T, class Alloc = alloc>
class vector{
public:
	// vector的嵌套定义
	typedef T				value_type;
	typedef value_type* 	pointer;
	typedef value_type&		reference;
	typedef	value_type*		iterator;
	typedef size_t 			size_type;
	typedef	ptrdiff_t		difference_type;
	
protected:
	// 以下，simple_alloc 是SGI STL的空间配置器；
	typedef simple_alloc<value_type, Alloc> data_allocator;
	iterator start;				// 表示空间使用的头
	iterator finish;			// 表示空间使用的尾
	iterator end_of_storage; 	// 表示可用空间的尾部


	void insert_aux(iterator position, const T& x);  // 插入
	void deallocate(){  // 释放空间
		if(start)
			data_allocator:deallocateI(start, end_of_storage - start);
	}
	
	// 填充并且初始化
	void fill_initialize(size_type n, const T& value){
		start = allocate_and_fill(n, value);
		finish = start + n;
		end_of_storage = finish;
	}
	
protected:
	// 配置空间并填满内存
	iterator allocate_and_fill(size_type n, const T& x){
		iterator result = data_allocator::allocate(n);
		uninitialized_fill_n(result, n, x);
		return result;
	}
	
	
	

public:
	iterator begin(){return start;} // 开始位置
	iterator end(){return finish;}	//
	size_type size()const {return size_type(end() - begin());}  //使用大小
	size_type capacity() const{  // 容量
		return end_of_storage - begin();
	}
	bool empty() const{return begin() == end();}  // 空
	reference operator[](size_type n){return *(begin() +n);}
	
	// 构造函数
	vector(): start(0), finish(0), end_of_storage(0)(){}
	vector(size_type n, const T& value){fill_initialize(n, value);}
	vector(int n , const T& value){fill_initialize(n, value);}
	vector(long n, const T& value){fill_initialize(n ,value);}
	explicit vector(size_type n){fill_initialize(n, T());}
	
	~vector(){
		destroy(start, finish);  // 全局函数
		deallocate(); 			// 成员函数
	}
	

	
	reference front(){return *begin();}
	reference back(){return *(end() -1);}
	
	
	
	
	void push_back(const T& x){		//还有剩余未使用空间
		if(finish != end_of_storage){
			construct(finish, x);
			++finish;
		}else{    //空间不足
			insert_aux(end(), x);
		}
	}
	
	void pop_back(){  //释放对象，内存保留
		--finish;
		destroy(finish);
	}
	
	iterator erase(iterator position){
		if(postion + 1 != end()){
			copy(position +1,finish, position );  // 后续元素往前移动
			--finish;
			destroy(finish);
			return position;
		}
	}
	
	// 清除某个区间上的元素
	iterator erase(iterator first, iterator last){
		iterator i = copy(last, finish, first);  // copy 全局函数
		destroy(i, finish);
		finish = finish - (last - first);
		return first;
	}
	
	
	
	
	void resise(size_type new_size, const T& x){
		if(new_size < size())
			erase(begin()+new_size, end());
		else{
			insert(end(), new_size - size(), x);
		}
	}
	
	void resise(size_type new_size){
		resize(new_size, T());
	}
	
	void clear(){return erase(begin(), end());}
		
	
};


template<class T, class Alloc>
void vector<T,Alloc>::insert_aux(iterator position, const T& x){
	if(finish != end_of_storage){ // 还有备用空间
		// 在备用空间起始处构造一个函数,并且以vector最后一个元素作为初始值
		construct(finish, *(finish -1));  
		++finish;
		T x_copy  = x;
		copy_backward(position, finish-2, finish-1); // 往后复制
		*position = x_copy;
	}else{   // 已无备用空间
		const size_type old_size = size();
		const size_type len = old_size !=0 ? 2* old_size: 1;
		// r如果原大小为0，配置一个元素大小；
		// 如果原大小不为0， 新配置空间为原来2倍
		// 前半段用来存放原始数据，后半段放新数据
		iterator new_start = data_allocator::allocate(new_size);
		iterator new_finish = new_start;
		try{
			// 将原来postion之前内容拷贝到新空间
			new_finish = uninitialized_copy(start,position, new_start);
			//为新元素赋值x
			construct(new_finish, x);
			++new_finish;
			// position之后半部分拷贝过来
			new_finish = uninitialized_copy(position,finish, new_finish);
		}catch(...){
			// commit or rollback
			destroy(new_start, new_finish);
			data_allocator::deallocate(new_start,len);
			throw;
		}
		
		//析构并释放原来空间
		destroy(begin(), end());
		deallocate();
		//调整迭代器，指向新vector
		start= new_start;
		finish = new_finish;
		end_of_storage = new_start+ len;
		
	}
}




// vector::insert()

// c从position 开始插入n个元素， 元素初始值为x；

template<Class T, class Alloc>
void vector<T, alloc>:: insert(iterator position, size_type n, const T& x){
	if(n != 0){// 当N！= 0的时候才可以进行
		if(size_type(end_of_storage - finish )>= n){
			// 备用空间足够
			T x_copy = x;
			// 一下计算插入点之后的现有元素个数
			const size_type elem_after = finish - position;
			iterator old_finish = finish;
			if(elem_after > n){  // “插入点之后现有元素个数” > “新增的元素个数”
				uninitialized_copy(finish - n, finish,finish);  // 覆盖
				finish += n;  // vector  后端标记后移
				copy_backward(position, old_finish - n, finish); // 前面向后移动n个位置
				fill(postion, position + n, x_copy);	// 从插入点开始填值
			}else{
				// 插入点的之后的现有元素个数 小于等于 “新增元素个数”
				uninitialized_fill_n(finish, n - elem_after, x_copy); 
				finish += n - elem_after;
				uninitialized_copy(position, old_finish, finish);
				finish += elem_after;
				fill(position, old_finish, x_copy);
			}
		}else{  // 备用空间 小于 “新增元素个数”（新配置额外的内存）
			// 首先决定新的长度： 旧的长度的两倍， 或者旧长度 + 新元素个数
			const size_type old_size = size();
			const size_type len  = old_size  + max(old_size, n);
			// 配置新的vector空间
			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;
			__STL_TRY{
				// 一下首先将旧的vector 插入点之前的元素插入到新的空间
				new_finish = uninitialized_copy(start, position, new_start);
				// 然后将新增元素填入新空间
				new_finish = uninitialized_fill_n(new_finish,n, x);
				//然后将position 之后的点插入到新空间
				new_finish = uninitialized_copy(position, finish, new_finish);
			}
			# ifdef __STL_USE_EXCEPTIONS
			catch(...){
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, len);
				throw;
			}
			# endif
			// 一下清除并且释放旧的vector
			destroy(start, finish);
			deallocate();
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + len;
		}
	}
	}































