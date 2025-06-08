#pragma once
#include "../../HttpServer/include/router/RouterHandler.h"
#include "../../HttpServer/include/utils/MysqlUtil.h"
#include "../../HttpServer/include/utils/JsonUtil.h"
#include "../../HttpServer/include/utils/FileUtil.h"
#include "../qBlocklyServer.h"

class GetfileHandler : public http::router::RouterHandler 
{
public:
    explicit GetfileHandler(qBlocklyServer* server) : server_(server) {}
    
    void handle(const http::HttpRequest& req, http::HttpResponse* resp) override;

private:
    std::tuple<int,bool,std::string> queryFileInfo(int fileId);
    qBlocklyServer*       server_;
    http::MysqlUtil     mysqlUtil_;
};