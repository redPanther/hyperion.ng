INCLUDE (CheckIncludeFiles)

# Define the current source locations
SET(CURRENT_HEADER_DIR ${CMAKE_SOURCE_DIR}/include/grabber)
SET(CURRENT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/libsrc/grabber/amlogic)

FILE ( GLOB AmlogicSOURCES "${CURRENT_HEADER_DIR}/Amlogic*.h"  "${CURRENT_SOURCE_DIR}/*.h"  "${CURRENT_SOURCE_DIR}/*.cpp" )

CHECK_INCLUDE_FILES ("amcodec/amports/amstream.h" HAVE_AML_HEADER)
IF (${HAVE_AML_HEADER})
	ADD_DEFINITIONS( -DHAVE_AML_HEADER )
ENDIF()

add_library(amlogic-grabber ${AmlogicSOURCES} )

target_link_libraries(amlogic-grabber
	hyperion
	${QT_LIBRARIES})
