#include "../../include/handlers/UpdateHandler.h"

void UpdateHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp) 
{
    std::string version = req.getQueryParameters("version");
    if (version.empty())
    {
        std::string latestVersion = getLatestVersion();
        json versionjson;
        versionjson["version"] = latestVersion;
        std::string versionBody = versionjson.dump(4);

        resp->setStatusLine(req.getVersion(), http::HttpResponse::Ok200, "OK");
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(versionBody.size());
        resp->setBody(versionBody);
        return;
    }
    else
    {
        std::string filePath = "/home/ubuntu/projects/qBlockServer/UpdateFiles/"+version+".zip";
    }
    json failureResp;
    failureResp["status"] = "error";
    std::string failureBody = failureResp.dump(4);
    resp->setStatusLine(req.getVersion(), http::HttpResponse::BadRequest400, "Bad Request");
    resp->setCloseConnection(true);
    resp->setContentType("application/json");
    resp->setContentLength(failureBody.size());
    resp->setBody(failureBody);
    return;
}


std::string UpdateHandler::getLatestVersion()
{
    std::string sql = "SELECT version FROM t_version ORDER BY update_time DESC LIMIT 1";
    sql::ResultSet* rs = mysqlUtil_.executeQuery(sql);
    if (rs->next())
    {
        return rs->getString("version");
    }
}