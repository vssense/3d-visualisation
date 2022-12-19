add_library(glad)

target_include_directories(glad
    PUBLIC
        ${OPENGL_SOURCE_DIR}/third_party/glad/include
    )

target_sources(glad
    PUBLIC
        ${OPENGL_SOURCE_DIR}/third_party/glad/include/glad/glad.h
        ${OPENGL_SOURCE_DIR}/third_party/glad/include/KHR/khrplatform.h
    PRIVATE
        ${OPENGL_SOURCE_DIR}/third_party/glad/src/glad.c
    )