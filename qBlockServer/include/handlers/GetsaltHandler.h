#pragma once
#include "../../HttpServer/include/router/RouterHandler.h"
#include "../../HttpServer/include/utils/MysqlUtil.h"
#include "../../HttpServer/include/utils/JsonUtil.h"
#include "../qBlocklyServer.h"

class GetsaltHandler : public http::router::RouterHandler 
{
public:
    explicit GetsaltHandler(qBlocklyServer* server) : server_(server) {}
    
    void handle(const http::HttpRequest& req, http::HttpResponse* resp) override;

private:
    std::string queryUserSalt(const std::string& username);

private:
    qBlocklyServer*       server_;
    http::MysqlUtil     mysqlUtil_;
};