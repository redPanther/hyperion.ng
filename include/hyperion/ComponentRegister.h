#pragma once

#include <utils/Components.h> 
#include <utils/Logger.h>

// STL includes
#include <map>
#include <QMap>

#include <QObject>

class ComponentRegister : public QObject
{
	Q_OBJECT

public:
	ComponentRegister();
	~ComponentRegister();

	QMap<hyperion::Components, bool> getRegister() { return _componentStates; };

public slots:
	void componentStateChanged(const hyperion::Components comp, const bool activated);

private:
	QMap<hyperion::Components, bool> _componentStates;
	Logger * _log;
};

