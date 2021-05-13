#include "Line.h"

Line::Line(SDL_Color color, Point positionA, Point positionB)
{
    SDL_Surface* surface = drawLine(positionA, positionB, color);
	Sprite* s = new Sprite({ surface });
	setSprite(s);
}

SDL_Surface* Line::drawLine(Point positionA, Point positionB, SDL_Color color) {
    Point a, b;
    int minX = positionA.x < positionB.x ? positionA.x : positionB.x;
    int minY = positionA.y < positionB.y ? positionA.y : positionB.y;
    a.x = positionA.x - minX;
    b.x = positionB.x - minX;
    a.y = positionA.y - minY;
    b.y = positionB.y - minY;
    int width = a.x > b.x ? a.x : b.x;
    int height = a.y > b.y ? a.y : b.y;
    SDL_Surface* surface;
    if (SDL_BYTEORDER == SDL_BIG_ENDIAN) { 
        surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width + 4, height + 4, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    }
    else {
        surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width + 4, height + 4, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
    }
    SDL_LockSurface(surface);
    int x1 = a.x, x2 = b.x;
    int y1 = a.y, y2 = b.y;
    // Bresenham's line algorithm
    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
    if (steep)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if (x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    const float dx = x2 - x1;
    const float dy = fabs(y2 - y1);

    float error = dx / 2.0f;
    const int ystep = (y1 < y2) ? 1 : -1;
    int y = (int)y1;

    const int maxX = (int)x2;

    for (int x = (int)x1; x <= maxX; x++)
    {
        if (steep)
        {
            set_pixel(surface, x, y, SDL_MapRGB(surface->format, color.r, color.g, color.b));
        }
        else
        {
            set_pixel(surface, x, y, SDL_MapRGB(surface->format, color.r, color.g, color.b));
        }

        error -= dy;
        if (error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
    SDL_UnlockSurface(surface);
    position = Point(minX, minY);
    return surface;
}

void Line::set_pixel(SDL_Surface* surface, int x, int y, Uint32 color)
{
    Uint8* pixel = (Uint8*)surface->pixels;
    pixel += (y * surface->pitch) + (x * sizeof(Uint32));
    *((Uint32*)pixel) = color;
}