/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "WiStatusBar.h"

#include "Font.h"

#include <sstream>

//-----------------------------------------------------------------
WiStatusBar::WiStatusBar(Font *new_font, const SDL_Color &color, int width)
        : m_color(color) {
    m_font = new_font;
    m_w = width;
}

//-----------------------------------------------------------------
WiStatusBar::~WiStatusBar() {
    delete m_font;
}

//-----------------------------------------------------------------
int WiStatusBar::getH() const {
    int lines = 1;
    for (size_t i = 0; i < m_label.size(); ++i) {
        if (m_label[i] == '\n') ++lines;
    }
    return m_font->getHeight() * lines;
}

//-----------------------------------------------------------------
void WiStatusBar::drawOn(SDL_Surface *screen, SDL_Renderer *renderer) {
    if (!m_label.empty()) {
        int y = m_shift.getY();
        int lineH = m_font->getHeight();

        std::istringstream stream(m_label);
        std::string line;
        while (std::getline(stream, line)) {
            SDL_Rect rect;
            rect.x = m_shift.getX();
            rect.y = y;

            SDL_Surface *rendered = m_font->renderTextOutlined(line, m_color);
            SDL_BlitSurface(rendered, NULL, screen, &rect);
            SDL_FreeSurface(rendered);

            y += lineH;
        }
    }
}
