#include "tcp/object.h"

namespace tcp
{
struct Object::Impl
{
    Impl(const Tasks& tasks) : tasks_(tasks)
    {
    }
    std::any context_;
    Tasks tasks_;
};
Object::Object(const Tasks& tasks) : impl_(std::make_unique<Impl>(tasks))
{
}

void Object::start()
{
}
} // namespace tcp