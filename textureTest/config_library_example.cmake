if(WIN32)
    # for windows find_package(GTest REQUIRED) DONOT work
    set(GTEST_ROOT "C:/Program Files/googletest-distribution")
	include_directories("C:/Program Files/googletest-distribution/include")
    set(GTEST_MSVC_SEARCH MT)
else()
    set(GTEST_ROOT /usr/local)
    find_package(GTest REQUIRED)
endif()