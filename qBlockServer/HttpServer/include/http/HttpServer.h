#pragma once 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>

#include "HttpContext.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "../router/Router.h"
#include "../session/SessionManager.h"

class HttpRequest;
class HttpResponse;

namespace http
{
    class HttpServer : muduo::noncopyable
    {
        public:
            using HttpCallback = std::function<void (const http::HttpRequest&, http::HttpResponse*)>;

                // 构造函数
            HttpServer(int port,
                    const std::string& name,
                    muduo::net::TcpServer::Option option = muduo::net::TcpServer::kNoReusePort);
            void setThreadNum(int numThreads)
            {
                server_.setThreadNum(numThreads);
            }

            void start();

            muduo::net::EventLoop* getLoop() const 
            { 
                return server_.getLoop(); 
            }

            void setHttpCallback(const HttpCallback& cb)
            {
                httpCallback_ = cb;
            }

            // 注册静态路由处理器
            void Get(const std::string& path, const HttpCallback& cb)
            {
                router_.registerCallback(HttpRequest::GET, path, cb);
            }
            
            // 注册静态路由处理器
            void Get(const std::string& path, router::Router::HandlerPtr handler)
            {
                router_.registerHandler(HttpRequest::GET, path, handler);
            }

            void Post(const std::string& path, const HttpCallback& cb)
            {
                router_.registerCallback(HttpRequest::POST, path, cb);
            }

            void Post(const std::string& path, router::Router::HandlerPtr handler)
            {
                router_.registerHandler(HttpRequest::POST, path, handler);
            }

            // 注册动态路由处理器
            void addRoute(HttpRequest::Method method, const std::string& path, router::Router::HandlerPtr handler)
            {
                router_.addRegexHandler(method, path, handler);
            }

            // 注册动态路由处理函数
            void addRoute(HttpRequest::Method method, const std::string& path, const router::Router::HandlerCallback& callback)
            {
                router_.addRegexCallback(method, path, callback);
            }

            // 设置会话管理器
            void setSessionManager(std::unique_ptr<session::SessionManager> manager)
            {
                sessionManager_ = std::move(manager);
            }

            // 获取会话管理器
            session::SessionManager* getSessionManager() const
            {
                return sessionManager_.get();
            }

        private:
            void initialize();

            void onConnection(const muduo::net::TcpConnectionPtr& conn);
            void onMessage(const muduo::net::TcpConnectionPtr& conn,
                        muduo::net::Buffer* buf,
                        muduo::Timestamp receiveTime);
            void onRequest(const muduo::net::TcpConnectionPtr&, const HttpRequest&);

            void handleRequest(const HttpRequest& req, HttpResponse* resp);
            
        private:
            muduo::net::InetAddress                      listenAddr_; // 监听地址
            muduo::net::TcpServer                        server_; 
            muduo::net::EventLoop                        mainLoop_; // 主循环
            HttpCallback                                 httpCallback_; // 回调函数
            router::Router                               router_; // 路由
            std::unique_ptr<session::SessionManager>     sessionManager_; // 会话管理器
    };
}