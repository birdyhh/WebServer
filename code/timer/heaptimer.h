#ifndef HEAP_TIMER_H
#define HEAP_TIMER_H

#include <time.h>
//#include <arpa/inet.h>
#include <queue>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <functional>
#include <assert.h>
#include <chrono>
#include "../log/log.h"

typedef std::function<void()> TimeoutCallBack;
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds MS;
typedef Clock::time_point TimeStamp;

struct TimerNode
{
    int id;
    TimeStamp expires;
    TimeoutCallBack cb;
    bool operator<(const TimerNode& t)
    {
        return expires < t.expires;
    }
};

class HeapTimer
{
public:
    HeapTimer() { heap_.reserve(64); }
    ~HeapTimer() { clear(); }
    //调整连接时间
    void adjust(int id, int timeout);
    //增加连接
    void add(int id, int timeOut, const TimeoutCallBack &cb);
    //删除指定id结点，并触发回调函数
    void doWork(int id);
    //清空vector和map
    void clear();
    //清除超时连接
    void tick();
    //弹出连接
    void pop();
    //获得下个连接的时间
    int GetNextTick();

private:
    //删除连接
    void del_(size_t i);
    //如果一个连接比它的父连接小（最小堆），那么需要将它同父连接交换位置。
    //这样是这个连接在数组的位置上升。
    void siftup_(size_t i);
    // 如果一个连接比它的子连接大（最小堆），那么需要将它向下移动。
    bool siftdown_(size_t index, size_t n);
    //交换两个连接
    void SwapNode_(size_t i, size_t j);
    //用数组表示最小堆
    std::vector<TimerNode> heap_;
    //判断id是否已经存在
    std::unordered_map<int, size_t> ref_;
};

#endif //HEAP_TIMER_H