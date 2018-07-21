#include < exception>

struct empty_stack: std::exception{
	const char* what() const throw();
};

// 线程安全栈定义
template<typename T>
class threadsafe_stack{
private:
	std::stack<T> data;
	mutable std::mutex m;
	
public:
	threadsafe_stack(){}
	threadsafe_stack(const threadsafe_stack& other){
		std::lock_guard<std::mutex> lock(other.m);
		data = other.data;
	}
	
	threadsafe_stack& operator=(const threadsafe_stack&) = delete;
	
public:
	void push(T new_value){
		std::lock_guard<std::mutex> lock(m);
		data.push(std::move(new_value));  // 如果空间不够可能会抛出异常,但是栈是安全的
	}
	
	
	std::shared_ptr<T> pop(){
		std::lock_guard<std::mutex> lock(m);
		if(data.empty()) throw empty_stack();   // 可能会抛出异常
		// std::make_shared 可能会抛出异常，但是对象会被正确销毁，没有内存泄露
		std::shared_ptr<T> const res(std::make_shared<T>(std::move(data.top())));
		data.pop();
		return res;
	}
	
	
	bool empty() const{
		std::lock_guard<std::mutex> lock(m);
		return data.empty();
	}
};






/*
	2. 使用锁和条件变量的线程安全队列
*/
template<typename T>
 class threadsafe_queue{
private:
	mutable std::mutex m;
	std::queue<T> data_queue;
	std::condition_variable data_cond;
	
public:
	threadsafe_queue(){}
	
	void push(T new_value){
		std::lock_guard<std::mutex> lk(m);
		data_queue.push(std::move(new_value));
		data_cond.notify_one();
	}
	
	
	void wait_and_pop(T& value){
		std::unique_lock<std::mutex> lk(m);
		data_cond.wait(lk,[this]->bool{return !data_queue.empty();});
		value = std::move(data_queue.front());
		data_queue.pop();
	}
	 
	std::shared_ptr<T> wait_and_pop(){
		std::unique_lock<std::mutex> lk(m);
		data_cond.wait(lk,[this]{return !data_queue.empty();});
		std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
		data_queue.pop();
		return res;
	}
	
	bool try_pop(T& value){
		std::lock_guard<std::mutex> lk(m);
		if(data_queue.empty())
			return false;
		vlaue = std::move(data_queue.front());
		data_queue.pop();
		return true;
	}
	
	std::shared_ptr<T> try_pop(){
		std::lock_guard<std::mutex> lk(m);
		if(data_queue.empty())
			return std::shared_ptr<T>();
		std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
		data_queue.pop();
		return res;
	}
	
	bool empty() const{
		std::lock_guard<std::mutex> lk(m);
		return data_queue.empty();
	}
	 
 };



// 线程安全的查找表
template<typename Key, typename Value, typename Hash = std::hash<Key>>
class threadsafe_lookup_table{
private:
	class bucket_type{
	private:
		typedef std::pair<Key,Value> bucket_value;
		typedef	std::list<bucket_value> bucket_data;
		typedef typename bucket_data::iterator bucket_iterator;
		
		bucket_data data;
		mutable boost::shared_mutex mutex; 
		
		bucket_iterator find_entry_for(const Key& key) const{
			return std::find_if(data.begin(),data.end(),
			[&](bucket_value const& item){return item.first == key;});
		}
		
	public:
		Value value_for(const Key& key ,const Value& default_value) const{
			boost::shared_lock<boost::shared_mutex> lock(mutex);
			bucket_iterator const found_entry  = find_entry_for(key);
			return (found_entry == data.end())?default_value : found_entry->second;
		}
		
		void add_or_updata_mapping(const Key& key ,const Value& value){
			std::unique_lock<boost::shared_mutex> lock(mutex);
			bucket_iterator const found_entry = find_entry_for(key);
			if(found_entry == data.end()){
				data.push_back(bucket_value(key,value));
			}
			else{
				found_entry->second = vlaue;
			}
		}
		
		void remove_mapping(const Key& key){
			std::unique_lock<boost::shared_mutex> lock(mutex);
			bucket_iterator const found_entry = find_entry_for(key);
			if(found_entry != data.end()){
				data.erase(found_entry);
			}
		}
	};
	
	
	std::vector<std::unique_ptr<bucket_type>> buckets;
	Hash hasher;
	
	bucket_type& get_bucket(const Key& key){
		std::size_t const bucket_index = hasher(key)% buckets.size();
		return *buckets[bucket_index];
	}
	
	
public:
	typedef Key key_type;
	typedef Value	mapped_value;
	
	typedef Hash hash_type;
	
	
	threadsafe_lookup_table(unsigned num_buckets = 19, 
		const Hash& hasher = Hash()):buckets(num_buckets),hasher(hasher) {
		for(unsigned i=0;i<num_buckets;i++){
			buckets[i].reset(new bucket_type);
		}
	}
	
	
	threadsafe_lookup_table(const threadsafe_lookup_table& other) = delete;
	threadsafe_lookup_table& operator=(const threadsafe_lookup_table&) = delete;
	
	Value value_for(const Key& key,cosnt Value& default_value= Value()){
		return get_bucket(key).value_for(key,default_value);
		
	}
	
	void add_or_update_mapping(Key const& key, const Value& value){
		get_bucket(key).add_or_updata_mapping(key, value);
	}
	 
	void remove_mapping(const Key& key){
		get_bucket(key).remove_mapping(key);
	}
	
};










































