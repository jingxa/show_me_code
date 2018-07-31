#ifndef THREADPOOL_H
#define THREADPOOL_H

#include<list>
#include<iostream>
#include<cassert>
#include<stdexcept>
#include<pthread.h>
#include  "locker.h"


template<typename T>
class threadpool{
public:
    threadpool(int thread_number =4, int max_requests = 10000);

    ~threadpool();
    // 将任务添加到线程池
    bool add(T* request);

private:
    // 线程入口函数 必须是static
    static void* worker(void* arg);

    // 真正的工作函数
    void run();

private:
    int _thread_numbers;        // 线程池中的数目，一般等于cpu核心
    int _max_request;           // 最多的排队请求个数
    pthread_t* _threads;          // 保护线程ID的数组
    std::list<T*> _workqueue;      // 请求队列
    locker _queuelocker;            // 保护请求队列的互斥锁
    semaphore   _queuestat;         // 用于指示请求队列状态的信号量
    bool _stop;                     // 用于指示线程池是否停止工作

};

// 实现

template<typename T>
threadpool<T>::threadpool(int thread_number, int max_requests):
    _thread_numbers(thread_number),_stop(false),_threads(NULL){

    assert((thread_number > 0)&& (max_requests)>0);
    _threads = new pthread_t[_thread_numbers];

    if(!_threads){
        throw std::runtime_error("线程池分配内存 失败！");
    }

    // 创建线程
    for(int i=0;i<_thread_numbers; i++){
        std::cout<<"创建线程编号："<<i<<std::endl;
        if(pthread_create(_threads+i,NULL, worker, this) != 0)
        {
            delete[] _threads;
            throw std::runtime_error("线程创建 失败！");
        }
        if(pthread_detach(_threads[i])){
            delete[] _threads;
            throw std::runtime_error("线程分离 失败！");
        }
    }
}


template<typename T>
threadpool<T>::~threadpool(){
    delete[] _threads;
    _stop = true;
}



// 添加任务
template<typename T>
bool threadpool<T>::add(T* request){
    _queuelocker.lock();  // 锁
    if(_workqueue.size() > _max_request){   // 请求队列达到最大
        _queuelocker.unlock();
        return false;
    }
    _workqueue.push_back(request);
    _queuelocker.unlock();
    _queuestat.post();   // 信号量通知
    return true;

}

// worker
template<typename T>
void* threadpool<T>::worker(void* arg){
    threadpool* pool = (threadpool*)arg;
    pool->run();
    return pool;
}





// run
template<typename T>
void threadpool<T>::run(){
    while(!_stop){
        _queuestat.wait();   // 等待信号量（类似消费者）
        _queuelocker.lock();
        if(_workqueue.empty()){  // 任务队列为空
            _queuelocker.unlock();
            continue;
        }
        T* request = _workqueue.front();
        _workqueue.pop_front();     // 减去一个任务
        _queuelocker.unlock();

        if(! request){   // 任务不存在
            continue;
        }
        request->process();
    }

}














#endif // THREADPOOL_H
