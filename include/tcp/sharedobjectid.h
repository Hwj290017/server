#pragma once

#include <cstddef>
#include <string>
namespace tcp
{

class SharedObjectId
{
  public:
    enum class SharedObjectType
    {
        kNone,
        kAcceptor,
        kConnection,
        kConnector
    };
    SharedObjectId(std::size_t id = 0, SharedObjectType type = SharedObjectType::kNone);
    SharedObjectId(const SharedObjectId& other);
    std::string toString() const;

    ~SharedObjectId() = default;

  protected:
    std::size_t id_;
    SharedObjectType type_;
};

} // namespace tcp