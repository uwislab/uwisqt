#include "../../include/handlers/LoginHandler.h"

void LoginHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp)
{
    auto contentType = req.getHeader("Content-Type");
    if (contentType.empty() || contentType != "application/json" || req.getBody().empty())
    {
        LOG_INFO << "content" << req.getBody();
        resp->setStatusLine(req.getVersion(), http::HttpResponse::BadRequest400, "Bad Request");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(0);
        resp->setBody("");
        return;
    }
    try
    {
        json parsed = json::parse(req.getBody());
        std::string username = parsed["username"];
        std::string password = parsed["password"];

        auto result = queryUserId(username, password);
        int userId = result.first;
        int permission = result.second;
        if (userId != -1)
        {
            // 获取会话
            auto session = server_->getSessionManager()->getSession(req, resp);
            // 会话都不是同一个会话，因为会话判断是不是同一个会话是通过请求报文中的cookie来判断的
            // 所以不同页面的访问是不可能是相同的会话的，只有该页面前面访问过服务端，才会有会话记录
            // 那么判断用户是否在其他地方登录中不能通过会话来判断
            
            // 在会话中存储用户信息
            session->setData("userId", std::to_string(userId));
            session->setData("username", username);
            session->setData("isLoggedIn", "true");
            session->setData("permission", std::to_string(permission));
            if (server_->onlineUsers_.find(userId) == server_->onlineUsers_.end() || server_->onlineUsers_[userId] == false)
            {
                {
                    std::lock_guard<std::mutex> lock(server_->mutexForOnlineUsers_);
                    server_->onlineUsers_[userId] = true;
                }
                // 用户存在登录成功
                // 封装json 数据。
                json successResp;
                successResp["success"] = true;
                successResp["userId"] = userId;
                successResp["username"] = username;
                successResp["permission"] = permission;
                std::string successBody = successResp.dump(4);

                resp->setStatusLine(req.getVersion(), http::HttpResponse::Ok200, "OK");
                resp->setCloseConnection(false);
                resp->setContentType("application/json");
                resp->setContentLength(successBody.size());
                resp->setBody(successBody);
                return;
            }
            else
            {
                // FIXME: 当前该用户正在其他地方登录中，将原有登录用户强制下线更好
                // 不允许重复登录，
                json failureResp;
                failureResp["success"] = false;
                failureResp["error"] = "账号已在其他地方登录";
                std::string failureBody = failureResp.dump(4);

                resp->setStatusLine(req.getVersion(), http::HttpResponse::Forbidden403, "Forbidden");
                resp->setCloseConnection(true);
                resp->setContentType("application/json");
                resp->setContentLength(failureBody.size());
                resp->setBody(failureBody);
                return;
            }
        }
        else // 账号密码错误，请重新登录
        {
            // 封装json数据
            json failureResp;
            failureResp["status"] = "error";
            failureResp["message"] = "Invalid username or password";
            std::string failureBody = failureResp.dump(4);

            resp->setStatusLine(req.getVersion(), http::HttpResponse::Unauthorized401, "Unauthorized");
            resp->setCloseConnection(false);
            resp->setContentType("application/json");
            resp->setContentLength(failureBody.size());
            resp->setBody(failureBody);
            return;
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

std::pair<int,int> LoginHandler::queryUserId(const std::string &username, const std::string &password)
{
    // 前端用户传来账号密码，查找数据库是否有该账号密码
    // 使用预处理语句, 防止sql注入
    std::string sql = "SELECT user_id, password, permission FROM t_user WHERE username = ? AND password_hash = ?";
    // std::vector<std::string> params = {username, password};
    sql::ResultSet* res = mysqlUtil_.executeQuery(sql, username, password);
    if (res->next())
    {
        int id = res->getInt("user_id");
        std::string password = res->getString("password");
        int permission = res->getInt("permission");
        std::string salt = randomChar_.rand_str(8);
        std::string hash = sha256(password + salt);
        // 更新数据库密码
        std::string updatesql = "UPDATE t_user SET password_hash = '"+hash+"', salt = '"+salt+"', last_login = NOW() WHERE user_id = "+std::to_string(id);
        mysqlUtil_.executeUpdate(updatesql);
        return std::make_pair(id, permission);
    }
    // 如果查询结果为空，则返回-1
    return std::make_pair(-1, -1);
}

std::string LoginHandler::sha256(const std::string& str)
{
    // 定义OpenSSL SHA-256上下文和哈希结果缓冲区
    SHA256_CTX context;
    unsigned char hash[SHA256_DIGEST_LENGTH];

    // 初始化上下文（失败返回空）
    if (SHA256_Init(&context) != 1) {
        return "";
    }

    // 更新哈希计算（输入数据）
    if (SHA256_Update(&context, str.c_str(), str.size()) != 1) {
        return "";
    }

    // 完成哈希计算并存储结果到hash数组（失败返回空）
    if (SHA256_Final(hash, &context) != 1) {
        return "";
    }

    // 将二进制哈希转换为十六进制字符串
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::setw(2) << static_cast<unsigned int>(hash[i]);
    }

    return ss.str();
}