#include "timer.h"
#include <cstdint>

const TimeSpec TimeSpec::inValidExpired({0, 0});
uint64_t Timer::count = 0;
