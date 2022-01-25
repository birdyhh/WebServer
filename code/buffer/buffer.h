#ifndef BUFFER_H
#define BUFFER_H

#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/uio.h>
#include <vector>
#include <atomic>//C++11 中的原子操作
#include <assert.h>
class Buffer{
public:
    Buffer(int initBufferSize = 1024);
    ~Buffer() = default;
    //可写字节
    size_t WritableBytes() const;
    //可读字节
    size_t ReadableBytes() const;
    //等待传送字节
    size_t PrependableBytes() const;

private:
    //返回Buffer字符串指针
    char *BeginPtr_();
    //返回Buffer字符串指针
    const char *BeginPtr_() const;

    void MakeSpace_(size_t len);

    std::vector<char> buffer_;
    std::atomic<std::size_t> readPos_;
    std::atomic<std::size_t> writePos_;
};

#endif //BUFFER_H