#include <sys/time.h>
#include "timer.h"


tk_pq_t tk_timer;  // 优先队列
size_t tk_current_msec;   // 

// 定时器比较
int timer_comp(void *ti, void *tj){
	tk_timer_t *timeri = (tk_timer_t*)ti;
	tk_timer_t *timerj = (tk_timer_t*)tj;
	return (timeri->key < timerj->key) ? 1 : 0; 
}

// 更新时间
void tk_time_update(){
	// 获取当前时间
	struct timeval tv;
	int rc = gettimeofday(&tv, NULL);
	tk_current_msec = ((tv.tv_sec * 1000) + (tv.tv_usec / 1000) );  // 转换成微秒
}

// 定时器初始化
int tk_timer_init(){
	// 建立连接立即初始化
	// 初始化优先队列大小 TK_PQ_DEFAULT_SIZE = 10
	int rc = tk_pq_init(&tk_timer, timer_comp, TK_PQ_DEFAULT_SIZE);

	// 更新当前时间
	tk_time_update();
	return 0;
}



int tk_find_timer(){
	int time;
	// 返回队列中最早时间和当前时间之差
	while(!tk_pq_is_empty(&tk_timer)){
		// 更新当前时间
		tk_time_update();
		// timer_node 指向最小的时间
		tk_timer_t *timer_node = (tk_timer_t*)tk_pq_min(&tk_timer);

		// 如果已经删除则释放此节点 （tk_del_timer只置位不删除）
		if(timer_node->deleted){
			int rc = tk_pq_delmin(&tk_timer);
			free(timer_node);
			continue;
		}
		// 此时timer_node 为最小时间点， time为优先队列最小时间减去当前时间
		time = (int)(timer_node->key - tk_current_msec);
		time = (time > 0) ? time : 0;
		break;
	}
	return time;
}

// 处理超时定时器
void tk_handle_expire_timers(){
	while(!tk_pq_is_empty(&tk_timer)){
		// 更新当前时间
		tk_time_update();
		tk_timer_t *timer_node = (tk_timer_t*)tk_pq_min(&tk_timer);
		// 如果已删除则释放此节点
		if(timer_node->deleted){
			int rc = tk_pq_delmin(&tk_timer);
			free(timer_node);
			continue;
		}

		// 最早入队节点超时时间大于当前时间（未超时）
		// 结束超时时间检查， 顺带删除下标记为删除的节点
		if(timer_node->key > tk_current_msec ){
			return;
		}

		// 出现了没被删除但是超时的节点，用handler处理
		if(timer_node->handler){
			timer_node->handler(timer_node->request);
		}

		int rc = tk_pq_delmin(&tk_timer);
		free(timer_node);

	}
}


// 添加定时器
void tk_add_timer(tk_http_request_t* request, size_t timeout, timer_handler_pt handler){
	tk_time_update();
	// 申请新的tk_time_t 节点，并加入到优先队列
	tk_timer_t *timer_node = (tk_timer_t*)malloc(sizeof(tk_timer_t));
	request->timer = timer_node;
	// 加入超时时间，删除信息
	timer_node->key = tk_current_msec + timeout;
	timer_node->deleted = 0;
	timer_node->handler = handler;
	// tk_timer_t 反向指向request
	timer_node->request = request;
	// 插入优先队列
	int rc = tk_pq_insert(&tk_timer, timer_node);
}


// 删除定时器
void tk_del_timer(tk_http_request_t *request){
	tk_time_update();
	tk_timer_t *timer_node = request->timer;
	// 惰性删除，
	// 标记为删除，由 find_timer 和 handler_expire_timer 检查队列是删除
	timer_node->deleted = 1;
}