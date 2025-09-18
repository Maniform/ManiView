cmake_minimum_required(VERSION 3.19)
project(ManiView LANGUAGES CXX VERSION 1.1)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

set(APP_NAME ManiView)
set(EXECUTABLE_NAME ${APP_NAME})
set(APP_ICON_MAC ${CMAKE_SOURCE_DIR}/mac/icon.icns)
set(CMAKE_INCLUDE_CURRENT_DIR True)

configure_file(
	${CMAKE_SOURCE_DIR}/mac/Info.plist.in
	${CMAKE_BINARY_DIR}/Info.plist @ONLY
)
set_source_files_properties(${APP_ICON_MAC} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

qt_standard_project_setup()

qt_add_executable(${APP_NAME}
    WIN32 MACOSX_BUNDLE
	${APP_ICON_MAC}
    main.cpp
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui
		ViewerApplication.cpp
		ViewerApplication.h
		GraphicsView.cpp
		GraphicsView.h
)

set_target_properties(${APP_NAME} PROPERTIES
		MACOSX_BUNDLE TRUE
		MACOSX_BUNDLE_INFO_PLIST ${CMAKE_BINARY_DIR}/Info.plist
		MACOSX_BUNDLE_ICON_FILE "icon.icns"
)

target_link_libraries(${APP_NAME}
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS ${APP_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ${APP_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})