/**

	1. 递归锁
*/

class dataAccess{
private:
	std::mutex dbMutex;
	...
	
public:
	void insertdata(...){
		std::lock_guard<std::mutex> lg(dbMutex);
		...
	}
	
	void createdata(...){
		std::lock_guard<std::mutex> lg(dbMutex);
		...
	}
	
	
	void createandInsertdata(...){
		std::lock_guard<std::mutex> lg(dbMutex);
		...
		createdata(...);   // ERROR ，错误，死锁，dbMutex已经被锁定了
	}
	
};

// recursive_mutex


class dataAccess{
private:
	std::recursive_mutex dbMutex;
	...	
public:
	void insertdata(...){
		std::lock_guard<std::recursive_mutex> lg(dbMutex);
		...
	}
	void createdata(...){
		std::lock_guard<std::recursive_mutex> lg(dbMutex);
		...
	}	
	void createandInsertdata(...){
		std::lock_guard<std::recursive_mutex> lg(dbMutex);
		...
		createdata(...);   // 没问题
	}
	
};




// try_lock
// try_lock 可能假性失败，即没被使用但也返回false
/*
	std::adopt_lock : 告诉 std::lock_guard 该互斥元被锁定，
	沿用互斥元上锁的所有权

*/
std::mutex m;
while(m.try_lock() == false ){
	dosomething();
}

std::lock_guard<std::mutex> lg(m, std::adopt_lock);


// time_lock
// 1. timed_lock
// 2. recursive_timed_mutex


std::timed_lock m;
if(m.try_lock_for(std::chrono::secondes(1))){
	std::lock_guard<std::timed_lock> lg(m, std::adopt_lock);
	...
}else{
	couldnotgetthelock();  // 不能获取lock
}




// 处理多个lock
/*
	std::adopt_lock : 告诉 std::lock_guard 该互斥元被锁定，
	沿用互斥元上锁的所有权

*/
std::mutex m1;
std::mutex m2;

...
{
	std::lock(m1,m2);  // 同时锁定成功，或者失败
	std::lock_guard<std::mutex> lockM1(m1,std::adopt_lock);
	std::lock_guard<std::mutex> lockM2(m2,std::adopt_lock);
	...
} // 自动释放所有锁




//try_lock
std::mutex m1;
std::mutex m2;
...
int idx = std::try_lock(m1,m2); 
if(idx < 0){  // 所有锁成功
	std::lock_guard<std::mutex> lockM1(m1, std::adopt_lock);
	std::lock_guard<std::mutex> lockM2(m2, std::adopt_lock);
	...
} // 自动释放所有锁
else{
	// idx是失败的lock的索引
}


// mutex仍会锁定

std::mutex m1;
std::mutex m2;

...
{
	std::lock(m1,m2);
	// no lock adopted
}
... // mutex 仍在










/**
	2. unique_lock
	

*/

// 构造函数
// 1. 传递try_to_lock,表示企图锁定mutex但不希望阻塞
std::unique_lock<std::mutex> lock(mutex, std::try_to_lock);

// 2. 传递一个时间段，表示尝试一段时间内锁定
std::unique_lock<std::mutex> lock(mutex, std::chrono::secondes(1));
if(lock){
	...
}

// 3. 传递defer_lock, 表示初始化这一lock对象，但尚未打算锁住mutex;
std::unique_lock<std::mutex> lock(mutex, std::defer_lock);



// 稍后锁住
std::mutex m1;
std::mutex m2;

std::unique_lock<std::mutex> lockM1(m1, std::defer_lock);
std::unique_lock<std::mutex> lockM2(m2, std::defer_lock);

...
std::loc(m1,m2);  // 延迟锁住






// 延迟初始化
// 单线程
bool initialized  = false;

...
if(!initialized){
	init();  // 某初始化操作
	initialized = true;
}

// 多线程，会发生data race(数据竞争)
std::once_flag oc; // 全局标志
...
std::call_once(oc, init); // 如果没有初始化 就执行






