#include "3D_ENGINE/renderer.hpp"
#include "3D_ENGINE/window.hpp"
#include "3D_ENGINE/player.hpp"
#include "3D_ENGINE/window_defines.hpp"
#include "3D_ENGINE/renderer.hpp"
#include <GLFW/glfw3.h>
#include <cmath>

Renderer renderer;

void Renderer::clipBehindPlayer(float *x1, float *y1, float *z1, float *x2, float *y2, float *z2) {
    if (*y1 < 1 && *y2 < 1) return; // If both points are behind the player, do nothing

    if (*y1 < 1) {
        float t = (1 - *y1) / (*y2 - *y1);
        *x1 = *x1 + t * (*x2 - *x1);
        *y1 = 1; // Clip y1 to near plane
        *z1 = *z1 + t * (*z2 - *z1);
    } else if (*y2 < 1) {
        float t = (1 - *y2) / (*y1 - *y2);
        *x2 = *x2 + t * (*x1 - *x2);
        *y2 = 1; // Clip y2 to near plane
        *z2 = *z2 + t * (*z1 - *z2);
    }
}

void Renderer::drawWall(int xPos1, int xPos2, int bottomPos1, int bottomPos2, int topPos1, int topPos2) {
    int distBottom  = bottomPos2 - bottomPos1;
    int distTop     = topPos2 - topPos1;
    int distX       = (xPos2 - xPos1 == 0) ? 1 : xPos2 - xPos1;

    if (xPos1 < 1) xPos1 = 1;
    if (xPos1 > SCR_WIDTH - 1) xPos1 = SCR_WIDTH - 1;
    if (xPos2 < 1) xPos2 = 1;
    if (xPos2 > SCR_WIDTH - 1) xPos2 = SCR_WIDTH - 1;

    for (int x = xPos1; x < xPos2; x++) {
        int y1 = bottomPos1 + distBottom * (x - xPos1) / distX;
        int y2 = topPos1 + distTop * (x - xPos1) / distX;

        // Y clipping
        if (y1 < 1) y1 = 1;
        if (y1 > SCR_HEIGHT - 1) y1 = SCR_HEIGHT - 1;
        if (y2 < 1) y2 = 1;
        if (y2 > SCR_HEIGHT - 1) y2 = SCR_HEIGHT - 1;
        
        for (int y = y1; y < y2; y++) {
            mainWindow.pixel(x, y, white);
        }
    }
}

void Renderer::draw3D() {
    float wx[4], wy[4], wz[4];
    float cs = cos(player.angle * M_PI / 180);
    float sn = sin(player.angle * M_PI / 180);

    float x1 = 40 - player.x, y1 = 10 - player.y;
    float x2 = 40 - player.x, y2 = 290 - player.y;

    wx[0] = x1 * cs - y1 * sn;
    wy[0] = y1 * cs + x1 * sn;
    
    wx[1] = x2 * cs - y2 * sn;
    wy[1] = y2 * cs + x2 * sn;

    wx[2] = wx[0];
    wy[2] = wy[0];

    wx[3] = wx[1];
    wy[3] = wy[1];

    wz[0] = 0 - player.z + ((player.lookAngle * wy[0]) / 32.0);
    wz[1] = 0 - player.z + ((player.lookAngle * wy[0]) / 32.0);
    wz[2] = wz[0] + 50;
    wz[3] = wz[1] + 50;

    if (wy[0] < 1 || wy[1] < 1) {
        clipBehindPlayer(&wx[0], &wy[0], &wz[0], &wx[1], &wy[1], &wz[1]);
        clipBehindPlayer(&wx[2], &wy[2], &wz[2], &wx[3], &wy[3], &wz[3]);
    }

    wx[0] = wx[0] * 200 / wy[0] + SCR_WIDTH_HALF;
    wy[0] = wz[0] * 200 / wy[0] + SCR_HEIGHT_HALF;
    wx[1] = wx[1] * 200 / wy[1] + SCR_WIDTH_HALF;
    wy[1] = wz[1] * 200 / wy[1] + SCR_HEIGHT_HALF;
    wx[2] = wx[2] * 200 / wy[2] + SCR_WIDTH_HALF;
    wy[2] = wz[2] * 200 / wy[2] + SCR_HEIGHT_HALF;
    wx[3] = wx[3] * 200 / wy[3] + SCR_WIDTH_HALF;
    wy[3] = wz[3] * 200 / wy[3] + SCR_HEIGHT_HALF;

    if (wy[0] < 1 && wy[1] < 1) return;

    drawWall(static_cast<int>(wx[0]), static_cast<int>(wx[1]), static_cast<int>(wy[0]), static_cast<int>(wy[1]), static_cast<int>(wy[2]), static_cast<int>(wy[3]));

    if (wx[0] >= 0 && wx[0] < SCR_WIDTH && wy[0] >= 0 && wy[0] < SCR_HEIGHT)
        mainWindow.pixel(static_cast<int>(wx[0]), static_cast<int>(wy[0]), yellow);
    if (wx[1] >= 0 && wx[1] < SCR_WIDTH && wy[1] >= 0 && wy[1] < SCR_HEIGHT)
        mainWindow.pixel(static_cast<int>(wx[1]), static_cast<int>(wy[1]), red);
    if (wx[2] >= 0 && wx[2] < SCR_WIDTH && wy[2] >= 0 && wy[2] < SCR_HEIGHT)
        mainWindow.pixel(static_cast<int>(wx[2]), static_cast<int>(wy[2]), green);
    if (wx[3] >= 0 && wx[3] < SCR_WIDTH && wy[3] >= 0 && wy[3] < SCR_HEIGHT)
        mainWindow.pixel(static_cast<int>(wx[3]), static_cast<int>(wy[3]), blue);
}
