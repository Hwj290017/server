#pragma once

namespace tcp
{
// 禁止复制、移动构造函数和赋值运算符
template <typename T> class TempPtr
{
  public:
    TempPtr(T* ptr) : ptr_(ptr)
    {
    }
    ~TempPtr() = default;
    TempPtr(const TempPtr&) = delete;
    TempPtr& operator=(const TempPtr&) = delete;
    TempPtr(TempPtr&&) = delete;
    TempPtr& operator=(TempPtr&&) = delete;

    T* operator->() const
    {
        return ptr_;
    }

  private:
    T* ptr_;
};

} // namespace tcp