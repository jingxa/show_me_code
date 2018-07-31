#include "locker.h"
/*
    semaphore 类的实现
*/
semaphore::semaphore(){
    // 创建一个匿名信号量，允许进程的线程共享
    // 初始信号量为0
    if(sem_init(&_sem, 0,0) != 0)
        throw std::runtime_error("信号量初始化 失败！");
}

semaphore::~semaphore(){
    sem_destroy(&_sem);
}

bool semaphore::wait(){
    return sem_wait(&_sem) == 0;
}

bool semaphore::post(){
    return sem_post(&_sem) == 0;
}

/**
    locker 类实现
*/

locker::locker(){
    if(pthread_mutex_init(&_mutex, NULL) != 0)  // 动态初始互斥量
        throw std::runtime_error("线程互斥量初始化 失败！");
}

locker::~locker(){
    pthread_mutex_destroy(&_mutex);
}

bool locker::lock(){
    return pthread_mutex_lock(&_mutex) == 0;
}

bool locker::unlock(){
    return pthread_mutex_unlock(&_mutex) == 0;
}


/**

    condition 类的实现
*/

condition::condition(){
    if(pthread_mutex_init(&_cond_mutex,NULL) != 0)
        throw std::runtime_error("条件变量的互斥量初始化 失败！");
    if(pthread_cond_init(&_cond, NULL) != 0)
    {
        pthread_mutex_lock(&_cond_mutex);
        throw std::runtime_error("条件变量初始化 失败！");
    }
}


condition::~condition(){
    pthread_mutex_destroy(&_cond_mutex);
    pthread_cond_destroy(&_cond);
}

bool condition::wait(){
    int ret = 0;
    pthread_mutex_lock(&_cond_mutex);
    ret = pthread_cond_wait(&_cond, &_cond_mutex);
    pthread_mutex_unlock(&_cond_mutex);
    return ret == 0;  // 返回值为0 表示成功
}



bool condition::signal(){
    return pthread_cond_signal(&_cond) == 0;
}




