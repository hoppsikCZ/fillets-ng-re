/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "NodeDrawer.h"

#include "ResImagePack.h"
#include "Font.h"

#include "Log.h"
#include "V2.h"
#include "Path.h"
#include "LevelNode.h"
#include "TimerAgent.h"
#include "minmax.h"

#include "SDL2/SDL2_gfxPrimitives.h"
#include "GameClock.h"
#include <iostream>
#include <sstream>
#include <string>
#include "BezierCurves.h"

//-----------------------------------------------------------------
NodeDrawer::NodeDrawer() {
    m_font = new Font(Path::dataReadPath("font/font_menu.ttf"), 22);

    m_imagePack = new ResImagePack();
    m_imagePack->addImage("solved",
                          Path::dataReadPath("images/menu/n0.png"));

    m_imagePack->addImage("open",
                          Path::dataReadPath("images/menu/n1.png"));
    m_imagePack->addImage("open",
                          Path::dataReadPath("images/menu/n2.png"));
    m_imagePack->addImage("open",
                          Path::dataReadPath("images/menu/n3.png"));
    m_imagePack->addImage("open",
                          Path::dataReadPath("images/menu/n4.png"));

    m_imagePack->addImage("far",
                          Path::dataReadPath("images/menu/n_far.png"));
}

//-----------------------------------------------------------------
NodeDrawer::~NodeDrawer() {
    m_imagePack->removeAll();
    delete m_imagePack;
    delete m_font;
}
//-----------------------------------------------------------------
/**
 * Draw blinking dot centred on node position.
 */
void NodeDrawer::drawNode(const LevelNode *node) const {
    V2 loc = node->getLoc();
    drawDot(m_imagePack->getRes("far"), loc);

    SDL_Surface *dot = NULL;
    switch (node->getState()) {
        case LevelNode::STATE_FAR:
            return;
        case LevelNode::STATE_OPEN: {
            int phase = (TimerAgent::agent()->getCycles() / GameClock::instance()->getSpeedup()) % 10;
            if (phase > 4) {
                phase--;
            }
            if (phase > 7) {
                phase--;
            }
            if (phase >= 4) {
                phase = 7 - phase;
            }
            dot = m_imagePack->getRes("open", phase);
        }
            break;
        case LevelNode::STATE_SOLVED:
            dot = m_imagePack->getRes("solved");
            break;
        default:
            LOG_WARNING(ExInfo("don't know how to draw node")
                                .addInfo("state", node->getState()));
            return;
    }
    drawDot(dot, loc);

    // std::stringstream ss = std::stringstream();
    // ss << "" << loc.getX() << "," << loc.getY() << "";
    // std::string coords = ss.str();
    // V2 textLoc = {loc.getX(), loc.getY()};
    // drawText(textLoc, coords);
}

//-----------------------------------------------------------------
/**
 * Draw centred.
 * @param x x cord. or centre
 * @param x y cord. or centre
 */
void NodeDrawer::drawDot(SDL_Surface *dot, const V2 &loc) const {
    SDL_Rect rect;
    rect.x = loc.getX() - dot->w / 2;
    rect.y = loc.getY() - dot->h / 2;
    SDL_BlitSurface(dot, NULL, m_screen, &rect);
}
//-----------------------------------------------------------------
/**
 * Highlightes selected node.
 */
void NodeDrawer::drawSelect(const V2 &loc) const {

    const SDL_Surface *dot = m_imagePack->getRes("solved");
    int radius = max(dot->w, dot->h) / 2 + 1;
    Uint32 colorRGBA = 0x8018c6ff; // RGBA 0xffc61880
    SDL_Renderer *renderer = this->renderer;
    filledCircleColor(renderer, loc.getX(), loc.getY(), radius, colorRGBA);

}
//-----------------------------------------------------------------
/**
 * Draws name of selected level.
 */
void NodeDrawer::drawSelected(const std::string &levelname) const {
    // TODO: draw deflected text
    int text_width = m_font->calcTextWidth(levelname);

    SDL_Rect rect;
    rect.x = (m_screen->w - text_width) / 2;
    rect.y = m_screen->h - 50;

    SDL_Color color = {255, 255, 0, 255};
    SDL_Surface *surface = m_font->renderTextOutlined(levelname, color);
    SDL_BlitSurface(surface, NULL, m_screen, &rect);
    SDL_FreeSurface(surface);
}

//-----------------------------------------------------------------
void NodeDrawer::drawEdge(const LevelNode *start, const LevelNode *end) const {
    // TODO: nice curves
    Sint16 x1 = start->getLoc().getX();
    Sint16 y1 = start->getLoc().getY();
    Sint16 x2 = end->getLoc().getX();
    Sint16 y2 = end->getLoc().getY();

    SDL_Renderer *renderer = this->renderer;
    Uint32 colorRGBA = 0xff00ffff; // RGBA 0xffff00ff

    std::vector<BezierCurve> curveList = BezierCurve::getBezierCurvesForPoints(x1, y1, x2, y2);
    bool foundCurve = false;
    for (int i = 0; i < curveList.size(); i++) {
        drawBezier(renderer, curveList[i].startX, curveList[i].startY, curveList[i].controlX1, curveList[i].controlY1,
                   curveList[i].controlX2, curveList[i].controlY2, curveList[i].endX, curveList[i].endY, colorRGBA);
        foundCurve = true;
    }
    if (!foundCurve) {
        drawLine(renderer, x1, y1, x2, y2, colorRGBA);
        std::cout << "need to define: " << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
    }
}

void NodeDrawer::drawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, Uint32 colorRGBA) const {
    drawBezier(renderer, x1, y1, x1, y1, x2, y2, x2, y2, colorRGBA);
}

void
NodeDrawer::drawBezier(SDL_Renderer *renderer, float startX, float startY, float c1X, float c1Y, float c2X, float c2Y,
                       float endX, float endY, Uint32 colorRGBA) const {
    // Number of points on the curve to calculate
    const int numPoints = 100;

    // Thickness of the Bezier curve
    const float thickness = 3.0f; // for example, a diameter of 3 pixels for the circles

    // Calculate points on the Bezier curve
    for (int i = 0; i <= numPoints; i++) {
        float t = i / static_cast<float>(numPoints);
        float u = 1 - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;

        // Bezier curve equation
        float pointX = uuu * startX + 3 * uu * t * c1X + 3 * u * tt * c2X + ttt * endX;
        float pointY = uuu * startY + 3 * uu * t * c1Y + 3 * u * tt * c2Y + ttt * endY;

        // Draw a filled circle at each calculated point on the curve
        filledCircleColor(renderer,
                          static_cast<Sint16>(pointX),
                          static_cast<Sint16>(pointY),
                          static_cast<Sint16>(thickness / 2),
                          colorRGBA); // Yellow color, for example
    }
}

void NodeDrawer::drawText(V2 loc, const std::string &text) const {
    // Set up the color for the text.
    SDL_Color textColoFG = {255, 255, 255, 255};
    SDL_Color textColoBG = {0, 0, 0, 255};

    // Create a surface from the string.
    TTF_Font *Sans = TTF_OpenFont("FiraCodeNerdFont-Bold.ttf", 10);
    SDL_Surface *textSurface = TTF_RenderText_LCD(Sans, text.c_str(), textColoFG, textColoBG);

    if (textSurface == NULL) {
        LOG_WARNING(ExInfo("Failed to create text surface"));
        return;
    }
    SDL_Renderer *renderer = this->renderer;
    // Create a texture from the surface.
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    // Set up the rectangle where the text will be rendered.
    SDL_Rect renderQuad = {loc.getX() - 20, loc.getY() + 10, textSurface->w, textSurface->h};

    // Render the text to the screen.
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

    // Clean up the surface and texture.
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}


