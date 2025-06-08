#include "../../include/handlers/SessionavailableHandler.h"

void SessionavailableHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp)
{
    try
    {
        // 获取会话
        auto session = server_->getSessionManager()->getSession(req, resp);
        // 获取用户id
        if(session->getData("isLoggedIn") != "true")
        {
            // 用户未登录，返回未授权错误
            json errorResp;
            errorResp["status"] = "error";
            errorResp["message"] = "Unauthorized";
            std::string errorBody = errorResp.dump(4);

            server_->packageResp(req.getVersion(), http::HttpResponse::Unauthorized401,
                                 "Unauthorized", true, "application/json", errorBody.size(),
                                 errorBody, resp);
            return;
        }
        else
        {
            // 用户已登录，返回可用会话信息
            json successResp;
            successResp["status"] = "success";
            successResp["message"] = "Available session";
            std::string successBody = successResp.dump(4);
            server_->packageResp(req.getVersion(), http::HttpResponse::Ok200,
                                            "OK", true, "application/json", successBody.size(),
                                            successBody, resp);
            return;
        }

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