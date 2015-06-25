/**
 * @file
 * @brief Qt qmake facade implementation
 * @author Alexander Kamyshnikov <axill777@gmail.com>
 * (C) 2014
 */

#include <QCoreApplication>
#include <QDir>
#include <QUrl>
#ifdef Q_OS_WIN32
#include <Windows.h>
#endif

#include "project.h"
#include "property.h"
#include "option.h"
#include "cachekeys.h"

#include "include/CQmakeProjectParser.h"

using namespace Piket::Source;

//
// Setup maximum path length
//
#ifdef Q_OS_WIN32
#define QMAKE_MAX_PATH MAX_PATH
#else
#define QMAKE_MAX_PATH 1024
#endif

//
// This is to work around lame implementation on Darwin. It has been noted that the getpwd(3) function
//   is much too slow, and called much too often inside of Qt (every fileFixify). With this we use a locally
//   cached copy because I can control all the times it is set (because Qt never sets the pwd under me).
//
static QString pwd;
QString qmake_getpwd ()
{
	if (pwd.isNull())
		pwd = QDir::currentPath ();
	return pwd;
}

bool qmake_setpwd(const QString &p)
{
	if(QDir::setCurrent(p))
	{
		pwd = QDir::currentPath();
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void
CQmakeProjectParser::init ()
{
	//
	// Initialize qmake
	//
	char** argv = (char**) new char* [2];
	argv[0] = new char [QMAKE_MAX_PATH];
	strcpy (argv[0], "qmake");
	argv[1] = new char [QMAKE_MAX_PATH];
	strcpy (argv[1], QFile::encodeName (m_file_name).data ());
	Option::init (2, argv);
	delete[] argv[0];
	delete[] argv[1];
	delete[] argv;

	//
	// Enable "dry run" mode, i.e. only project file parsing without makefile/etc. generation
	//
	// NOTE: original qmake use -E option for this
	//
	Option::mkfile::do_preprocess = true;

	//
	// Obtain current directory
	//
	QString oldpwd = qmake_getpwd();
#ifdef Q_WS_WIN
	if(!(oldpwd.length() == 3 && oldpwd[0].isLetter() && oldpwd.endsWith(":/")))
#endif
	{
		if (oldpwd.right (1) != QString (QDir::separator ()))
			oldpwd += QDir::separator();
	}
	//
	// Setup output dir
	// FIXME: output dir? in MY parser??
	//
	Option::output_dir = oldpwd;
}

void
CQmakeProjectParser::parse ()
{
	if ((Option::qmake_mode == Option::QMAKE_GENERATE_MAKEFILE) || (Option::qmake_mode == Option::QMAKE_GENERATE_PRL))
	{
		//
		// Check whether file exists
		//
		QString fn = Option::fixPathToLocalOS (m_file_name);
		if (!QFile::exists (fn))
		{
			m_last_error = QCoreApplication::translate ("Cannot find file: %s.\n", "qmake")
														.arg (fn.toLatin1().constData());
			return;
		}

		//
		// Save absolute project path (however, qmake use relative one)
		//
		QString abs_fn = fn;

		//
		// Get current dir
		//
		QString oldpwd = qmake_getpwd();
		//
		// Set new current dir - with project file one
		//
		qmake_setpwd (oldpwd);
		int di = fn.lastIndexOf (QDir::separator ());
		if(di != -1)
		{
			if(!qmake_setpwd(fn.left(di)))
				m_last_error = QCoreApplication::translate ("Cannot find directory: %s\n", "qmake")
															.arg (fn.left(di));
			fn = fn.right (fn.length() - di - 1);
		}

		//
		// Get absolute paths to project headers and sources
		//
		QStringList project_dirs;
		collectSubdirs (abs_fn, project_dirs);
	}
}

void
CQmakeProjectParser::collectSubdirs (const QString& _base_project, QStringList& _subprojects)
{
	//
	// First time, add the base project to the list
	//
	_subprojects << _base_project;

	//
	// Setup qmake variables
	//
	QMakeProperty prop;
	if(Option::qmake_mode == Option::QMAKE_QUERY_PROPERTY ||
			Option::qmake_mode == Option::QMAKE_SET_PROPERTY ||
			Option::qmake_mode == Option::QMAKE_UNSET_PROPERTY)
	{
		prop.exec();
		return;
	}

	//
	// Init qmake project object
	//
	QMakeProject project (&prop);

	//
	// Parse project
	//
	if(!project.read (_base_project))
	{
		m_last_error = QCoreApplication::translate ("Error processing project file: %s\n", "qmake")
													.arg (_base_project);
		return;
	}

	//
	// Get project C/C++ headers and sources
	//
	QStringList headers = project.values ("HEADERS");
	QStringList sources = project.values ("SOURCES");

	//
	// Convert relative paths to absolute ones
	//
	for (QStringList::iterator iHeader = headers.begin (); iHeader != headers.end (); ++iHeader)
	{
		const QString abs_path = QDir::cleanPath (QFileInfo (_base_project).dir ().absoluteFilePath ((*iHeader)));
		(*iHeader) = QUrl::fromLocalFile (abs_path).toString ();

		Q_ASSERT (QFile::exists ((*iHeader)));
	}
	for (QStringList::iterator iSrc = sources.begin (); iSrc != sources.end (); ++iSrc)
	{
		const QString abs_path = QDir::cleanPath (QFileInfo (_base_project).dir ().absoluteFilePath ((*iSrc)));

		(*iSrc) = QUrl::fromLocalFile (abs_path).toString ();

		Q_ASSERT (QFile::exists ((*iSrc)));
	}
	m_headers += QSet<QString>::fromList (headers);
	m_sources += QSet<QString>::fromList (sources);

	//
	// Parse all subprojects
	//
	QStringList subdirs = project.values ("SUBDIRS");
	foreach (QString subdir, subdirs)
	{
		//
		// Check project file existance
		//
		QString subdir_full = _base_project;
		QFileInfo fi (subdir_full);
		subdir_full = fi.absolutePath ();
		if (!subdir_full.endsWith (QDir::separator ()))
			subdir_full += QDir::separator ();
		subdir_full += subdir;
		if (!subdir_full.endsWith (QDir::separator ()))
			subdir_full += QDir::separator ();
		subdir_full += subdir + ".pro";
		subdir_full = Option::fixPathToLocalOS (subdir_full);
		if (!QFile::exists(subdir_full))
		{
			m_last_error = QCoreApplication::translate ("Cannot find file: %s.\n", "qmake")
														.arg (subdir_full);
			return;
		}

		//
		// Recursively collect subprojects
		//
		collectSubdirs (subdir_full, _subprojects);
	}
}

static QString
url2localPath (const QString& _url)
{
	QString result = _url;
#ifdef Q_OS_WIN32
	result.remove ("file:///");
#else
	result.remove ("file://");
#endif
	return result;
}

CQmakeProjectParser::CQmakeProjectParser (const QString& _file_name)
{
	m_file_name = url2localPath (_file_name);

	init ();
	parse ();
}

CQmakeProjectParser::~CQmakeProjectParser ()
{
	//
	// It's a finaaaal cleanuuuuup, we leaving togetheeeeer!!
	//
	qmakeClearCaches();
}

QSet<QString>
CQmakeProjectParser::headers () const
{
	return m_headers;
}

QSet<QString>
CQmakeProjectParser::sources () const
{
	return m_sources;
}

QString
CQmakeProjectParser::lastError () const
{
	return m_last_error;
}
