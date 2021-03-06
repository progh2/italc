FILE(GLOB ic_INCLUDES
						${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
						${CMAKE_CURRENT_SOURCE_DIR}/include/Ipc/*.h
						${CMAKE_CURRENT_SOURCE_DIR}/include/Configuration/*.h)
FILE(GLOB ic_SOURCES
						${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/src/Ipc/*.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/src/Configuration/*.cpp)
FILE(GLOB ic_UI ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/*.ui)

SET(LIBVNCCLIENT_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/../ica/x11/common/minilzo.c
${CMAKE_CURRENT_SOURCE_DIR}/../ica/x11/common/turbojpeg.c
${CMAKE_CURRENT_SOURCE_DIR}/../ica/x11/libvncclient/cursor.c
${CMAKE_CURRENT_SOURCE_DIR}/../ica/x11/libvncclient/listen.c
${CMAKE_CURRENT_SOURCE_DIR}/../ica/x11/libvncclient/rfbproto.c
${CMAKE_CURRENT_SOURCE_DIR}/../ica/x11/libvncclient/sockets.c
${CMAKE_CURRENT_SOURCE_DIR}/../ica/x11/libvncclient/tls_none.c
${CMAKE_CURRENT_SOURCE_DIR}/../ica/x11/libvncclient/vncviewer.c)


SET(QRC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/ItalcCore.qrc)
QT4_WRAP_CPP(ic_MOC_out ${ic_INCLUDES})
QT4_WRAP_UI(ic_UIC_out ${ic_UI})
QT4_ADD_RESOURCES(ic_RCC_out ${QRC_FILE})
IF(WIN32)
	QT4_ADD_RESOURCES(ic_qt_qm_out ${CMAKE_CURRENT_BINARY_DIR}/qt_qm.qrc)
ENDIF()
QT4_TRANSLATIONS_FOR_QRC(${QRC_FILE} ${ic_SOURCES} ${ic_INCLUDES})

ADD_DEFINITIONS(-DBUILD_ICA -DBUILD_LIBRARY)
IF(ITALC_BUILD_WIN32)
	LINK_LIBRARIES(-lws2_32 -lwtsapi32 -lnetapi32 -luserenv -ladvapi32)
	SET(CMAKE_MODULE_LINKER_FLAGS -Wl,-export-all-symbols)
ENDIF(ITALC_BUILD_WIN32)

INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/../ica/x11/common/)
ADD_LIBRARY(ItalcCore SHARED ${ic_SOURCES} ${ic_INCLUDES} ${ic_MOC_out} ${ic_UIC_out} ${ic_RCC_out} ${ic_qt_qm_out} ${LIBVNCCLIENT_SOURCES})

IF(ITALC_BUILD_WIN32)
	SET_TARGET_PROPERTIES(ItalcCore PROPERTIES PREFIX "")
	SET_SOURCE_FILES_PROPERTIES(src/Inject.cpp PROPERTIES COMPILE_FLAGS "-O0")
	INSTALL(TARGETS ItalcCore RUNTIME DESTINATION ${LIB_DIR})
ELSE()
	INSTALL(TARGETS ItalcCore LIBRARY DESTINATION ${LIB_DIR})
ENDIF(ITALC_BUILD_WIN32)

