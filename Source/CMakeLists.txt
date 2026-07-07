add_executable(Cosmicstone
    entry.cpp
    PlayerInkAction.cpp
)

set(WUT $ENV{DEVKITPRO}/wut)

target_include_directories(Cosmicstone PRIVATE
    ${WUT}/include
    ${CMAKE_SOURCE_DIR}/Include
    ${CMAKE_SOURCE_DIR}/Gambit-Reverse/Include
)

target_link_directories(Cosmicstone PRIVATE
    "${WUT}/lib"
    Gambit
)

target_link_libraries(Cosmicstone PRIVATE
    wut
)

wut_add_exports(Cosmicstone exports.def)
wut_create_rpl(Cosmicstone)
