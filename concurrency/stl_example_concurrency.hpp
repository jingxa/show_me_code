class join_threads{
public:
	explicit join_threads(std::vector<std::thread>& threads_):
		threads(threads_){}
	~join_threads(){
		for(unsigned long i=0;i<threads.size();i++){
			if(threads[i].joinable())
				threads[i].join();
		}
	}
};


/*for_each 并行实现*/

/*
	使用std::packaged_task 和 std::future 
*/
template<typename Iterator, typename Func>
void parallel_for_each(Iterator fir	, Iterator last ,Func f){
	unsigned long const length  = std::distance(first, last);
	
	if(!length)
		return;
	
	unsigned long const min_per_thread = 25; // 线程处理元素数量
	unsigned long const max_threads = 
		(length + min_per_thread -1)/min_per_thread; // 最大线程数
		
	unsigned long const hardware_threads = 
		std::thread:hardware_concurrency();   // 并行线程数量
		
	unsigned long const num_threads = 
		std::min(hardware_threads !=0 ? hardware_threads:2, max_threads);
		
	unsigned long const block_size = length  / num_threads;  // 线程处理元素的数量大小
	
	std::vector<std::future<void>> futures(num_threads-1); // 线程future
	std::vector<std::thread> threads(num_threads -1);
	join_threads joiner(threads);  
	
	
	Iterator block_start = first;
	for(unsigned long i=0;i<num_threads -1,; ++i){  // 每个线程
		Iterator block_end = block_start;
		std::advance(block_end, block_size);  // block 范围
		std::packaged_task<void(void)>task(  // 设定一个任务
			[=](){
				std::for_each(block_start,block_end,f);
			});
		futures[i] = task.get_future();  // 关联到future上
		threads[i] = std::thread(std::move(task));  // 使用任务初始化一个线程
		block_start = block_end; // 更新位置
	}
	
	std::for_each(block_start,last,f); // 剩下的元素
	for(unsigned long i=0; i<num_threads -1;++1){ // 获取结果
		futures[i].get();
	}
}




/*
	2. std::find的并行实现
	std::promise 设置值和异常
	std::atomic 保持状态
	
	
*/

template<typename Iterator, typename MatchType>
Iterator parallel_find(Iterator first, Iterator last, MatchType match){
	struct find_element{
		void operator()(Iterator begin, Iterator end,
						MatchType match,
						std::promise<Iterator>* result,
						std::atomic<bool>* done_flag)
		{
			try{
				for(;(begin != end)&& !done_flag->load(); ++begin ){
					if(*begin == match)
					{
						result->set_value(begin);  // 设置值
						done_flag->store(true);   // 终止
						return;
					}
				}
			}catch(...){
				try{
					// 设置异常
					result->set_excption(std::current_exception());
					done_flag->store(true);
				}catch(...){
					
				}
			}
		}
	};
	
	
	unsigned long const length = std::distance(first, last);
	if(!length)
		return last;
	
	unsigned long const min_per_thread =25;
	unsigned long const max_threads = 
		(length + min_per_thread-1) / min_per_thread;
	
	unsigned long const hardware_threads = 
		std::thread::hardware_concurrency();
		
	unsigned long const num_threads = 
		std::min(hardware_threads != 0 ? hardware_threads:2, max_threads);
	
	unsigned long const block_size = length / num_threads;
	
	std::promise<Iterator> result;
	std::atomic<bool> done_flag(false);  // 初始化
	
	std::vector<std::thread> threads(num_threads -1);
	
	{
		join_threads joiner(threads);
		
		Iterator block_start = first;
		for(unsigned long = 0; i< num_threads -1; i++){
			Iterator block_end = block_start;
			std::advance(block_end, block_size);
			threads[i] = std::thread(find_element(),
									block_start,
									block_end,
									match, &result,&done_flag);
			block_start = block_end;
		}
		find_element()(block_start,last, match,&result,&done_flag);
	}
	if(!done_flag.load()){ // 遍历完，未找到
		return last;
	}
	
	return result.get_future().get();
	
}




/*
	使用std::async 实现的find算法
*/

template< typename Iterator, typename MatchType>
Iterator parallel_find_impl(Iterator first, Iterator last,
	MatchType match, std::atomic<bool>& done){

		try{
			unsigned long const  length = std::distance(first, last);
			unsigned long const min_per_thread = 25；
			if(length < (2*min_per_thread))
			{
				for(;(first != last) && !done.load();++first){
					if(*first == match){
						done = true;
						return first;
					}
				}
				return last;
			}else{  // 元素多余2倍，使用多线程
				Iterator const mid_point = first + (length /2);
				// 异步查找后半截
				std::future<Iterator> async_result = 
					std::async(&parallel_find_impl<Iterator,MatchType>,
						mid_point, last, match, std::ref(done));
				
				主线程 查找前半截
				Iterator const direct_result = 
					parallel_find_impl(first, mid_point,match,done);
					
				return (direct_result == mid_point)?
					async_result.get():direct_result;
			}
		}catch(...){
			done = true;
			throw;
		}
}

template<typename Iterator, typename MatchType>
Iterator parallel_find(Iterator first, Iterator last, MatchType match){
	std::atomic<bool> done(false);
	return parallel_find_impl(first,last, match, done);
}

















