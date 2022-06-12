add_executable(dump_monster
main.cpp
Monster_generated.h
)

add_library(PInvoke SHARED
PInvoke.h
PInvoke.cpp
Monster_generated.h
)