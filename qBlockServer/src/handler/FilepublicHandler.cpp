#include "../../include/handlers/FilepublicHandler.h"

void FilepublicHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp) 
{
    std::cout<<"FilepublicHandler::handle"<<std::endl;
    try
    {
        // 获取会话
        auto session = server_->getSessionManager()->getSession(req, resp);
        std::cout<<"FilepublicHandler::handle2"<<std::endl;
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
        std::cout <<userId;
        // 获取文件信息
        json parsed = json::parse(req.getBody());
        std::string title = parsed["title"];
        std::string description = parsed["description"];
        std::cout <<req.getQueryParameters("fileId");
        int fileId = std::stoi(req.getQueryParameters("fileId"));
        std::cout << "fileId: " << fileId << std::endl;

        bool isPublic = updateFilelist(userId, fileId, title, description);
        if(isPublic)
        {
            resp->setStatusLine(req.getVersion(), http::HttpResponse::Ok200, "OK");
            resp->setCloseConnection(false);
            resp->setContentType("application/json");
            resp->setContentLength(0);
            resp->setBody("");
        }
        else
        {
            resp->setStatusLine(req.getVersion(), http::HttpResponse::Forbidden403, "Forbidden");
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

bool FilepublicHandler::updateFilelist(int userId, int fileId, const std::string& title, const std::string& description) 
{
    // 连接数据库
    std::string query = "SELECT user_id FROM t_projects WHERE project_id = ?";

    // 查询是否有权限修改文件
    sql::ResultSet* res = mysqlUtil_.executeQuery(query, userId);
    if(res->next())
    {
        int fileUserId = res->getInt("user_id");
        if(fileUserId == userId)
        {
            // 有权限修改文件
            // 更新文件信息
            query = "select * from t_projects_public where project_id = ? and user_id = ?";
            res = mysqlUtil_.executeQuery(query, fileId, userId);
            if(res->next())
            {
                std::cout << "update file info" << std::endl;
                // 更新文件信息
                query = "UPDATE t_projects_public SET title = ?, description = ?, update_time = NOW() WHERE project_id = ? and user_id = ?";
                mysqlUtil_.executeQuery(query, title, description, fileId, userId);
            }
            else
            {
                std::cout << "insert file info" << std::endl;
                query = "UPDATE t_projects SET is_public = 1 WHERE project_id = ?";
                mysqlUtil_.executeQuery(query, fileId);
                // 插入文件信息
                query = "INSERT INTO t_projects_public (project_id, user_id, title, description, public_time, update_time) VALUES (?, ?, ?, ?, NOW(), NOW())";
                mysqlUtil_.executeQuery(query, fileId, userId, title, description);
            }
            query = "select * from t_projects_public where project_id = ? and user_id = ?";
            res = mysqlUtil_.executeQuery(query, fileId, userId);
            std::cout << "update file info2" << std::endl;
            if(res->next())
            {
                // 更新成功
                return true;
            }
            // 更新失败
            return false;
        }
        else
        {
            // 无权限修改文件
            return false;
        }
    }
    else
    {
        // 文件不存在
        return false;
    }
}