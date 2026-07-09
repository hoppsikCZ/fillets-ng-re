cmake_minimum_required(VERSION 3.12)
project(fillets VERSION 0.1.0)

include(ProcessorCount)
ProcessorCount(num_jobs)

if(num_jobs EQUAL 0)
    set(num_jobs 1)
endif()

message("Number of available processors: ${num_jobs}")
set(CMAKE_BUILD_PARALLEL_LEVEL ${num_jobs})

include(CTest)
include(CPack)
enable_testing()

file(GLOB_RECURSE fillets_SRC CONFIGURE_DEPENDS "src/*.cpp")
list(FILTER fillets_SRC EXCLUDE REGEX ".*FsPath_boost\.cpp$")
add_executable(fillets ${fillets_SRC})

file(GLOB fillets_INCLUDE_DIRS LIST_DIRECTORIES true "src/*/")

foreach(dir ${fillets_INCLUDE_DIRS})
    target_include_directories(fillets BEFORE PRIVATE ${dir})
endforeach()

if(MSVC)
    target_compile_options(fillets PRIVATE "/MP${num_jobs}")
endif()

if(UNIX)
    target_compile_definitions(fillets PRIVATE PLATFORM_UNIX)

    find_package(PkgConfig REQUIRED)

    pkg_check_modules(LUA lua QUIET)
    if(NOT LUA_FOUND)
        pkg_check_modules(LUA REQUIRED lua5.4)
    endif()

    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
    pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
    pkg_check_modules(SDL2_GFX REQUIRED SDL2_gfx)
    pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)

    if(APPLE)
        list(FILTER SDL2_LIBRARIES EXCLUDE REGEX "SDL2main")
        target_link_directories(fillets PRIVATE ${SDL2_LIBRARY_DIRS})
    endif()

    target_include_directories(fillets PRIVATE
        ${LUA_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_GFX_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
    )
    target_link_libraries(fillets PRIVATE
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_GFX_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${LUA_LIBRARIES}
    )

elseif(WIN32)
    target_compile_definitions(fillets PRIVATE PLATFORM_WINDOWS)

    find_package(Lua REQUIRED)
    target_include_directories(fillets PRIVATE ${LUA_INCLUDE_DIR})

    find_package(SDL2 CONFIG REQUIRED)
    find_package(sdl2-gfx CONFIG REQUIRED)
    find_package(SDL2_image CONFIG REQUIRED)
    find_package(SDL2_mixer CONFIG REQUIRED)
    find_package(SDL2_ttf CONFIG REQUIRED)

    target_link_libraries(fillets PRIVATE
        SDL2::SDL2_gfx
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
        $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
        $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
        ${LUA_LIBRARIES}
    )
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})