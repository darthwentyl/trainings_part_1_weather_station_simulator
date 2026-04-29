file(GLOB SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/helpers/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/ipc/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/logger/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/exceptions/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/proc_managers/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/proc_managers/workers/*.cpp"
)

add_library(weather_lib ${SOURCES})
if(CMAKE_BUILD_TYPE STREQUAL coverage)
    target_compile_options(weather_lib PRIVATE -Wall -Wextra -Wpedantic --coverage)
    target_link_options(weather_lib PRIVATE --coverage)
else()
    target_compile_options(weather_lib PRIVATE  -Wall -Wextra -Wpedantic -g)
endif()

if(UT)
    target_compile_definitions(weather_lib PUBLIC UT=1)
endif()

if(RELEASE)
    target_compile_definitions(weather_lib PRIVATE RELEASE=1)
endif()

include_directories(weather_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
