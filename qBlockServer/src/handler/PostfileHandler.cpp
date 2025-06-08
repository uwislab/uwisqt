#include "../../include/handlers/PostfileHandler.h"

void PostfileHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp) 
{
    std::string userId=req.getQueryParameters("userId");
    std::string fileName=req.getQueryParameters("fileName");
    std::string fileId = req.getQueryParameters("fileId");
    if(userId.empty() || fileName.empty())
    {
        resp->setStatusLine(req.getVersion(), http::HttpResponse::BadRequest400, "Bad Request");
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(0);
        resp->setBody("");
        return;
    }

    if(fileId.empty())
    {
        fileId = std::to_string(queryFileNum()+1);
        if(!insertFile(std::stoi(userId), fileName, "/home/ubuntu/projects/qBlockServer/UserFiles/"+fileId, req.contentLength()/1024.0))
        {
            resp->setStatusLine(req.getVersion(), http::HttpResponse::InternalServerError500, "Error");
            resp->setCloseConnection(false);
            resp->setContentType("application/json");
            resp->setContentLength(0);
            resp->setBody("");
            return;
        }
    }
    else
    {
        if(!updateFile(std::stoi(fileId)))
        {
            resp->setStatusLine(req.getVersion(), http::HttpResponse::InternalServerError500, "Error");
            resp->setCloseConnection(false);
            resp->setContentType("application/json");
            resp->setContentLength(0);
            resp->setBody("");
            return;
        }
    }

    std::string filePath = "/home/ubuntu/projects/qBlockServer/UserFiles/"+fileId+"/"+fileName+".zip";
    std::filesystem::create_directories(
        std::filesystem::path(filePath).parent_path()
    );
    FileUtil fileOperator(filePath);
    std::vector<char> buffer(req.contentLength());
    for(int i=0; i<req.contentLength(); i++) {
        std::cout<<req.getBody()[i];
    }
    const std::string& body = req.getBody();  // 确保body长度与contentLength一致

    // 使用memcpy代替strncpy来处理二进制数据
    memcpy(buffer.data(), body.data(), req.contentLength());
    // for(int i=0; i<req.contentLength(); i++) {
    //     std::cout<<buffer[i];
    // }
    fileOperator.write(buffer); // 写出文件数据
    fileOperator.close(); // 关闭文件
    if(fileOperator.size() == req.contentLength())
    {
        json json;
        json["size1"] = fileOperator.size();
        json["size2"] = std::filesystem::file_size(filePath);
        json["size3"] = req.contentLength();
        std::string jsonStr = json.dump();
        

        resp->setStatusLine(req.getVersion(), http::HttpResponse::Ok200, "OK");
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(jsonStr.length());
        resp->setBody(jsonStr);
        return;
    }
    else
    {
        json json;
        json["size1"] = fileOperator.size();
        json["size2"] = std::filesystem::file_size(filePath);
        json["size3"] = req.contentLength();
        std::string jsonStr = json.dump();
        
        resp->setStatusLine(req.getVersion(), http::HttpResponse::InternalServerError500, "Error");
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(jsonStr.length());
        resp->setBody(jsonStr);
        return;
    }

}

int PostfileHandler::queryFileNum()
{
    std::string sql = "select project_id from t_projects order by project_id desc limit 1";

    sql::ResultSet* res = mysqlUtil_.executeQuery(sql);
    if(res->rowsCount() == 0)
    {
        return 0;
    }
    else 
    {
        res->next();
    }
    return res->getInt("project_id");
}

bool PostfileHandler::insertFile(int userId, const std::string& fileName, const std::string& filePath, double fileSize)
{
    try
    {
        std::string sql = "insert into t_projects(user_id, project_name, size, file_path, is_public, create_time, update_time) values(?,?,?,?,0,Now(),Now())";
        return mysqlUtil_.executeUpdate(sql, userId, fileName, fileSize, filePath);
    }
    catch(sql::SQLException& e)
    {
        std::cout<<e.what()<<std::endl;
        return false;
    }
}

bool PostfileHandler::updateFile(int fileId)
{
    std::string sql = "update t_projects set update_time=Now() where project_id=?";
    return mysqlUtil_.executeUpdate(sql, fileId);
}