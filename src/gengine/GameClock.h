#ifndef HEADER_GAMECLOCK_H
#define HEADER_GAMECLOCK_H

class GameClock {
private:
    int m_tick;
    int m_speedup;
    int m_offset;
    bool m_movingfish;

    GameClock();

public:
    static GameClock *s_clock;

    static GameClock *instance() {
        if (!s_clock)
            s_clock = new GameClock();
        return s_clock;
    }

    void advanceTick() { ++m_tick; }

    int getTick() const { return m_tick; }
    int getSpeedup() const { return m_speedup; }
    int getOffset() const { return m_offset; }
    bool isMovingfish() const { return m_movingfish; }

    void setMovingfish(bool moving) { m_movingfish = moving; }
    void setSpeedup(int speedup) { m_speedup = speedup; }
};

#endif
