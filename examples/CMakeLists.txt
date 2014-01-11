cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

project(libfreelica-examples)

include(FindPkgConfig)

pkg_check_modules(LIBNFC REQUIRED libnfc)
include_directories(${LIBNFC_INCLUDE_DIRS})
link_directories(${LIBNFC_LIBRARY_DIRS})
add_definitions(${LIBNFCL_DEFINITIONS})

pkg_check_modules(LIBFREELICA REQUIRED libfreelica)
include_directories(${LIBFREELICA_INCLUDE_DIRS})
link_directories(${LIBFREELICA_LIBRARY_DIRS})
add_definitions(${LIBFREELICAL_DEFINITIONS})

add_executable(libfreelica-test libfreelica_test.cpp)
target_link_libraries(libfreelica-test ${LIBNFC_LIBRARIES} ${LIBFREELICA_LIBRARIES})

add_executable(octopus-balance octopus_balance.cpp)
target_link_libraries(octopus-balance ${LIBNFC_LIBRARIES} ${LIBFREELICA_LIBRARIES})
