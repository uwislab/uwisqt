#include "../../include/handlers/RegisterHandler.h"

void RegisterHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp)
{
    // 解析body(json格式)
    json parsed = json::parse(req.getBody());
    std::string username = parsed["username"];
    std::string password = parsed["password"];
    std::string email = parsed["email"];

    // 判断用户是否已经存在，如果存在则注册失败
    int userId = insertUser(username, password, email);
    if (userId >= 0)
    {
        // 插入成功
        // 封装成功响应
        json successResp;   
        successResp["status"] = "success";
        successResp["message"] = "Register successful";
        successResp["userId"] = userId;
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
        // 插入失败
        json failureResp;
        failureResp["status"] = "error";
        if(userId == -1)
        {
            failureResp["message"] = "username already exists";
        }
        else if(userId == -2)
        {
            failureResp["message"] = "email already exists";
        }
        else
        {
            failureResp["message"] = "unknown error";
        }
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersion(), http::HttpResponse::Conflict409, "Conflict");
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }
}

int RegisterHandler::insertUser(const std::string &username, const std::string &password, const std::string &email)
{
    // 判断用户是否存在，如果存在则返回-1，否则返回用户id
    if(isUserExist(username))
    {
        return -1;
    }
    else if(isEmailExist(email))
    {
        return -2;
    }
    else
    {
        // 用户不存在，插入用户
        std::string salt = randomChar_.rand_str(8);
        std::string password_hash = sha256(password + salt);
        std::string sql = "INSERT INTO t_user (username, email, password, salt, password_hash, created_time, last_login) VALUES ('" + username + "', '" +email+ "', '" + password + "', '" + salt + "', '" + password_hash + "', NOW(), NOW())";
        mysqlUtil_.executeUpdate(sql);
        std::string sql2 = "SELECT user_id FROM t_user WHERE username = '" + username + "'";
        sql::ResultSet* res = mysqlUtil_.executeQuery(sql2);
        if (res->next())
        {
            return res->getInt("user_id");
        }
    }
    return -3;
}

bool RegisterHandler::isUserExist(const std::string &username)
{
    std::string sql = "SELECT user_id FROM t_user WHERE username = '" + username + "'";
    sql::ResultSet* res = mysqlUtil_.executeQuery(sql);
    if (res->next())
    {
        return true;
    }
    return false;
}

bool RegisterHandler::isEmailExist(const std::string &email)
{
    std::string sql = "SELECT user_id FROM t_user WHERE email = '" + email + "'";
    sql::ResultSet* res = mysqlUtil_.executeQuery(sql);
    if (res->next())
    {
        return true;
    }
    return false;
}

std::string RegisterHandler::sha256(const std::string& str)
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