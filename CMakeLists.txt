﻿# CMakeList.txt : CMake project for Limbo, include source and define
# project specific logic here.

cmake_minimum_required(VERSION 3.8)

# -------------------------------------------------------
# Force CMake to use gcc and g++ instead of MSVC/Clang/etc.
# (This must appear BEFORE the project() command)
# -------------------------------------------------------
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
endif()

project("Limbo")

# If using MSVC hot reload (Edit and Continue), skip or remove these lines
# since this is for Visual Studio only:
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
    "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")

# Add source to this project's executable.
add_executable(Limbo 
"Limbo.cpp" 
"Limbo.h"
"src/core/logger.cpp"
"src/core/logger.h"
"src/core/macros.h"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Limbo PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
