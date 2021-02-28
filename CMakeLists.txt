cmake_minimum_required(VERSION 3.17)
project(Engineering_calc C)

set(CMAKE_C_STANDARD 11)

add_executable(Engineering_calc
        main.c
        linear_map.h linear_map.c
        pair_string_double.c pair_string_double.h
        test.c test.h
        stack_double.c stack_double.h
        calculation.c calculation.h
        rpn_creating.c rpn_creating.h
        constants.h)

target_link_libraries(Engineering_calc PRIVATE m)