#include "../../include/handlers/GetpublicfileinfoHandler.h"

void GetpublicfileinfoHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp)
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
        int projectId = std::stoi(req.getQueryParameters("projectId"));
        auto fileinfo = queryFile(projectId);
        std::string responseBody = fileinfo.dump();
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

json GetpublicfileinfoHandler::queryFile(int fileId)
{
    std::string sql = "select project_id, project_name, user_id, title, description, picture_path, update_time from t_projects_public where project_id = ?";
    sql::ResultSet* result = mysqlUtil_.executeQuery(sql, fileId);
    json file;
    while(result->next())
    {
        file = {
            {"projectId", result->getInt("project_id")},
            {"projectName", result->getString("project_name")},
            {"userId", result->getInt("user_id")},
            {"title", result->getString("title")},
            {"description", result->getString("description")},
            {"picturePath", result->getString("picture_path")},
            {"updateTime", result->getString("update_time")}
        };   
        return file;
    }
    return file;
}