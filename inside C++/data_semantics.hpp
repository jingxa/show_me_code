/*
	3.1 data member 的绑定

		
*/

typedef	int length;

class Point3d{
public:
	// length 被决议为global length
	void mumble(length val){_val = val;}
	length mumble(){return _val;}
	//...
private:

	// length 的声明要放在“本class 对它的第一个参考之前”
	// 这个length会导致前面的操作 非法
	typedef float length;
	length _val;
	// ...
};




/*
	3.2 data member 的布局
	
	1. Nonstatic data member在class object 的
		同一access section（public, private, protected）中的排列顺序和声明顺序一样

*/

// 例子

class Point3d{
public:
	// ..
private:
	float x;
	static List<Point3d*> *freeeList;
	float y;
	static const int chunksize = 250;
	float z;
	
};

 // 等价于
class Point3d{
public:
	// ..
private:
	float x;
	static List<Point3d*> *freeeList;
	
private:
	float y;
	static const int chunksize = 250;
	
private:
	float z;
	
}; 
 

 
/*
 
	3.3 data Member 的存取
	对于一个
	```
	Point3d origin, *pt = &origin;
	origin.x  = 0.0;
	pt -> x = 0.0;
	
	```
	这两种存取方式的差异
	
*/

// static data member 的存取
// static data member 存放在 global data segment中，
// 取static data member 的地址得到的是一个指向static data的指针





/*
	3.4 继承

*/


/*  3.4.1  只有继承没有多态 */

/*  3.4.2  加上多态 */

struct no_virts{
	int d1,d2;
	
};


class has_virts: public no_virts{
public:
	virtual void foo();
	// ...
private:
	int d3;
};

/*	3.4.3 	虚拟继承  */













