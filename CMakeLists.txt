cmake_minimum_required(VERSION 3.6)

project(xautoclick VERSION 0.34)

include(CheckIncludeFile)
check_include_file("unistd.h" HAVE_UNISTD)
if(${HAVE_UNISTD})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_UNISTD_H")
    # TODO add_compile_definitions(HAVE_UNISTD_H)
endif()

find_package(X11)
if(X11_FOUND)
add_library(x11clicker STATIC
    x11clicker.c
    x11clicker.h
)
target_link_libraries(x11clicker ${X11_LIBRARIES} ${X11_XTest_LIB})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_X11_CLICKER")
list(APPEND CLICKERS x11clicker)
endif()

find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
pkg_check_modules(LIBEVDEV libevdev)
if(LIBEVDEV_FOUND)
add_library(udevclicker STATIC
    udevclicker.c
    udevclicker.h
)
target_include_directories(udevclicker PRIVATE ${LIBEVDEV_INCLUDE_DIRS})
target_link_libraries(udevclicker ${LIBEVDEV_LIBRARIES})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_UDEV_CLICKER")
list(APPEND CLICKERS udevclicker)
endif(LIBEVDEV_FOUND)
endif(PKG_CONFIG_FOUND)

if(NOT clickers)
    message(FATAL_ERROR "No clicker engines could be built.")
endif()

add_library(clicker STATIC
    clicker.c
    clicker.h
    gui.c
    gui.h
    main.c
    main.h
    options.c
    options.h
    osdep.h
)
target_link_libraries(clicker ${CLICKERS})

add_executable(aautoclick
    guiascii.c
)
target_link_libraries(aautoclick clicker)
install(TARGETS aautoclick DESTINATION bin)

add_executable(cautoclick
    guicommandline.c
)
target_link_libraries(cautoclick clicker)
install(TARGETS cautoclick DESTINATION bin)

set(FLTK_SKIP_OPENGL ON)
find_package(FLTK)
if(FLTK_FOUND)
    add_executable(fltkautoclick
        guifltk.cpp
    )
    target_link_libraries(fltkautoclick clicker ${FLTK_LIBRARIES})
    install(TARGETS fltkautoclick DESTINATION bin)
endif()

pkg_check_modules(GTK3 gtk+-3.0)
if(GTK3_FOUND)
    add_executable(gautoclick3
        guigtk3.c
    )
    target_include_directories(gautoclick3 PRIVATE ${GTK3_INCLUDE_DIRS})
    target_link_libraries(gautoclick3 clicker ${GTK3_LIBRARIES})
    install(TARGETS gautoclick3 DESTINATION bin)
    install(FILES xautoclick.desktop DESTINATION share/applications)
    install(FILES xautoclick.svg DESTINATION share/icons/hicolor/scalable/apps)
endif()

find_package(Qt5 COMPONENTS Widgets)
if(Qt5_FOUND)
    set(CMAKE_AUTOMOC ON)
    add_executable(qt5autoclick
        clickwidget.cpp
        clickwidget.h
        guiqt.cpp
    )
    target_link_libraries(qt5autoclick clicker Qt5::Widgets)
    install(TARGETS qt5autoclick DESTINATION bin)
endif()

install(FILES xautoclick.1 DESTINATION share/man/man1)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "none")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)

include(CPack)
