#ifndef HEADER_VIEWEFFECT_H
#define HEADER_VIEWEFFECT_H

#include "SDL2/SDL.h"

/**
 * Graphic effect before blit.
 */
class ViewEffect {
protected:
    // To fix all effects, that broke because of the new sprites.
    const int SPRITE_PADDING = 5;
    // With the new anti-aliased textures some effects needs this guard. (130 looks good enough to me, but can be tweeked if needed)
    const int ALPHA_THRESHOLD = 130;
public:
    virtual ~ViewEffect() {}

    /**
     * Update effect after blit.
     */
    virtual void updateEffect() {}

    virtual const char *getName() const = 0;

    virtual bool isDisintegrated() const { return false; }

    virtual bool isInvisible() const { return false; }

    virtual void blit(SDL_Surface *screen, SDL_Surface *surface,
                      int x, int y) = 0;
};

#endif
