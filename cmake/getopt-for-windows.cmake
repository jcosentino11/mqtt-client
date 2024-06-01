if(WIN32)
    include(FetchContent)
    FetchContent_Declare(
        getopt
        URL https://github.com/Chunde/getopt-for-windows/archive/refs/heads/master.zip
        URL_HASH SHA256=fb4a7be022653e66f2025d4132fe8f3b3f1043f264a095caebc1792089585c59
    )
    FetchContent_MakeAvailable(getopt)

    list(APPEND SOURCES ${getopt_SOURCE_DIR}/getopt.c)
    list(APPEND INCLUDE_DIRS ${getopt_SOURCE_DIR})
endif()
