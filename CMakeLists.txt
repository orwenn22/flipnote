cmake_minimum_required(VERSION 3.22)

project(Flipnote)

set(CMAKE_BUILD_TYPE Debug)

file(COPY ./res DESTINATION ./)

add_subdirectory(./SDL)
add_subdirectory(./SDL_ttf)
add_subdirectory(./SDL_image)

include_directories(
    ./SDL/include/SDL3
    ./SDL/include
    ./SDL_ttf/include/SDL3
    ./SDL_image/include/SDL3
)

link_directories(
    ./build/SDL
    ./build/SDL_ttf
    ./build/SDL_image
)

add_executable(Flipnote 
    src/main.cpp 
    src/BaseUI/DSThumbnail.cpp
    src/BaseUI/Editor/BrushMenu/BrushButton.cpp
    src/BaseUI/Editor/BrushMenu/BrushSizeButton.cpp
    src/BaseUI/Editor/EditorButtons/EditorButton.cpp
    src/BaseUI/Editor/EditorButtons/EditorBrushButton.cpp
    src/BaseUI/Editor/EditorButtons/EditorPenButton.cpp
    src/BaseUI/Editor/EditorButtons/EditorPlayButton.cpp
    src/BaseUI/Editor/EditorButtons/EditorTimelineButton.cpp
    src/BaseUI/Editor/FlipnoteDisplay.cpp
    src/BaseUI/Editor/FlipnoteEditor.cpp
    src/BaseUI/Editor/FlipnoteTimeline.cpp
    src/BaseUI/Editor/Menu/MenuAligner.cpp
    src/BaseUI/Editor/PenMenu/ColorButton.cpp
    src/BaseUI/Generic/Background.cpp 
    src/BaseUI/Generic/CheckBox.cpp 
    src/BaseUI/Generic/ChildContainer.cpp 
    src/BaseUI/Generic/ClickableWidget.cpp 
    src/BaseUI/Generic/IconButton.cpp 
    src/BaseUI/Generic/Label.cpp 
    src/BaseUI/Generic/PopupMenuTop.cpp
    src/BaseUI/Generic/PopupMenu.cpp
    src/BaseUI/Generic/TopBar.cpp
    src/BaseUI/Generic/WidgetContainer.cpp
    src/BaseUI/Generic/Widget.cpp
    src/BaseUI/Generic/WinWidgetContainer.cpp
    src/Core/Ressources.cpp 
    src/Core/Flipnote/Flipnote.cpp
    src/Core/Flipnote/FlipnoteFrame.cpp
    src/Core/Flipnote/FlipnotePainter.cpp
    src/Core/Tileset.cpp
    src/Core/Utils.cpp
    src/Reusable/RunState.cpp 
    )


target_link_libraries(Flipnote SDL3 SDL3_ttf SDL3_image)