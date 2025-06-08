#pragma once

#include <atomic>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <mutex>

#include "../HttpServer/include/http/HttpServer.h"
#include "../HttpServer/include/utils/MysqlUtil.h"
#include "../HttpServer/include/utils/FileUtil.h"
#include "../HttpServer/include/utils/JsonUtil.h"
#include "../HttpServer/include/utils/RandomCharUtil.h"

class LoginHandler;
class RegisterHandler;
class GetSaltHandler;
class LogoutHandler;
class GetfileHandler;
class PostfileHandler;
class GetuserfilelistHandler;
class GetuserinfoHandler;
class GetpictureHandler;
class PostpictureHandler;
class FilepublicHandler;
class GetpublicfilelistHandler;
class GetpublicfileinfoHandler;
class UserinfochangeHandler;
class PostcommentHandler;
class GetcommentlistHandler;
class SessionavailableHandler;
class DeleteHandler;
class UpdateHandler;

class qBlocklyServer {
    public:
        qBlocklyServer(int port, const std::string& name,  muduo::net::TcpServer::Option option = muduo::net::TcpServer::kNoReusePort);

        void setThreadNum(int numThreads);
        void start();
    private:
        void initializeServer();
        void initializeSession();
        void initializeRouter();

        void setSessionManager(std::unique_ptr<http::session::SessionManager> manager)
        {
            httpServer_.setSessionManager(std::move(manager));
        }

        http::session::SessionManager*  getSessionManager() const
        {
            return httpServer_.getSessionManager();
        }

        void packageResp(const std::string& version, http::HttpResponse::HttpStatusCode statusCode,
                    const std::string& statusMsg, bool close, const std::string& contentType,
                    int contentLen, const std::string& body, http::HttpResponse* resp);
    private:
        friend class LoginHandler;
        friend class RegisterHandler;
        friend class GetSaltHandler;
        friend class LogoutHandler;
        friend class GetfileHandler;
        friend class PostfileHandler;
        friend class GetuserfilelistHandler;
        friend class GetuserinfoHandler;
        friend class GetpictureHandler;
        friend class PostpictureHandler;
        friend class FilepublicHandler;
        friend class GetpublicfilelistHandler;
        friend class GetpublicfileinfoHandler;
        friend class UserinfochangeHandler;
        friend class PostcommentHandler;
        friend class GetcommentlistHandler;
        friend class SessionavailableHandler;
        friend class DeleteHandler;
        friend class UpdateHandler;
    private:
        http::HttpServer                                 httpServer_;
        http::MysqlUtil                                  mysqlUtil_;
        std::unordered_map<int, bool>                    onlineUsers_;
        std::mutex                                       mutexForOnlineUsers_; 
};