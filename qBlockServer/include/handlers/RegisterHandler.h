#pragma once
#include "../../HttpServer/include/router/RouterHandler.h"
#include "../../HttpServer/include/utils/MysqlUtil.h"
#include "../../HttpServer/include/utils/JsonUtil.h"
#include "../../HttpServer/include/utils/RandomCharUtil.h"
#include "../qBlocklyServer.h"
#include <sstream>
#include <openssl/sha.h>


class RegisterHandler : public http::router::RouterHandler 
{
    public:
        explicit RegisterHandler(qBlocklyServer* server) : server_(server) {}

        void handle(const http::HttpRequest& req, http::HttpResponse* resp) override;
    private:
        int insertUser(const std::string& username, const std::string& password,const std::string& email);
        bool isUserExist(const std::string& username);
        bool isEmailExist(const std::string& email);
        std::string sha256(const std::string& str);
    private:
        qBlocklyServer*     server_;
        http::MysqlUtil     mysqlUtil_;
        http::RandomChar    randomChar_;
};

