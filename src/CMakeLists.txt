cmake_minimum_required(VERSION 3.2)
project(RayTracer)

set(SOURCE_FILES main.cpp)

add_executable(RayTracer ${SOURCE_FILES})
target_link_libraries(RayTracer
    PRIVATE glm
)
install(TARGETS RayTracer DESTINATION ${RAYTRACER_INSTALL_BIN_DIR})
