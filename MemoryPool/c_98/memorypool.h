#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H


#include <limits.h>
#include< stddef.h>

template<typename T, size_t BlockSize = 4096>
class MemoryPool{
public:
	typedef 	T				value_type;  
	typedef		T*				pointer;
	typedef		T&				reference;
	typedef		const T*		const_pointer;
	typedef		const T&		const_reference;
	typedef		size_t			size_type;
	typedef		ptrdiff_t		difference_type;
	
	// 来自allocate的标准
	template<typename U>
	struct rebind{
		typedef MemoryPool<U> other;
	};
	
	// 构造函数
	MemoryPool() throw();
	MemoryPool(const MemoryPool& memoryPool) throw();
	template<class U> MemoryPool(const MemoryPool<U>& memoryPool) throw();
	
	// 析构函数
	~MemoryPool() throw();
	
	// 元素取地址
	pointer address(reference x)const throw();
	const_pointer address(const_reference x)const throw();
	
	// 分配/回收 一个元素的空间
	pointer allocate(size_type n=1, const_pointer hint =0);
	void deallocate(pointer p, size_type n=1);
	
	// 最大容量
	size_type max_size() const throwd();
	
	// 基于内存池的元素构造和析构
	void construct(pointer p , const_reference val);
	vod destroy(pointer p);
	
	
	// 自带申请内存和释放内存的构造和析构
	pointer newElement(const reference val);
	void deleteElement(pointer p);
	
	
private:
	// union ， 用于存放指针（存放free-list的每个列表的指针）
	union Slot_{
		value_type element;
		Slot_* next;
	};
	
	typedef char* data_pointer_;   // char* 指向内存首地址
	typedef Slot_ slot_type_;		// Slot_的值类型
	typedef	Slot_*	slot_pointer_;	// Slot_指针
	
	slot_pointer_ currentBlock_;    // 内存块链表的头指针
	slot_pointer_ currentSlot_;		// 元素链表的头指针
	slot_pointer_ lastSlot_;		// 可存放元素的最后指针
	slot_pointer_ freeSlots_;		// 元素构造后释放的内存链表头指针
	
	// 内存对齐所需空间
	size_type padPonter(data_pointer_ p, size_type align) const throw();
	
	void allocateBlock();  // 申请内存块放进内存池
	
	
};


#include "MemoryPool.hpp"


#endif  // MEMORY_POOL_H