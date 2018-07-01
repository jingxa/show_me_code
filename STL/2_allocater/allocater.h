template<class T， class Alloc>
class simple_alloc{
public:
	static T* allocate(size_t n){
		return 0 == n ? 0 : (T*)Alloc::allocate(n*sizeof(T));
	}
	
	static T* allocate(void){
		return (T*) Alloc::allocate(sizeof(T));
	}
	
	static void deallocate(T* p, size_t n){
		if(0 != n)Alloc::deallocate(p, n*sizeof(T));
	}
	
	static void deallocate(T* p){
		Alloc::deallocate(p, sizeof(T));
	}
};

template<int inst>  // 无template参数，“非型别参数”inst没用
class __malloc_alloc_template{

private:

// 处理内存不足的函数指针
//  oom: out of memory
	static void *oom_malloc(size_t);
	static void *oom_realloc(void*, size_t);
	static void (*__malloc_alloc_oom_handler)();
	
	
public:
//	分配内存
	static void * allocate(size_t n){
		void* result = malloc(n); 	// 直接使用malloc
		if(0 == result)result = oom_malloc(n); // 如果分配不成功，改用oom_malloc()
		return result;
	}
// 释放内存
	static void* deallocate(void* p , size_t /*n*/){
		free(p);		//直接使用free
	}
	
//....
};


/**
	第二配置器的部分代码
*/

enum{__ALIGN = 8};   // 小型区块的上调边界  ，分配内存不足8的补齐8
enum {__MAX_BYTES = 128};		//小型区块的上限
enum {__NFREELISTS = __MAX_BYTES / __ALIGN};  // free-lists


template<bool threads, in inst>  // 第一参数 多线程， 第二参数 无用
class __default_alloc_template{
	
private:
// round_up() 将byte上调至8的倍数
static size_t ROUND_UP(size_t bytes){
	return (((bytes) + __ALIGN -1) & ~(__ALIGN -1));
}

private:
	union obj{		// free-lists 的节点构造
		union obj * free_list_link;
		char client_data[1];  
	};
	
private:
	static obj* vaolatile free_list[__NFREELISTS];   // 16个列表
	static size_t FREELIST_INDEX(size_t bytes){
		return (((bytes) + __ALIGN -1) / __ALIGN - );
	}
	
	static void* refill(size_t n);	// 返回一个大小为n的对象，可能加入大小为n的其他区块到free list
	
	static char* chunk_alloc(size_t size, int & nobjs);		//配置一大块内存，容纳nojbs个“size”的区块
	
	
// static data members

static char* start_free;  // 内存池起始位置
static char* end_free;
static size_t heap_size;

// 成员函数
// 省略

public:
	static void* allocate(size_t n){  // n 大于0
		obj* volatile * my_free_list;
		obj* result;
		
		// 大于128bytes
		if(n > (size_t)__MAX_BYTES){
			return (malloc_alloc::allocate(n));
		}
		// 寻找16个free_list中的一个
		my_free_list = free_list + FREELIST_INDEX(n);
		result = *my_free_list;
		if(0 == result){
			// 没有可用的free_list,准备填充free list
			void* r = refill(ROUND_UP(n));
			return r;
		}
		// 调整free list
		* my_free_list = result->free_list_link;  // 将result指向的区块移除，my_free_list 指向result的后续
		return result;
		
		
	}

	static void deallocate(void* p , size_t n){
		obj* q = (obj*)p;
		obj* volatile * my_free_list;
		//大于128bytes 调用第一级配置器
		if(n>(size_t)__MAX_BYTES)
		{
			malloc::deallocate(p, n);
			return;
		}
		// 寻找对应的free list
		my_free_list = free_list + FREELIST_INDEX(n);
		q->free_list_link = *my_free_list;
		*my_free_list = q;
	}
	
	static void* reallocate(void* p, size_t old_size, size_t new_size);
};

// 静态变量的初始化 
// 省略



template<bool threads, int inst>
void* __default_alloc_template<threads, inst>::refill(size_t n){
	int nobjs = 20;
	// 调用chunk_alloc()，nobjs 是引用传值
	char* chunk = chunk_alloc(n, nobjs);  
	
	obj* volatile * my_free_list;
	obj* result;
	obj* current_obj, * next_obj;
	int i;
	
	// 如果只获得一个区块，直接分给调用者，free list 无新节点
	if(1 == nobjs )retun (chunk);
	
	//否则准备调整free list , 接入新节点
	my_free_list = free_list + FREELIST_INDEX(n);
	
	// 以下在chunk空间建立free list 
	result = (obj*) chunk;  // 这一块返回客户端
	
	//导引free list 指向新配置的空间
	*my_free_list = next_obj = (obj*) (chunk+n);
	
	// 从1开始，第o个返回给客端
	for（int i= 1;; i++){
		current_obj = next_obj;
		next_obj = (obj*)((char*)next_obj + n); //分块
		if(nobjs -1 == i){   // 最后一块
			current_obj->free_list_link =0;
			break;
		}else{
			current_obj ->free_list_link = next_obj;
		}
	}
	return result;
}


// chunk_alloc
template<bool threads, in inst>
char* __default_alloc_template<threads, inst>::chunk_alloc(size_t size, int& nobjs){
	char* result;
	size_t total_bytes = size * nobjs;
	size_t bytes_left = end_free - start_free;  // 内存池剩余空间
	
	if(bytes_left >= total_bytes){
		result = start_free;
		start_free += total_bytes;
		return result;
	}else if(bytes_left >=size){
		// 剩余空间不能满足所有需求量，但足够供应一个以上的区块
		nobjs = bytes_left / size;
		total_bytes = size * nobjs;
		result = start_free;
		start_free += total_bytes;
		return result;
	}else{
		//内存池剩余空间不能满足一个区块
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
		
		if(bytes_left > 0){
			//内存池还有零头，线分配给free list
			//首先寻找合适的free list
			obj* volatile* my_free_list = free_list + 
				FREELIST_INDEX(bytes_left);
			// 调整free list, 将内存中的残余空间编入
			((obj*)start_free)->free_list_link = *my_free_list;
			*my_free_list = (obj*)start_free;
		}
		
		
		// 配置heap空间，用来补充内存池
		start_free = (char*)malloc(bytes_to_get);
		if(0 == start_free){
			// heap 空间不足， malloc失败
			int i;
			obj* volatile* my_free_list,*p;
			// 搜索适当的free list: 指的是“尚有未用完，而且区块够大”的free list
			for(i = size;i<__MAX_BYTES; i+=__ALIGN){
				my_free_list = free_list + FREELIST_INDEX(i);
				p = *my_free_list;
				if(0 != p){ // free_list 有未用完区块
					*my_free_list = p->free_list_link;
					start_free = (char*)p;
					end_free = start_free +i;
					//递归调用自己，为了修正nobjs
					return chunk_alloc(size, nobjs);
					// 注意： 任何残零头部都被编入到free-list 中备用
				}
			}
			end_free =0; // 如果出现意外，没有任何内存可用
			// 调用第一级配置器，看看是oom否有用
			start_free = (char*)malloc_alloc:allocate(bytes_to_get);
			// 这会导致异常，或者存不足的情况获得改善
		}
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get；
		//递归调用自己， 为了修正nobjs
		return chunk_alloc(size, nobjs);
		
	}	
	
}




/**
	内存处理基本工具
	1. uninitialized_copy
	2. uninitialized_fill
	3. uninitialized_fill_n
	[要求]：
	- commit or rollback : 要么全部成功，要么不做任何事
*/


// uninitialized_copy
template<class InputIterator, class ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,
	ForwardIterator result){
		retun __uninitialized_copy(first,last, result, value_type(result));
		// 利用value_type() 取出first de value_type
	}
	
	
// POD  判断
template<class InputIterator, class ForwardIterator, class T>
inline ForwardIterator
__uninitialized_copy(InputIterator first, InputIterator last, 
		ForwardIterator result ,T*){
			typedef typename __type_traits<T>::is_POD_type is_POD;
			return __uninitialized_copy_aux(first, last, result, is_POD());
}
	

// POD型别
template<class InputIterator, class ForwardIterator, class T>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, __true_type){
			return copy(first, last, result);   // 调用STL算法 copy()
		}	

// non-POD型别

template<class InputIterator, class ForwardIterator, class T>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, __false_type){
			ForwardIterator cur = result;
			// 异常处理省略
			for(;first != last; ++first, ++cur){
				construct(&*cur, *first);   // 必须一个一个元素构造
			}
			return cur;

		}	
	
	
// 针对char\*, wchar_t\*的版本
inline char* uninitialized_copy(const char* first, const char* last, char* result){
	memmove(result, first, last - first);
	return result + (last - first);
} 
	
inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last,
	wchar_t* result){
	memmove(result, first, sizeof(wchar_t)*(last - first));
	return result+(last - first);
}
	
	
	
	
	
	
	
	
	
// uninitialized_fill
// 在i出调用construct(&*i, x)
template<class ForwardIterator, class T>
void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x){
	__uninitialized_fill(first, last, x, value_type(first));
}

// 判断POD
template<class ForwardIterator , class T, class T1>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last,
	const T& x, T1*){
		typedef typename __type_traits<T>::is_POD_type is_POD;
		__uninitialized_fill_aux(first, last, x, is_POD());
	}

	
	
// POD型别
template<class ForwardIterator , class T, class T1>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last,
	const T& x, __true_type){
		fill(first, last, x);  // 调用STL算法 fill()
		
}

// non-POD型别
template<class ForwardIterator , class T, class T1>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last,
	const T& x, __false_type){
					ForwardIterator cur = result;
			// 异常处理省略
			for(;first != last; ++first, ++cur){
				construct(&*cur, *first);   // 必须一个一个元素构造
			}
			return cur;
		
}




// uninitialized_fill_n
/**
	1. 首先使用萃取器取出first的value type,
	2. 然后判断型别是否为POD型别（plain old data）,即标量型别，或者传统的 c struct型别；
	3. pod必然有用无用的trivial ctor/copy/dtor/assignment函数
	4. 对pod采用memcpy之类高效率的函数,non-pod采用保险的做法；
*/
template<class ForwardIterator , class Size, class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x){
	return __uninitialized_fill_n(first,n, x, value_type(first));
}

// POD型别判断
template <class ForwardIterator , class Size, class T,class T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,
	Size n , const T& x, T1*){
		typedef typename __type_traits<T1>::is_POD_type is_POD;
		return __uninitialized_fill_n_aux(first, n, x, is_POD());
	}



// POD 型别
template <class ForwardIterator , class Size, class T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,
	Size n , const T& x, __true_type){
		return fill_n(first, n, x);  // 告诫函数执行
	}

	
// non-POD型别
template<class ForwardIterator, class Size, class T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,
	Size n, const T& x, __false_type){
		ForwardIterator cur = first;
		for(; n>0; --n, ++cur){
			construct(&*cur, x);
		}
		return cur;
	}



































