/**
	hash table 节点定义
	bucket维护的linked list，不采用stl的list或slist，而是维护
	上述的hash table node;
	bucket聚合体，以vector完成；
*/

template<class Value>
struct __hashtable_node{
	__hashtable_node* next;
	Value val;

};


/**
	hashtable 的迭代器
	hashtable的迭代器器没有后退操作，也没有双向迭代器
*/

template<class Value, class Key, class HashFcn,
		class ExtratKey, class Equalkey, class Alloc>
struct __hashtable_iterator{
	typedef hashtable<Value, Key,HashFcn,ExtratKey,Equalkey,Alloc>
		hashtable;
		
	typedef __hashtable_iterator<Value,Key,HashFcn,ExtratKey,
									Equalkey,Alloc>  iterator;
					
	typedef __hashtable_iterator<Value,Key,HashFcn,ExtratKey,
									Equalkey,Alloc>  const_iterator;
	
	typedef __hashtable_node<Value> node;
	
	typedef forward_iterator_tag  iterator_category;
	typedef	Value 				  value_type;
	typedef ptrdiff_t			  difference_type;
	typedef size_t				  size_type;
	typedef Value& 				  reference;
	typedef Value*                pointer;
	
	node* cur;   // 迭代器当前指向节点
	hashtable* ht; // 保持对容器的连接关系
	
	
	
	__hashtable_iterator(node* n, hashtable* tab):cur(n),ht(tab){}
	__hashtable_iterator(){}
	
	reference operator*()const {return cur->val;}
	pointer operator->() const {return &(operator*());}
	
	
	// hashtable的迭代器器没有后退操作，也没有双向迭代器
	// 如果存在下一节点，直接next指针就可以；
	// 如果节点为list的尾端，就跳到下一个bucket，即指向下一个list的头部节点；
	iterator oprator++(){
		const node* old = cur;
		cur = cur->next; // 如果存在， 否则进入一下if
		if(! cur){
			// 根据元素值，定位下一个bucket，起始处就为答案
			size_type bucket = ht->bkt_num(old->val);
			while(!cur && ++bucket < ht->bucket.size()){
				cur = ht->buckets[bucket];
			}
			return *this;
		}
	}
	iterator operator++(int){
		iterator tmp = *this;
		++*this;
		return tmp;
	}
	
	
	
	bool operator== (const iterator& it){return cur == it.cur;}
	bool operator!= (const iterator& it){return cur != it.cur;}
};


/**
	开链法的表格大小预先设计
	sgi stl 以质数来设计表格大小，现将28个质数计算好，用来被查询
	
*/

static const int __stl_num_primes = 28;

static const unsigned long __stl_prime_list[__stl_num_primes] = 
{
	53，97， 193， 389， 769，
	...
	...
	...  , 4294967291
}

// 一下用来找处28个质数中最近于n并大于n的质数
inline unsigned long __stl_next_prime(unsigned long n){
	const unsigned long* first = __stl_prime_list;
	const unsigned long* last = __stl_prime_list + __stl_num_primes;
	const unsigned long* pos = lower_bound(first,last,n); // 泛型算法，序列已排序
	return pos == last ? *(last -1):*pos;
}

size_type max_bucket_count() const{
	return __stl_prime_list[__stl_num_primes -1];
	// 最大值为4294967291
}



/**
	hashtable的数据结构
	Value: 实值
	Key: 键值
	HashFcn: hash function类型
	ExtratKey: 从节点中取出键值的方法
	Equalkey : 判断键值相同与否的方法
	Alloc: 空间配置；
	
*/


template<class Value, class Key, class HashFcn,
			class ExtratKey,class Equalkey, class Alloc = alloc>
class hashtable{
public:
	typedef		HashFcn		hasher;   // 型别参数重新定义一个名字
	typedef		Equalkey	key_equal;	
	typedef		size_t		size_type;
	
	
private:

	// 以下三者都是函数对象 <stl_hash_fun.h>定义了数个
	// 标准型（如int,c-style string等）的hasher;
	hasher  hash;
	key_equal equals;
	ExtratKey  get_key;
	
	typedef		__hashtable_node<Value> node;
	typedef		simple_alloc<node, Alloc> node_allocator;
	
	
	vector<node*, Alloc> buckets;  // vector完成
	size_type	num_elements;
	
public:
	// bucket 个数 即 buckets vector的大小
	size_type  bucket_count() const {return buckets.size();}
	
	
	
protected:
	node* new_node(const value_type& obj){
		node* n = node_allocator::allocate();
		n->next = 0;
		__STL_TRY{
			construct(&n->val, obj);
			return n;
		}
		__STL_UNWIND(node_allocator::deallocate(n));
	}
	
	
	void delete_node(node* n){
		destroy(&n->val);
		node_allocator::deallocate(n);
	}
	
	// 初始构造 n个bunckets
	void iniitalize_buckets(size_type n){
		const size_type n_buckets = next_size(n);
		buckets.reserve(n_buckets);
		buckets.insert(buckets.end(),n_buckets,(node*)0);
		num_elements = 0;
	}
	
	// 就会最接近于n并且大于n的质数
	size_type next_size(size_type n) const {return __stl_next_prime(n);}
	

	
public:
	// hashtable没有默认构造函数
	hashtable<size_type n, const HashFcn& hf, const Equalkey& eql)
		:hash(hf),equals(eql),get_key(ExtratKey()),num_elements(0){
			iniitalize_buckets(n);
		}
		
		
	
	// 复制、删除
	void copy_from(const hashtable& ht){
		// 先清除自己的buckets vector,调用vector::clear()
		buckets.clear();
		// 如果自己的空间大于对方，就不动，小于的话，增大
		buckets.reserve(ht.buckets.size());
		// 此时buckets为空，end()就是起头
		buckets.insert(buckets.end(),ht.buckets.size(), (node*)0);
		__STL_TRY{
			// 每个bucket
			for(size_type i=0 ;i<ht.buckets.size(); ++i){
				// 每个bucket 开头
				if(const node* cur = ht.bucket[i]) {
					node* cp = new_node(cur->val);
					buckets[i] = cp;
				
				
					// 每个bucket的list
					for(node* next = cur->next; next; cur =next, next = cur->next){
						cop ->next = new_node(next->val);
						cp = cp->next;
					}
				}	
			}
			num_elements = ht.num_elements;
		}
		__STL_UNWIND(clear());
	}
	
		
	void clear(){  // buckets空间为释放
		// 对每一个bucket
		for(size_type i=0; i<buckets.size(); ++i){
			node* cur = bucket[i];
			// 将list每一个节点删除
			while(cur != 0){
				node* next = cur->next;
				delete_node(cur);
				cur = next;
			}
			bucket[i] = 0;  // bucket为NULL
		}
		num_elements = 0;  // 总数为0
	}	
		
		
public:
	// 计算 元素的键值,即hashtable上的bucket序号
	
	// 版本1： 接受实值和buckets个数
	size_type bkt_num(const value_type& obj, size_t n) const{
		return bkt_num_key(get_key(obj),n);  // 版本4
	}
	// 版本2： 只接受实值value
	size_type bkt_num(const value_type& obj) const{
		return bkt_num_key(get_key(obj));   // 版本3
	}

	// 版本3： 只接受键值
	size_type bkt_num(const key_type& key) const{
		return bkt_num_key(key,buckets.size());   // 版本4
	}

	// 版本4： 只接受键值和buckets个数
	size_type bkt_num(const key_type& key，size_t n) const{
		return hash(key) % n; // SGI内建 hash()
	}




	// 插入元素，不允许重复
	pair<iterator, bool> insert_unique(const value_types obj){
		resize(num_elements +1);  // 判断是否需要重建表格，如果需要就重新扩充
		return insert_unique_noresize(obj);
	}
	
	// 函数判断是否需要重建表示，如果不要，立刻返回，如果需要，就动手
	void resize(size_type num_elements_hint){
		// 判断方法： 元素个数和bucker vector个数比较
		// 如果前者大于后者，就重建
		// 每个list的最大容量 和bucket vector大小相同
		const size_type old_n = buckets.size();
		if(num_elements_hint > old_n){   // 需要重新配置
			const size_type n = next_size(num_elements_hint);  // 新大小
			if(n > old_n){
				vector<node*, Alloc>tmp(n, (node*)0);  // 设立新的buckets;
				__STL_TRY{
					// 处理旧的bucket
					for(size_type bucket = 0; bucket < old_n;++bucket){
						node* first = buckets[bucket];
						// 处理每个bucket的list
						while(first){
							// 寻找节点落在哪一个bucket
							size_type new_bucket =  bkt_num(first->val,n);
							// (1) 令旧的bucket 指向下一个节点
							buckets[bucket] = first->next;
							//(2)(3)将当前节点插入到新bucket中,前插法
							first->next = tmp[new_bucket];
							tmp[new_bucket] = first;
							//(4) 回到旧的bucket所指的list,准备处理下一个节点
							first = buckets[bucket];
						}
					}
					buckets.swap(tmp);  // vector::swap, 新旧两个bucket对换
					// 对换后，如果大小不一样，大的会变小，小的会变大
					// 离开后tmp释放
				}
			}
  			
		}
		
	}
	
	
    // 在不要建表格的情况下插入新节点，键值不允许重复
	pair<iterator,bool> insert_unique_noresize(const value_type& obj){
		const size_type n = bkt_num(obj); // 决定obj的bucket位置
		node* first = buckets[n];
		
		// 如果buckets[n]已经被占用，此时first将不为0
		// 迭代到list最尾节点
		for(node* cur = first; cur; cur = cur->next){
			// 如果发现存在相同键值，就返回，不插入
			if(equals(get_key(cur->val), get_key(obj))
				return pair<iterator,bool>(iterator(cur,this),false);
		}
		
		node* tmp = new_node(obj);  // 产生新节点
		tmp->next = first;
		buckets[n] = tmp ;   // 前插法
		++num_elements;
		return pair<iterator,bool> (iterator(tmp,this),true);
	}
	
	
	// 插入元素，允许重复
	iterator insert_equal(const value_type& obj){
		resize(num_elements + 1);  // 判断是否重建表格
		return insert_equal_noresize(obj);
	}
	
	iterator insert_equal_noresize(const value_type& obj){
		const size_type n = bkt_num(obj);
		node* first = buckets[n] ; 
		
		for(node* cur = first; cur; cur = cur->next){
			if(equals(get_key(cur->val),get_key(obj)){
				// list中存在相同键值的实值，就马上插入，在返回
				node* tmp = new_node(obj);  // 新节点
				tmp->next = cur->next;
				cur->next = tmp;
				++num_elements;
				return iterator(tmp, this);  // 返回
			}
		}
		
		// 未发现重复值，到达链表尾端
		node* tmp = new_node(obj);  // 新节点
		tmp->next = first;
		buckets[n] = tmp;
		++num_elements;
		return iterator(tmp, this);  // 返回
	}
	
	
public:
	iterator find(const key_type& key){
		size_type n = btk_num_key(key);  // 位于哪一个bucket
		node* first;
		for(first = buckets[n];first&& equals(get_key(first->val),key);
			first = first->next){}
		return iterator (first,*this);
	}
	
	size_type count(const key_type& key) const{
		const size_type n = btk_num_key(key);
		size_type result = 0;
		
		for(const node* cur = buckets[n]; cur; cur= cur->next)
			if(equals(get_key(cur->val),key))
				++result;
		return result;
	}
	

	
	
};




/**
 hash function类型
 <stl_hash_fun.h>
*/




template<class Key>
struct hash{
	
	
};

inline size_t __stl_hash_string(const char* s){
	unsigned long  h =0;
	for(;*s;++s)
		h = 5*h + *s;
	return size_t(h);
}


// __STL_TAMPLATE_NULL 定义为template<> 在;<stl_config.h>中

__STL_TAMPLATE_NULL
struct hash<char*>{
	size_t operator()(char* s){
		return __stl_hash_string(s);
	}
}



__STL_TAMPLATE_NULL
struct hash<const char*>{
	size_t operator()(const char* s){
		return __stl_hash_string(s);
	}
}



__STL_TAMPLATE_NULL
struct hash<char>{
	size_t operator()(char s){
		return s;
	}
}


__STL_TAMPLATE_NULL
struct hash<unsigned char>{
	size_t operator()(unsigned char s){
		return s;
	}
}

__STL_TAMPLATE_NULL
struct hash<signed char>{
	size_t operator()(unsigned char s){
		return s;
	}
}

__STL_TAMPLATE_NULL
struct hash<short>{
	size_t operator()(short s){
		return s;
	}
}

__STL_TAMPLATE_NULL
struct hash<unsigned short>{
	size_t operator()(unsigned short s){
		return s;
	}
}


__STL_TAMPLATE_NULL
struct hash<int>{
	size_t operator()(int s){
		return s;
	}
}


__STL_TAMPLATE_NULL
struct hash<unsigned int>{
	size_t operator()( unsigned int s){
		return s;
	}
}


__STL_TAMPLATE_NULL
struct hash<long>{
	size_t operator()(long s){
		return s;
	}
}


__STL_TAMPLATE_NULL
struct hash<unsigned long>{
	size_t operator()( unsigned long s){
		return s;
	}
}



/**


	hash_set 的部分实现
*/


template<class Value, class HashFcn = hash<Value>
		class Equalkey= equal_to<Value>, class Alloc = alloc>
class hash_set{
private:
	// 以下使用identity<>定义于<stl_function.h>中
	typedef	hashtable<Value,Value,HashFcn, identity<Value>,Equalkey,Alloc> ht;
	
	ht  rep;  // hashtable 作为底层；
	
public:
	typedef typename	 	ht::key_type   	key_type;
	typedef typename		ht::value_type	value_type;
	typedef	typename		ht::hasher		hasher;
	typedef	typename		ht::key_equal	key_equal;
	
	typedef	typename		ht::size_type	size_type;
	typedef	typename		ht::difference_type	difference_type;
	typedef	typename		ht::const_pointer	pointer;
	typedef	typename		ht::const_pointer	const_pointer;
	typedef	typename		ht::const_reference	reference;
	typedef	typename		ht::const_reference	const_reference;
	
	typedef	typename		ht::const_iterator	iterator;
	typedef	typename		ht::const_iterator	const_iterator;
	
	hasher hash_funct() const {return rep.hash_funct();}
	key_equal key_eq() const{return rep.key_eq();}
	
	
public:
	// 构造函数
	// 缺省大小为100 的表格， 江北hashtable调整为最近的质数
	hash_set():rep(100, hasher(),key_equal()){}
	explicit hash_set(size_type n):rep(n,hasher(), key_equal()){}
	hash_set(size_type n , const hasher& hf): rep(n, hf, key_equal()){}
	hash_set(size_type n , const hasher& hf, const key_equal& eql):rep(n,hf,eql){}
	
	// 插入使用insert_unique()，不允许重复	
	template<class InputIterator>
	hash_set(InputIterator f, InputIterator l):rep(100,hasher(),key_equal()){
		rep.insert_unique(f,l);
	}
	template<class InputIterator>
	hash_set(InputIterator f, InputIterator l, size_type n)
		:rep(n,hasher(),key_equal()){
		rep.insert_unique(f,l);
	}
	template<class InputIterator>
	hash_set(InputIterator f, InputIterator l, size_type n, const haser& hf)
		:rep(n,hf,key_equal()){
		rep.insert_unique(f,l);
	}
	template<class InputIterator>
	hash_set(InputIterator f, InputIterator l, size_type n, 
			const haser& hf, const key_equal& eql):rep(n,hf,eql){
		rep.insert_unique(f,l);
	}

public:
	// 操作
	size_type size() const {return rep.size();}
	size_type max_size() const {return rep.max_size();}
	bool empty() const {return rep.empty();}
	
	void swap(hash_set& hs){
		rep.swap(hs.rep);
	}
	
	iterator begin()const{return rep.begin();}
	iterator end() const{return rep.end();}
	
	
	friend bool operator== __STL_NULL_TMPL_ARGS(const hash_set&, const hash_set&);


public:
	par<iterator ,bool> insert(const value_type& obj){
		pair<typename ht::iterator  ,bool> p = rep.insert_unique(obj);
		return pair<iterator, bool>(p.first,p.second);
	}
	template<class InputIterator>
	void insert(InputIterator f, InputIterator l){
		rep.insert_unique(f, l);
	}
	
	pair<iterator, bool> insert_noresize(const value_type& obj){
		pair<typename ht::iterator,bool> p = rep.insert_unique_noresize(f,l);
		return  pair<iterator, bool>(p.first,p.second);
	}
	
	iterator find(const key_type& key)const{return rep.find(key);}
	
	size_type count(const key_type& key) const{return rep.count(key);}
	
	pair<iterator, bool> equal_range(const key_type& key)const{
		return rep.equal_range(key);
	}
	
	size_type erase(const key_type& key){return rep.erase(key);}
	void erase(iterator it){ rep.erase(it);}
	void erase(iterator f, iterator l){rep.erase(f,l);}
	void clear(){rep.clear();}
	
	
	
public:
	void resize(size_type hint){rep.resize(hint);}
	size_type bucket_count() const{retur rep.bucket_count();}
	size_type max_bucket_count() const {rep.max_bucket_count();}
	size_type elems_in_bucket(size_type n) const{
		return rep.elems_in_bucket(n);
	}
	
	template<class Value,class HashFcn, class Equalkey,class Alloc>
	inline bool operator==(const hash_set<Value,HashFcn,Equalkey,Alloc>& hs1,
							const hash_set<Value,HashFcn,Equalkey,Alloc>& hs2)
	{
		return hs1.rep == hs2.rep;
	}
};		





/**
	hash_multiset 部分实现

*/
	

template<class Value, class HashFcn = hash<Value>
		class Equalkey= equal_to<Value>, class Alloc = alloc>
class hash_multiset{
private:
	// 以下使用identity<>定义于<stl_function.h>中
	typedef	hashtable<Value,Value,HashFcn, identity<Value>,Equalkey,Alloc> ht;
	
	ht  rep;  // hashtable 作为底层；
	
public:
	typedef typename	 	ht::key_type   	key_type;
	typedef typename		ht::value_type	value_type;
	typedef	typename		ht::hasher		hasher;
	typedef	typename		ht::key_equal	key_equal;
	
	typedef	typename		ht::size_type	size_type;
	typedef	typename		ht::difference_type	difference_type;
	typedef	typename		ht::const_pointer	pointer;
	typedef	typename		ht::const_pointer	const_pointer;
	typedef	typename		ht::const_reference	reference;
	typedef	typename		ht::const_reference	const_reference;
	
	typedef	typename		ht::const_iterator	iterator;
	typedef	typename		ht::const_iterator	const_iterator;
	
	hasher hash_funct() const {return rep.hash_funct();}
	key_equal key_eq() const{return rep.key_eq();}
	
	
public:
	// 构造函数
	// 缺省大小为100 的表格， 江北hashtable调整为最近的质数
	hash_multiset():rep(100, hasher(),key_equal()){}
	explicit hash_set(size_type n):rep(n,hasher(), key_equal()){}
	hash_multiset(size_type n , const hasher& hf): rep(n, hf, key_equal()){}
	hash_multiset(size_type n , const hasher& hf, const key_equal& eql):rep(n,hf,eql){}
	
	// insert_equal()，可重复	
	template<class InputIterator>
	hash_multiset(InputIterator f, InputIterator l):rep(100,hasher(),key_equal()){
		rep.insert_equal(f,l);
	}
	template<class InputIterator>
	hash_multiset(InputIterator f, InputIterator l, size_type n)
		:rep(n,hasher(),key_equal()){
		rep.insert_equal(f,l);
	}
	template<class InputIterator>
	hash_multiset(InputIterator f, InputIterator l, size_type n, const haser& hf)
		:rep(n,hf,key_equal()){
		rep.insert_equal(f,l);
	}
	template<class InputIterator>
	hash_multiset(InputIterator f, InputIterator l, size_type n, 
			const haser& hf, const key_equal& eql):rep(n,hf,eql){
		rep.insert_equal(f,l);
	}

public:
	// 操作
	size_type size() const {return rep.size();}
	size_type max_size() const {return rep.max_size();}
	bool empty() const {return rep.empty();}
	
	void swap(hash_multiset& hs){
		rep.swap(hs.rep);
	}
	
	iterator begin()const{return rep.begin();}
	iterator end() const{return rep.end();}
	
	
	friend bool operator== __STL_NULL_TMPL_ARGS(const hash_multiset&, const hash_multiset&);


public:
	par<iterator ,bool> insert(const value_type& obj){
		pair<typename ht::iterator  ,bool> p = rep.insert_equal(obj);
		return pair<iterator, bool>(p.first,p.second);
	}
	template<class InputIterator>
	void insert(InputIterator f, InputIterator l){
		rep.insert_equal(f, l);
	}
	
	pair<iterator, bool> insert_noresize(const value_type& obj){
		pair<typename ht::iterator,bool> p = rep.insert_equal_noresize_noresize(f,l);
		return  pair<iterator, bool>(p.first,p.second);
	}
	
	iterator find(const key_type& key)const{return rep.find(key);}
	
	size_type count(const key_type& key) const{return rep.count(key);}
	
	pair<iterator, bool> equal_range(const key_type& key)const{
		return rep.equal_range(key);
	}
	
	size_type erase(const key_type& key){return rep.erase(key);}
	void erase(iterator it){ rep.erase(it);}
	void erase(iterator f, iterator l){rep.erase(f,l);}
	void clear(){rep.clear();}
	
	
	
public:
	void resize(size_type hint){rep.resize(hint);}
	size_type bucket_count() const{retur rep.bucket_count();}
	size_type max_bucket_count() const {rep.max_bucket_count();}
	size_type elems_in_bucket(size_type n) const{
		return rep.elems_in_bucket(n);
	}
	
	template<class Value,class HashFcn, class Equalkey,class Alloc>
	inline bool operator==(const hash_multiset<Value,HashFcn,Equalkey,Alloc>& hs1,
							const hash_multiset<Value,HashFcn,Equalkey,Alloc>& hs2)
	{
		return hs1.rep == hs2.rep;
	}
};









/**


	hash_map 的部分实现
*/


template<class Key,class T, class HashFcn = hash<Key>
		class Equalkey= equal_to<Key>, class Alloc = alloc>
class hash_map{
private:
	// 以下使用 selectlst<> 定义于<stl_function.h>中
	typedef	hashtable<Key,T,HashFcn, selectlst<pair<const Key, T> >,Equalkey,Alloc> ht;
	
	ht  rep;  // hashtable 作为底层；
	
public:
	typedef typename	 	ht::key_type   	key_type;
	typedef	T								data_type;
	typedef	T								mapped_type;
	typedef typename		ht::value_type	value_type;
	typedef	typename		ht::hasher		hasher;
	typedef	typename		ht::key_equal	key_equal;
	
	typedef	typename		ht::size_type	size_type;
	typedef	typename		ht::difference_type	difference_type;
	typedef	typename		ht::pointer	pointer;
	typedef	typename		ht::const_pointer	const_pointer;
	typedef	typename		ht::reference	reference;
	typedef	typename		ht::const_reference	const_reference;
	
	typedef	typename		ht::iterator	iterator;
	typedef	typename		ht::const_iterator	const_iterator;
	
	hasher hash_funct() const {return rep.hash_funct();}
	key_equal key_eq() const{return rep.key_eq();}
	
	
public:
	// 构造函数
	// 缺省大小为100 的表格， 江北hashtable调整为最近的质数
	hash_map():rep(100, hasher(),key_equal()){}
	explicit hash_map(size_type n):rep(n,hasher(), key_equal()){}
	hash_map(size_type n , const hasher& hf): rep(n, hf, key_equal()){}
	hash_map(size_type n , const hasher& hf, const key_equal& eql):rep(n,hf,eql){}
	
	// 插入使用insert_unique()，不允许重复	
	template<class InputIterator>
	hash_map(InputIterator f, InputIterator l):rep(100,hasher(),key_equal()){
		rep.insert_unique(f,l);
	}
	template<class InputIterator>
	hash_map(InputIterator f, InputIterator l, size_type n)
		:rep(n,hasher(),key_equal()){
		rep.insert_unique(f,l);
	}
	template<class InputIterator>
	hash_map(InputIterator f, InputIterator l, size_type n, const haser& hf)
		:rep(n,hf,key_equal()){
		rep.insert_unique(f,l);
	}
	template<class InputIterator>
	hash_map(InputIterator f, InputIterator l, size_type n, 
			const haser& hf, const key_equal& eql):rep(n,hf,eql){
		rep.insert_unique(f,l);
	}

public:
	// 操作
	size_type size() const {return rep.size();}
	size_type max_size() const {return rep.max_size();}
	bool empty() const {return rep.empty();}
	
	void swap(hash_map& hs){
		rep.swap(hs.rep);
	}
	
	iterator begin(){return rep.begin();}
	iterator end() {return rep.end();}
	iterator begin()const{return rep.begin();}
	iterator end() const{return rep.end();}
	
	
	friend bool operator== __STL_NULL_TMPL_ARGS(const hash_map&, const hash_map&);


public:
	par<iterator ,bool> insert(const value_type& obj){
		pair<typename ht::iterator  ,bool> p = rep.insert_unique(obj);
		return pair<iterator, bool>(p.first,p.second);
	}
	template<class InputIterator>
	void insert(InputIterator f, InputIterator l){
		rep.insert_unique(f, l);
	}
	
	pair<iterator, bool> insert_noresize(const value_type& obj){
		pair<typename iterator,bool> p = rep.insert_unique_noresize(f,l);

	}
	
	iterator find(const key_type& key){return rep.find(key);}
	const_iterator find(const key_type& key)const {return rep.find(key);}
	
	T& operator[](const key_type& key){
		return rep.find_or_insert(value_type(key,T()))).second;
	}
	
	size_type count(const key_type& key) const{return rep.count(key);}
	
	pair<iterator, bool> equal_range(const key_type& key){
		return rep.equal_range(key);
	}
	pair<const_iterator, bool> equal_range(const key_type& key)const{
		return rep.equal_range(key);
	}
	
	size_type erase(const key_type& key){return rep.erase(key);}
	void erase(iterator it){ rep.erase(it);}
	void erase(iterator f, iterator l){rep.erase(f,l);}
	void clear(){rep.clear();}
	
	
	
public:
	void resize(size_type hint){rep.resize(hint);}
	size_type bucket_count() const{retur rep.bucket_count();}
	size_type max_bucket_count() const {rep.max_bucket_count();}
	size_type elems_in_bucket(size_type n) const{
		return rep.elems_in_bucket(n);
	}
	
	template<class Key,class T,class HashFcn, class Equalkey,class Alloc>
	inline bool operator==(const hash_map<Key,T,HashFcn,Equalkey,Alloc>& hm1,
							const hash_map<Key,T,HashFcn,Equalkey,Alloc>& hm2)
	{
		return hm1.rep == hm2.rep;
	}
};	




/**


	hash_multimap 的部分实现
*/


template<class Key,class T, class HashFcn = hash<Key>
		class Equalkey= equal_to<Key>, class Alloc = alloc>
class hash_multimap{
private:
	// 以下使用 selectlst<> 定义于<stl_function.h>中
	typedef	hashtable<Key,T,HashFcn, selectlst<pair<const Key, T> >,Equalkey,Alloc> ht;
	
	ht  rep;  // hashtable 作为底层；
	
public:
	typedef typename	 	ht::key_type   	key_type;
	typedef	T								data_type;
	typedef	T								mapped_type;
	typedef typename		ht::value_type	value_type;
	typedef	typename		ht::hasher		hasher;
	typedef	typename		ht::key_equal	key_equal;
	
	typedef	typename		ht::size_type	size_type;
	typedef	typename		ht::difference_type	difference_type;
	typedef	typename		ht::pointer	pointer;
	typedef	typename		ht::const_pointer	const_pointer;
	typedef	typename		ht::reference	reference;
	typedef	typename		ht::const_reference	const_reference;
	
	typedef	typename		ht::iterator	iterator;
	typedef	typename		ht::const_iterator	const_iterator;
	
	hasher hash_funct() const {return rep.hash_funct();}
	key_equal key_eq() const{return rep.key_eq();}
	
	
public:
	// 构造函数
	// 缺省大小为100 的表格， 江北hashtable调整为最近的质数
	hash_multimap():rep(100, hasher(),key_equal()){}
	explicit hash_multimap(size_type n):rep(n,hasher(), key_equal()){}
	hash_multimap(size_type n , const hasher& hf): rep(n, hf, key_equal()){}
	hash_multimap(size_type n , const hasher& hf, const key_equal& eql):rep(n,hf,eql){}
	
	// insert_equal()，允许重复	
	template<class InputIterator>
	hash_multimap(InputIterator f, InputIterator l):rep(100,hasher(),key_equal()){
		rep.insert_equal(f,l);
	}
	template<class InputIterator>
	hash_multimap(InputIterator f, InputIterator l, size_type n)
		:rep(n,hasher(),key_equal()){
		rep.insert_equal(f,l);
	}
	template<class InputIterator>
	hash_multimap(InputIterator f, InputIterator l, size_type n, const haser& hf)
		:rep(n,hf,key_equal()){
		rep.insert_equal(f,l);
	}
	template<class InputIterator>
	hash_multimap(InputIterator f, InputIterator l, size_type n, 
			const haser& hf, const key_equal& eql):rep(n,hf,eql){
		rep.insert_equal(f,l);
	}

public:
	// 操作
	size_type size() const {return rep.size();}
	size_type max_size() const {return rep.max_size();}
	bool empty() const {return rep.empty();}
	
	void swap(hash_multimap& hs){
		rep.swap(hs.rep);
	}
	
	iterator begin(){return rep.begin();}
	iterator end() {return rep.end();}
	iterator begin()const{return rep.begin();}
	iterator end() const{return rep.end();}
	
	
	friend bool operator== __STL_NULL_TMPL_ARGS(const hash_multimap&, const hash_multimap&);


public:
	par<iterator ,bool> insert(const value_type& obj){
		pair<typename ht::iterator  ,bool> p = rep.insert_equal(obj);
		return pair<iterator, bool>(p.first,p.second);
	}
	template<class InputIterator>
	void insert(InputIterator f, InputIterator l){
		rep.insert_equal(f, l);
	}
	
	pair<iterator, bool> insert_noresize(const value_type& obj){
		pair<typename iterator,bool> p = rep.insert_equal_noresize(f,l);

	}
	
	iterator find(const key_type& key){return rep.find(key);}
	const_iterator find(const key_type& key)const {return rep.find(key);}
	
	T& operator[](const key_type& key){
		return rep.find_or_insert(value_type(key,T()))).second;
	}
	
	size_type count(const key_type& key) const{return rep.count(key);}
	
	pair<iterator, bool> equal_range(const key_type& key){
		return rep.equal_range(key);
	}
	pair<const_iterator, bool> equal_range(const key_type& key)const{
		return rep.equal_range(key);
	}
	
	size_type erase(const key_type& key){return rep.erase(key);}
	void erase(iterator it){ rep.erase(it);}
	void erase(iterator f, iterator l){rep.erase(f,l);}
	void clear(){rep.clear();}
	
	
	
public:
	void resize(size_type hint){rep.resize(hint);}
	size_type bucket_count() const{retur rep.bucket_count();}
	size_type max_bucket_count() const {rep.max_bucket_count();}
	size_type elems_in_bucket(size_type n) const{
		return rep.elems_in_bucket(n);
	}
	
	template<class Key,class T,class HashFcn, class Equalkey,class Alloc>
	inline bool operator==(const hash_multimap<Key,T,HashFcn,Equalkey,Alloc>& hm1,
							const hash_multimap<Key,T,HashFcn,Equalkey,Alloc>& hm2)
	{
		return hm1.rep == hm2.rep;
	}
};	






