cmake_minimum_required(VERSION 3.14)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

set(PROJECT_NAME systemc_example)
project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT DEFINED ENV{SYSTEMC_HOME})

    find_program(CONAN_EXE NAMES conan REQUIRED)

    if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
        file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                TLS_VERIFY ON)
    endif ()

    include(${CMAKE_BINARY_DIR}/conan.cmake)

    conan_add_remote(NAME minres URL https://artifactory.minres.com/artifactory/api/conan/oss)

    conan_cmake_configure(REQUIRES systemc/2.3.3 systemc-cci/1.0.0 systemc-scv/2.0.1
            OPTIONS systemc-cci:shared=False
            SETTINGS compiler.cppstd=${CMAKE_CXX_STANDARD}
            GENERATORS cmake_find_package)

    conan_cmake_autodetect(settings)

    conan_cmake_install(PATH_OR_REFERENCE .
            BUILD missing
            SETTINGS ${settings} compiler.cppstd=${CMAKE_CXX_STANDARD})

    find_package(SystemCLanguage)
    find_package(systemc-scv)
    find_package(systemc-cci)

    add_subdirectory(src)
    target_link_libraries(${PROJECT_NAME} systemc-cci::systemc-cci systemc-scv::systemc-scv SystemC::systemc)

else ()
    find_package(SystemC REQUIRED)
    add_subdirectory(src)
    target_include_directories(${PROJECT_NAME} PUBLIC ${SystemC_INCLUDE_DIRS} ${SCV_INCLUDE_DIRS})
    target_link_directories(${PROJECT_NAME} PUBLIC ${SCV_LIBRARY_DIRS} ${SystemC_LIBRARY_DIRS})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${SCV_LIBRARIES} ${SystemC_LIBRARIES})
endif ()

# CTest is a testing tool that can be used to test your project.
# TODO: Uncomment when at least one test added
# enable_testing()
add_test(NAME example
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        COMMAND example)
