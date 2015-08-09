/**
 * @file
 * @brief Qt qmake facade interface
 * @author Alexander Kamyshnikov <axill777@gmail.com>
 * (C) 2014
 */

#ifndef __CQMAKEPROJECTPARSER_H
#define __CQMAKEPROJECTPARSER_H

#include <libqmakeglobal.h>

#include <QSet>
#include <QStringList>


/**
 * @brief Qt qmake project file parser
 *
 * Parser based on original qmake code and support all pro-file features
 */
class QMAKE_LIBRARY_EXPORT CQmakeProjectParser
{
	/**
	 * @brief qmake project file path
	 * @note Only local files are supported
	 */
	QString m_file_name;

	/**
	 * @brief The last critical error (when parser stops parsing)
	 */
	QString m_last_error;

	/**
	 * @brief Absolute paths to project C/C++ headers
	 */
	QSet<QString> m_headers;

	/**
	 * @brief Absolute paths to project C/C++ sources
	 */
	QSet<QString> m_sources;

	void init ();
	void parse ();
	void collectSubdirs (const QString& _base_project, QStringList& _subprojects);

public:
	/**
	 * @brief Create parser object
	 * @param _file_name File name as URL with file scheme ("file://test")
	 */
	CQmakeProjectParser (const QString& _file_name);
	~CQmakeProjectParser ();

	/**
	 * @brief Absolute paths to qmake project C/C++ headers
	 */
	QSet<QString> headers () const;

	/**
	 * @brief Absolute paths to qmake project C/C++ sources
	 */
	QSet<QString> sources () const;

	/**
	 * @brief The last critical error (when parser stops parsing)
	 */
	QString lastError () const;
};


#endif // __CQMAKEPROJECTPARSER_H
