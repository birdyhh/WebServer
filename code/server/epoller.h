#ifndef EPOLLER_H
#define EPOLLER_H

#include <sys/epoll.h>
#include<fcntl.h>
#include <unistd.h>//close()
#include <assert.h>//assert()
#include <vector>
#include <errno.h>

class Epoller{
public:
    explicit Epoller(int maxEvent = 1024);

    ~Epoller();

    bool AddFd(int Fd, uint32_t event);

    bool ModFd(int Fd, uint32_t event);

    bool DelFd(int Fd);

    int Wait(int timeouMs = -1);

    int GetEvenFd(size_t i) const;

    uint32_t GetEvents(size_t i) const;

private:
    int epollFd_;

    std::vector<struct epoll_event> events_;
};

#endif //EPOLLER_H