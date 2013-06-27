ENABLE_QT()

# Header files.
FILE(GLOB HDRS *.h)
QT4_WRAP_CPP(MOC_SRCS ${HDRS})

# Source files.
FILE(GLOB SRCS *.cpp)
SET(SRCS ${HDRS} ${SRCS} ${MOC_SRCS})

ADD_LIBRARY(evernote ${SRCS})

TARGET_LINK_LIBRARIES(evernote
  database
  evernote_api
  ${THRIFT_LIBRARIES}
  ${Boost_LIBRARIES}
  ${QT_LIBRARIES}
  ${ADD_LIB})

ADD_EXECUTABLE(test_evernote_export_note test/test_export_note.cpp)
TARGET_LINK_LIBRARIES(test_evernote_export_note
 evernote)

ADD_EXECUTABLE(test_evernote_prepare_content test/test_prepare_content.cpp)
TARGET_LINK_LIBRARIES(test_evernote_prepare_content
 evernote)

ADD_EXECUTABLE(test_evernote_import_xml test/test_import_xml.cpp)
TARGET_LINK_LIBRARIES(test_evernote_import_xml
 evernote)