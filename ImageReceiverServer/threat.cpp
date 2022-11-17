#include "threat.h"

threat::threat()
{
    initfist = false;
}

threat::~threat()
{
    QThread * tmp;
    for (auto it = threadSize.begin(); it != threadSize.end(); ++it)
    {
        tmp = it.key();
        tmp->exit();
        tmp->wait(3000);
        delete tmp;
    }
}

threat &threat::get_class()
{
    static threat th;
    return th;
}

QThread *threat::get_thread()
{
    if (!initfist)
    {
        init_thread_type(THREAD_SIZE,10);
    }
    if (type == THREAD_SIZE)
        return find_thread_size();
    else
        return find_handle_size();
}

void threat::remove_thread(QThread * thread)
{
    auto t = threadSize.find(thread);
    if (t != threadSize.end())
    {
        t.value() --;
        if (type == HANDLE_SIZE && t.value() == 0 && threadSize.size() > 1)
        {
            threadSize.remove(thread);
            thread->exit();
            thread->wait(3000);
            delete thread;
        }
    }
}

void threat::init_thread_type(thread_type type, unsigned int max)
{
    if (!initfist)
    {
        this->type = type;
        this->size = max;
        if (this->size == 0)
        {
            if(type == THREAD_SIZE)
                this->size = 10;
            else
                this->size = 1000;
        }

        if (type == THREAD_SIZE)
            init_thread_size();
        else
        {
            QThread * tmp = new QThread;
            threadSize.insert(tmp,0);
            tmp->start();
        }
    }
    initfist = true;
}

void threat::init_thread_size()
{
    QThread * tmp;
    for (unsigned int i = 0; i < size;++i)
    {
        tmp = new QThread;
        threadSize.insert(tmp,0);
        tmp->start();
    }
}

QThread * threat::find_handle_size()
{
    for (auto it  = threadSize.begin();it != threadSize.end() ;++it)
    {
        if (it.value() < size)
        {
            it.value() ++;
            return it.key();
        }
    }
    QThread * tmp = new QThread;
    threadSize.insert(tmp,1);
    tmp->start();
    return tmp;
}

QThread *threat::find_thread_size()
{
    auto it = threadSize.begin();
    auto ite = threadSize.begin();
    for (++it ; it != threadSize.end(); ++it)
    {
        if (it.value() < ite.value())
        {
            ite = it;
        }
    }
    ite.value() ++;
    return ite.key();
}

void threat::clear()
{
    for (auto it  = threadSize.begin();it != threadSize.end() ;++it)
    {
        it.value()  = 0;
    }
}
