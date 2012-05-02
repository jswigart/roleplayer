#include "tiletools.h"

#include <QtCore/QtPlugin>
#include "tiletoolsplugin.h"
#include "tiletools.h"

TileWidgets::TileWidgets( QObject *parent ) {
	widgets.append( new QGraphicsTileViewPlugin( this ) );
	widgets.append( new QPropertyTreePlugin( this ) );
}
QList<QDesignerCustomWidgetInterface*> TileWidgets::customWidgets() const {
	return widgets;
}

Q_EXPORT_PLUGIN2(tilewidgetsplugin, TileWidgets)
