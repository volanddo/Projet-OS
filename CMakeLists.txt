cmake_minimum_required(VERSION 3.19)
project(conception_os C)

set(CMAKE_C_STANDARD 11)

include_directories(src_de_base)

add_executable(conception_os
        src_de_base/clock_print.c
        src_de_base/clock_print.h
        src_de_base/console.c
        src_de_base/console.h
        src_de_base/cpu.h
        src_de_base/ctype.h
        src_de_base/debug.h
        src_de_base/debugger.c
        src_de_base/div64.c
        src_de_base/div64.h
        src_de_base/doprnt.c
        src_de_base/doprnt.h
        src_de_base/empty.c
        src_de_base/inttypes.h
        src_de_base/panic.c
        src_de_base/printf.c
        src_de_base/processor_structs.c
        src_de_base/processor_structs.h
        src_de_base/processus.c
        src_de_base/processus.h
        src_de_base/segment.h
        src_de_base/sprintf.c
        src_de_base/start.c
        src_de_base/start.h
        src_de_base/stdarg.h
        src_de_base/stdbool.h
        src_de_base/stddef.h
        src_de_base/stdio.h
        src_de_base/stdlib.h
        src_de_base/string.c
        src_de_base/string.h
        src_de_base/strtol.c
        src_de_base/strtoul.c
        src_de_base/tinyalloc.c
        src_de_base/tinyalloc.h
        src_de_base/traitants_.c
        src_de_base/traitants_.h
        src_de_base/types.h)
