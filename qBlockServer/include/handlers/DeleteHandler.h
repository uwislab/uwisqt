#pragma once
#include "../../HttpServer/include/router/RouterHandler.h"
#include "../../HttpServer/include/utils/MysqlUtil.h"
#include "../../HttpServer/include/utils/JsonUtil.h"
#include "../qBlocklyServer.h"

class DeleteHandler : public http::router::RouterHandler 
{
public:
    explicit DeleteHandler(qBlocklyServer* server) : server_(server) {}
    
    void handle(const http::HttpRequest& req, http::HttpResponse* resp) override;

private:
    bool deleteFile(int userId, int fileId, int permission);
    bool deletePublicFile(int userId, int fileId, int permission);
    bool deleteComment(int userId, int commentId, int permission);
private:
    qBlocklyServer*       server_;
    http::MysqlUtil     mysqlUtil_;
};