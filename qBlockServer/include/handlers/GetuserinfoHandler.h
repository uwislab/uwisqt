#pragma once
#include "../../HttpServer/include/router/RouterHandler.h"
#include "../../HttpServer/include/utils/MysqlUtil.h"
#include "../../HttpServer/include/utils/JsonUtil.h"
#include "../../HttpServer/include/utils/FileUtil.h"
#include "../qBlocklyServer.h"

class GetuserinfoHandler : public http::router::RouterHandler 
{
public:
    explicit GetuserinfoHandler(qBlocklyServer* server) : server_(server) {}
    
    void handle(const http::HttpRequest& req, http::HttpResponse* resp) override;

private:
    json queryUserInfo(int Id);
    qBlocklyServer*       server_;
    http::MysqlUtil     mysqlUtil_;
};