cmake_minimum_required(VERSION 3.22)

project(Flipnote)

#set(CMAKE_BUILD_TYPE Debug)

file(COPY ./res DESTINATION ./)

# Add directories of libraries
add_subdirectory(./SDL)
add_subdirectory(./SDL_ttf)
add_subdirectory(./SDL_image)

add_executable(Flipnote)

target_sources( Flipnote PRIVATE
    src/main.cpp 
    
    src/BaseUI/DSThumbnail.cpp
    src/BaseUI/Editor/BrushMenu/BrushButton.cpp
    src/BaseUI/Editor/BrushMenu/BrushSizeButton.cpp
    src/BaseUI/Editor/EditorButtons/EditorButton.cpp
    src/BaseUI/Editor/EditorButtons/EditorBrushButton.cpp
    src/BaseUI/Editor/EditorButtons/EditorPenButton.cpp
    src/BaseUI/Editor/EditorButtons/EditorPlayButton.cpp
    src/BaseUI/Editor/EditorButtons/EditorTimelineButton.cpp
    src/BaseUI/Editor/EditorButtons/FrogMenuButton.cpp
    src/BaseUI/Editor/EditorButtons/LayerButton.cpp
    src/BaseUI/Editor/FrogMenu/SpeedSetting.cpp
    src/BaseUI/Editor/FlipnoteDisplay.cpp
    src/BaseUI/Editor/FlipnoteEditor.cpp
    src/BaseUI/Editor/FlipnoteTimeline.cpp
    src/BaseUI/Editor/Menu/MenuAligner.cpp
    src/BaseUI/Editor/PenMenu/ColorButton.cpp
    src/BaseUI/Editor/Popups/FilenamePopup.cpp
    src/BaseUI/Gallery/FlipnoteGallery.cpp
    src/BaseUI/Gallery/GalleryEntry.cpp
    
    src/Core/FlipnoteRessources.cpp 
    src/Core/Flipnote/Flipnote.cpp
    src/Core/Flipnote/FlipnoteFrame.cpp
    src/Core/Flipnote/FlipnoteLayer.cpp
    src/Core/Flipnote/FlipnotePainter.cpp
    src/Core/RoundCorner.cpp
    src/Core/States/EditorState.cpp
    src/Core/States/GalleryState.cpp
    src/Core/States/MainMenuState.cpp
    
    src/Reusable/DebugOverlay.cpp
    src/Reusable/DeltaTime.cpp 
    src/Reusable/gui/Background.cpp 
    src/Reusable/gui/CheckBox.cpp 
    src/Reusable/gui/ChildContainer.cpp 
    src/Reusable/gui/ClickableWidget.cpp
    src/Reusable/gui/FullscreenPopup.cpp
    src/Reusable/gui/IconButton.cpp 
    src/Reusable/gui/Label.cpp 
    src/Reusable/gui/PopupMenuTop.cpp
    src/Reusable/gui/PopupMenu.cpp
    src/Reusable/gui/TextField.cpp
    src/Reusable/gui/TopBar.cpp
    src/Reusable/gui/WidgetContainer.cpp
    src/Reusable/gui/Widget.cpp
    src/Reusable/gui/WinWidgetContainer.cpp
    src/Reusable/Ressources.cpp 
    src/Reusable/RunState.cpp 
    src/Reusable/State/SimpleState.cpp 
    src/Reusable/State/State.cpp 
    src/Reusable/State/StateManager.cpp 
    src/Reusable/Tileset.cpp
    src/Reusable/Utils.cpp
)

set_property(TARGET Flipnote PROPERTY CXX_STANDARD 17)


# SDL3::SDL3, SDL3_ttf::SDL3_ttf, SDL3_image::SDL3_image are defined in their respective CMakeList
# registered by the add_library calls previously
target_link_libraries(Flipnote SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image)
