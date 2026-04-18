file(GLOB SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/helpers/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/ipc/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/proc_managers/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/proc_managers/workers/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/mocks/*.cpp"
)

add_executable(weather_tests ${SOURCES})

if(CMAKE_BUILD_TYPE STREQUAL coverage)
    target_compile_options(weather_tests PRIVATE -Wall -Wextra -Wpedantic --coverage)
    target_link_options(weather_tests PRIVATE --coverage)
else()
    target_compile_options(weather_tests PRIVATE -g -Wall -Wextra -Wpedantic)
endif()

include_directories(weather_tests
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
        ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(weather_tests
    PRIVATE
        gtest
        gmock
        gtest_main
        weather_lib
        dl
)
