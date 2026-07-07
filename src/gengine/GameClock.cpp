#include "GameClock.h"
#include <cstddef>

GameClock *GameClock::s_clock = nullptr;

GameClock::GameClock()
    : m_tick(0), m_speedup(5), m_offset(-5), m_movingfish(false) {}
