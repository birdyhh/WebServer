#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <unordered_map>
#include <fcntl.h> //open
#include <unistd.h> //close
#include <sys/stat.h>//stat
#include <sys/mman.h>//mmap,munmap

#include "../buffer/buffer.h"
#include "../log/log.h"

class HttpResponse
{
public:
    HttpResponse();
    ~HttpResponse();
    //初始化
    void Init(const std::string &srcDir, std::string &path, bool isKeepAlive = false, int code = -1);
    //创建response包
    void MakeResponse(Buffer &buff);
    //释放stat
    void UnmapFile();
    //返回mmFile_
    char *File();
    //返回文件大小
    size_t FileLen() const;
    //插入错误内容
    void ErrorContent(Buffer &buff, std::string message);
    //返回code
    int Code() const { return code_; }

private:
    //插入状态行
    void AddStateLine_(Buffer &buff);
    //插入头部行
    void AddHeader_(Buffer &buff);
    //插入内容
    void AddContent_(Buffer &buff);

    void ErrorHtml_();
    std::string GetFileType_();

    int code_;
    bool isKeepAlive_;

    std::string path_;
    std::string srcDir_;

    char *mmFile_;
    struct stat mmFileStat_;

    static const std::unordered_map<std::string, std::string> SUFFIX_TYPE;
    static const std::unordered_map<int, std::string> CODE_STATUS;
    static const std::unordered_map<int, std::string> CODE_PATH;
};

#endif //HTTPRESPONSE.H