project( LIBHGE )
cmake_minimum_required( VERSION 2.8 )

# this is to have DLL version declare __declspec(dllexport) on exported symbols
add_definitions( "-DHGEDLL_EXPORT" )

#------ sources ------
set( LIBHGE_SRC
    demo.cpp
    graphics.cpp
    ini.cpp
    input.cpp
    power.cpp
    random.cpp
    resource.cpp
    sound.cpp
    system.cpp
    timer.cpp
    hge_impl.h
    BASS/bass.h
    ${HGEPP_SOURCE_DIR}/include/hge.h
    ${HGEPP_SOURCE_DIR}/include/hgeanim.h
    ${HGEPP_SOURCE_DIR}/include/hgecolor.h
    ${HGEPP_SOURCE_DIR}/include/hgedistort.h
    ${HGEPP_SOURCE_DIR}/include/hgefont.h
    ${HGEPP_SOURCE_DIR}/include/hgegui.h
    ${HGEPP_SOURCE_DIR}/include/hgeguictrls.h
    ${HGEPP_SOURCE_DIR}/include/hgeparticle.h
    ${HGEPP_SOURCE_DIR}/include/hgerect.h
    ${HGEPP_SOURCE_DIR}/include/hgeresource.h
    ${HGEPP_SOURCE_DIR}/include/hgesprite.h
    ${HGEPP_SOURCE_DIR}/include/hgestrings.h
    ${HGEPP_SOURCE_DIR}/include/hgevector.h
    hgeanim.cpp
    hgecolor.cpp
    hgedistort.cpp
    hgefont.cpp
    hgegui.cpp
    hgeguictrls.cpp
    hgeparticle.cpp
    hgepmanager.cpp
    hgerect.cpp
    hgeresource.cpp
    hgesprite.cpp
    hgestrings.cpp
    hgevector.cpp
    parser.cpp
    resources.cpp
    parser.h
    resources.h
    )

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
add_library( hgepp SHARED ${LIBHGE_SRC} )
add_dependencies( hgepp hgezlib )

if( MSVC )
    SET_TARGET_PROPERTIES( hgepp PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libci" )
endif()

#------ target (for creating static HGE.LIB) -----
add_library( hgepp-static STATIC ${LIBHGE_SRC} )
