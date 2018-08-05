#include <stdlib.h>
#include <string.h>
#include "priority_queue.h"


/*
	priority_queue 以连续空间为存储；
	1. 地址空间中0 保留，从1 开始存储，因此存储size大小为：（1~size+1）
	2. 以小根堆作为存储方式；
	3. 因此，根节点k的子节点为k/2,k/2+1

*/


// 交换
void exch(tk_pq_t *tk_pq, size_t i, size_t j){
	void *tmp = tk_pq->pq[i];
	tk_pq->pq[i] = tk_qp->pq[j];
	tk_pq->pq[j] = tmp;
}


// 上浮操作，将元素浮到上层
void swim(tk_pq_t *tk_pq, size_t k){
	while( K > 1 && tk_pq->comp(tk_pq->pq[k], tk_pq->pq[k/2])){  // 和父节点比较
		exch(tk_pq,k, k/2);
		k /=2;
	}
}

// 调整整个存储空间，满足小根堆的方式
int sink(tk_pq_t *tk_pq, size_t k){
	size_t	 j;
	size_t	 nalloc = tk_pq->nalloc;  // 队列已使用空间
	while((k<<1) <= nalloc){   // 从上到下调整
		j = k<<1;
		if((j<nalloc) && (tk_pq->comp(tk_pq[j+1], tk_pq[j]) )) // 比较左子节点和右子节点
			j++; // 右节点

		if(!tk_pq->comp(tk_pq->pq[j], tk_pq->pq[k])) // 父节点和右子节点比较
			break;
		exch(tk_pq, j, k);  // 交换
		k = j;   // 向下传递
	}
	return k;
}


// 调整堆
int tk_pq_sink(tk_pq_t *tk_pq, size_t i){
	return sink(tk_pq, i);
}



// 优先队列初始化
int tk_pq_init(tk_pq_t *tk_pq, tk_pq_comparator_pt comp, size_t size){
	// tk_pq_t分配节点
	tk_pq->pq = (void**)malloc(sizeof(void*) * (size + 1));
	if(!tk_pq->pq)
		return -1;

	tk_pq->nalloc = 0;   // 使用空间
	tk_pq->size = size +1;  // 分配大小
	tk_pq->comp = comp;   // 比较方式
	return 0;
}


// 判断为空
int tk_pq_is_empty(tk_pq_t *tk_pq){
	// 通过 nalloc值快速判断
	return (tk_pq->nalloc == 0) ? 1 : 0;
}

// 存储空间大小
size_t tk_pq_size(tk_pq_t *tk_pq){
	// 获取队列大小
	return tk_pq->nalloc;
}


// 取得优先队列最小值
void* tk_pq_min (tk_pq_t *tk_pq){
	if(tk_pq_is_empty(tk_pq))
		return (void*)(-1);

	return tk_pq->pq[1];
}


// 重新分配空间
int resize(tk_pq_t *tk_pq, size_t new_size){
	if(new_size <= tk_pq->nalloc)
		return -1;

	void **new_ptr = (void**)malloc(sizeof(void*) * new_size);
	if(!new_ptr)
		return -1;

	// 将原本 nalloc+1元素值拷贝到new_ptr指向的位置
	memcpy(new_ptr, tk_pq->pq, sizeof(void*)*(tk_pq->nalloc +1));
	// 释放旧的元素
	free(tk_pq->pq);
	tk_pq->pq = new_ptr;
	tk_pt->size = new_size;

	return 0;
}


// 删除最小元素
int tk_pq_delmin(tk_pq_t *tk_pq){
	if(tk_pq_is_empty(tk_pq)){
		return 0;
	}
	exch(tk_pq, 1, tk_pq->nalloc); // 和最后一个元素交换
	-- tk_pq->nalloc;  // 修改使用空间， 相当于最后一个元素删除
	sink(tk_pq,1);  // 调整堆

	if((tk_pq->nalloc>0)&&(tk_pq->nalloc <= (tk_pq->size -1)/4) )  // 当使用空间不足完整空间1/4, 调整
		if(resize(tk_pq, tk_pq->size /2) < 0)  // 分配为原来空间的一半
			return -1;

	return 0;
}


// 插入元素
int tk_pq_insert(tk_pq_t *tk_pq, void *item){
	if(tk_pq->nalloc + 1 == tk_pq->size){
		if(resize(tk_pq, tk_pq->size * 2) < 0){   // 空间不足，分配为原来2倍
			return -1;
		}
	}

	tk_pq->pq->pq[++tk_pq->nalloc] = item;  // 插入新项
	swim(tk_pq, tk_pq->nalloc);  // 上浮
	return 0;
}