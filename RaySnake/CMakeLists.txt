set(RAYGAME_SRC
    "RaySnake.cpp"

    "Graphics/DrawGeometry.cpp"
    
    "Layer/GameLayer.cpp"
    "Layer/MenuChoice.cpp"
    "Layer/MenuLayer.cpp"
    
    "Objects/Snake.cpp"
    
    "Util/Random.cpp"
)
source_group(
    TREE ${CMAKE_CURRENT_LIST_DIR}
    FILES ${RAYGAME_SRC}
)

set(RAYGAME_HEADER
    "RaySnake.h"

    "Graphics/DrawGeometry.h"
    
    "Layer/GameLayer.h"
    "Layer/MenuChoice.h"
    "Layer/MenuLayer.h"
    
    "Objects/Snake.h"
    
    "Util/Random.h"
    "Util/Timer.h"
)
source_group(
    TREE ${CMAKE_CURRENT_LIST_DIR}
    FILES ${RAYGAME_HEADER}
)

add_executable(RaySnake
    ${RAYGAME_SRC}
    ${RAYGAME_HEADER}
)

target_link_libraries(
    RaySnake
    PRIVATE
    raylib
)

target_include_directories(
    RaySnake
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

add_custom_command(
    TARGET RaySnake POST_BUILD
    COMMAND cmake -E make_directory $<TARGET_FILE_DIR:RaySnake>/res
)

add_custom_command(
    TARGET RaySnake POST_BUILD
    COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:RaySnake>/res
)