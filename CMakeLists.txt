cmake_minimum_required(VERSION 3.25)
project(pathtracer LANGUAGES CXX)

# Language level. EXTENSIONS OFF means -std=c++20 rather than -std=gnu++20,
# so we compile against the standard and not GCC's dialect of it.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Emit compile_commands.json in the build directory. clangd reads this to learn
# how each file is compiled; without it the editor cannot resolve includes.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(PATHTRACER_WERROR "Treat compiler warnings as errors" ON)
option(PATHTRACER_SANITIZE "Build with AddressSanitizer + UndefinedBehaviorSanitizer" OFF)

add_executable(pathtracer
    src/main.cpp
)

target_compile_options(pathtracer PRIVATE
    -Wall            # the common mistakes
    -Wextra          # the less common ones
    -Wpedantic       # anything outside strict ISO C++
    -Wshadow         # a local that hides an outer name
    -Wconversion     # implicit narrowing, e.g. double -> int
    -Wsign-conversion
)

if(PATHTRACER_WERROR)
    target_compile_options(pathtracer PRIVATE -Werror)
endif()

# Sanitizers are codegen, not a separate tool: the flag must reach both the
# compiler (to insert the checks) and the linker (to pull in the runtime).
if(PATHTRACER_SANITIZE)
    target_compile_options(pathtracer PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer   # keeps stack traces readable
    )
    target_link_options(pathtracer PRIVATE -fsanitize=address,undefined)
endif()
