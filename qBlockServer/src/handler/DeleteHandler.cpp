#include "../../include/handlers/DeleteHandler.h"

void DeleteHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp) 
{
    try
    {
        // 获取会话
        auto session = server_->getSessionManager()->getSession(req, resp);
        // 获取用户id
        int userId = std::stoi(session->getData("userId"));
        int permission = std::stoi(session->getData("permission"));

        std::string type = req.getQueryParameters("type");
        std::string id = req.getQueryParameters("ID");
        int xid = 0;
        if(!id.empty())
        {
            xid = std::stoi(id);
        }
        if(type.empty() || id.empty())
        {
            json failureResp;
            failureResp["status"] = "error";
            failureResp["message"] = "Bad Request";
            std::string failureBody = failureResp.dump(4);
            resp->setStatusLine(req.getVersion(), http::HttpResponse::BadRequest400, "Bad Request");
            resp->setCloseConnection(true);
            resp->setContentType("application/json");
            resp->setContentLength(failureBody.size());
            resp->setBody(failureBody);
            return;
        }
        if(type == "communityproject")
        {
            deletePublicFile(userId,xid,permission);
        }
        else if(type == "cloudproject")
        {
            deleteFile(userId,xid,permission);
        }
        else if(type == "comment")
        {
            deleteComment(userId,xid,permission);
        }
        else{
            json failureResp;
            failureResp["status"] = "error";
            failureResp["message"] = "Bad Request";
            std::string failureBody = failureResp.dump(4);
            resp->setStatusLine(req.getVersion(), http::HttpResponse::BadRequest400, "Bad Request");
            resp->setCloseConnection(true);
            resp->setContentType("application/json");
            resp->setContentLength(failureBody.size());
            resp->setBody(failureBody);
            return;
        }        
        json response;
        response["message"] = "delete successful";
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

bool DeleteHandler::deleteFile(int userId, int fileId, int permission)
{
    std::string sql = "delete from t_projects where project_id = ?";
    //sql::ResultSet* res = mysqlUtil_.executeQuery(sql, fileId);
    if(permission == 1)
    {
        // 管理员可以删除任何文件
        mysqlUtil_.executeQuery(sql, fileId);
        return true;
    }
    else
    {
        // 普通用户只能删除自己的文件
        std::string selectsql = "select user_id from t_projects where project_id = ?";
        sql::ResultSet* res = mysqlUtil_.executeQuery(selectsql, fileId);
        int fileUserId = 0;
        while(res->next())
        {
            fileUserId = res->getInt("user_id");
        }
        if(fileUserId == userId)
        {
            // 该文件属于该用户，可以删除
            mysqlUtil_.executeQuery(sql, fileId);
            return true;
        }
        else
        {
            // 该文件不属于该用户，不能删除
            return false;
        }
    }
    return false;
}
bool DeleteHandler::deletePublicFile(int userId, int fileId, int permission)
{
    std::string deletesql1 = "delete from t_projects_public where project_id = ?";
    std::string deletesql2 = "delete from t_projects where project_id = ?";
    //sql::ResultSet* res = mysqlUtil_.executeQuery(sql, fileId);
    if(permission == 1)
    {
        // 管理员可以删除任何文件
        mysqlUtil_.executeQuery(deletesql1, fileId);
        mysqlUtil_.executeQuery(deletesql2, fileId);
        return true;
    }
    else
    {
        // 普通用户只能删除自己的文件
        std::string selectsql = "select user_id from t_projects_public where project_id = ?";
        sql::ResultSet* res = mysqlUtil_.executeQuery(selectsql, fileId);
        int fileUserId = 0;
        while(res->next())
        {
            fileUserId = res->getInt("user_id");
        }
        if(fileUserId == userId)
        {
            // 该文件属于该用户，可以删除
            mysqlUtil_.executeQuery(deletesql1, fileId);
            mysqlUtil_.executeQuery(deletesql2, fileId);
            return true;
        }
        else
        {
            // 该文件不属于该用户，不能删除
            return false;
        }
    }
    return false;
}
bool DeleteHandler::deleteComment(int userId, int commentId, int permission)
{
    std::string sql = "delete from t_comments where comment_id = ?";
    //sql::ResultSet* res = mysqlUtil_.executeQuery(sql, fileId);
    if(permission == 1)
    {
        // 管理员可以删除任何评论
        mysqlUtil_.executeQuery(sql, commentId);
        return true;
    }
    return false;
}