#include "HttpServer.h"
#include "InetAddress.h"
#include "httpRequest.h"
#include "httpResponse.h"

int main()
{
    InetAddress listenAddr(LOCALHOST, PORT);
    HttpServer server(listenAddr);
    server.setHttpCallback([](const HttpRequest& req, HttpResponse& resp) {
        resp.setStatusCode(HttpResponse::Ok);
        resp.setBody(req.body() + "hello world");
    });
    server.start();
    return 0;
}