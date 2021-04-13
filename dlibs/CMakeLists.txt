# 使用特定的源码为项目增加lib
add_library(dlibs SHARED mysqrt.cpp)  # 生成动态库 dlibs.so
# add_library(dlibs STATIC mysqrt.cpp)

install (TARGETS dlibs DESTINATION bin)
install (FILES mysqrt.h DESTINATION include)