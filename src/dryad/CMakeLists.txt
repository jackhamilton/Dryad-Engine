add_library(dryad STATIC
    AssetLibrary.cpp
    Button.cpp
    Camera.cpp
    CompositeSprite.cpp
    GameLoop.cpp
    GameObject.cpp
    Hitbox.cpp
    HorizontalStackFrame.cpp
    Input.cpp
    Line.cpp
    Mouse.cpp
    Physics.cpp
    Point.cpp
    Renderer.cpp
    Scene.cpp
    Sprite.cpp
    Spritesheet.cpp
    Text.cpp
    TextField.cpp
    UIBlock.cpp
    Vector.cpp
    VerticalStackFrame.cpp
    Window.cpp
    World.cpp
)

add_library(dryad::dryad ALIAS dryad)

target_compile_features(dryad PUBLIC cxx_std_20)

target_link_libraries(dryad PUBLIC SDL2 SDL2main SDL2_ttf)
