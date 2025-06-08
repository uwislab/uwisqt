#pragma once
#include "../../HttpServer/include/router/RouterHandler.h"
#include "../../HttpServer/include/utils/MysqlUtil.h"
#include "../../HttpServer/include/utils/JsonUtil.h"
#include "../qBlocklyServer.h"

class UserinfochangeHandler : public http::router::RouterHandler {
    public:
        explicit UserinfochangeHandler(qBlocklyServer* server) : server_(server) {}

        void handle(const http::HttpRequest& req, http::HttpResponse* resp) override;
    private:
        int updateUserInfo(int userId, const std::string& username, const std::string& email);
    private:
        qBlocklyServer*     server_;
        http::MysqlUtil     mysqlUtil_;
};