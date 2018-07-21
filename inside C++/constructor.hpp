// implicit default constructor

/*
	1. 带有默认构造函数的成员对象 class 
	
*/

/****************** 例子1 ****************/
class Foo {
public:
	Foo();
	Foo(int);
	...
};

class Bar{
public:
	Foo foo;
	char *str;
	
};


// 编译器合成的版本
// 伪代码
Bar::Bar(){
		foo.Foo::FOO();  // 编译器代码
}


// 为了代码正确执行
// str 必须初始化， 程序员的代码
Bar::Bar(){
	str =0;  // explicit user code
}

// 编译器扩展版本
Bar::Bar(){
	foo.Foo::FOO();  // 编译器代码
	str =0;  // explicit user code
}



/************* 例子2 ：   ***************/
class Dopey{
public:
	Dopey();
	...
};

class Sneezy{
public:
	Sneezy();
	Sneezy(int);
	...
};

class Bashful{
public:
	Bashful();
	...
};

Class Snow_white{
public:
	Dopey dopey;
	Sneezy sneezy;
	Bashful bashful;
	...

private:
	int mumble;
};


// 如果未定义默认函数
// 编译器合成版本
Snow_white::Snow_white(){
	dopey.Dopey::Dopey();
	sneezy.Sneezy::Sneezy();
	bashful.Bashful::Bashful();
	
}

/* 程序员的代码 */
Snow_white::Snow_white():sneezy(1024){
	mumble = 2028;
}

// 编译器扩充为：
/* 程序员的代码 */
Snow_white::Snow_white():sneezy(1024){
	
	// 插入代码
	dopey.Dopey::Dopey();
	sneezy.Sneezy::Sneezy(1024);
	bashful.Bashful::Bashful();
	mumble = 2028;
}





/*
	2. 带有 “default constructor ”的Base class 
	如果一个没有任何构造函数的class 派生带有“default constructor”
	的base class。这个派生类的合成默认构造函数为non-trivial
*/


/*
	3. "带有一个virtual Function"的 Class 
	- 1. class  声明或者继承一个 virtual Function;
	- 2. class 派生子一个继承串链，其中有一个或者多个 virtual base classes;

	如果程序员没有声明constructor， 编译器就会合成一个
	default constructor;
	
	【编译器动作】
	- 1. virtual function table(vtpl)会被编译器产生出来，
		内放	virtual function  的地址；
	- 2. 在每一个class object中，一个额外的pointer member（vptr)
		会被编译器合成出来，内含相关的class vbtl的地址；
*/




/*
	4. "带有一个 virtual base class" 的class
	
*/


/*
	总结
	1. 在合成的default constructor中，只有base class subobjects和
		member class objects被初始化
	2. 其他的non-static data member, 如整数，整数指针，数组等不会被
		初始化，应该是程序员提供；
*/





 