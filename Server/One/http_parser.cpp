#include "http_parser.h"

//构造函数
http_parser::http_parser(const std::string request){
	assert(request.size() > 0);
	this->request = request;
}

// 析构函数
http_parser::~http_parser(){}


// 解析结果
http_request http_parser::get_parse_result(){
	parse_line();
	parse_requestline();
	parse_header();
	return parse_result;
}


/**
	private 函数实现
*/

void http_parser::parse_line(){
	std::string::size_type line_begin = 0;   // 行首索引
	std::string::size_type check_index = 0;  // 正在解析的字符索引

	while(check_index < request.size())
	{
		// 如果当前字符为‘\r’,则有可能读完一行
		if(request[check_index] == '\r')
		{
			// 如果当前字符是最后一个字符说明请求没有读取完整
			if((check_index + 1) == request.size())
			{
				std::cout<<" 请求未读取完整."<<std::endl;
				return;
			}

			// 如果下一个字符是 '\n'，说明读取完整一行
			else if( '\n' == request[check_index + 1])
			{
				lines.push_back(std::string(request,
						line_begin,check_index - line_begin)); // 存储读取的一行
				check_index +=2;  // '\r\n'
				line_begin = check_index;
			}
			else  // 下一个字符不是 \n,格式错误
			{
				std::cout<<"请求发生错误."<<std::endl;
			}
		}
		else
		{
			++check_index; // 继续查询下一个字符
		}

	}
	return;
}



void http_parser::parse_requestline()
{
	std::string requestline = lines[0];

	// first_ws 指向请求行的第一个空白字符(' '或'\t')
	auto first_ws = std::find_if(requestline.cbegin(), requestline.cend(),
			[](char c)->bool{return(' '== c || '\t'==c);});
	// 如果请求行中没有空白则格式错误
	if(first_ws == requestline.cend())
	{
		std::cout<<"请求格式错误."<<std::endl;
		return;
	}

	// http解析请求方法
	parse_result.method = std::string(requestline.cebin(),first_wt);


	// http解析version
	auto reverse_list_ws = std::find_if(requestline.crbegin(), requestline.crend(),  //reverse_list_ws 指向请求行中
					[](char c)->bool{return (' '== c || '\t' == c);}); 				// 的最后一个空白字符

	auto last_ws = reverse_list_ws.base();  // 转换为正向迭代器
	parse_result.version = std::string(last_ws,requestline.cend());

	// http解析uri
	while((' '== *first_ws || '\t'==*first_ws) && first_ws != requestline.cend())
		++first_ws;  // 跳过空白字符

	-- last_ws; // 向前查找普通字符
	while((' '==*last_ws || '\t'==*last_ws)&& last_ws != requestline.cbegin())
		--last_ws;

	parse_result.uri = std::string(first_ws,last_ws+1);  // last_ws当前为最后一个字符

	return;
}



/* 解析头部 */
void http_parser::parse_header()
{
	for(int i=0; i< lines.size(); i++)
	{
		if(lines[i].empty()) // 遇到空行，说明头部解析完成
			return;
		// 处理host头部字段
		else if(strncasecmp(lines[i].c_str(),"Host:",5)==0)
		{
			auto iter = lines[i].cbegin() +5; // 跳过 “host:”
			while(' '== *iter || '\t' == *iter)
				++iter;
			parse_result.host = std::string(iter,lines[i].cend());
		// 处理connection 字段
		}else if(strncasecmp(lines[i].c_str(),"Connection:",11) == 0)
		{
			auto iter = lines[i].cbegin() + 11;
			while(' '== *iter || '\t' == *iter)
				++iter;
			parse_result.connection = std::string(iter,lines[i].cend());
		}
		else{
			 // 其他字段忽略
		}
	}
	return;
}
















