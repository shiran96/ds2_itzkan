cmake_minimum_required(VERSION 3.22)
project(ds_2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ds_2
        company.cpp
        company.h
        ds_manager.cpp
        ds_manager.h
        dynamic_hash_table.cpp
        dynamic_hash_table.h
        employee.cpp
        employee.h
        exceptions.h
        library2.cpp
        library2.h
        linked_list.h
        main2.cpp
        rank_tree.cpp
        rank_tree.h
        unionFind.cpp
        unionFind.h)
