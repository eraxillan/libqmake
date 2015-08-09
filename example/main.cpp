#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

#include <CQmakeProjectParser.h>

int main (int argc, char *argv[])
{
	QCoreApplication app (argc, argv);

	CQmakeProjectParser parser( QDir::cleanPath (
								QFileInfo (QDir::currentPath ())
								.dir ()
								.absoluteFilePath ("../libqmake/libqmake.pro")));
	if (!parser.lastError ().isEmpty ())
	{
		qDebug() << "ERROR: " << parser.lastError ();
		return 1;
	}

	qDebug() << "Project headers:";
	foreach( QString header, parser.headers ()) qDebug() << "\t" << header;

	qDebug() << "Project sources:";
	foreach( QString src, parser.sources ()) qDebug() << "\t" << src;

	return app.exec();
}
