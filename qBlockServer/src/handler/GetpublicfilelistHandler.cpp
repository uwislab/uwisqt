#include "../../include/handlers/GetpublicfilelistHandler.h"

void GetpublicfilelistHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp)
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
        auto filelist = queryFilelist();
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

json GetpublicfilelistHandler::queryFilelist()
{
    std::string sql = "select project_id, user_id, title, picture_path, update_time from t_projects_public";
    sql::ResultSet* result = mysqlUtil_.executeQuery(sql);
    json filelist = json::array();
    while(result->next())
    {
        json file = {
            {"projectId", result->getInt("project_id")},
            {"userId", result->getInt("user_id")},
            {"title", result->getString("title")},
            {"picturePath", result->getString("picture_path")},
            {"updateTime", result->getString("update_time")}
        };
        filelist.push_back(file);    
    }
    return filelist;
}