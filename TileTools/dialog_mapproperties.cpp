#include <QFileInfo>
#include <QtConcurrentRun>
#include <QtConcurrentMap>
#include <QTime>

#include "dialog_mapproperties.h"
#include "ui_dialog_mapproperties.h"

//////////////////////////////////////////////////////////////////////////
class QDialogMapPropertiesImpl {
public:
	QDialogMapPropertiesImpl( QGraphicsTileView * view, QDialogMapProperties * widget ) :
		editView( view ) {
		ui.setupUi( widget );

		QtProperty * group = ui.propTree->AddGroup( NULL, "Map Properties", "Map Properties" );
		props.propGridSize = ui.propTree->AddIntSpinBox( NULL, "Map Grid Size", "Grid Size", view->getGridSize() );
		props.propBgColor = ui.propTree->AddColor( NULL, "Background Color For Map", "Background Color", view->backgroundBrush().color() );
	}

	void commitProperties() {
		editView->setGridSize( ui.propTree->propManagers.editIntSpinBox->value( props.propGridSize ) );
		editView->setBackgroundBrush( QBrush( ui.propTree->propManagers.editColor->value( props.propBgColor ) ) ); 

		editView->scene()->invalidate();
	}
private:
	Ui::DialogMapProperties			ui;
	QGraphicsTileView *				editView;

	struct properties_t {
		QtProperty *	propGridSize;
		QtProperty *	propBgColor;
	} props;
};

QDialogMapProperties::QDialogMapProperties( QGraphicsTileView * view, QWidget *parent, Qt::WindowFlags f ) :
	QDialog( parent, f ),
	impl( new QDialogMapPropertiesImpl( view, this ) ) {	
	
	connect( this, SIGNAL(finished(int)), this, SLOT(Slot_Finished(int)) );
}

QDialogMapProperties::~QDialogMapProperties() {
	delete impl;
}

void QDialogMapProperties::Slot_Finished( int result ) {
	if ( result == QDialog::Accepted ) {
		impl->commitProperties();
	}
}