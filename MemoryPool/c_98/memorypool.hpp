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

// 返回地址
template<typename T, size_t BlockSize>
inline typename MemoryPool<T,BlockSize>::pointer
MemoryPool<T,BlockSize>::address(reference x)
const throw(){
	return &x;
}



// 返回地址的const 
template<typename T, size_t BlockSize>
inline typename MemoryPool<T,BlockSize>::const_pointer
MemoryPool<T,BlockSize>::address(const_reference x)
const throw(){
	return &x;
}


// 申请一块空闲的block放进内存池
template<typename T, size_t BlockSize>
void MemoryPool<T,BlockSize>::allocateBlock(){
	// 申请一块新内存
	data_pointer_ newBlock = reinterpret_cast<data_pointer_>
						(operator new(BlockSize));
	// 原来的block连接到newblock
	reinterpret_cast<slot_pointer_>(newBlock)->next = currentBlock_;
	// currentBlock_ 调整
	currentBlock_ = reinterpret_cast<slot_pointer_>(newBlock);
	
	// 对齐内存
	data_pointer_ body  = newBlock + sizeof(slot_pointer_);  // 内存加地址信息
	size_type bodypadding = padPointer(body, sizeof(slot_type_));
	
	// currentSlot_ 为该block 开始的地方加油bodypadding 个 char* 空间
	currentSlot_ = reinterpret_cast<slot_pointer_>(body+bodypadding);
	
	// 计算最后一个放置slot_type_ 的位置
	lastSlot_ = reinterpret_cast<slot_pointer_>(newBlock + 
					BlockSize - sizeof(slot_type_) +1);
	
}

// 返回指向分配新元素的所需要内存的指针
template<typename T, size_t BlockSize>
inline typename MemoryPool<T,BlockSize>::pointer
MemoryPool<T,BlockSize>::allocate(size_type , const_pointer){
	// 如果freeSlot_ 费空间，就在freeSlot_中取内存
	if(freeSlots_ !=0){
		pointer result = reinterpret_cast<pointer>(freeSlots_);
		// 更新 freeSlots_
		freeSlots_  = freeSlots_ ->next;
		return result;
	}else{
		if(currentSlot_->=lastSlot_){
			// 之前的内存用完了，分配新的Block
			allocateBlock();

		}
					// 从分配的block中划分出去
		return reinterpret_cast<pointer> (currentSlot_++);
	}
}


// 将元素内存归还给free内存链表
template<typename T, size_t BlockSize>
void MemoryPool<T,BlockSize>::allocate(pointer p , size_type){
	if(p!=0){
		// 转换成slot_pointer_ 指针， next 指向freeSlot_ 链表
		reinterpret_cast<slot_pointer_>(p)->next = freeSlots_;
		// 新的 freeSlots_ 头为p
		freeSlots_ = reinterpret_cast<slot_pointer_>(p);
	}
}


// 计算可以容纳的最多元素
template<typename T, size_t BlockSize>
inline typename MemoryPool<T,BlockSize>::size_type
MemoryPool<T,BlockSize>::max_size() const throw(){
	
	size_type maxBlocks = -1 / BlockSize;
	return (BlockSize - sizeof(data_pointer_))/sizeof(slot_type_)*maxBlocks;
	
}

// 在意分配的内存上构造对象
template<typename T, size_t BlockSize>
inline void
MemoryPool<T,BlockSize>::construct(pointer p, const_reference val){

	new (p) value_type(val);
}



// 销毁对象
template <typename T, size_t BlockSize>
inline void
MemoryPool<T, BlockSize>::destroy(pointer p)
{
  // placement new 中需要手动调用元素 T 的析构函数
  p->~value_type();
}



// 创建新元素
template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::pointer
MemoryPool<T, BlockSize>::newElement(const_reference val)
{
  // 申请内存
  pointer result = allocate();
  // 在内存上构造对象
  construct(result, val);
  return result;
}

// 删除元素
template <typename T, size_t BlockSize>
inline void
MemoryPool<T, BlockSize>::deleteElement(pointer p)
{
  if (p != 0) {
    // placement new 中需要手动调用元素 T 的析构函数
    p->~value_type();
    // 归还内存
    deallocate(p);
  }
}



#endif MEMORY_BLOCK_TCC