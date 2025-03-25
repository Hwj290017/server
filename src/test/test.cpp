

#include "buffer.h"
#include "httpContext.h"
#include "httpRequest.h"
#include "httpResponse.h"
#include "timer.h"
int main()
{
    HttpContext a;
    Buffer b;
    HttpResponse c(false);
    b += "GET /index.html HTTP/1.1\r\n"
         "Host: www.chenshuo.com\r\n"
         "\r\nhello";
    a.parseRequest(b, TimeSpec());
    c.setBody("hello");
    c.setContentType("keep");
    c.message(b);
    return 0;
}