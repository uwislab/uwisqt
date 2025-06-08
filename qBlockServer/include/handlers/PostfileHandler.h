#pragma once
#include "../../HttpServer/include/router/RouterHandler.h"
#include "../../HttpServer/include/utils/MysqlUtil.h"
#include "../../HttpServer/include/utils/JsonUtil.h"
#include "../../HttpServer/include/utils/FileUtil.h"
#include "../qBlocklyServer.h"

class PostfileHandler : public http::router::RouterHandler
{
    public:
    explicit PostfileHandler(qBlocklyServer* server) : server_(server) {}
    
    void handle(const http::HttpRequest& req, http::HttpResponse* resp) override;

private:
    bool insertFile(int userId, const std::string& fileName, const std::string& filePath , double fileSize);
    bool updateFile(int fileId);
    int  queryFileNum();
    qBlocklyServer*       server_;
    http::MysqlUtil     mysqlUtil_;
};