TARGET = libqmake-1.0
TEMPLATE = lib

DEFINES += QT_BUILD_QMAKE_LIBRARY

# Build configuration
CONFIG += qt thread warn_on plugin

#win32-msvc*: DEFINES += _CRT_SECURE_NO_WARNINGS

# Required for correct RTTI work under Unix
unix:QMAKE_LFLAGS += -Wl,-E

# Build mode (release by default)
buildmode = release
CONFIG(debug, debug|release):buildmode = debug

# Setup build directories
DESTDIR     = $${buildmode}
UI_DIR      = $${buildmode}
OBJECTS_DIR = $${buildmode}
MOC_DIR     = $${buildmode}

# Setup installation
win32 {
	isEmpty(PLUGIN_INSTALL_PATH):PLUGIN_INSTALL_PATH = /bin/plugins
}
else {
	isEmpty(PLUGIN_INSTALL_PATH):PLUGIN_INSTALL_PATH = /usr/lib/plugins
}

# Library install path
target.path = $${PLUGIN_INSTALL_PATH}

# Install
INSTALLS += target

SOURCES += \
    src/option.cpp \
    src/project.cpp \
    src/property.cpp \
    src/CQmakeProjectParser.cpp

HEADERS += \
    src/cachekeys.h \
    src/option.h \
    src/project.h \
    src/property.h \
	include/CQmakeProjectParser.h
