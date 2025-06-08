#include "../../include/handlers/GetpictureHandler.h"

void GetpictureHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp)
{
    std::string type = req.getQueryParameters("type");
    if(type == "user")
    {
        if(req.getQueryParameters("id").empty())
        {
            resp->setStatusLine(req.getVersion(), http::HttpResponse::BadRequest400, "BadRequest");
            resp->setCloseConnection(false);
            resp->setContentType("application/json");
            resp->setContentLength(0);
            resp->setBody("");
            return;
        }
        int id = std::stoi(req.getQueryParameters("id"));
        std::string result = queryUserAvatar(id);

        std::string contentType = (result=="jpg")?"jpeg":"png";
        if(result.empty())
        {
            resp->setStatusLine(req.getVersion(), http::HttpResponse::NotFound404, "NotFound");
            resp->setCloseConnection(false);
            resp->setContentType("application/json");
            resp->setContentLength(0);
            resp->setBody("");
        }
        else
        {
            std::string filePath = "/home/ubuntu/projects/qBlockServer/UserAvatar/"+std::to_string(id)+"."+result;
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
                resp->setContentType("image/"+contentType);
                resp->setContentLength(bufStr.size());
                resp->setBody(bufStr);
                return;
            }
        }

    }
    else if(type == "file")
    {
        if(req.getQueryParameters("id").empty())
        {
            resp->setStatusLine(req.getVersion(), http::HttpResponse::BadRequest400, "BadRequest");
            resp->setCloseConnection(false);
            resp->setContentType("application/json");
            resp->setContentLength(0);
            resp->setBody("");
            return;
        }
        int id = std::stoi(req.getQueryParameters("id"));
        std::string result = queryFileImg(id);

        std::string contentType = (result=="jpg")?"jpeg":"png";
        if(result.empty())
        {
            resp->setStatusLine(req.getVersion(), http::HttpResponse::NotFound404, "NotFound");
            resp->setCloseConnection(false);
            resp->setContentType("application/json");
            resp->setContentLength(0);
            resp->setBody("");
        }
        else
        {
            std::string filePath = "/home/ubuntu/projects/qBlockServer/FileImg/"+std::to_string(id)+"."+result;
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
                resp->setContentType("image/"+contentType);
                resp->setContentLength(bufStr.size());
                resp->setBody(bufStr);
                return;
            }
        }
    }
    else
    {
        resp->setStatusLine(req.getVersion(), http::HttpResponse::BadRequest400, "BadRequest");
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(0);
        resp->setBody("");
    }
}


std::string GetpictureHandler::queryUserAvatar(int userId)
{
    std::string sql = "SELECT avatar FROM t_user WHERE user_id = ?";
    sql::ResultSet* res = mysqlUtil_.executeQuery(sql, userId);
    if (res->next())
    {
        std::string result = res->getString("avatar");
        return result;
    }
}

std::string GetpictureHandler::queryFileImg(int fileId)
{
    std::string sql = "SELECT picture_path FROM t_projects_public WHERE project_id = ?";
    sql::ResultSet* res = mysqlUtil_.executeQuery(sql, fileId);
    if (res->next())
    {
        std::string result = res->getString("picture_path");
        return result;
    }
}