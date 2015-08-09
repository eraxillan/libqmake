QT       += core
QT       -= gui

TARGET = example
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

BUILD_MODE = debug

LIBQMAKE_DIR = $$PWD/..
LIBQMAKE_INC = $$LIBQMAKE_DIR/include
LIBQMAKE_LIB = $$LIBQMAKE_DIR/$$BUILD_MODE

!build_pass {
	message( Detecting libqmake library... )
	message( $$LIBQMAKE_DIR )
	message( $$LIBQMAKE_INC )
	message( $$LIBQMAKE_LIB )
	!exists( $$LIBQMAKE_DIR ): error ( libqmake library root directory not found! )
	!exists( $$LIBQMAKE_INC ): error ( libqmake include directory not found! )
	!exists( $$LIBQMAKE_LIB ): error ( libqmake library directory not found! )
}

INCLUDEPATH += $$LIBQMAKE_DIR/include
LIBS += -L$$LIBQMAKE_DIR/debug -llibqmake-1.0

SOURCES += main.cpp
