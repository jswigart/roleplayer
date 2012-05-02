#include <QFileInfo>

#include "dialog_importtiles.h"
#include "flowlayout.h"

QDialogImportTiles::QDialogImportTiles(const QString & tileSetImg, QWidget *parent, Qt::WindowFlags f) :
	QDialog( parent, f ) {
	ui.setupUi( this );
	ui.scrollAreaWidgetContents->setLayout( new QFlowLayout( ui.scrollAreaWidgetContents ) );

	tileSetFile = tileSetImg;
	tileSet = QImage( tileSetImg );

	connect(ui.spinBoxTileWidth, SIGNAL(valueChanged(int)), this, SLOT(updateTileList()));
	connect(ui.spinBoxTileHeight, SIGNAL(valueChanged(int)), this, SLOT(updateTileList()));
	connect(ui.spinBoxTileSpacing, SIGNAL(valueChanged(int)), this, SLOT(updateTileList()));

	updateTileList();
}

QDialogImportTiles::~QDialogImportTiles() {
}

void QDialogImportTiles::updateTileList() {
	QList<QWidget *> widgets = ui.scrollAreaWidgetContents->findChildren<QWidget *>();
	foreach(QWidget * widget, widgets) {
		delete widget;
	}

	tiles.clear();

	int tileX = 0;
	int tileY = 0;

	const int tileWidth = ui.spinBoxTileWidth->value();
	const int tileHeight = ui.spinBoxTileHeight->value();
	
	for ( int x = 0; x < tileSet.width(); x += tileWidth, ++tileX ) {
		for ( int y = 0; y < tileSet.height(); y += tileHeight, ++tileY ) {
			QImage tile = tileSet.copy( x, y, tileWidth, tileWidth );
			
			QPixmap thumb = QPixmap::fromImage( tile, Qt::AutoColor | Qt::NoOpaqueDetection );
			QLabel * label = new QLabel( ui.scrollAreaWidgetContents );
			label->setScaledContents( true );
			label->setPixmap( thumb );
			label->setAlignment( Qt::AlignCenter );
			label->setFixedSize( thumb.width(), thumb.height() );
			label->setToolTip( QFileInfo( tileSetFile ).baseName() );

			ui.scrollAreaWidgetContents->layout()->addWidget( label );

			tiles.append( tile );
		}
	}
}