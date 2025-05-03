#pragma once
#include "baseobjectimpl.h"
#include "tcp/acceptor.h"
#include "tcp/baseobject.h"
namespace tcp
{

template <> class BaseObject<Acceptor>::Impl : public BaseImpl<Acceptor>
{
}
} // namespace tcp