cmake_minimum_required(VERSION 3.2)

project(xautoclick)

find_package(X11 REQUIRED)

include(CheckIncludeFile)
CHECK_INCLUDE_FILE("unistd.h" HAVE_UNISTD)
if(${HAVE_UNISTD})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_UNISTD_H")
endif()

add_library(x11clicker STATIC
    x11clicker.c
    x11clicker.h
)
target_link_libraries(x11clicker ${X11_LIBRARIES} ${X11_XTest_LIB})

set(SOURCE_COMMON
    main.c
    main.h
    osdep.c
    osdep.h
)

add_executable(aautoclick
    ${SOURCE_COMMON}
    guiascii.c
)
target_link_libraries(aautoclick x11clicker)
install(TARGETS aautoclick DESTINATION bin)

add_executable(cautoclick
    ${SOURCE_COMMON}
    guicommandline.c
)
target_link_libraries(cautoclick x11clicker)
install(TARGETS cautoclick DESTINATION bin)

set(FLTK_SKIP_OPENGL ON)
find_package(FLTK)
if(FLTK_FOUND)
    add_executable(fltkautoclick
        ${SOURCE_COMMON}
        guifltk.cpp
    )
    target_link_libraries(fltkautoclick x11clicker ${FLTK_LIBRARIES})
    install(TARGETS fltkautoclick DESTINATION bin)
endif()

find_package(PkgConfig)
if(PkgConfig_FOUND)
    PKG_CHECK_MODULES(GTK3 gtk+-3.0)
    if(GTK3_FOUND)
        add_executable(gautoclick3
            ${SOURCE_COMMON}
            guigtk3.c
        )
        target_include_directories(gautoclick3 PRIVATE ${GTK3_INCLUDE_DIRS})
        target_link_libraries(gautoclick3 x11clicker ${GTK3_LIBRARIES})
        install(TARGETS gautoclick3 DESTINATION bin)
        install(FILES xautoclick.desktop DESTINATION share/applications)
        install(FILES xautoclick.svg DESTINATION share/icons/hicolor/scalable/apps)
    endif()
endif()

find_package(Qt4 COMPONENTS QtCore QtGui)
if(QT4_FOUND)
    set(CMAKE_AUTOMOC ON)
    add_executable(qt4autoclick
        ${SOURCE_COMMON}
        clickwidget.cpp
        clickwidget.h
        guiqt4.cpp
    )
    target_link_libraries(qt4autoclick x11clicker Qt4::QtCore Qt4::QtGui)
    install(TARGETS qt4autoclick DESTINATION bin)
endif()

install(FILES xautoclick.1 DESTINATION share/man/man1)
