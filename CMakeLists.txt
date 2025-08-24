cmake_minimum_required(VERSION 3.10)

include(FetchContent)

project(ssnd VERSION 1.0
	     DESCRIPTION "A lightweight dbus notification daemon with scripting being its first-class citizen."
	     LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_SOURCE_DIR}/extern/libsigc++
                 ${CMAKE_BINARY_DIR}/extern/libsigc++-build)

# create a small pkgconfig dir (in the build tree) and write a .pc file that points to sigc++'s build/install locations.
set(_pkgdir "${CMAKE_BINARY_DIR}/_pkgconfig")
file(MAKE_DIRECTORY "${_pkgdir}")

set(_sigc_inc "${CMAKE_SOURCE_DIR}/extern/libsigc++")
set(_sigc_libdir "${CMAKE_BINARY_DIR}/extern/libsigc++-build/lib")

file(WRITE "${_pkgdir}/sigc++-3.0.pc" "prefix=${CMAKE_BINARY_DIR}/extern/libsigc++-build\n")
file(APPEND "${_pkgdir}/sigc++-3.0.pc" "exec_prefix=\${prefix}\n")
file(APPEND "${_pkgdir}/sigc++-3.0.pc" "libdir=${_sigc_libdir}\n")
file(APPEND "${_pkgdir}/sigc++-3.0.pc" "includedir=${_sigc_inc}\n\n")
file(APPEND "${_pkgdir}/sigc++-3.0.pc" "Name: libsigc++\n")
file(APPEND "${_pkgdir}/sigc++-3.0.pc" "Description: Generated pkg-config file for in-tree libsigc++\n")
file(APPEND "${_pkgdir}/sigc++-3.0.pc" "Version: 3.0.0\n")
file(APPEND "${_pkgdir}/sigc++-3.0.pc" "Libs: -L${libdir} -lsigc-3.0\n")
file(APPEND "${_pkgdir}/sigc++-3.0.pc" "Cflags: -I${CMAKE_SOURCE_DIR}/extern/libsigc++ -I${CMAKE_BINARY_DIR}/extern/libsigc++-build\n")

# add generated pc file to pkgconfig path for dbus
set(ENV{PKG_CONFIG_PATH} "${_pkgdir}:$ENV{PKG_CONFIG_PATH}")

add_subdirectory(${CMAKE_SOURCE_DIR}/extern/dbus-cxx
                 ${CMAKE_BINARY_DIR}/extern/dbus-cxx-build)

file(GLOB_RECURSE ALL_SRC CONFIGURE_DEPENDS
  ${CMAKE_SOURCE_DIR}/dbus-signatures/*.cpp
  ${CMAKE_SOURCE_DIR}/json-builder/*.cpp
  ${CMAKE_SOURCE_DIR}/cli-utils/*.cpp
)

add_executable(ssnd main.cpp ${ALL_SRC})

target_include_directories(ssnd PRIVATE  ${CMAKE_SOURCE_DIR}/dbus-signatures)
target_include_directories(ssnd PRIVATE ${CMAKE_SOURCE_DIR}/json-builder)
target_include_directories(ssnd PRIVATE ${CMAKE_SOURCE_DIR}/cli-utils)

target_link_libraries(ssnd PRIVATE dbus-cxx)