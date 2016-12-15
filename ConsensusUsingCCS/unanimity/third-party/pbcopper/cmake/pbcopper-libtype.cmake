
if(pbcopper_build_shared)
    set(BUILD_SHARED_LIBS    ON)
    set(PBCOPPER_LIB_MODE    SHARED)
    set(PBCOPPER_LIB_SUFFIX  ${CMAKE_SHARED_LIBRARY_SUFFIX})
else()
    set(BUILD_SHARED_LIBS    OFF)
    set(PBCOPPER_LIB_MODE    STATIC)
    set(PBCOPPER_LIB_SUFFIX  ${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()

