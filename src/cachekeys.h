#ifndef __QMAKE_CACHEKEYS_H
#define __QMAKE_CACHEKEYS_H

#include "project.h"

#include <QString>
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QHash>

///////////////////////////////////////////////////////////////////////////////////////////////////
struct FixStringCacheKey
{
	mutable uint hash;
	QString string, pwd;
	uchar flags;
	FixStringCacheKey(const QString &s, uchar f)
	{
		hash = 0;
		pwd = qmake_getpwd();
		string = s;
		flags = f;
	}
	bool operator==(const FixStringCacheKey &f) const
	{
		return (hashCode() == f.hashCode() &&
				f.flags == flags &&
				f.string == string &&
				f.pwd == pwd);
	}
	inline uint hashCode() const {
		if(!hash)
			hash = qHash(string) ^ qHash(flags);
		return hash;
	}
};
inline uint qHash(const FixStringCacheKey &f) { return f.hashCode(); }

///////////////////////////////////////////////////////////////////////////////////////////////////
struct FileInfoCacheKey
{
	mutable uint hash;
	QString file, pwd;
	FileInfoCacheKey(const QString &f)
	{
		hash = 0;
		if(isRelativePath(f))
			pwd = qmake_getpwd();
		file = f;
	}
	bool operator==(const FileInfoCacheKey &f) const
	{
		return (hashCode() == f.hashCode() && f.file == file &&
				f.pwd == pwd);
	}
	inline uint hashCode() const {
		if(!hash)
			hash = qHash(file);
		return hash;
	}
	inline bool isRelativePath(const QString &file) {
		int length = file.length();
		if (!length)
			return true;

		const QChar c0 = file.at(0);
		const QChar c1 = length >= 2 ? file.at(1) : QChar(0);
		return !(c0 == QLatin1Char('/')
				 || c0 == QLatin1Char('\\')
				 || (c0.isLetter() && c1 == QLatin1Char(':'))
				 || (c0 == QLatin1Char('/') && c1 == QLatin1Char('/'))
				 || (c0 == QLatin1Char('\\') && c1 == QLatin1Char('\\')));
	}
};
inline uint qHash(const FileInfoCacheKey &f) { return f.hashCode(); }

///////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
inline void qmakeDeleteCacheClear(void *i) { delete reinterpret_cast<T*>(i); }

inline void qmakeFreeCacheClear(void *i) { free(i); }

typedef void (*qmakeCacheClearFunc)(void *);
void qmakeAddCacheClear(qmakeCacheClearFunc func, void **);
void qmakeClearCaches();

#endif // __QMAKE_CACHEKEYS_H
