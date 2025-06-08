#include "../../include/handlers/PostcommentHandler.h"


void PostcommentHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp)
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
        std::string userName = session->getData("username");
        // 获取文件信息
        json parsed = json::parse(req.getBody());
        std::string fileId = parsed["fileId"];
        std::string content = parsed["content"];
        std::string time= parsed["time"];
        int PId = std::stoi(fileId);

        int commentId = insertComment(userId, userName, PId, content, time);
        if(commentId)
        {
            // 插入成功，返回成功信息
            json successResp;
            successResp["status"] = "success";
            successResp["message"] = "Comment added successfully";
            successResp["commentId"] = commentId;
            successResp["userId"] = userId;
            successResp["username"] = userName;
            std::string successBody = successResp.dump(4);
            resp->setStatusLine(req.getVersion(), http::HttpResponse::Ok200, "OK");
            resp->setCloseConnection(false);
            resp->setContentType("application/json");
            resp->setContentLength(successBody.size());
            resp->setBody(successBody);
        }
        else
        {
            // 插入失败，返回失败信息
            resp->setStatusLine(req.getVersion(), http::HttpResponse::Conflict409, "Conflict");
            resp->setCloseConnection(false);
            resp->setContentType("application/json");
            resp->setContentLength(0);
            resp->setBody("");
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
        return;
    }
}

int PostcommentHandler::insertComment(int userId, const std::string& userName, int fileId, const std::string& comment, const std::string& time)
{
    std::string sql = "INSERT INTO t_comments (project_id, user_id, username, content, created_at) VALUES (?,?,?,?,?)";
    mysqlUtil_.executeQuery(sql, fileId, userId, userName, comment, time);
    sql = "select MAX(comment_id) as id from t_comments where project_id = ? and user_id = ?";
    sql::ResultSet *res = mysqlUtil_.executeQuery(sql, fileId, userId);
    if(res->next())
    {
        return res->getInt("id");
    }
    return 0;
}