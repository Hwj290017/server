#pragma once

#include <string>
class ServiceImpl
{

  public:
    virtual std::string run(const std::string& input) = 0;
};
