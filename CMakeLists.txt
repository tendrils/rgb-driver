cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)

project(rgb-driver C CXX ASM)

pico_sdk_init()

if(NOT DEFINED LIGHT_SDK_PATH)
    if(DEFINED ENV{LIGHT_SDK_PATH})
        set(LIGHT_SDK_PATH "${ENV{LIGHT_SDK_PATH}}")
    else()
        message(FATAL_ERROR "'LIGHT_SDK_PATH' not defined for this build")
    endif()
endif()

get_filename_component(LIGHT_SDK_PATH "${LIGHT_SDK_PATH}" REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
if (NOT EXISTS ${LIGHT_SDK_PATH})
    message(FATAL_ERROR "Directory '${LIGHT_SDK_PATH}' not found")
endif ()

add_subdirectory(${LIGHT_SDK_PATH} light_sdk)

configure_file(${CMAKE_CURRENT_LIST_DIR}/openocd.cfg ${CMAKE_CURRENT_BINARY_DIR}/openocd.cfg COPYONLY)

add_executable(rgb-driver)

target_sources(rgb-driver PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src/main.c)

target_link_libraries(
        rgb-driver PUBLIC
        mod_display_panel
        mod_display_gui
        pico_stdlib
        hardware_pwm
        )

pico_add_extra_outputs(rgb-driver)
