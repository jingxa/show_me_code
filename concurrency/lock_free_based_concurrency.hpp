/**
1. 不使用锁的线程安全
*/

template<typename T>
class lock_free_stack{
private:
	strcat node{
		T data;
		node* next;
		
		node(const T& data_):data(data_){}
	};
	
private:
	std::atomic<node*> head;
	
public:
	void push(T const& data){
		node* const new_node(data);
		new_node->next = head.load();
		while(!head.compare_exchange_weak(new_node->next, new_node));
	}
	
	std::shared_ptr<T> pop(){
		node* old_head = head.load();
		while(old_head&&  // old_head存在
			!head.compare_exchange_weak(old_head,old_head->next));
		result old_head ? old_head->data : std::shared_ptr<T>();
	}
	
};





















