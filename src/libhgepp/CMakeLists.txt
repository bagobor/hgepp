project( LIBHGE )
cmake_minimum_required( VERSION 2.8 )

# this is to have DLL version declare __declspec(dllexport) on exported symbols
add_definitions( "-DHGEDLL_EXPORT" )

#------ sources ------
set( LIBHGE_SRC
    ${HGEPP_SOURCE_DIR}/include/hgepp.h
    ${HGEPP_SOURCE_DIR}/include/hgepp/hge_pimpl.h
    ${HGEPP_SOURCE_DIR}/include/hgepp/config.h
    ${HGEPP_SOURCE_DIR}/include/hgepp/gapi.h
    ${HGEPP_SOURCE_DIR}/include/hgepp/color.h
    ${HGEPP_SOURCE_DIR}/include/hgepp/distort.h
    ${HGEPP_SOURCE_DIR}/include/hgepp/font.h
    ${HGEPP_SOURCE_DIR}/include/hgepp/hgegui.h
    ${HGEPP_SOURCE_DIR}/include/hgepp/hgeguictrls.h
    ${HGEPP_SOURCE_DIR}/include/hgepp/particle.h
    ${HGEPP_SOURCE_DIR}/include/hgepp/rect.h
    ${HGEPP_SOURCE_DIR}/include/hgepp/res_manager.h
    ${HGEPP_SOURCE_DIR}/include/hgepp/sprite.h
    ${HGEPP_SOURCE_DIR}/include/hgepp/stringtable.h
    ${HGEPP_SOURCE_DIR}/include/hgepp/vector.h
    ${HGEPP_SOURCE_DIR}/include/hgepp/types.h
    ${HGEPP_SOURCE_DIR}/include/hgepp/res_script.h
    ${HGEPP_SOURCE_DIR}/include/hgepp/hge_impl.h
    hgelogo.cpp
    graphics.cpp
    ini.cpp
    input.cpp
    power.cpp
    random.cpp
    resource.cpp
    sound.cpp
    system.cpp
    timer.cpp
    BASS/bass.h
    animation.cpp
    color.cpp
    distort.cpp
    font.cpp
    hgegui.cpp
    hgeguictrls.cpp
    particle.cpp
    particlemanager.cpp
    rect.cpp
    res_manager.cpp
    sprite.cpp
    stringtable.cpp
    vector.cpp
    res_script.cpp
    res_parser.cpp

    sfmt/SFMT.c sfmt/SFMT.h
    sfmt/SFMT-alti.h sfmt/SFMT-params.h sfmt/SFMT-params11213.h
    sfmt/SFMT-params1279.h sfmt/SFMT-params132049.h sfmt/SFMT-params19937.h
    sfmt/SFMT-params216091.h sfmt/SFMT-params2281.h sfmt/SFMT-params4253.h
    sfmt/SFMT-params44497.h sfmt/SFMT-params607.h sfmt/SFMT-params86243.h sfmt/SFMT-sse2.h
    )

source_group( MersenneTwister REGULAR_EXPRESSION "sfmt/*" )

#------ additional includes ------
include_directories( ${HGEPP_SOURCE_DIR}/include )
include_directories( ${HGEPP_SOURCE_DIR}/src/libPNG )
include_directories( ${HGEPP_SOURCE_DIR}/src/zlib-1.2.5 )
include_directories( ${HGEPP_SOURCE_DIR}/src/zlib-1.2.5/contrib/minizip )
## include_directories( ${HGEPP_SOURCE_DIR}/src )

include_directories( ${DIRECTX_SDK_DIR}/include )

#------ link directories listed ------
link_directories( ${HGEPP_SOURCE_DIR}/linklib )
link_directories( ${DIRECTX_SDK_DIR}/lib ${DIRECTX_SDK_DIR}/lib/x86 ${DIRECTX_SDK_DIR}/lib/x64 )
if( ${HGE_DIRECTX_VER} MATCHES "8" )
    link_libraries( d3d8 d3dx8 )
endif()
if( ${HGE_DIRECTX_VER} MATCHES "9" )
    link_libraries( d3d9 d3dx9 )
endif()
link_libraries( winmm )
link_libraries( zlib )

#------ precompiled headers support (not done for now) ------
## if (MSVC)
##     set( PRECOMP_FILENAME "precompiled_headers" )
##     set_source_files_properties( ${PRECOMP_FILENAME}.cpp PROPERTIES COMPILE_FLAGS "/Yc${PRECOMP_FILENAME}.h" )
##     foreach( src_file ${LIBHGE_SRC} )
##         set_source_files_properties( ${src_file} PROPERTIES COMPILE_FLAGS "/Yu${PRECOMP_FILENAME}.h" )
##     endforeach( src_file ${LIBHGE_SRC} )
##     list( APPEND LIBHGE_SRC ${PRECOMP_FILENAME}.cpp ${PRECOMP_FILENAME}.h )
## endif (MSVC)

#------ target (for creating HGE.DLL) -----
# add_library( hgepp SHARED ${LIBHGE_SRC} )
# add_dependencies( hgepp hgezlib )

# if( MSVC )
#     SET_TARGET_PROPERTIES( hgepp PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libci" )
# endif()

#------ target (for creating static HGE.LIB) -----
add_library( hgepp-static STATIC ${LIBHGE_SRC} )
