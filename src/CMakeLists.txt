add_executable(opengl)

target_sources(opengl
    PUBLIC
        main.cpp
        LoadShader.cpp
    )

target_link_libraries(opengl
    PUBLIC
        glad
        glfw
        # ${CMAKE_DL_LIBS}
    )