cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

project(libfreelica)

include(FindPkgConfig)

pkg_check_modules(LIBNFC REQUIRED libnfc)
include_directories(${LIBNFC_INCLUDE_DIRS})
link_directories(${LIBNFC_LIBRARY_DIRS})
add_definitions(${LIBNFC_DEFINITIONS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE libfreelica_SOURCES src/*.cpp)
file(GLOB libfreelica_HEADERS include/libfreelica/*.h include/libfreelica/*.hpp)
file(GLOB libfreelica_COMMANDS_HEADERS include/libfreelica/commands/*.h include/libfreelica/commands/*.hpp)

add_library(freelica SHARED ${libfreelica_SOURCES})
target_link_libraries(freelica ${LIBNFC_LIBRARIES})

configure_file(libfreelica.pc.in libfreelica.pc @ONLY)

install(TARGETS freelica LIBRARY DESTINATION lib)
install(FILES ${libfreelica_HEADERS} DESTINATION include/libfreelica)
install(FILES ${libfreelica_COMMANDS_HEADERS} DESTINATION include/libfreelica/commands)
install(FILES ${PROJECT_BINARY_DIR}/libfreelica.pc DESTINATION lib/pkgconfig)