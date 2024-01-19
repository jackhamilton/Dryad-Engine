add_executable(dryad_sample
    main.cpp
)

target_link_libraries(dryad_sample SDL2::SDL2main SDL2_ttf::SDL2_ttf-static dryad)
