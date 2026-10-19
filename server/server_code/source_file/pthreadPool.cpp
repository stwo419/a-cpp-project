#include"pthreadPool.h"

pthreadPool::pthreadPool(int num):iswork(true),maxPthread(10000),minPthread(3),workingPthread(0),existthread(num)
    {
        pthread_mutex_init(&this->mutex,NULL);
        pthread_cond_init(&this->cond_variaty,NULL);
        pthread_cond_init(&this->admini_cond_variaty,NULL);
        num=num>5?num:minPthread;
        for(int i=0;i<num;++i)
        {
            pthread_t pid;
            pthread_mutex_lock(&this->mutex);
            pthread_create(&pid,NULL,working,this);
            pthread_detach(pid);
            this->ptid.push_back(pid);
            pthread_mutex_unlock(&this->mutex);
        }
        
        //启动管理线程用于管理线程数
        pthread_create(&this->admini_pid,NULL,admini_thread,this);
    }

    void *pthreadPool::working(void *arg)     //函数还需要增加阻塞等
    {
        pthreadPool * pool=(pthreadPool *)arg;
        while(true)
        {
            pthread_mutex_lock(&pool->mutex);
            while(pool->iswork&&pool->tast_queue.empty())
                pthread_cond_wait(&pool->cond_variaty,&pool->mutex);

            if(!pool->tast_queue.empty()&&pool->iswork)
            {
                pool->workingPthread++;
                task_t task;
                task=pool->tast_queue.front();
                pool->tast_queue.pop();
                pthread_mutex_unlock(&pool->mutex);
                task.function(task.argc);
                pthread_mutex_lock(&pool->mutex);
                pool->workingPthread--;
                pthread_mutex_unlock(&pool->mutex);
                //解决当线程还在执行且线程池停止工作时造成的冗余，资源占有
                if(!pool->iswork)
                    pool->remove_self_thread();
            }
            else                //任务队列为空或则和线程池停止工作
            {
                pthread_mutex_unlock(&pool->mutex);
                pool->remove_self_thread();
                break;
            }
            
        }
    }

    void *pthreadPool::admini_thread(void *argc)
    {
        pthreadPool *pool=( pthreadPool *)argc;
        while(pool->iswork)
        {
            pthread_mutex_lock(&pool->mutex);
            int myWORKING=pool->workingPthread;
            int myEXIXTING=pool->existthread;
            int queneCount=pool->tast_queue.size();
            pthread_mutex_unlock(&pool->mutex);
            if(myEXIXTING==pool->minPthread&&myWORKING<3)
            {}
            else if(myEXIXTING<pool->minPthread)
            {
                int a=pool->minPthread-myEXIXTING;
                for(int i=0;i<a;++i)
                {
                    pthread_t pid;
                    pthread_mutex_lock(&pool->mutex);
                    pthread_create(&pid,NULL,working,pool);
                    pthread_detach(pid);
                    pool->ptid.push_back(pid);
                    pool->existthread++;
                    pthread_mutex_unlock(&pool->mutex);
                }
            }
            else        //存在的线程数大于最小线程数
            {
                double master_rate=(double)(myWORKING)/myEXIXTING;
                if(master_rate>0.8)     //线程占用比例较大，增加线程数
                {
                    int add=(int)((master_rate*2-1)*myEXIXTING);
                    for(int i=0;i<add;++i)
                        pool->add_one_thread(pool);
                }
                else if(master_rate<0.3)
                {
                    int reduce=(1-2*master_rate)*myEXIXTING;
                    int remain=myEXIXTING-reduce;
                    if(remain<pool->minPthread)
                    {
                        reduce+=pool->minPthread-remain;
                    }
                    for(int i=0;i<reduce;++i)
                        pthread_cond_signal(&pool->cond_variaty);
                }
            }
            //每10秒轮询查看是否需要调整线程数
            
            sleep(1);
        }
    }

    pthreadPool::~pthreadPool()
    {
        if(this->iswork)
        {
            pthread_mutex_lock(&this->mutex);
            this->iswork=false;
            while(!this->tast_queue.empty())
                this->tast_queue.pop();
            pthread_mutex_unlock(&this->mutex);
            pthread_cond_broadcast(&cond_variaty);
        }
    }

    void pthreadPool::add_task_auto_working(task_t task)  //增加任务到任务队列中
    {
        pthread_mutex_lock(&this->mutex);
        this->tast_queue.push(task);
        pthread_mutex_unlock(&this->mutex);
        pthread_cond_signal(&this->cond_variaty);
    }
    
    void pthreadPool::remove_self_thread()      //回收自生线程
    {
        pthread_mutex_lock(&this->mutex);
        int joinPid=(int)pthread_self();
        auto it=std::find(this->ptid.begin(),this->ptid.end(),joinPid);
        this->ptid.erase(it);
        this->existthread--;
        pthread_mutex_unlock(&this->mutex);
    }

    void pthreadPool::add_one_thread(pthreadPool *pool)
    {
        if(pool->existthread>=pool->maxPthread)
        {
            write(STDOUT_FILENO,"max thread count",sizeof("max thread count"));
            return ;
        }
        pthread_mutex_lock(&pool->mutex);
        pthread_t pid;
        pthread_create(&pid,NULL,working,pool);
        pthread_detach(pid);
        pool->ptid.push_back(pid);
        pool->existthread++;
        pthread_mutex_unlock(&pool->mutex);

    }