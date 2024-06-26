set(SOURCE_FILES
    main.cpp
    AudioService.cpp
    GameEngine.cpp
    GameScreen.cpp
    GameSelectScreen.cpp
    GeoSketchLogger.cpp
    MainMenuScreen.cpp
    ResourceManager.cpp
    Screen.cpp
    SettingsScreen.cpp
    SFMLUtils.cpp
    SplashScreen.cpp
    TileManager.cpp
    OSUtils.cpp
    Data/RegionDatabase.cpp
    GameControllers/GameController.cpp
    GameControllers/GameStartController.cpp
    GameControllers/TimedGameController.cpp
)

set(HEADER_FILES
    core.h
    AudioService.h
    GameEngine.h
    GameScreen.h
    GameSelectScreen.h
    GeoSketchLogger.h
    MainMenuScreen.h
    OSUtils.h
    PollResult.h
    ResourceManager.h
    Screen.h
    SettingsScreen.h
    SFMLUtils.h
    SplashScreen.h
    TileManager.h
    Data/RegionDatabase.h
    GameControllers/GameController.h
    GameControllers/GameStartController.h
    GameControllers/TimedGameController.h
)

# if(WIN32)
#     set(GUI_TYPE WIN32)
#     set(RESOURCE_FILES 
#         "${CMAKE_SOURCE_DIR}/tools/windows/ttvg.rc")

#     # can this be safely removed? 
#     set(CPACK_NSIS_MODIFY_PATH "ON")
# endif()

 if(APPLE)
     SET(GUI_TYPE MACOSX_BUNDLE)
#     set(RESOURCE_FILES
#         "${CMAKE_SOURCE_DIR}/tools/macos/ttvg.icns")
 endif()

SET_SOURCE_FILES_PROPERTIES(
    ${RESOURCE_FILES}
    PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
)

add_executable(GeoSketch ${GUI_TYPE}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${RESOURCE_FILES}
)


target_link_libraries(GeoSketch
    PUBLIC
        ${CONAN_LIBS}
        ${SFML_Libraries}
        ${CONAN_LIBS_FRAMEWORKS_LUADIST}
    PRIVATE 
        # TGUI::TGUI
        # ${TGUI_LIBRARY}
        tgui
)

target_link_directories(GeoSketch
PUBLIC 
    ${CONAN_LIB_DIRS}
)

target_include_directories(GeoSketch
PUBLIC 
    ${CONAN_INCLUDE_DIRS}
)

# if(WIN32)
#     set_target_properties(tooter PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
#     set_target_properties(tooter PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
#     set_target_properties(tooter PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
#     set_target_properties(tooter PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
# endif()

# ### INSTALL

# # Destination paths below are relative to ${CMAKE_INSTALL_PREFIX}

# if (APPLE)
#     configure_file("${CMAKE_SOURCE_DIR}/tools/macos/Info.plist.in" 
#         "${CMAKE_BINARY_DIR}/Info.plist" @ONLY)

#     install(
#         TARGETS tooter 
#         COMPONENT tooter
#         RUNTIME DESTINATION bin
#         LIBRARY DESTINATION lib
#         BUNDLE DESTINATION .
#     )

#     install(
#         DIRECTORY ${CMAKE_SOURCE_DIR}/resources/
#         DESTINATION ./tooter.app/Contents/Resources
#         COMPONENT tooter
#         PATTERN "src" EXCLUDE
#     )

# endif(APPLE)

# if (WIN32)

#     install(
#         TARGETS tooter 
#         COMPONENT tooter
#         RUNTIME DESTINATION bin
#         LIBRARY DESTINATION lib
#         BUNDLE DESTINATION .
#     )

#     set(CMAKE_INSTALL_UCRT_LIBRARIES FALSE)
#     include(InstallRequiredSystemLibraries)
#     install(
#         PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
#         DESTINATION bin
#         COMPONENT tooter
#     )

#     install(
#         DIRECTORY ${CMAKE_SOURCE_DIR}/resources/
#         DESTINATION bin/resources
#         COMPONENT tooter
#         PATTERN "src" EXCLUDE
#     )

#     install(
#         FILES "${CMAKE_SOURCE_DIR}/tools/windows/oalinst.exe" 
#         DESTINATION redist
#         COMPONENT tooter
#     )

# endif()

# foreach(_CONFIG_FILE_ TTVGCPackConfig.cmake TTVGPackage.cmake)
#     configure_file("CPack/${_CONFIG_FILE_}.in" "${CMAKE_BINARY_DIR}/${_CONFIG_FILE_}" @ONLY)
# endforeach()

# set(CPACK_PACKAGE_CONTACT "Support <max@amb.dog>")

# include(CPack)

