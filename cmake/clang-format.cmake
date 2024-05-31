find_program(CLANG_FORMAT "clang-format")

if (CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCE_FILES src/*.cpp include/*.h test/*.cpp)

    add_custom_target(
        clang-format
        ALL
        COMMAND clang-format -i ${ALL_SOURCE_FILES}
    )
endif()
