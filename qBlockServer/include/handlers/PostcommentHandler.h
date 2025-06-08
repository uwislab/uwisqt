#pragma once
#include "../../HttpServer/include/router/RouterHandler.h"
#include "../../HttpServer/include/utils/MysqlUtil.h"
#include "../../HttpServer/include/utils/JsonUtil.h"
#include "../../HttpServer/include/utils/FileUtil.h"
#include "../qBlocklyServer.h"

class PostcommentHandler : public http::router::RouterHandler
{
    public:
    explicit PostcommentHandler(qBlocklyServer* server) : server_(server) {}
    
    void handle(const http::HttpRequest& req, http::HttpResponse* resp) override;

private:
    int insertComment(int userId, const std::string& userName, int fileId, const std::string& comment, const std::string& time);
    qBlocklyServer*       server_;
    http::MysqlUtil     mysqlUtil_;
};