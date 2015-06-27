#include <include/qmakeprojectparser.h>

#include <include/CQmakeProjectParser.h>

extern "C" {

bool qmakeProjectHeaders (const char* _file_name, char** _headers)
{
	CQmakeProjectParser parser (_file_name);
	if (!parser.lastError ().isEmpty ()) return false;

	QSet<QString> headerSet = parser.headers ();
	if(headerSet.isEmpty ()) return true;

	int i = 0;
	_headers = (char**)malloc( headerSet.size () );
	for (QSet<QString>::iterator iter = headerSet.begin (); iter != headerSet.end (); ++iter)
	{
		QByteArray rawHeaderStr = iter->toUtf8 ();
		_headers[i] = (char*)malloc (rawHeaderStr.size ());
		memcpy( _headers[i], rawHeaderStr.constData (), rawHeaderStr.size ());
		++i;
	}
	return true;
}

bool qmakeProjectSources (const char* _file_name, char** _sources)
{
	CQmakeProjectParser parser (_file_name);
	if (!parser.lastError ().isEmpty ()) return false;

	QSet<QString> sourceSet = parser.headers ();
	if (sourceSet.isEmpty ()) return true;

	int i = 0;
	_sources = (char**)malloc( sourceSet.size () );
	for (QSet<QString>::iterator iter = sourceSet.begin (); iter != sourceSet.end (); ++iter)
	{
		QByteArray rawHeaderStr = iter->toUtf8 ();
		_sources[i] = (char*)malloc (rawHeaderStr.size ());
		memcpy( _sources[i], rawHeaderStr.constData (), rawHeaderStr.size ());
		++i;
	}
	return true;
}

}
