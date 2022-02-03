#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H



#include <unordered_map>
#include <unordered_set>
#include <string>
#include <regex>
#include <error.h>
#include <mysql/mysql.h>

#include "../buffer/buffer.h"
#include "../log/log.h"
#include "../pool/sqlconnpool.h"
#include "../pool/sqlconnRAII.h"

class HttpRequest
{
public:
    enum PARSE_STATE
    {
        REQUEST_LINE,
        HEADERS,
        BODY,
        FINISH,
    };

    enum HTTP_CODE
    {
        NO_REQUEST = 0,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURSE,
        FORBIDDENT_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNESTION,
    };

    HttpRequest() { Init(); }
    ~HttpRequest() = default;
    //初始化
    void Init();
    //解析
    bool parse(Buffer &buff);
    //返回地址
    std::string path() const;
    //返回&地址
    std::string &path();
    
    std::string method() const;
    
    std::string version() const;
    
    std::string GetPost(const std::string &key) const;
    
    std::string GetPost(const char *key) const;
    //是否保持连接
    bool IsKeepAlive() const;

private:
    //解析请求行
    bool ParseRequestLine_(const std::string &line);
    //解析请求头部 
    void ParseHeader_(const std::string &line);
    //解析请求正文
    void ParseBody_(const std::string &line);
    //解析地址
    void ParsePath_();
    //解析post
    void ParsePost_();
    //解析url
    void ParseFormUrlencoded_();

    static bool UserVerify(const std::string &name, const std::string &pwd, bool isLogin);

    PARSE_STATE state_;
    std::string method_, path_, version_, body_;
    std::unordered_map<std::string, std::string> header_;
    std::unordered_map<std::string, std::string> post_;

    static const std::unordered_set<std::string> DEFAULT_HTML;
    static const std::unordered_map<std::string, int> DEFAULT_HTML_TAG;
    static int ConverHex(char ch);
};

#endif//HTTP_REQUEST_H