#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include <sys/time.h>

template <class T>
class BlockDeque{
public:
    explicit BlockDeque(size_t MaxCapacity = 1000);

    ~BlockDeque();
    //清空队列
    void clear();
    //判断是否为空
    bool empty();
    //判断是否超过或等于最大容量
    bool full();
    //关闭并唤醒所有线程
    void Close();
    //返回队列大小
    size_t size();
    //返回最大容量
    size_t capacity();
    //返回队列前端对象
    T front();
    //返回队列后端对象
    T back();
    //压入队列后端
    void push_back(const T &item);
    //压入队列前端
    void push_front(const T &item);
    //弹出，并将item赋值为弹出对象
    bool pop(T &item);
    //在指定时间内弹出，并将item赋值为弹出对象
    bool pop(T &item, int timeout);
    //唤醒一个线程
    void flush();

private:
    std::deque<T> deq_;
    //队列容量
    size_t capacity_;
    //锁
    std::mutex mtx_;

    bool isClose_;
    //定义两个条件变量，只要有一个进入wait状态，这个线程就会进入wait状态
    //当队列为空时，消费者条件变量将锁定线程，当队列非空时将唤醒线程
    std::condition_variable condConsumer_;
    //当队列为满时，生产者条件变量将锁定线程，当队列非满的时候将唤醒线程
    std::condition_variable condProducer_;
};

template <class T>
BlockDeque<T>::BlockDeque(size_t MaxCapacity):capacity_(MaxCapacity)
{
    assert(MaxCapacity > 0);
    isClose_ = false;
}

template <class T>
BlockDeque<T>::~BlockDeque()
{
    Close();
}

template <class T>
void BlockDeque<T>::Close()
{
    {
        //locker在离开该局部的时候会被析构，同时解锁
        std::lock_guard<std::mutex> locker(mtx_);
        dep_.clear();
        isClose_ = true;
    }
    condProducer_.notify_all();
    condConsumer_.notify_all();
}

//激活一个被消费者条件变量锁定的线程
template <class T>
void BlockDeque<T>::flush()
{
    condConsumer_.notify_one();
}

template <class T>
void BlockDeque<T>::clear()
{
    std::lock_guard<std::mutex> locker(mtx_);
    deq_.clear();
}

template <class T>
T BlockDeque<T>::front()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.front();
}

template <class T>
T BlockDeque<T>::back()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.back();
}

template <class T>
size_t BlockDeque<T>::size()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.size();
}

template <class T>
size_t BlockDeque<T>::capacity()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return capacity_();
}

template <class T>
void BlockDeque<T>::push_back(const T &item)
{
    std::lock_guard<std::mutex> locker(mtx_);
    while(deq_.size() >= capacity_)
    {//被虚假唤醒后再次判断，若数据依旧超过容量，则应继续休眠。
        condProducer_.wait(locker);
    }
    deq_.push_back(item);
    condConsumer_.notify_one();
}

template <class T>
void BlockDeque<T>::push_front(const T &item)
{
    std::lock_guard<std::mutex> locker(mtx_);
    while(deq_.size() >= capacity_)
    {//被虚假唤醒后再次判断，若数据依旧超过容量，则应继续休眠。
        condProducer_.wait(locker);
    }
    deq_.push_front(item);
    condConsumer_.notify_one();
}

template <class T>
bool BlockDeque<T>::empty()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.empty();
}

template <class T>
bool BlockDeque<T>::full()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.size() >= capacity_;
}

template <class T>
bool BlockDeque<T>::pop(T &item)
{
    std::lock_guard<std::mutex> locker(mtx_);
    while(deq_.empty())
    {
        condConsumer_.wait(locker);
        if(isClose_)
        {
            return false;
        }
    }
    item = deq_.front();
    deq_.pop_front();
    condProducer_.notify_one();
    return true;
}

template <class T>
bool BlockDeque<T>::pop(T &item,int timeout)
{
    std::lock_guard<std::mutex> locker(mtx_);
    while(deq_.empty())
    {
        if(condConsumer_.wait_for(locker,std::chrono::seconds(timeout))
            == std::cv_status::timeout)
        {
            return false;
        }
        if(isClose_)
        {
            return false;
        } 
    }
    item = deq_.front();
    deq_.pop_front();
    condProducer_();
    return true;
}

#endif BLOCKQUEUE_H