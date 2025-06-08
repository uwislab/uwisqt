#include "../../include/handlers/PostpictureHandler.h"

void PostpictureHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp)
{
    std::string type = req.getQueryParameters("type");
    std::string folder = "";
    int id = std::stoi(req.getQueryParameters("id"));
    std::string pictureType = req.getHeader("Content-Type")=="image/jpeg"?"jpg":"png";
    if(type == "user")
    {
        folder = "UserAvatar/";
        updateUserAvatar(id, pictureType);
    }
    else if(type == "file")
    {
        folder = "FileImg/";
        updateFileImg(id, pictureType);
    }
    else
    {
        resp->setStatusLine(req.getVersion(), http::HttpResponse::BadRequest400, "Bad Request");
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(0);
        resp->setBody("");
        return;
    }

    std::string filePath = "/home/ubuntu/projects/qBlockServer/"+folder+std::to_string(id)+"." + pictureType;
    std::filesystem::create_directories(
        std::filesystem::path(filePath).parent_path()
    );
    FileUtil fileOperator(filePath);
    std::vector<char> buffer(req.contentLength());
    // for(int i=0; i<req.contentLength(); i++) {
    //     std::cout<<req.getBody()[i];
    // }
    const std::string& body = req.getBody();  // 确保body长度与contentLength一致

    // 使用memcpy代替strncpy来处理二进制数据
    memcpy(buffer.data(), body.data(), req.contentLength());
    // for(int i=0; i<req.contentLength(); i++) {
    //     std::cout<<buffer[i];
    // }
    fileOperator.write(buffer); // 写出文件数据
    if(fileOperator.size() == req.contentLength())
    {
        resp->setStatusLine(req.getVersion(), http::HttpResponse::Ok200, "OK");
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(0);
        resp->setBody("");
        return;
    }
    else
    {
        resp->setStatusLine(req.getVersion(), http::HttpResponse::InternalServerError500, "Error");
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(0);
        resp->setBody("");
        return;
    }
}


void PostpictureHandler::updateUserAvatar(int userId, const std::string& pictureType)
{
    std::string sql = "UPDATE t_user SET avatar =? WHERE user_id =?";
    mysqlUtil_.executeUpdate(sql, pictureType, userId);
}

void PostpictureHandler::updateFileImg(int fileId, const std::string& pictureType)
{
    std::string sql = "UPDATE t_projects_public SET picture_path =? WHERE project_id =?";
    mysqlUtil_.executeUpdate(sql, pictureType, fileId);
}