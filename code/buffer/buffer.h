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
    //读取数据？
    const char *Buffer::Peek() const;
    //确保len字节内可写
    void EnsureWriteable(size_t len);
    //已写字节
    void HasWritten(size_t len);
    //跳转到len长度后
    void Retrieve(size_t len);
    //跳转到end字符串后
    void RetrieveUntil(const char *end);
    //清空可读缓存?
    void RetrieveAll();
    //返回可读缓存，并清空
    std::string RetrieveAllToStr();
    //返回待写const字符串
    const char *BeginWriteConst() const;
    //返回待写字符串
    char *BeginWrite();
    //附加额外的传输缓存
    void Append(const std::string &str);
    void Append(const char *str, size_t len);
    void Append(const void *data, size_t len);
    void Append(const Buffer &buff);
    //读取数据到缓存区
    ssize_t ReadFd(int fd, int *Error);
    //
    ssize_t WriteFd(int fd, int *Error);

private:
    //返回Buffer字符串指针
    char *BeginPtr_();
    //返回Buffer字符串指针
    const char *BeginPtr_() const;
    //情空
    void MakeSpace_(size_t len);

    std::vector<char> buffer_;
    std::atomic<std::size_t> readPos_;
    std::atomic<std::size_t> writePos_;
};

#endif //BUFFER_H