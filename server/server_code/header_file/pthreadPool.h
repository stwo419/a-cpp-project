#include <iostream>
#include<algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include<vector>
#include<queue>

class task_t
{
public:
    void (*function)(void *argc);
    void* argc;
};

class pthreadPool
{
private:

    pthread_mutex_t mutex;      //除有新客户端连接进来的通用锁
    pthread_cond_t cond_variaty;//条件变量
    std::queue<task_t> tast_queue;   //任务队列
    std::vector<pthread_t> ptid;     //线程id

    //管理线程的参数
    pthread_cond_t admini_cond_variaty;//管理员条件变量
    pthread_t admini_pid;       //管理员线程id
    int workingPthread;         //正在工作的线程数
    int maxPthread;             //线程数最多
    int minPthread;             //最少线程数
    int existthread;            //存在线程数

    bool iswork;

public:

    pthreadPool(int num);


    static void *working(void *arg);     //函数还需要增加阻塞等
    

    static void *admini_thread(void *argc);

    ~pthreadPool();

    void add_task_auto_working(task_t task); //增加任务到任务队列中

    
    void remove_self_thread();      //回收自生线程
   

    void add_one_thread(pthreadPool *pool);
};