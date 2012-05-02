#include <QtConcurrentMap>
#include <QDir>
#include <QLabel>
#include <QCompleter>
#include <QMessageBox>
#include <QFileDialog>

#include "roleplayer.h"
#include "flowlayout.h"
#include "dialog_importtiles.h"

RolePlayer::RolePlayer(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	uiTileGroup.setupUi( ui.tileTools );

	uiTileGroup.tileScrollAreaContents->setLayout( new QFlowLayout( ui.materialScrollAreaContents ) );

	//////////////////////////////////////////////////////////////////////////
	// test prop window
	ui.propTree->AddBool( NULL, "Bool Tooltip", "Bool", true );
	ui.propTree->AddColor( NULL, "Color Tooltip", "Color", QColor( "red" ) );
	ui.propTree->AddDate( NULL, "Date Tooltip", "Date", QDate( 1979, 7,31 ) );
	ui.propTree->AddFloat( NULL, "Float Tooltip", "Float", 23.194327 );
	ui.propTree->AddIntSlider( NULL, "Int Tooltip", "Int Slider", 3213 );
	ui.propTree->AddIntSpinBox( NULL, "Int Tooltip", "Int Spinbox", 387435 );
	ui.propTree->AddString( NULL, "Int Tooltip", "String", "Here is my string" );
	ui.propTree->AddVariant( NULL, "Variant Tooltip", "Variant", QVariant::String, QVariant( "Test String" ) );
	ui.propTree->AddKeySequence( NULL, "Key Tooltip", "Key Sequence", QKeySequence::Copy );

	QtProperty * group = ui.propTree->AddGroup( NULL, "Group 1", "Group 1" );
	ui.propTree->AddFloat( group, "x", "x", 23.0, -10, 10 );
	ui.propTree->AddFloat( group, "y", "y", 24.0, -10, 10 );
	ui.propTree->AddFloat( group, "z", "z", 25.0, -10, 10 );
	//////////////////////////////////////////////////////////////////////////

	PopulateTileList();

	connect(ui.actionImport_Tiles, SIGNAL(triggered(bool)), this, SLOT(Action_ImportTiles()));
}

RolePlayer::~RolePlayer()
{

}

void RolePlayer::PopulateTileList() {
	fileList.tiles.clear();

	QStringList imageExtensions;
	imageExtensions << "*.png";
	FindAllFileTypes( QDir( "./resources/tiles" ).path(), imageExtensions, fileList.tiles );
	
	//fileWatcher.addPath( "./resources/tiles" );

	if ( !fileList.tiles.isEmpty() ) {
		fileList.future_LoadTiles = QtConcurrent::mapped( fileList.tiles, &RolePlayer::Async_LoadImages );
		fileList.futureWatcher_LoadTiles.setFuture( fileList.future_LoadTiles );

		connect(&fileList.futureWatcher_LoadTiles, SIGNAL(resultReadyAt(int)), this, SLOT(Async_ImageLoadedAt(int)));
		connect(&fileList.futureWatcher_LoadTiles, SIGNAL(finished()), this, SLOT(Async_ImageLoadFinished()));
	}
}

void RolePlayer::FindAllFileTypes( const QString & path, const QStringList & fileTypes, QFileInfoList & files ) {
	QDir dir( path );
	if ( dir.exists() ) {
		QFileInfoList f = dir.entryInfoList( fileTypes, QDir::Files, QDir::Name );
		files.append( f );

		// recurse into sub folders
		QFileInfoList subdirs = dir.entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDir::Name );
		for ( int i = 0; i < subdirs.count(); ++i ) {
			FindAllFileTypes( subdirs.at( i ).filePath(), fileTypes, files );
		}
	}
}

QImage RolePlayer::Async_LoadImages( const QFileInfo & file ) {
	QImage img( file.canonicalFilePath() );
	if ( img.isNull() ) {
		qDebug() << "async_LoadImages: " << file.canonicalFilePath() << " failed";
	} else {
		qDebug() << "async_LoadImages: " << file.canonicalFilePath() << " success";
	}
	return img;
}

void RolePlayer::Async_ImageLoadedAt( int index ) {
	qDebug() << "async_ImageLoadedAt:( " << index << " / " << fileList.tiles.count() <<  " ) : " << fileList.tiles.at( index ).canonicalFilePath();
}

void RolePlayer::Async_ImageLoadFinished() {
	qDebug() << "async_ImageLoadFinished";

	RebuildTileThumbnails();
}

void RolePlayer::RebuildTileThumbnails() {
	const int pixSize = 64;
	
	for ( int i = 0; i < fileList.future_LoadTiles.resultCount(); ++i ) {
		QImage image = fileList.future_LoadTiles.resultAt( i );
		if ( !image.isNull() ) {
			QLabel * label = new QLabel( ui.materialScrollAreaContents );

			QPixmap thumb = QPixmap::fromImage( image.scaled( pixSize, pixSize, Qt::KeepAspectRatio, Qt::SmoothTransformation ), Qt::AutoColor | Qt::NoOpaqueDetection );
			label->setScaledContents( true );
			label->setPixmap( thumb );
			label->setAlignment( Qt::AlignCenter );
			label->setFixedSize( thumb.width(), thumb.height() );
			label->setToolTip( fileList.tiles.at( i ).baseName() );

			uiTileGroup.tileScrollAreaContents->layout()->addWidget( label );
		}
	}

	uiTileGroup.tileScrollAreaContents->layout()->invalidate();
}

void RolePlayer::Action_ImportTiles() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "./", tr("Image Files (*.png *.jpg *.bmp *.tga)"));
	if ( !fileName.isEmpty() ) {
		QDialogImportTiles dlg( fileName, this );
		dlg.exec();

		if ( dlg.getTiles().count() > 0 ) {

		}
	}
}
