#pragma once
#include "../../HttpServer/include/router/RouterHandler.h"
#include "../../HttpServer/include/utils/MysqlUtil.h"
#include "../../HttpServer/include/utils/JsonUtil.h"
#include "../qBlocklyServer.h"

class FilepublicHandler : public http::router::RouterHandler 
{
public:
    explicit FilepublicHandler(qBlocklyServer* server) : server_(server) {}
    
    void handle(const http::HttpRequest& req, http::HttpResponse* resp) override;

private:
    bool updateFilelist(int userId, int fileId, const std::string& title, const std::string& description);
private:
    qBlocklyServer*       server_;
    http::MysqlUtil     mysqlUtil_;
};