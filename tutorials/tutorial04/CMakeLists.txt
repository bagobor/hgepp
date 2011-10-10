project( TUT04 )
cmake_minimum_required( VERSION 2.8 )

#------ sources ------
set( TUT04_SRC
    hge_tut04.cpp )

#------ additional includes ------
include_directories( ${HGEPP_SOURCE_DIR}/include )
include_directories( ${HGEPP_SOURCE_DIR}/src/libPNG )
include_directories( ${HGEPP_SOURCE_DIR}/src/zlib-1.2.5 )
include_directories( ${DIRECTX_SDK_DIR}/include )

#------ link directories listed ------
link_directories( ${HGEPP_SOURCE_DIR}/linklib )
link_directories( ${DIRECTX_SDK_DIR}/lib ${DIRECTX_SDK_DIR}/lib/x86 )
if( ${HGE_DIRECTX_VER} MATCHES "8" )
    link_libraries( d3d8 d3dx8 )
endif()
if( ${HGE_DIRECTX_VER} MATCHES "9" )
    link_libraries( d3d9 d3dx9 )
endif()
link_libraries( winmm )
link_libraries( hgepp-static zlib )

#------ target -----
add_executable( hge_tut04 WIN32 ${TUT04_SRC} )
add_dependencies( hge_tut04 hgepp-static zlib )

if( MSVC )
    SET_TARGET_PROPERTIES( hge_tut04 PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libci" )
endif()
