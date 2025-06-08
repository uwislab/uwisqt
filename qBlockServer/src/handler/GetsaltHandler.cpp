#include "../../include/handlers/GetsaltHandler.h"

void GetsaltHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp)
{
    // json parsed = json::parse(req.getBody());
    // std::string username = parsed["username"];
    // std::string salt = queryUserSalt(username);

    std::string salt = queryUserSalt(req.getQueryParameters("username"));
    if (salt.empty())
    {
        json failureResp;
        failureResp["status"] = "fail";
        failureResp["salt"] = "";
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersion(), http::HttpResponse::Conflict409, "Conflict");
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
    }
    else
    {
        json successResp;   
        successResp["status"] = "success";
        successResp["salt"] = salt;
        std::string successBody = successResp.dump(4);

        resp->setStatusLine(req.getVersion(), http::HttpResponse::Ok200, "OK");
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(successBody.size());
        resp->setBody(successBody);
    }   
}

std::string GetsaltHandler::queryUserSalt(const std::string& username)
{
    // 使用预处理语句, 防止sql注入
    std::string sql = "SELECT salt FROM t_user WHERE username = ?";
    sql::ResultSet* res = mysqlUtil_.executeQuery(sql, username);
    if (res->next())
    {
        std::string salt = res->getString("salt");
        return salt;
    }
    // 如果查询结果为空，则返回NULL
    return "";
}