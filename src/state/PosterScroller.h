#ifndef HEADER_POSTERSCROLLER_H
#define HEADER_POSTERSCROLLER_H

class Path;

class Picture;

#include "PosterState.h"
#include "GameClock.h"

/**
 * Scroll very height picture.
 */
class PosterScroller : public PosterState {
private:
    static const int SHIFT_SPEED = 4;
    int m_shift;
    int m_screenH;
    int m_scrollAccum;

protected:
    virtual void own_initState();

    virtual void own_updateState();

public:
    PosterScroller(const Path &picture);

    virtual bool allowBg() const { return true; }
};

#endif
