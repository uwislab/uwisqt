#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <chrono>

namespace http{
    namespace session{
        class SessionManager;

        class Session : public std::enable_shared_from_this<Session>
        {
            public:
                Session(const std::string& sessionId, SessionManager* sessionManager, int maxAge = 3600);

                const std::string& getId() const { return sessionId_; };

                void setManager(SessionManager* sessionManager) { sessionManager_ = sessionManager; }
                SessionManager* getManager() const { return sessionManager_; }

                void setData(const std::string& key, const std::string& value);
                std::string getData(const std::string& key) const;
                void removeData(const std::string& key);
                void clearData();

                bool isExpired() const;
                void refreshExpiryTime();
                void setMaxAge(int maxAge) { maxAge_ = maxAge; }

            private:
                std::string                                  sessionId_;
                std::unordered_map<std::string, std::string> data_;
                std::chrono::system_clock::time_point        expiryTime_;
                int                                          maxAge_; // 过期时间（秒）
                SessionManager*                              sessionManager_;
        };
    }
}