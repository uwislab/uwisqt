#pragma once
#include "../../HttpServer/include/router/RouterHandler.h"
#include "../../HttpServer/include/utils/MysqlUtil.h"
#include "../../HttpServer/include/utils/JsonUtil.h"
#include "../qBlocklyServer.h"

class PostpictureHandler : public http::router::RouterHandler 
{
public:
    explicit PostpictureHandler(qBlocklyServer* server) : server_(server) {}
    
    void handle(const http::HttpRequest& req, http::HttpResponse* resp) override;

private:
    void updateUserAvatar(int userId, const std::string& pictureType);
    void updateFileImg(int fileId, const std::string& pictureType);

private:
    qBlocklyServer*       server_;
    http::MysqlUtil     mysqlUtil_;
};