#include "Tileset.h"

#include <SDL3/SDL.h>

Tileset::Tileset(SDL_Texture* texture, int tilewidth, int tileheight) {
    m_texture = texture;
    m_tilewidth = tilewidth;
    m_tileheight = tileheight;

    int texturewidth, textureheight;
    SDL_QueryTexture(m_texture, NULL, NULL, &texturewidth, &textureheight);

    m_tilecountx = texturewidth / tilewidth;
    m_tilecounty = textureheight / tileheight;
    m_tilecount = m_tilecountx*m_tilecounty;
}

Tileset::~Tileset() {}


int Tileset::XYToIndex(int tilex, int tiley) {
    if(tilex < 0 || tilex >= m_tilecountx) return -1;
    if(tiley < 0 || tiley >= m_tilecounty) return -1;

    return tiley*m_tilecountx + tilex;
}


//////////////////////
// DrawTile

void Tileset::DrawTile(SDL_Renderer* renderer, int tilenum, SDL_FRect* dest) {
    if(tilenum < 0 || tilenum >= m_tilecount) return;

    SDL_FRect src = {
        (float) (( (int)(tilenum%m_tilecountx) ) * m_tilewidth),
        (float) (( (int)(tilenum/m_tilecountx) ) * m_tilewidth),
        (float) m_tilewidth,
        (float) m_tileheight
    };

    SDL_RenderTexture(renderer, m_texture, &src, dest);
}

void Tileset::DrawTile(SDL_Renderer* renderer, int tilex, int tiley, SDL_FRect* dest) {
    if(tilex < 0 || tilex >= m_tilecountx) return;
    if(tiley < 0 || tiley >= m_tilecounty) return;

    SDL_FRect src = {
        (float)(tilex*m_tilewidth),
        (float)(tiley*m_tilewidth),
        (float)m_tilewidth,
        (float)m_tileheight
    };

    SDL_RenderTexture(renderer, m_texture, &src, dest);
}


void Tileset::DrawTile(SDL_Renderer* renderer, int tilenum, int destx, int desty) {
    SDL_FRect dest = {
        (float)destx,
        (float)desty,
        (float)m_tilewidth,
        (float)m_tileheight
    };
    DrawTile(renderer, tilenum, &dest);
}

void Tileset::DrawTile(SDL_Renderer* renderer, int tilex, int tiley, int destx, int desty) {
    SDL_FRect dest = {
        (float)destx,
        (float)desty,
        (float)m_tilewidth,
        (float)m_tileheight
    };
    DrawTile(renderer, tilex, tiley, &dest);
}



void Tileset::DrawRectangle(SDL_Renderer* renderer, int topleftx, int toplefty, int bottomrightx, int bottomrighty, TilesetRectData* rectdata) {
    float wwb = (float)(bottomrightx-topleftx - (m_tilewidth*2));   //WidthWithoutBorders   (width without the 2 tiles at the edges)
    float hwb = (float)(bottomrighty-toplefty - (m_tileheight*2));  //HeightWithoutBorders  (height without the 2 tiles at the edges)

    //Corners
    DrawTile(renderer, rectdata->topleftindex       , topleftx                  , toplefty);
    DrawTile(renderer, rectdata->toprightindex      , bottomrightx-m_tilewidth  , toplefty);
    DrawTile(renderer, rectdata->bottomleftindex    , topleftx                  , bottomrighty-m_tileheight);
    DrawTile(renderer, rectdata->bottomrightindex   , bottomrightx-m_tilewidth  , bottomrighty-m_tileheight);


    //Sides
    SDL_FRect topdest       = {(float)(topleftx+m_tilewidth)    , (float)toplefty                   , wwb                   , (float)m_tileheight};
    SDL_FRect leftdest      = {(float)topleftx                  , (float)(toplefty+m_tileheight)    , (float)m_tilewidth    , hwb};
    SDL_FRect rightdest     = {(float)(bottomrightx-m_tilewidth), (float)(toplefty+m_tileheight)    , (float)m_tilewidth    , hwb};
    SDL_FRect bottomdest    = {(float)(topleftx+m_tilewidth)    , (float)(bottomrighty-m_tileheight), wwb                   , (float)m_tileheight};
    DrawTile(renderer, rectdata->topindex   , &topdest);
    DrawTile(renderer, rectdata->leftindex  , &leftdest);
    DrawTile(renderer, rectdata->rightindex , &rightdest);
    DrawTile(renderer, rectdata->bottomindex, &bottomdest);

    //Center
    SDL_FRect centerdest = {(float)(topleftx+m_tilewidth), (float)(toplefty+m_tileheight), wwb, hwb};
    DrawTile(renderer, rectdata->centerindex, &centerdest); 
}


void Tileset::DrawRectangle(SDL_Renderer* renderer, SDL_FRect* rect, TilesetRectData* rectdata) {
    //We will call the other DrawRectangle with these values
    int topleftx, toplefty, bottomrightx, bottomrighty;

    //if rect is null we need to fill the entire render target, like the other sdl drawing functions
    if(rect == NULL) {
        topleftx = 0;
        toplefty = 0;

        SDL_Texture* render_target = SDL_GetRenderTarget(renderer);
        if(render_target == NULL) SDL_GetRenderOutputSize(renderer, &bottomrightx, &bottomrighty);              //no render target -> fill the window
        else SDL_QueryTexture(render_target, NULL, NULL, &bottomrightx, &bottomrighty);     //render target -> fill the render target

        DrawRectangle(renderer, topleftx, toplefty, bottomrightx, bottomrighty, rectdata);
        return;
    }

    //handle negative width
    if(rect->w < 0) {
        topleftx = (int)(rect->x + rect->w);
        bottomrightx = (int)(rect->x);
    }
    else {
        topleftx = (int)(rect->x);
        bottomrightx = (int)(rect->x + rect->w);
    }

    //handle negative height
    if(rect->h < 0) {
        toplefty = (int)(rect->y + rect->h);
        bottomrighty = (int)(rect->y);
    }
    else {
        toplefty = (int)(rect->y);
        bottomrighty = (int)(rect->y + rect->h);
    }

    DrawRectangle(renderer, topleftx, toplefty, bottomrightx, bottomrighty, rectdata);
}
