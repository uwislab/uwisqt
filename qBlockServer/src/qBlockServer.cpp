#include "../include/qBlocklyServer.h"
#include "../include/handlers/LoginHandler.h"
#include "../include/handlers/GetsaltHandler.h"
#include "../include/handlers/RegisterHandler.h"
#include "../include/handlers/LogoutHandler.h"
#include "../include/handlers/GetfileHandler.h"
#include "../include/handlers/PostfileHandler.h"
#include "../include/handlers/GetuserfilelistHandler.h"
#include "../include/handlers/GetuserinfoHandler.h"
#include "../include/handlers/GetpictureHandler.h"
#include "../include/handlers/PostpictureHandler.h"
#include "../include/handlers/FilepublicHandler.h"
#include "../include/handlers/GetpublicfilelistHandler.h"
#include "../include/handlers/GetpublicfileinfoHandler.h"
#include "../include/handlers/UserinfochangeHandler.h"
#include "../include/handlers/PostcommentHandler.h"
#include "../include/handlers/GetcommentlistHandler.h"
#include "../include/handlers/SessionavailableHandler.h"
#include "../include/handlers/DeleteHandler.h"
#include "../include/handlers/UpdateHandler.h"
#include "../HttpServer/include/http/HttpRequest.h"
#include "../HttpServer/include/http/HttpResponse.h"
#include "../HttpServer/include/http/HttpServer.h"
#include <muduo/base/Logging.h>

using namespace http;

qBlocklyServer::qBlocklyServer(int port, const std::string& name,  muduo::net::TcpServer::Option option):httpServer_(port, name, option)
{
    LOG_INFO << "qBlocklyServer started";
    initializeServer();
}

void qBlocklyServer::setThreadNum(int numThreads)
{
    httpServer_.setThreadNum(numThreads);
}

void qBlocklyServer::start()
{
    httpServer_.start();
}

void qBlocklyServer::initializeServer()
{
    // 初始化数据库连接池
    http::MysqlUtil::init("tcp://127.0.0.1:3306", "root", "root", "qblock", 10);

    // 初始化会话
    initializeSession();
    // 初始化路由
    initializeRouter();
}

void qBlocklyServer::initializeSession()
{
    // 创建会话存储
    auto sessionStorage = std::make_unique<http::session::MemorySessionStorage>();
    // 创建会话管理器
    auto sessionManager = std::make_unique<http::session::SessionManager>(std::move(sessionStorage));
    // 设置会话管理器
    setSessionManager(std::move(sessionManager));
}

void qBlocklyServer::initializeRouter()
{
    httpServer_.Post("/login", std::make_shared<LoginHandler>(this));
    httpServer_.Post("/register",std::make_shared<RegisterHandler>(this));
    httpServer_.Get("/getsalt",std::make_shared<GetsaltHandler>(this));
    httpServer_.Get("/logout",std::make_shared<LogoutHandler>(this));
    httpServer_.Get("/getfile",std::make_shared<GetfileHandler>(this));
    httpServer_.Post("/postfile",std::make_shared<PostfileHandler>(this));
    httpServer_.Get("/getuserfilelist",std::make_shared<GetuserfilelistHandler>(this));
    httpServer_.Get("/getuserinfo",std::make_shared<GetuserinfoHandler>(this));
    httpServer_.Get("/getpicture",std::make_shared<GetpictureHandler>(this));
    httpServer_.Post("/postpicture",std::make_shared<PostpictureHandler>(this));
    httpServer_.Post("/filepublic",std::make_shared<FilepublicHandler>(this));
    httpServer_.Get("/getpublicfilelist",std::make_shared<GetpublicfilelistHandler>(this));
    httpServer_.Get("/getpublicfileinfo",std::make_shared<GetpublicfileinfoHandler>(this));
    httpServer_.Post("/userinfochange",std::make_shared<UserinfochangeHandler>(this));
    httpServer_.Post("/postcomment",std::make_shared<PostcommentHandler>(this));
    httpServer_.Get("/getcommentlist",std::make_shared<GetcommentlistHandler>(this));
    httpServer_.Get("/sessionavailable",std::make_shared<SessionavailableHandler>(this));
    httpServer_.Get("/delete",std::make_shared<DeleteHandler>(this));
    httpServer_.Get("/update",std::make_shared<UpdateHandler>(this));
}

void qBlocklyServer::packageResp(const std::string &version,
                             http::HttpResponse::HttpStatusCode statusCode,
                             const std::string &statusMsg,
                             bool close,
                             const std::string &contentType,
                             int contentLen,
                             const std::string &body,
                             http::HttpResponse *resp)
{
    if (resp == nullptr) 
    {
        LOG_ERROR << "Response pointer is null";
        return;
    }

    try 
    {
        resp->setVersion(version);
        resp->setStatusCode(statusCode);
        resp->setStatusMessage(statusMsg);
        resp->setCloseConnection(close);
        resp->setContentType(contentType);
        resp->setContentLength(contentLen);
        resp->setBody(body);
        
        LOG_INFO << "Response packaged successfully";
    }
    catch (const std::exception& e) 
    {
        LOG_ERROR << "Error in packageResp: " << e.what();
        // 设置一个基本的错误响应
        resp->setStatusCode(http::HttpResponse::InternalServerError500);
        resp->setStatusMessage("Internal Server Error");
        resp->setCloseConnection(true);
    }
}