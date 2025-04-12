#include "fileserver.h"
#include "HttpServer.h"
#include "httpRequest.h"
#include "httpResponse.h"
#include "log.h"
#include <fstream>

FileServer::FileServer(const InetAddress& addr, const std::string& name, const std::string& root)
    : localAddr_(addr), name_(name), root_(root), httpServer_(addr)
{
    httpServer_.setHttpCallback(
        std::bind(&FileServer::onHttpRequest, this, std::placeholders::_1, std::placeholders::_2));
}

void FileServer::start()
{
    httpServer_.start();
}

void FileServer::onHttpRequest(const HttpRequest& req, HttpResponse& resp)
{
    if (req.getMethod() == HttpRequest::Method::Get)
    {
        auto filePath = req.getPath();
        if (filePath == "/")
        {
            filePath = "/index.html";
        }
        auto file = std::ifstream(root_ + filePath);
        if (file.is_open())
        {
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            Logger::logger << content;
            resp.setStatusCode(HttpResponse::HttpStatusCode::Ok);
            resp.setBody(content);
            file.close();
        }
    }
}