#pragma once
#include "../../HttpServer/include/router/RouterHandler.h"
#include "../../HttpServer/include/utils/MysqlUtil.h"
#include "../../HttpServer/include/utils/JsonUtil.h"
#include "../../HttpServer/include/utils/RandomCharUtil.h"
#include "../qBlocklyServer.h"
#include <sstream>
#include <openssl/sha.h>

class LoginHandler : public http::router::RouterHandler 
{
public:
    explicit LoginHandler(qBlocklyServer* server) : server_(server) {}
    
    void handle(const http::HttpRequest& req, http::HttpResponse* resp) override;

private:
    std::pair<int,int> queryUserId(const std::string& username, const std::string& password);
    std::string sha256(const std::string& str);
private:
    qBlocklyServer*       server_;
    http::MysqlUtil     mysqlUtil_;
    http::RandomChar    randomChar_;
};