/**
	计算两个操作数的总和
	func1() + func2()
	
	采用异步操作：总时间 = max(func1的时间, func2的时间)
*/

int func1(){
	// return dosomething();
}

int func2(){
	// return dosomething();
}


// 使用异步调用
std::future<int> result1 (std::async(func1));
int result2 = func2();

int result = result1.get() + result2;

