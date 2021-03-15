cmake_minimum_required(VERSION 3.14)

project(rt-main)

list(APPEND PRIVATE_HDR_FILES
    AssertUtils.h
    ColorUtils.h
    CPUImage.h
    Ray.h
    TypedBitmask.h
    Vec3.h
)

list(APPEND PRIVATE_SRC_FILES
    main.cpp
    AssertUtils.cpp
    ColorUtils.cpp
)

add_executable(${PROJECT_NAME}
    ${PRIVATE_HDR_FILES}
    ${PRIVATE_SRC_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    SDL2-static
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)