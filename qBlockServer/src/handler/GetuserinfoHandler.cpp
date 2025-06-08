#include "../../include/handlers/GetuserinfoHandler.h"

void GetuserinfoHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp)
{
    try
    {
        // 获取会话
        auto session = server_->getSessionManager()->getSession(req, resp);
        if (session->getData("isLoggedIn") != "true")
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
        // 获取用户id
        int userId = std::stoi(session->getData("userId"));
        // 获取文件信息
        auto filelist = queryUserInfo(userId);
        std::string responseBody = filelist.dump();
        resp->setStatusLine(req.getVersion(), http::HttpResponse::Ok200, "OK");
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(responseBody.size());
        resp->setBody(responseBody);
    }
    catch (const std::exception &e)
    { 
        json response = {
            {"status", "error"},
            {"message", e.what()}};
        std::string responseBody = response.dump();
        server_->packageResp(req.getVersion(), http::HttpResponse::InternalServerError500, "Internal Server Error", false, "application/json", responseBody.size(), responseBody, resp);
    }
}

json GetuserinfoHandler::queryUserInfo(int userId)
{
    std::string sql = "SELECT * FROM t_user WHERE user_id = ?";
    sql::ResultSet* result = mysqlUtil_.executeQuery(sql, userId);
    if(result->next())
    {
        json userinfo = {
            {"user_id", result->getInt("user_id")},
            {"username", result->getString("username")},
            {"email", result->getString("email")},
            {"avatar", result->getString("avatar")},
            {"last_login", result->getString("last_login")},
            {"permission", result->getInt("permission")}
        };
        return userinfo;
    }
}