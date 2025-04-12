#ifndef FILESERVER_H
#define FILESERVER_H

#include "HttpServer.h"
#include "InetAddress.h"
class FileServer
{
  public:
    FileServer(const InetAddress& addr, const std::string& name, const std::string& root = ".");
    void start();
    void stop();

  private:
    InetAddress localAddr_;
    std::string name_;
    std::string root_;
    HttpServer httpServer_;
    void onHttpRequest(const HttpRequest& req, HttpResponse& resp);
};

#endif