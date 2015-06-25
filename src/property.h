#ifndef __QMAKE_PROPERTY_H
#define __QMAKE_PROPERTY_H

#include <qglobal.h>
#include <qstring.h>

class QSettings;

class QMakeProperty
{
	QSettings *settings;
	void initSettings();
	QString keyBase(bool =true) const;
	QString value(QString, bool just_check);
public:
	QMakeProperty();
	~QMakeProperty();

	bool hasValue(QString);
	QString value(QString v) { return value(v, false); }
	void setValue(QString, const QString &);
	void remove(const QString &);

	bool exec();
};

#endif // __QMAKE_PROPERTY_H
