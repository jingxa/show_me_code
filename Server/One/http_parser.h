#ifdef HTTP_HTTP_PARSER_H
#define HTTP_HTTP_PARSER_H


#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include <iostream>

#include <cstring>

/**
	正在解析的请求状态：
		PARSE_REQUESTLINE 表示正在解析请求行
		PARSE_HEADER 表示正在解析头部
*/

enum PARSE_STATE{PARSE_REQUESTLINE = 0, PARSE_HEADER};


// 表示正在解析行的状态：即：读到一个完整的行、 行出粗， 和接受到的数据不完整；
enum LINE_STATE{LINK_OK =0, LINE_ERROR, LINE_MORE};

/**
	服务器处理http请求过程：
	状态：
		- MORE_DATA ： 表示请求不完整，需要继续读取请求
		- GET_REQUEST: 表示获得了一个完整的客户请求
		- REQUEST_ERROR: 表示客户端错误的请求语法
		- FORBIDDEN_REQUEST ： 表示客户对资源没有访问权限
		- INTERNAL_ERROR : 表示服务器内部错误
		- CLOSE_CONNECTION : 表示客户端关闭连接
*/

enum HTTP_CODE{MORE_DATA = 0, GET_REQUEST, REQUEST_ERROR, FORBIDDEN_REQUEST,
			INTERNAL_ERROR,CLOSE_CONNECTION};

			
// 客户端请求方法
enum METHOD{GET = 0, POST, HEAD, PUT, DELETE,TRACE,OPTIONS, CONNECT,PATCH};

// 解析后的数据存储在http_request结构体
typedef struct{
	std::string method;  	// 请求方法
	std::string url; 		// 请求的url
	std::string version; 	// http版本
	std::string host;		// 请求的主机名
	std::string connection; // connection首部
	
} http_request;


class http_parser{
public:
	http_parser(const std::string request);
	~http_parser();
	http_parser get_parse_result();  // 返回解析结果
	
	
private:
	void parse_line();					// 解析分割出一行内容
	void parse_requestline();			// 解析请求行
	void parse_header();				// 解析头部字段
	
	
	std::string request;				// 客户请求内容
	std::vector<std::string> lines;   	// 存储每一行
	http_request parse_result;			// 存储解析结果
};


#endif  // 	HTTP_HTTP_PARSER_H





















