#pragma once

#include "service.pb.h"
#include <string>

using String = std::string;
::rpc::OutputEcho echo(::rpc::InputEcho msg);
