# All sub directories.
add_subdirectory(evernote)
add_subdirectory(oauth)
IF(UNIX OR BUILD_FOR_ARM)
add_subdirectory(fcitx)
ENDIF(UNIX OR BUILD_FOR_ARM)



