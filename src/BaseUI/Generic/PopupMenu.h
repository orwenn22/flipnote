#ifndef POPUPMENU_H
#define POPUPMENU_H

#include <vector>

#include "WidgetContainer.h"

class Widget;

struct SDL_Renderer;
struct SDL_Texture;

//This can't be used as a child container
class PopupMenu : public WidgetContainer {
    public:
    PopupMenu(int originx, int originy, int width, int height);
    virtual ~PopupMenu();

    void Update();
    void Render(SDL_Renderer* renderer);

    //Methods to set the absolute position of the popup menu. Will move all the edges
    void SetPosition(int x, int y);
    void SetX(int x);
    void SetY(int y);

    //Get position of topleft
    int GetContainerX();
    int GetContainerY();

    //Methods to set the (absolute) size of the popup menu. Will call CalculateEdgesPos and SetupContent.
    void SetSize(int w, int h);
    void SetWidth(int w);
    void SetHeight(int h);

    //Get the distance between the opposite borders (not m_width or m_height)
    //Intended to be used by the menu's items
    int GetContainerWidth();
    int GetContainerHeight();

    //Get the relative mouse position from the top left of the popup menu.
    void GetRelativeMousePos(int* x, int* y);

    //Check if the mouse is overring the popup menu.
    bool IsMouseOvering();


    private:
    void SetupContent();

    //Called by Update. Update all the animations values.
    void UpdateAnimation();

    void CalculateEdgesPos();

    //Render the popup using the animation values
    void RenderPopup(SDL_Renderer* renderer);

    //Calculate the width and height without counting the border tiles's size
    float WidthWithoutBorder();
    float HeightWithoutBorder();

    //Absolute position of the popup menu. Can be set with SetPosition, SetX and SetY.
    int m_originx;
    int m_originy;

    //Can be modified with SetSize, SetWidth and SetHeight.
    //Can be negatives to expend the popup menu in other directions than the bottom right.
    int m_width;
    int m_height;

    //Absolute position of the corners of the popup.
    //Calculated by CalculateEdgesPos using the origin and the size each tile the width and height are modified (and also the origin pos)
    int m_topleftx;
    int m_toplefty ;
    int m_bottomrightx;
    int m_bottomrighty;

    int m_animationspeed;
    //These values are used by RenderPopup
    int m_animationtopleftx;
    int m_animationtoplefty;
    int m_animationbottomrightx;
    int m_animationbottomrighty;

    //This is on this texture that the content of the menu is drawn.
    SDL_Texture* m_content;
};

#endif