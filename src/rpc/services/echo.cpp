#include "echo.h"

String echo(String msg)
{
    return std::move(msg);
}