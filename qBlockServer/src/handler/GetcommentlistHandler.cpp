#include "../../include/handlers/GetcommentlistHandler.h"

void GetcommentlistHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp)
{
    std::string PID = req.getQueryParameters("fileId");
    if(PID.empty())
    {
        resp->setStatusLine(req.getVersion(), http::HttpResponse::BadRequest400, "Bad Request");
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(0);
        resp->setBody("");
        return;
    }
    else
    {
        auto commentList = selectComment(std::stoi(PID));
        resp->setStatusLine(req.getVersion(), http::HttpResponse::Ok200, "OK");
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        std::string body = commentList.dump();
        resp->setContentLength(body.length());
        resp->setBody(body);
        return;
    }

}

json GetcommentlistHandler::selectComment(int fileId)
{
    std::string sql = "SELECT * FROM t_comments WHERE project_id = ? ORDER BY created_at ASC";
    sql::ResultSet* result = mysqlUtil_.executeQuery(sql, fileId);
    json commentList = json::array();
    while (result->next())
    {
        json comment = {
            {"commentId", result->getInt("comment_id")},
            {"projectId", result->getInt("project_id")},
            {"userId", result->getInt("user_id")},
            {"userName", result->getString("username")},
            {"content", result->getString("content")},
            {"time", result->getString("created_at")}
        };
        commentList.push_back(comment); 
    }
    return commentList;
}