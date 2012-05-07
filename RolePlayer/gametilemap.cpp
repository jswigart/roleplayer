#include <QPen>
#include <QFile>
#include <QPainter>
#include <QGraphicsScene>
#include <QVarLengthArray>

#include "gametilemap.h"
#include "dialog_mapproperties.h"

QGameTileMap::QGameTileMap( QDeclarativeItem *parent ) :
	QDeclarativeItem( parent ),
	gridSize( 32.0f ), 
	gridRender( true ),
	gridColor( 127, 127, 127, 127 ) {
	setFlag( QGraphicsItem::ItemHasNoContents, false );
	//setFlag( QGraphicsItem::ItemIsSelectable, true );
	setAcceptedMouseButtons( Qt::LeftButton );
}

QGameTileMap::~QGameTileMap() {
}

void QGameTileMap::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget ) {
	if ( getDrawGrid() ) {
		//painter->setPen( Qt::DashLine );
		//painter->setWorldMatrixEnabled(true);
		QPen gridPen( gridColor );
		painter->setPen( gridPen );

		const QRectF rect = boundingRect();

		qreal left = int( rect.left() ) - (int(rect.left()) % getGridSize() );
		qreal top = int( rect.top() ) - (int(rect.top()) % getGridSize() );

		QVarLengthArray<QLineF, 100> linesX;
		for ( qreal x = left; x < rect.right(); x += getGridSize() )
			linesX.append( QLineF(x, rect.top(), x, rect.bottom()) );

		QVarLengthArray<QLineF, 100> linesY;
		for ( qreal y = top; y < rect.bottom(); y += getGridSize() )
			linesY.append( QLineF(rect.left(), y, rect.right(), y) );

		painter->drawLines( linesX.data(), linesX.size() );
		painter->drawLines( linesY.data(), linesY.size() );
	}
	QDeclarativeItem::paint( painter, option, widget );
}

void QGameTileMap::redrawView() {
	scene()->invalidate();
}
void QGameTileMap::save( const QUrl & file ) {
	setSaveFileUrl( file );

	QFile saveFile( file.path() );
	if ( !saveFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate) ) {
		return;
	}
	
	QTextStream out( &saveFile );

	out << "Render: " << gridRender << "\n";
	out << "CellSize: " << gridSize << "\n";
	out << "GridColor: " << gridColor.red() << " " << gridColor.green() << " " << gridColor.blue() << " " << gridColor.alpha()  << "\n";
	
	QList<QByteArray> dynProps = dynamicPropertyNames();
	for ( int i = 0; i < dynProps.count(); ++i ) {
		out << QString( dynProps[ i ] ) << "\n";
	}

	saveFile.close();
}

void QGameTileMap::mousePressEvent( QGraphicsSceneMouseEvent *event ) {
	
}
