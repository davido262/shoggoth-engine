cmake_minimum_required(VERSION 2.6)
project(shoggoth-engine)

# Compiler flags
if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Weffc++ -Wconversion -Wswitch-default -Wswitch-enum -Wshadow -Wmissing-include-dirs")
#     set(CMAKE_EXE_LINKER_FLAGS "-lSDL")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})

# installation directories
set(INSTALL_DIR_INCLUDE ${CMAKE_INSTALL_PREFIX}/include)
set(INSTALL_DIR_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib)


include_directories(include)
add_subdirectory(src)
add_subdirectory(include)
