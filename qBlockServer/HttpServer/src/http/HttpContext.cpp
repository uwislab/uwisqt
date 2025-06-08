#include "../../include/http/HttpContext.h"

using namespace muduo;
using namespace muduo::net;

namespace http {

    bool HttpContext::parseRequest(Buffer *buf, Timestamp receiveTime)
    {
        bool ok = true;
        bool hasMore = true;
        while(hasMore)
        {
            if(state_ == ExpectRequestLine)
            {
                const char *crlf = buf->findCRLF();
                if(crlf)
                {
                    ok = processRequestLine(buf->peek(), crlf);
                    if(ok)
                    {
                        request_.setReceiveTime(receiveTime);
                        buf->retrieveUntil(crlf + 2);
                        state_ = ExpectHeaders;
                    }
                    else
                    {
                        hasMore = false;
                    }
                }
                else
                {
                    hasMore = false;
                }
            }
            else if(state_ == ExpectHeaders)
            {
                const char *crlf = buf->findCRLF();
                if(crlf)
                {
                    const char *colon = std::find(buf->peek(), crlf, ':');
                    if (colon < crlf)
                    {
                        request_.addHeader(buf->peek(), colon, crlf);
                    }
                    else if (buf->peek() == crlf)
                    { 
                        // 空行，结束Header
                        // 根据请求方法和Content-Length判断是否需要继续读取body
                        if (request_.method() == HttpRequest::POST || request_.method() == HttpRequest::PUT)
                        {
                            std::string contentLength = request_.getHeader("Content-Length");
                            if (!contentLength.empty())
                            {
                                request_.setContentLength(std::stoi(contentLength));
                                if (request_.contentLength() > 0)
                                {
                                    state_ = ExpectBody;
                                }
                                else
                                {
                                    state_ = GotAll;
                                    hasMore = false;
                                }
                            }
                            else
                            {
                                // POST/PUT 请求没有 Content-Length，是HTTP语法错误
                                ok = false;
                                hasMore = false;
                            }
                        }
                        else
                        {
                            // GET/HEAD/DELETE 等方法直接完成（没有请求体）
                            state_ = GotAll; 
                            hasMore = false;
                        }
                    }
                    else
                    {
                        // 非法Header，结束Header
                        ok = false;
                        hasMore = false;
                    }
                    buf->retrieveUntil(crlf + 2); // 开始读指针指向下一行数据
                }
                else
                {
                    hasMore = false;
                }
            }
            else if(state_ == ExpectBody)
            {
                if (buf->readableBytes() < request_.contentLength())
                {
                    hasMore = false; // 数据不完整，等待更多数据
                    return true;
                }

                std::string body(buf->peek(), buf->peek() + request_.contentLength());
                request_.setBody(body);

                // 准确移动读指针
                buf->retrieve(request_.contentLength());

                state_ = GotAll;
                hasMore = false;
            }
        }
        return ok;
    }
    bool HttpContext::processRequestLine(const char *begin, const char *end)
    {
        bool succeed = false;
        const char *start = begin;
        const char *space = std::find(start, end, ' ');
        if (space != end && request_.setMethod(start, space))
        {
            start = space + 1;
            space = std::find(start, end, ' ');
            if (space != end)
            {
                const char *argumentStart = std::find(start, space, '?');
                if (argumentStart != space) // 请求带参数
                {
                    request_.setPath(start, argumentStart); // 注意这些返回值边界
                    request_.setQueryParameters(argumentStart + 1, space);
                }
                else // 请求不带参数
                {
                    request_.setPath(start, space);
                }

                start = space + 1;
                succeed = ((end - start == 8) && std::equal(start, end - 1, "HTTP/1."));
                if (succeed)
                {
                    if (*(end - 1) == '1')
                    {
                        request_.setVersion("HTTP/1.1");
                    }
                    else if (*(end - 1) == '0')
                    {
                        request_.setVersion("HTTP/1.0");
                    }
                    else
                    {
                        succeed = false;
                    }
                }
            }
        }
        return succeed;
    }
} // namespace http