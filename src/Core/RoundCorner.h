
#ifndef ROUNDCORNER_H
#define ROUNDCORNER_H

struct SDL_Texture;

//The texture must be a render target and have a pixel format of ARGB8888
void RoundTextureCorner(SDL_Texture* texture);

#endif //ROUNDCORNER_H
