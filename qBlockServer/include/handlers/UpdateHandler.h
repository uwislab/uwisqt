#pragma once
#include "../../HttpServer/include/router/RouterHandler.h"
#include "../../HttpServer/include/utils/MysqlUtil.h"
#include "../../HttpServer/include/utils/JsonUtil.h"
#include "../qBlocklyServer.h"

class UpdateHandler : public http::router::RouterHandler 
{
public:
    explicit UpdateHandler(qBlocklyServer* server) : server_(server) {}
    
    void handle(const http::HttpRequest& req, http::HttpResponse* resp) override;

private:
    std::string getLatestVersion();
private:
    qBlocklyServer*       server_;
    http::MysqlUtil     mysqlUtil_;
};