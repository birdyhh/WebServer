#include "buffer.h"

Buffer::Buffer(int initBufferSize) : 
    buffer_(initBufferSize),
    readPos_(0),
    writePos_(0)
    {}

size_t Buffer::ReadableBytes() const
{
    return writePos_ - readPos_;
}

size_t Buffer::WritableBytes() const
{
    return buffer_.size() - writePos_;
}

size_t Buffer::PrependableBytes() const
{
    return readPos_;
}

char* Buffer::BeginPtr_() 
{
    return &*buffer_.begin();
}

const char* Buffer::BeginPtr_() const
{
    return &*buffer_.begin();
}