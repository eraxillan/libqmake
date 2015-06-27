#ifndef __LIBQMAKEGLOBAL
#define __LIBQMAKEGLOBAL

#include <QtCore/QtGlobal>

#if defined(QMAKE_LIBRARY)
#  define QMAKE_LIBRARY_EXPORT Q_DECL_EXPORT
#else
#  define QMAKE_LIBRARY_EXPORT Q_DECL_IMPORT
#endif

#endif // __LIBQMAKEGLOBAL
