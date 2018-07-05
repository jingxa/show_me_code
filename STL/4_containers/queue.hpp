/**
	queue: 先进先出
	1. 不允许遍历
	2. deque作为底层容器
*/

template<class T, class Sequence = deque<T> >
class queue{
	// __STL_NULL_TMPL_ARGS展开为<>
	friend bool operator== __STL_NULL_TMPL_ARGS(const queue& x, const queue& y);
	friend bool operator< __STL_NULL_TMPL_ARGS(const queue& x, const queue& y);
	
public:
	typedef typename	Sequence::value_type	value_type;
	typedef	typename	Sequence::size_type		size_type;
	typedef	typename	Sequence::reference		reference;
	typedef	typename	Sequence::const_reference	const_reference;
	
protected:
	Sequence c;  // 底层容器

public:
	bool empty() const{return c.empty();}
	size_type size() const{return c.size();}
	reference front(){return c.front();}
	const_reference front() const{return c.front();}
	reference back(){return c.back();}
	const_reference back(){return c.back();}
	
	// queue 末端进，前端出
	void push(const value_type& x){c.push_back(x);}
	void pop(){c.pop_front();}
	
	
};


template<class T, class Sequence>
bool operator==(const queue<T, Sequence>& x, const queue<T,Sequence>& y){
	return x.c == y.c;
}

template<class T, class Sequence>
bool operator,(const queue<T, Sequence>& x, const queue<T,Sequence>& y){
	return x.c , y.c;
}















