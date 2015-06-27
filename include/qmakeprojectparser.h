#ifndef __QMAKEPROJECTPARSER
#define __QMAKEPROJECTPARSER

#include <include/libqmakeglobal.h>

extern "C" {
/**
 * @brief Absolute paths to qmake project C/C++ headers
 * @note Function allocates memory internally for the @a _sources array using @a malloc;
 *       caller is responsible to @a free() it
 */
bool QMAKE_LIBRARY_EXPORT qmakeProjectHeaders (const char* _file_name, char** _headers);

/**
 * @brief Absolute paths to project C/C++ sources
 * @note Function allocates memory internally for the @a _sources array using @a malloc;
 *       caller is responsible to @a free() it
 */
bool QMAKE_LIBRARY_EXPORT qmakeProjectSources (const char* _file_name, char** _sources);
}

#endif // __QMAKEPROJECTPARSER
