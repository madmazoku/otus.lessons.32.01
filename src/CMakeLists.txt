# global
cmake_minimum_required(VERSION 3.2)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(realty VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(realty VERSION 0.0.1)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost REQUIRED)
find_package(dlib REQUIRED)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT madmazoku@gmail.com)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -std=c++11 -pthread")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -std=c++11 -pthread")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -std=c++11 -pthread")

include(CPack)

# local
configure_file(in.version.h version.h)

add_executable(rclst "rclst.cpp")
add_executable(rclss "rclss.cpp")

set_target_properties(rclst rclss PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_link_libraries(rclst
    ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} dlib::dlib ${PNG_LIBRARY}
)

target_link_libraries(rclss
    ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} dlib::dlib ${PNG_LIBRARY}
)

install(TARGETS rclst rclss RUNTIME DESTINATION bin)
