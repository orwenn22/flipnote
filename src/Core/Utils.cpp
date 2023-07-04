#include "Utils.h"

#include <math.h>

Uint32 SDLColorToUint32(SDL_Color c) {
    return ((c.a << 24) | (c.r << 16) | (c.g << 8) | c.b);
}
