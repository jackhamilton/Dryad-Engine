#include "Renderer.h"
#include <utility>

Renderer::Renderer(SDL_Window* window)
{
	renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr) {
        printf("%s", SDL_GetError());
    }
}

SDL_Renderer* Renderer::getSDLRenderer()
{
	if (!renderer) {
		printf("Error: Could not retrieve renderer. One potential cause is a floating scene - make sure all your scenes are part of a world.\n");
	}
	return renderer;
}

void Renderer::render(shared_ptr<SDL_Texture> texture, SDL_Rect dstrect)
{
	SDL_RenderCopy(renderer, texture.get(), NULL, &dstrect);
}

void Renderer::render(shared_ptr<SDL_Texture> texture, SDL_Rect srcrect, SDL_Rect dstrect)
{
	SDL_RenderCopy(renderer, texture.get(), &srcrect, &dstrect);
}

void Renderer::render(SDL_Texture* texture, SDL_Rect dstrect)
{
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
}

void Renderer::render(SDL_Texture* texture, SDL_Rect srcrect, SDL_Rect dstrect)
{
	SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
}

void Renderer::renderPresent() {
	SDL_RenderPresent(renderer);
}

void Renderer::renderBackground()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
}

void Renderer::computeLighting(double globalIllumination, pair<vector<Light>, vector<vector<Polygon>>> masks)
{
    SDL_Surface* surface;
    pair<int, int> dim = Window::calculateResolution(res);
    int width = dim.first, height = dim.second;
    surface = SDL_CreateRGBSurfaceWithFormat(SDL_SWSURFACE, width, height, 32, SDL_PIXELFORMAT_ABGR32);
    SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, ((1 - globalIllumination) * 255.0)));
    //Perform blitting with light surfaces
    vector<Light> lights = masks.first;
    vector<vector<Polygon>> lightMasks = masks.second;
    for (int x = 0; x < lights.size(); x++) {
        Light light = lights.at(x);
        vector<Polygon> maskPolys = lightMasks.at(x);
        for (Polygon poly : maskPolys) {
            SDL_Color color = { 0, 0, 0, 0 };
            drawFilledTriangle(surface, poly, color);
        }
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, tex, NULL, NULL);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(tex);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(renderer);
}

bool Renderer::DrawFilledPolygon(Polygon poly, const SDL_Color color) {
    int topY;            // used to hold the y coordinate of the top vertex
    int topCnt;            // used to hold the index of the top vertex
    int leftCnt;            // used to hold the index of the vertex left of the top vertex
    int rightCnt;           // used to hold the index of the vertex right of the top vertex
    int startX;            // Starting point to draw the line, uses FP math
    int endX;            // ending point to draw the line, uses FP math
    int cntY;            // y position of the current line
    int leftSlope;              // Slope of the left side, uses FP math
    int rightSlope;             // Slope of the right side, uses FP math
    int cnt;            // counting variable used in loops
    int numVerts = poly.shape.size();    // number of vertices in the polygon being drawn, initialize immediately
    int numVertsProc = 1;           // number of vertices that have been processed, initialize to 1

    Point center = poly.getCenter();
    Point* verts = poly.shape.data();      // Vertex information of the polygon

    topY = verts[0].y;          // Initialize the top y coordinate to the first point
    topCnt = 0;            // set to top point to 0

    // assumes points in counterclockwise order
    // find the true top point
    for (cnt = 1; cnt < numVerts; cnt++)      // for all the vertices in the polygon
    {
        if (verts[cnt].y < topY)        // if vertex of the current vertex is above the top vertex
        {
            topY = verts[cnt].y;        // set the top vertex to the current vertex
            topCnt = cnt;           // set the reference number of the top vertex
        }
    }

    // find point to left
    leftCnt = topCnt - 1;           // set the left point to one less than the top point
    if (leftCnt < 0)             // if the left vertex specified is less than 0
        leftCnt = numVerts - 1;        // set the left vertex to the top vertex

    // find point to right
    rightCnt = topCnt + 1;          // set the right vertex to one more than the top vertex
    if (rightCnt >= numVerts)        // if the right vertex specified is more than the number of vertices
        rightCnt = 0;          // set it equal to 0

    startX = endX = (int)(verts[topCnt].x + center.x) << 16;  // set the starting and ending points of the line
    cntY = verts[topCnt].y;         // set the first y value to draw at

    if (verts[leftCnt].y != verts[topCnt].y)     // if the top and left vertex do not have the same y value
        leftSlope = ((int)(verts[leftCnt].x - verts[topCnt].x) << 16) / (verts[leftCnt].y - verts[topCnt].y);    // find the left side slope
    if (verts[rightCnt].y != verts[topCnt].y)    // if the top and right vertex do not have the same y value
        rightSlope = ((int)(verts[rightCnt].x - verts[topCnt].x) << 16) / (verts[rightCnt].y - verts[topCnt].y); // find the right side slope

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // find slopes
    while (numVertsProc < numVerts)          // if there remain vertices to be processed
    {
        // rasterize to first point
        while (cntY < verts[leftCnt].y && cntY < verts[rightCnt].y)  // while one of the two side points hasn't been reached yet
        {
            SDL_RenderDrawLine(renderer, startX >> 16, cntY + center.y, endX >> 16, cntY + center.y);
            //DrawLine(Point(startX >> 16, cntY + center.y), Point(endX >> 16, cntY + center.y), col);  // draw a line between the sides
            cntY++;             // increment the y position by 1
            startX += leftSlope;        // increase the starting x value by the left slope
            endX += rightSlope;        // increase the ending x value by the right slope
        }
        // set top point to point met
        // set point met to next point
        // find new slope
        if (verts[leftCnt].y <= cntY)      // if the raster line passes the left coordinate
        {
            topCnt = leftCnt;        // set the top point to the left side
            leftCnt--;          // decrement the left point
            if (leftCnt < 0)         // if the left point is less than zero
                leftCnt = numVerts - 1;      // wrap around
            if (verts[leftCnt].y != verts[topCnt].y)     // if the top and new left vertex do not have the same y value
                leftSlope = ((int)(verts[leftCnt].x - verts[topCnt].x) << 16) / (verts[leftCnt].y - verts[topCnt].y);    // find the left side slope

            startX = (int)(verts[topCnt].x + center.x) << 16;    // set the starting x position
            numVertsProc++;         // increment the number of vertices processed
        }

        if (verts[rightCnt].y <= cntY)      // if the raster line passes the right coordinate
        {
            topCnt = rightCnt;        // set the top point to the right side
            rightCnt++;          // increment the right point
            if (rightCnt == numVerts)      // if the right point is more than the number of vertices
                rightCnt = 0;        // set the right point to zero
            if (verts[rightCnt].y != verts[topCnt].y)  // if the top and new right vertex do not have the same y value
                rightSlope = ((int)(verts[rightCnt].x - verts[topCnt].x) << 16) / (verts[rightCnt].y - verts[topCnt].y); // find the right side slope

            endX = (int)(verts[topCnt].x + center.x) << 16;  // set the ending x position
            numVertsProc++;         // increment the number of vertices processed
        }
        SDL_RenderDrawLine(renderer, startX >> 16, cntY + center.y, endX >> 16, cntY + center.y);
        //DrawLine(Point(startX >> 16, cntY + center.y), Point(endX >> 16, cntY + center.y), col); // draw a line between the sides 
    }
    // continue until the number of vertices p0 has touched == number of vertices

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, color.a);
    vector<pair<Point, Point>> sides = poly.getSides();
    for (pair<Point, Point> sideLine : sides) {
        SDL_RenderDrawLine(renderer, sideLine.first.x, sideLine.first.y, sideLine.second.x, sideLine.second.y);
    }

    return true;            // return success
}

void Renderer::drawFilledTriangle(SDL_Surface* surf, Polygon poly, const SDL_Color color) {
    //if 3 changed to variable could do other things
    pair<int, int> dim = Window::calculateResolution(res);
    int width = dim.first, height = dim.second;
    int polyX[3], polyY[3];
    for (int x = 0; x < 3; x++) {
        polyX[x] = poly.shape.at(x).x;
        polyY[x] = poly.shape.at(x).y;
    }
    int x0 = polyX[0], x1 = polyX[1], x2 = polyX[2];
    int y0 = polyY[0], y1 = polyY[1], y2 = polyY[2];
    SDL_LockSurface(surf);
    // sort the points vertically
    if (y1 > y2)
    {
        swap(x1, x2);
        swap(y1, y2);
    }
    if (y0 > y1)
    {
        swap(x0, x1);
        swap(y0, y1);
    }
    if (y1 > y2)
    {
        swap(x1, x2);
        swap(y1, y2);
    }

    double dx_far = double(x2 - x0) / (y2 - y0 + 1);
    double dx_upper = double(x1 - x0) / (y1 - y0 + 1);
    double dx_low = double(x2 - x1) / (y2 - y1 + 1);
    double xf = x0;
    double xt = x0 + dx_upper; // if y0 == y1, special case
    for (int y = y0; y <= (y2 > height - 1 ? height - 1 : y2); y++)
    {
        if (y >= 0)
        {
            for (int x = (xf > 0 ? int(xf) : 0);
                x <= (xt < width ? xt : width - 1); x++)
                set_pixel(surf, x, y, SDL_MapRGBA(surf->format, color.r, color.g, color.b, color.a));
            for (int x = (xf < width ? int(xf) : width - 1);
                x >= (xt > 0 ? xt : 0); x--)
                set_pixel(surf, x, y, SDL_MapRGBA(surf->format, color.r, color.g, color.b, color.a));
        }
        xf += dx_far;
        if (y < y1)
            xt += dx_upper;
        else
            xt += dx_low;
    }
    SDL_UnlockSurface(surf);
}

void Renderer::drawFilledTriangle(Polygon poly, const SDL_Color color) {
    pair<int, int> dim = Window::calculateResolution(res);
    int width = dim.first, height = dim.second;
    SDL_Surface* surface;
    surface = SDL_CreateRGBSurfaceWithFormat(SDL_SWSURFACE, width, height, 32, SDL_PIXELFORMAT_ABGR32);
    drawFilledTriangle(surface, poly, color);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, tex, NULL, NULL);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(tex);
}

void Renderer::set_pixel(SDL_Surface* surface, int x, int y, Uint32 color)
{
    Uint8* channel = (Uint8*)surface->pixels;
    channel += (y * surface->pitch) + (x * sizeof(Uint32));
    Uint8 r, g, b, a;
    SDL_GetRGBA(*((Uint32*)channel), surface->format, &r, &g, &b, &a);
    *((Uint32*)channel) = color;
}