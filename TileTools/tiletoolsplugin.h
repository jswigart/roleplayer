#ifndef TILETOOLSPLUGIN_H
#define TILETOOLSPLUGIN_H

#include <QDesignerCustomWidgetInterface>

// widget collection
class TileWidgets: public QObject, public QDesignerCustomWidgetCollectionInterface
{
	Q_OBJECT
	Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

public:
	TileWidgets( QObject *parent = 0 );

	virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const;

private:
	QList<QDesignerCustomWidgetInterface*> widgets;
};

#endif // TILETOOLSPLUGIN_H
