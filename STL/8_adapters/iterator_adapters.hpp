/**
	1. back_insert_iterator
*/
template<class Container>
class back_insert_iterator{
protected:
	Container* container; // 底层容器

public:
	typedef output_iterator_tag 		iterator_category;   
	typedef	void						value_type;
	typedef	void 						difference_type;
	typedef void						pointer;
	typedef	void						reference;
	
	
	explicit back_insert_iterator(Container& x):container(x){}
	back_insert_iterator<Container>&
		operator=(const typename Container::value_type& value){
			container->push_back(value);  // 调用容器操作
			return *this;
	}
	
	// 一下三个操作对 back_insert_iterator 不起作用
	// 三个返回都是自己
	back_insert_iterator<Container>& operator*(){return *this;}
	back_insert_iterator<Container>& operator++(){return *this;}
	back_insert_iterator<Container>& operator++(int){return *this;}
		
		
};

// 辅助函数
template<class Container>
inline back_insert_iterator<Container> back_inserter(Container& x){
	return back_insert_iterator<Container>(x);
}



/**

	2. front_insert_iterator
*/
template<class Container>
class front_insert_iterator{
protected:
	Container* container; // 底层容器

public:
	typedef output_iterator_tag 		iterator_category;   
	typedef	void						value_type;
	typedef	void 						difference_type;
	typedef void						pointer;
	typedef	void						reference;
	
	
	explicit front_insert_iterator(Container& x):container(x){}
	front_insert_iterator<Container>&
		operator=(const typename Container::value_type& value){
			container->push_front(value);  // 调用容器操作
			return *this;
	}
	
	// 一下三个操作对 front_insert_iterator 不起作用
	// 三个返回都是自己
	front_insert_iterator<Container>& operator*(){return *this;}
	front_insert_iterator<Container>& operator++(){return *this;}
	front_insert_iterator<Container>& operator++(int){return *this;}
		
		
};

// 辅助函数
template<class Container>
inline front_insert_iterator<Container> front_inserter(Container& x){
	return front_insert_iterator<Container>(x);
}




/**
	3. insert_iterator
*/

template<class Container>
class insert_iterator{
protected:
	Container* container; // 底层容器
	typename Container::iterator  iter;

public:
	typedef output_iterator_tag 		iterator_category;   
	typedef	void						value_type;
	typedef	void 						difference_type;
	typedef void						pointer;
	typedef	void						reference;
	
	
	insert_iterator(Container& x,typename Container::iterator i)
		:container(x),iter(i){}
	
	
	insert_iterator<Container>&
		operator=(const typename Container::value_type& value){
			iter = container->insert(value);  // 调用容器操作
			++iter;  // 随着目标移动
			return *this;
	}
	
	// 一下三个操作对 insert_iterator 不起作用
	// 三个返回都是自己
	insert_iterator<Container>& operator*(){return *this;}
	insert_iterator<Container>& operator++(){return *this;}
	insert_iterator<Container>& operator++(int){return *this;}
		
		
};

// 辅助函数
template<class Container>
inline insert_iterator<Container> front_inserter(Container& x，Iterator i){
	typedef typename Container::iterator iter;
	return insert_iterator<Container>(x,iter(i));
}





/**
	二、 reverse Iterator

*/
template<class Iterator>
class reverse_iterator{
	
protected:
	Iterator	current;   // 记录正向迭代器

public:
	// 逆向迭代器的5中类型
	typedef typename iterator_traits<Iterator>:: iterator_category iterator_category;
	typedef typename iterator_traits<Iterator>::value_type 			value_type;
	typedef typename iterator_traits<Iterator>::difference_type		difference_type;
	typedef typename iterator_traits<Iterator>::pointer				pointer;
	typedef typename iterator_traits<Iterator>::reference			reference;
	
	typedef	Iterator	iterator_type;  // 正向迭代器
	typedef	reverse_iterator<Iterator> self;		// 逆向迭代器
	

public:

	reverse_iterator(){}
	
	explicit reverse_iterator(iterator_type x):current(x){}

	reverse_iterator(const self& x):current(x.current){}
	
	
	iterator_type base() const{return current;}   // 取出正向迭代器
	reference operator*() const {  
	// 对逆向迭代器取值，就是讲“对应的正向迭代器”退一格在取值
		Iterator tmp = current;
		treturn *--tmp;
	}
	
	pointer operator->() const{return &(operator*());}
	
	self& operator++(){
		-- current;
		return *this;
	}
	
	self operator++(int){
		self tmp = *this;
		-- current;
		return tmp;
	}
	
	// 后退操作
	self& operator--(){
		++current;
		return *this;
	}
	
	self operator--(int){
		self  tmp = *this;
		++current;
		return tmp;
	}
	
	// 前进和后退完全逆转
	self  operator+(difference_type n) const{
		return self(current -n);
	}
	self&  operator+=(difference_type n) const{
		current -= n;
		return *this;
	}
	self  operator-(difference_type n) const{
		return self(current + n);
	}
	self&  operator-=(difference_type n) const{
		current += n;
		return *this;
	}
	
	
	// 下面第一个* 和 + 都会调用本类的operator* 和operator+
	// 第二个 * 不会调用本类，而是调用正向迭代器的operator*；
	reference operator[](difference_type n )const{
		return *(*this + n);
	}
	
};




/**

	三、 stream iterators
*/

// istream_iterator

template<class T, class Distance = prdiff_t>
class istream_iterator{
	// __STL_NULL_TMPL_ARGS	在<stl_config.h>中定义为 template<>
	friend bool 
		operator== __STL_NULL_TMPL_ARGS(const istream_iterator<T,Distance>& x,
			const istream_iterator<T,Distance>& y>;
	
protected:
	istream* stream;
	T value;
	bool end_marker;
	void read(){
		end_marker = (*stream)? true: false;
		if(end_marker)*stream >>value;
		//以上输入之后，stream状态改变，所以在下面在判断一次决定end_marker
		// 当读到eof或者型别不符合的时候，stream处于false状态
		end_marker = (*stream)? true: false;
	}
	
public:
	typedef input_iterator_tag					iterator_category;
	typedef	T									value_type;
	typedef	Distance							difference_type;
	typedef	const T*							pointer;
	typedef	const T&							reference;
	// 以上，因为input iterator ,所以采用const
	
	istream_iterator(): stream(&cin),end_marker(false){}
	istream_iterator(istream& s):stream(&s){red();}
	// 以上两行用法：
	// istream_iterator<int> eos;  造成end_marker为false;
	// istream_iterator<int> initer(cin); 引发read(), 程序会等待
	// 因此，下面这两行：
	// istream_iterator<int> initer(cin); (A)
	// cout<<"please input .."<<endl;     (B)
	// 会停留在A等待一个输入，然后才执行(B)的提示信息，不合理；
	// 规避：永远在合适的时候定义一个 istream_iterator
	
	reference operator*()const{return value;}
	pointer operator->() const{return &(operator*());}
	
	// 迭代器前进一个位置，读取一个资料
	istream_iterator<T,Distance>& operator++(){
		read();
		return *this;
	}
	
	istream_iterator<T,Distance> operator++(int){
		istream_iterator<T,Distance> tmp = *this;
		read();
		return tmp;
	}
	
	
	
	
};


// ostream_iterator

template<class T>
class ostream_iterator{
protected:
	ostream* stream;
	const char* string; // 每次输出的间隔符号
	
public:
	typedef output_iterator_tag		iterator_category;
	typedef	void					value_type;
	typedef	void					pointer;
	typedef	void					difference_type;
	typedef	void					reference;
	
	
	ostream_iterator(ostream& s):stream(&s),string(0){}
	ostream_iterator(ostream& s, const char* c):stream(&s),string(c){}
	// 以上，用法：
	// ostream_iterator<int> outlier(cout,' '); 输出到cout,每次间隔一个空格
	
	// 迭代器赋值操作，代表输出一笔数据
	ostream_iterator<T>& operator=(const T& value){
		*stream<<value;
		if(string) *stream<<string;  // 如果状态无误，输出间隔符号
		return *this;
	}
	
	// 一下三个操作，都是返回自己
	ostream_iterator<T>& operator*(){return *this;}
	ostream_iterator<T>& operator++(){return *this;}
	ostream_iterator<T>& operator++(int){return *this;}
};





































