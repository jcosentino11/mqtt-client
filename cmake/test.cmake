enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SOURCES ${CMAKE_SOURCE_DIR}/test/*.cpp ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/include/*.h)
list(REMOVE_ITEM TEST_SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)

add_executable(
  MqttClientTests
  ${TEST_SOURCES}
)

target_include_directories(MqttClientTests PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(
  MqttClientTests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(MqttClientTests)
