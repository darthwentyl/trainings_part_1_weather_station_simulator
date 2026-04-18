add_executable(weather main.cpp)
target_compile_options(weather PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(weather
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
)
target_link_libraries(weather
    PRIVATE
        weather_lib
)