#ifndef MEMORY_BLOCK_TCC
#define MEMORY_BLOCK_TCC

// 计算对齐所需要空间

template<typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::size_type
MemoryPool<T,BlockSize>::padPointer(data_pointer_ p, 
	size_type align) const throw{
	size_t result = reinterpret_cast<size_t>(p); // 内存地址转型为size_t
	return ((align - result)%align); // 差额
}

// 构造函数
template<typename T, size_t BlockSize>
MemoryPool<T,BlockSize>::MemoryPool() throw(){
	currentBlock_ = 0;
	currentSlot_ = 0;
	lastSlot_ = 0;
	freeSlots_ =0;
}

// 复制构造函数
template<typename T, size_t BlockSize>
MemoryPool<T,BlockSize>::MemoryPool( const MemoryPool& memoryPool) 
throw(){
	
	MemoryPool();
}

template<typename T, size_t BlockSize>
template<class U>
MemoryPool<T,BlockSize>::MemoryPool( const MemoryPool<U>& memoryPool) 
throw(){
	
	MemoryPool();
}


// 析构函数
template<typename T, size_t BlockSize>
MemoryPool<T,BlockSize>::~MemoryPool() throw(){
	slot_pointer_ curr = currentBlock_;
	while(curr !=0){
		slot_pointer_ prev = curr->next;
		// 转化为void* ，不需要析构，直接释放空间
		operator delete(reinterpret_cast<void*>(curr));
		curr = prev;
	}
}


















#endif MEMORY_BLOCK_TCC