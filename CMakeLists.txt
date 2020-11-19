cmake_minimum_required(VERSION 3.17)
project(test_decklink_8k_pro)

set(CMAKE_CXX_STANDARD 14)

set(LDFLAGS -lm -ldl -lpthread)

set(BLACKMAGIC_SDK_DIR ../include/)
MESSAGE( "Searching for Blackmagic SDK in: ${BLACKMAGIC_SDK_DIR}" )
find_path( BLACKMAGIC_DECKLINK_INCLUDE_DIR "DeckLinkAPI.h"
        PATHS ${BLACKMAGIC_SDK_DIR} )
if( NOT BLACKMAGIC_DECKLINK_INCLUDE_DIR )
    MESSAGE( FATAL_ERROR "Cannot find DeckLinkAPI.h, please give a path to the Blackmagic SDK by setting BLACKMAGIC_SDK_DIR" )
endif()
MESSAGE( "Including Blackmagic SDK from: ${BLACKMAGIC_DECKLINK_INCLUDE_DIR}" )

include_directories(
        ../include/
        ${BLACKMAGIC_DECKLINK_INCLUDE_DIR}
)





add_executable(test_decklink_8k_pro main.cpp CaptureDisplay3D.cpp CaptureDisplay3D.h)
target_link_libraries(test_decklink_8k_pro  ${LDFLAGS})



