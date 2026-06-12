cmake_minimum_required(VERSION 3.22.0)

set(ZEPHYR_EXTRA_MODULES "${CMAKE_SOURCE_DIR}/modules/led_indicator")

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(pm_shell)

target_sources(app PRIVATE src/main.c)
