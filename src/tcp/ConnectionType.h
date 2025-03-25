#ifndef CONNECTIONTYPE_H
#define CONNECTIONTYPE_H

#include <functional>
class TcpConnection;
class Buffer;
class TimeSpec;
typedef std::function<void(int)> CloseCb;
typedef std::function<void(TcpConnection*, const Buffer&, const TimeSpec&)> MessageCb;
typedef std::function<void(TcpConnection*)> OnConnectionCb;

#endif // CONNECTIONTYPE_H