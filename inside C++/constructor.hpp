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




/*
	二。 copy  constructor
	
*/
class ZooAnimal{
public:
	ZooAnimal();
	virtual  ~ZooAnimal();
	
	virtual void animate();
	virtual void draw();
	// ...
private:
	// animate()和draw()需要的数据
};

class Bear: public ZooAnimal{
public:
	Bear();
	virtual void animate();
	virtual void draw();
	virtual void dance();
	// ...
private:
	// 几个函数的数据
};


/*
	当ZooAnimal利用另一个 ZooAnimal 作为初值，Bear 以另一个 Bear作为初值，
	都可以直接靠“ bitwise copy semantics ”完成
	
	yoqi被默认构造函数初始化， vptr被设定为Bear class 的虚函数表，
	而winnie 拷贝 yoqi 的vptr是安全的；
*/

Bear yoqi;
Bear winnie = yoqi;


/*
	但是，当一个基类使用派生类的object 内容做初始化操作，
	franny 的vptr不能指向Bear的虚函数表，（如果使用“ bitwise copy”）
	导致错误；
	
	因此，合成出来的 ZooAnimal copy constructor 会明确设定object的vptr指向
	ZooAnimal 的虚函数表，而不是直接从 右边的class object拷贝过来；
	*/
ZooAnimal franny = yoqi; // 这回发生切割行为；



/*
	处理 virtual Base Class Subobject
	
	1. virtual base  class 
	2. 一个class object 以另一个object作为初值，后者拥有virtual base class subojbect,
	   会使 “bitwise copy”失效；
	   
	编译器都虚拟继承的承诺： 让派生类对象中的“virtual base class subojbect位置”在
	执行器准备好；“Bitwise copy”会破坏这个位置，所有在合成的copy constructor中
	
*/

class Raccon: public virtual ZooAnimal{
public:
	Raccon(){ /*设定 private的初值*/}
	Raccon(int val){/* 初始值设定*/}
	// ..
private:
	// 所有数据
};

/*
	编译器产生的代码：
	1. 调用 ZooAnimal 的默认构造函数，
	2. 将 Raccon 的vptr 初始化
	3. 并且在 Raccon 中定位出 ZooAnimal subojbect的位置
	这些代码安插在 Raccon 的两个constructor 中的最前部分；
*/


/*
	如果 Raccon object	 利用另一个 Raccon object 作为初始值；
	那么 bitwise copy  足够
	
	但是如果使用 派生类来作为基类的初值，编译器必须合成一个copy constructor，
	为了使不同类的vptr指向正确的位置；然后对每一个members执行必要的memberwise的初始化；
*/






































 