#include "../../include/handlers/UserinfochangeHandler.h"

void UserinfochangeHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp)
{
     try
    {
        // 获取会话
        auto session = server_->getSessionManager()->getSession(req, resp);
        // 获取用户id
        int userId = std::stoi(session->getData("userId"));
        json parsed = json::parse(req.getBody());
        std::string username = parsed["username"];
        std::string email = parsed["email"];
        // 更新用户信息
        int res = updateUserInfo(userId, username, email);
        if (res)
        {
            // 更新成功
            json successResp;
            successResp["status"] = "success";
            successResp["message"] = "User information updated successfully.";
            std::string successBody = successResp.dump(4);
            resp->setStatusLine(req.getVersion(), http::HttpResponse::Ok200, "OK");
            resp->setCloseConnection(true);
            resp->setContentType("application/json");
            resp->setContentLength(successBody.size());
            resp->setBody(successBody);
        }
        else
        {
            // 更新失败
            json failureResp;
            failureResp["status"] = "error";
            failureResp["message"] = "Failed to update user information.";
            std::string failureBody = failureResp.dump(4);
            resp->setStatusLine(req.getVersion(), http::HttpResponse::Conflict409, "Conflict");
            resp->setCloseConnection(true);
            resp->setContentType("application/json");
            resp->setContentLength(failureBody.size());
            resp->setBody(failureBody);
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

int UserinfochangeHandler::updateUserInfo(int userId, const std::string& username, const std::string& email)
{
        std::string sql = "UPDATE t_user SET username = ?, email = ? WHERE user_id = ?";
        // std::vector<std::string> params = {username, password};
        int res = mysqlUtil_.executeUpdate(sql, username, email, userId);
        return res;
}