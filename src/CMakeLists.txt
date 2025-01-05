# src/CMakeLists.txt

add_library(LimboEngine STATIC
    core/logger.cpp)

# Include the top-level "include" folder if needed
target_include_directories(LimboEngine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Create an executable target called "LimboMain" from main.cpp
add_executable(LimboMain
    main/main.cpp)

# Link the engine library to the main app:
target_link_libraries(LimboMain PRIVATE
    LimboEngine
)
