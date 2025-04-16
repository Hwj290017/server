#include "servicehandler.h"

auto ServiceHandler::call(const Request& request) -> Response
{
    auto method = request.method();
    auto it = services_.find(method);
    Response response;
    if (it != services_.end())
    {
        response.set_output(it->second(request.input()));
    }
    response.set_method(std::move(method));
    return response;
}

void ServiceHandler::registerService(const String& method, std::function<String(String)> func)
{
    services_[method] = std::move(func);
}