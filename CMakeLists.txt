cmake_minimum_required(VERSION 3.5)

project(birthdays LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
  if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
    if (MSVC)
        # warning level 4 and all warnings as errors
        add_compile_options(/O2)
    else()
        # lots of warnings and all warnings as errors
        add_compile_options(-O3)
    endif()
  endif()
endif()

add_executable(birthdays birthdays.cpp)
