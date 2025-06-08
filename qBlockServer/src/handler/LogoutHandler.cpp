#include "../../include/handlers/LogoutHandler.h"

void LogoutHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp)
{
     try
    {
        // 获取会话
        auto session = server_->getSessionManager()->getSession(req, resp);
        // 获取用户id
        int userId = std::stoi(session->getData("userId"));
        // 清除会话数据
        session->clearData();
        // 销毁会话
        server_->getSessionManager()->destroySession(session->getId());

        {   // 释放资源
            std::lock_guard<std::mutex> lock(server_->mutexForOnlineUsers_);
            server_->onlineUsers_.erase(userId);
        }

        // 返回响应报文
        json response;
        response["message"] = "logout successful";
        std::string responseBody = response.dump(4);
        resp->setStatusLine(req.getVersion(), http::HttpResponse::Ok200, "OK");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(responseBody.size());
        resp->setBody(responseBody);
    }
    catch (const std::exception &e)
    {
        // 捕获异常，返回错误信息
        json failureResp;
        failureResp["status"] = "error";
        failureResp["message"] = e.what();
        std::string failureBody = failureResp.dump(4);
        resp->setStatusLine(req.getVersion(), http::HttpResponse::BadRequest400, "Bad Request");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
    }
}