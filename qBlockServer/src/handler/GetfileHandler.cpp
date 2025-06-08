#include "../../include/handlers/GetfileHandler.h"

void GetfileHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp) 
{
    std::string fileId = req.getQueryParameters("fileId");
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
        auto fileInfo = queryFileInfo(std::stoi(fileId));

        if (std::get<0>(fileInfo) != userId && !std::get<1>(fileInfo))
        {
            resp->setStatusLine(req.getVersion(), http::HttpResponse::Forbidden403, "Forbidden");
            resp->setCloseConnection(false);
            resp->setContentType("application/json");
            resp->setContentLength(0);
            resp->setBody("");
            return;
        }
        std::string filePath = "/home/ubuntu/projects/qBlockServer/UserFiles/"+fileId+"/"+std::get<2>(fileInfo)+".zip";
        FileUtil fileOperator(filePath);

        if (!fileOperator.exists())
        {
            resp->setStatusLine(req.getVersion(), http::HttpResponse::NotFound404, "NotFound");
            resp->setCloseConnection(false);
            resp->setContentType("application/json");
            resp->setContentLength(0);
            resp->setBody("");
            return;
        }
        else
        {
            std::vector<char> buffer(fileOperator.size());
            fileOperator.read(buffer); // 读出文件数据
            std::string bufStr = std::string(buffer.data(), buffer.size());
            
            resp->setStatusLine(req.getVersion(), http::HttpResponse::Ok200, "OK");
            resp->setCloseConnection(false);
            resp->setContentType("application/octet-stream");
            resp->setContentLength(bufStr.size());
            resp->setBody(bufStr);
            return;
        }
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


std::tuple<int,bool,std::string> GetfileHandler::queryFileInfo(int fileId)
{
    std::string sql = "SELECT user_id, is_public, project_name FROM t_projects WHERE project_id = ?";
    sql::ResultSet* res = mysqlUtil_.executeQuery(sql, fileId);
    if (res->next())
    {
        std::tuple<int,bool,std::string> result(res->getInt("user_id"), res->getInt("is_public") == 1?true:false, res->getString("project_name"));
        return result;
    }
    // 如果查询结果为空，则返回NULL
    return std::make_tuple(-1, false, "");
}