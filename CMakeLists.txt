cmake_minimum_required(VERSION 3.2)

project(xautoclick)

find_package(X11 REQUIRED)
link_libraries(${X11_LIBRARIES} ${X11_XTest_LIB})

include(CheckIncludeFile)
CHECK_INCLUDE_FILE("unistd.h" HAVE_UNISTD)
if(${HAVE_UNISTD})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_UNISTD_H")
endif()

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

add_executable(cautoclick
    ${SOURCE_COMMON}
    guicommandline.c
)

set(FLTK_SKIP_OPENGL ON)
find_package(FLTK)
if(FLTK_FOUND)
    add_executable(fltkautoclick
        ${SOURCE_COMMON}
        guifltk.cpp
    )
    target_link_libraries(fltkautoclick ${FLTK_LIBRARIES})
endif()

find_package(GTK2 COMPONENTS gtk)
if(GTK2_FOUND)
    add_executable(gautoclick2
        ${SOURCE_COMMON}
        guigtk1.c
    )
    target_include_directories(gautoclick2 PRIVATE ${GTK2_INCLUDE_DIRS})
    target_link_libraries(gautoclick2 ${GTK2_LIBRARIES})
endif()

find_package(Qt4 COMPONENTS QtCore QtGui)
if(QT4_FOUND)
    set(CMAKE_AUTOMOC ON)
    add_executable(qt4autoclick
        ${SOURCE_COMMON}
        guiqt4.cpp
        guiqt4.h
    )
    target_link_libraries(qt4autoclick Qt4::QtCore Qt4::QtGui)
endif()
