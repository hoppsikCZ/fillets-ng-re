/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "EffectZx.h"

#include "SurfaceLock.h"
#include "PixelTool.h"
#include "PixelIterator.h"
#include "Random.h"
#include "GameClock.h"

const char *EffectZx::NAME = "zx";
const double EffectZx::STRIPE_STANDARD = 38.5;
const double EffectZx::STRIPE_NARROW = 3.4;
//-----------------------------------------------------------------
/**
 * Read colors from all four corners.
 */
EffectZx::EffectZx() {
    m_zx = ZX1;
    m_phase = 0;
    m_countHeight = 0;
    m_stripeHeight = STRIPE_STANDARD;
}
//-----------------------------------------------------------------
/**
 * Update sprite height as ZX Spectrum does.
 */
void EffectZx::updateEffect() {
    int sp = GameClock::instance()->getSpeedup();

    m_countHeight++;

    // Phase/stripeHeight transitions gated by speedup
    if (GameClock::instance()->getTick() % sp == 0) {
        m_phase = (m_phase + 1) % 500;
        if (m_phase == 1) {
            m_zx = ZX1;
            m_stripeHeight = STRIPE_STANDARD;
        } else if (2 <= m_phase && m_phase <= 51) {
            m_stripeHeight = (m_stripeHeight * 3 * (0.97 + Random::randomReal(0.06)) + STRIPE_STANDARD) / 4.0;
        } else if (m_phase == 52) {
            m_zx = ZX3;
            m_stripeHeight = STRIPE_NARROW;
        } else {
            m_stripeHeight = (m_stripeHeight * 3 * (0.95 + Random::randomReal(0.1)) + STRIPE_NARROW) / 4.0;
        }
    }
}
//-----------------------------------------------------------------
/**
 * Draw ZX spectrum loading.
 */
void EffectZx::blit(SDL_Surface *screen, SDL_Surface *surface, int x, int y) {
    SurfaceLock lock1(screen);
    SurfaceLock lock2(surface);

    Uint32 colorZX1 = PixelTool::convertColor(screen->format,
                                              PixelTool::getColor(surface, SPRITE_PADDING, SPRITE_PADDING));
    Uint32 colorZX2 = PixelTool::convertColor(screen->format,
                                              PixelTool::getColor(surface, SPRITE_PADDING, surface->h - 1 - SPRITE_PADDING));
    Uint32 colorZX3 = PixelTool::convertColor(screen->format,
                                              PixelTool::getColor(surface, surface->w - 1 - SPRITE_PADDING, SPRITE_PADDING));
    Uint32 colorZX4 = PixelTool::convertColor(screen->format,
                                              PixelTool::getColor(surface, surface->w - 1 - SPRITE_PADDING, surface->h - 1 - SPRITE_PADDING));
                                            
    bool firstPair = (m_zx == ZX1 || m_zx == ZX2);

    PixelIterator pit(surface);
    for (int py = 0; py < surface->h; ++py) {
        int parity = (int)((m_countHeight + py) / m_stripeHeight);
        Uint32 usedColor;
        if (firstPair) {
            usedColor = (parity % 2 == 0) ? colorZX1 : colorZX2;
        } else {
            usedColor = (parity % 2 == 0) ? colorZX3 : colorZX4;
        }

        for (int px = 0; px < surface->w; ++px) {
            SDL_Color c = pit.getColor();
            if (c.a > ALPHA_THRESHOLD) {
                PixelTool::putPixel(screen,
                                    x + px, y + py, usedColor);
            }
            pit.inc();
        }
    }
}
