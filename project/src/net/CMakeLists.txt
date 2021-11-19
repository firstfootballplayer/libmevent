#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
add_compile_options(-std=c++11 -Wall -g) 


include_directories (../../include)
include_directories (../../include/libmevent/base)
include_directories (../../include/libmevent/net)

aux_source_directory(. DIR_LIB_SRCS)#生成链接库

find_library(TESTFUNC_LIB pthread HINTS /usr/lib)

link_libraries(${TESTFUNC_LIB})

add_library (libmevent_net SHARED  ${DIR_LIB_SRCS})

#target_link_libraries(libmevent_net libmevent_base)