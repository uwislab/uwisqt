#pragma once
#include "../../HttpServer/include/router/RouterHandler.h"
#include "../../HttpServer/include/utils/MysqlUtil.h"
#include "../../HttpServer/include/utils/JsonUtil.h"
#include "../qBlocklyServer.h"

class GetuserfilelistHandler : public http::router::RouterHandler 
{
public:
    explicit GetuserfilelistHandler(qBlocklyServer* server) : server_(server) {}
    
    void handle(const http::HttpRequest& req, http::HttpResponse* resp) override;

private:
    json queryFilelist(int userId);
private:
    qBlocklyServer*       server_;
    http::MysqlUtil     mysqlUtil_;
};