#pragma once
#include "../../HttpServer/include/router/RouterHandler.h"
#include "../../HttpServer/include/utils/MysqlUtil.h"
#include "../../HttpServer/include/utils/JsonUtil.h"
#include "../qBlocklyServer.h"

class GetpictureHandler : public http::router::RouterHandler 
{
public:
    explicit GetpictureHandler(qBlocklyServer* server) : server_(server) {}
    
    void handle(const http::HttpRequest& req, http::HttpResponse* resp) override;

private:
    std::string queryUserAvatar(int userId);
    std::string queryFileImg(int fileId);

private:
    qBlocklyServer*       server_;
    http::MysqlUtil     mysqlUtil_;
};