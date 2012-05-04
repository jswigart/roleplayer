#include <QVarLengthArray>

#include "tiletools.h"
#include "dialog_mapproperties.h"

QGraphicsTileViewPlugin::QGraphicsTileViewPlugin( QObject *parent /* = 0 */ ) {
}

void QGraphicsTileViewPlugin::initialize( QDesignerFormEditorInterface * core ) {
	if (initialized)
		return;

	initialized = true;
}

bool QGraphicsTileViewPlugin::isInitialized() const {
	return initialized;
}

QWidget *QGraphicsTileViewPlugin::createWidget(QWidget *parent ) {
	return new QGraphicsTileView( parent );
}

QString QGraphicsTileViewPlugin::name() const {
	return "QGraphicsTileView";
}

QString QGraphicsTileViewPlugin::group() const {
	return "Tile Tools";
}

QIcon QGraphicsTileViewPlugin::icon() const {
	return QIcon();
}

QString QGraphicsTileViewPlugin::toolTip() const {
	return QString( "View widget for showing and manipulating tiled graphics" );
}

QString QGraphicsTileViewPlugin::whatsThis() const {
	return QString();
}

bool QGraphicsTileViewPlugin::isContainer() const {
	return false;
}

QString QGraphicsTileViewPlugin::domXml() const {
	return "<widget class=\"QGraphicsTileView\" name=\"tileView\">\n"
		" <property name=\"geometry\">\n"
		"  <rect>\n"
		"   <x>0</x>\n"
		"   <y>0</y>\n"
		"   <width>100</width>\n"
		"   <height>100</height>\n"
		"  </rect>\n"
		" </property>\n"
		"</widget>\n";
}

QString QGraphicsTileViewPlugin::includeFile() const {
	return "tiletools.h";
}

//////////////////////////////////////////////////////////////////////////

QGraphicsTileView::QGraphicsTileView( QWidget *parent ) :
	QGraphicsView( parent ) {
	init();
}
QGraphicsTileView::QGraphicsTileView( QGraphicsScene *scene, QWidget *parent ) :
	QGraphicsView( scene, parent ) {
	init();
}
//QGraphicsTileView::QGraphicsTileView( const QUrl &source, QWidget *parent ) :
//	QGraphicsView( source, parent ) {
//	init();
//}

void QGraphicsTileView::init() {
	gridSize = 32.0f;
	gridRender = true;
	initialized = false;

	connect(this, SIGNAL(gridEnabledChanged(bool)), this, SLOT(redrawView()));
	connect(this, SIGNAL(gridSizeChanged(int)), this, SLOT(redrawView()));
}

QGraphicsTileView::~QGraphicsTileView() {
}

void QGraphicsTileView::drawBackground( QPainter * painter, const QRectF & rect ) {
	QGraphicsView::drawBackground( painter, rect );

	static float GRID_SCALE = 1.0f;

	if ( getDrawGrid() ) {		
		qDebug() << "DRAW GRID " << this << " " << rect;
		
		//painter->setPen( Qt::DashLine );
		//painter->setWorldMatrixEnabled(true);
		QPen gridPen( QColor( 127, 127, 127, 127 ) );
		painter->setPen( gridPen );

		qreal left = int(rect.left()) - (int(rect.left()) % getGridSize() );
		qreal top = int(rect.top()) - (int(rect.top()) % getGridSize() );

		QVarLengthArray<QLineF, 100> linesX;
		for (qreal x = left; x < rect.right(); x += getGridSize() )
			linesX.append(QLineF(x, rect.top(), x, rect.bottom()));

		QVarLengthArray<QLineF, 100> linesY;
		for (qreal y = top; y < rect.bottom(); y += getGridSize() )
			linesY.append(QLineF(rect.left(), y, rect.right(), y));

		painter->drawLines(linesX.data(), linesX.size());
		painter->drawLines(linesY.data(), linesY.size());
	}
}

void QGraphicsTileView::drawForeground( QPainter * painter, const QRectF & rect ) {
	QGraphicsView::drawForeground( painter, rect );
}

void QGraphicsTileView::redrawView() {
	scene()->invalidate();
}

void QGraphicsTileView::save() const {

}
bool QGraphicsTileView::load( const QString & file ) {
	return false;
}

void QGraphicsTileView::changeMapPropertyDialog() {
	QDialogMapProperties dlg( this, this );
	dlg.exec();
}

//////////////////////////////////////////////////////////////////////////

QPropertyTreePlugin::QPropertyTreePlugin( QObject *parent /* = 0 */ ) {
}

void QPropertyTreePlugin::initialize( QDesignerFormEditorInterface * core ) {
	if (initialized)
		return;

	initialized = true;
}

bool QPropertyTreePlugin::isInitialized() const {
	return initialized;
}

QWidget *QPropertyTreePlugin::createWidget(QWidget *parent ) {
	return new QPropertyTree( parent );
}

QString QPropertyTreePlugin::name() const {
	return "QPropertyTree";
}

QString QPropertyTreePlugin::group() const {
	return "Tile Tools";
}

QIcon QPropertyTreePlugin::icon() const {
	return QIcon();
}

QString QPropertyTreePlugin::toolTip() const {
	return QString( "Property Manager" );
}

QString QPropertyTreePlugin::whatsThis() const {
	return QString();
}

bool QPropertyTreePlugin::isContainer() const {
	return false;
}

QString QPropertyTreePlugin::domXml() const {
	return "<widget class=\"QPropertyTree\" name=\"propTree\">\n"
		" <property name=\"geometry\">\n"
		"  <rect>\n"
		"   <x>0</x>\n"
		"   <y>0</y>\n"
		"   <width>100</width>\n"
		"   <height>100</height>\n"
		"  </rect>\n"
		" </property>\n"
		"</widget>\n";
}

QString QPropertyTreePlugin::includeFile() const {
	return "tiletools.h";
}

//////////////////////////////////////////////////////////////////////////

QPropertyTree::QPropertyTree( QWidget *parent ) :
	QtTreePropertyBrowser( parent ) {

	propManagers.editBool = new QtBoolPropertyManager( parent );
	propManagers.editColor = new QtColorPropertyManager( parent );
	propManagers.editDate = new QtDatePropertyManager( parent );
	propManagers.editDateTime = new QtDateTimePropertyManager( parent );
	propManagers.editDouble = new QtDoublePropertyManager( parent );
	propManagers.editEnum = new QtEnumPropertyManager( parent );
	propManagers.editFlag = new QtFlagPropertyManager( parent );
	propManagers.editFont = new QtFontPropertyManager( parent );
	propManagers.editGroup = new QtGroupPropertyManager( parent );
	propManagers.editIntSlider = new QtIntPropertyManager( parent );
	propManagers.editIntSpinBox = new QtIntPropertyManager( parent );	
	propManagers.editPoint = new QtPointPropertyManager( parent );
	propManagers.editRect = new QtRectPropertyManager( parent );
	propManagers.editSize = new QtSizePropertyManager( parent );
	propManagers.editSizePolicy = new QtSizePolicyPropertyManager( parent );
	propManagers.editString = new QtStringPropertyManager( parent );
	propManagers.editTime = new QtTimePropertyManager( parent );
	propManagers.editVariant = new QtVariantPropertyManager( parent );
	propManagers.editKeySequence = new QtKeySequencePropertyManager( parent );
	
	setFactoryForManager( propManagers.editBool, new QtCheckBoxFactory );
	setFactoryForManager( propManagers.editColor, new QtColorEditorFactory );
	setFactoryForManager( propManagers.editDate, new QtDateEditFactory );
	setFactoryForManager( propManagers.editDateTime, new QtDateTimeEditFactory );
	setFactoryForManager( propManagers.editDouble, new QtDoubleSpinBoxFactory );
	setFactoryForManager( propManagers.editEnum, new QtEnumEditorFactory );
	//setFactoryForManager( propManagers.editFlag, new QtFlagE );
	setFactoryForManager( propManagers.editFont, new QtFontEditorFactory );
	//setFactoryForManager( propManagers.editGroup, new QtFontEditorFactory );
	setFactoryForManager( propManagers.editIntSlider, new QtSliderFactory );
	setFactoryForManager( propManagers.editIntSpinBox, new QtSpinBoxFactory );	
	//setFactoryForManager( propManagers.editPoint, new QtSliderFactory );
	//setFactoryForManager( propManagers.editRect, new QtSliderFactory );
	//setFactoryForManager( propManagers.editSize, new QtSliderFactory );
	//setFactoryForManager( propManagers.editSizePolicy, new QtSliderFactory );
	setFactoryForManager( propManagers.editString, new QtLineEditFactory );
	setFactoryForManager( propManagers.editTime, new QtTimeEditFactory );
	setFactoryForManager( propManagers.editVariant, new QtVariantEditorFactory );
	setFactoryForManager( propManagers.editKeySequence, new QtKeySequenceEditorFactory );
}

QPropertyTree::~QPropertyTree() {
}

QtProperty * QPropertyTree::AddBool( QtProperty * parent, const char * tooltip, const char * name, const bool val ) {
	QtProperty * prop = propManagers.editBool->addProperty( name );
	prop->setToolTip( tooltip ? tooltip : "" );
	propManagers.editBool->setValue( prop, val );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

QtProperty * QPropertyTree::AddColor( QtProperty * parent, const char * tooltip, const char * name, const QColor & val ) {
	QtProperty * prop = propManagers.editColor->addProperty( name );
	prop->setToolTip( tooltip ? tooltip : "" );
	propManagers.editColor->setValue( prop, val );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

QtProperty * QPropertyTree::AddDate( QtProperty * parent, const char * tooltip, const char * name, const QDate & val ) {
	QtProperty * prop = propManagers.editDate->addProperty( name );
	prop->setToolTip( tooltip ? tooltip : "" );
	propManagers.editDate->setValue( prop, val );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

QtProperty * QPropertyTree::AddDateTime( QtProperty * parent, const char * tooltip, const char * name, const QDateTime & val ) {
	QtProperty * prop = propManagers.editDateTime->addProperty( name );
	prop->setToolTip( tooltip ? tooltip : "" );
	propManagers.editDateTime->setValue( prop, val );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

QtProperty * QPropertyTree::AddFloat( QtProperty * parent, const char * tooltip, const char * name, const float val, const float valMin, const float valMax ) {
	QtProperty * prop = propManagers.editDouble->addProperty( name );
	prop->setToolTip( tooltip ? tooltip : "" );
	propManagers.editDouble->setRange( prop, valMin, valMax );
	propManagers.editDouble->setValue( prop, val );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

QtProperty * QPropertyTree::AddIntSlider( QtProperty * parent, const char * tooltip, const char * name, const int val, const int valMin, const int valMax ) {
	QtProperty * prop = propManagers.editIntSlider->addProperty( name );
	prop->setToolTip( tooltip ? tooltip : "" );
	propManagers.editIntSpinBox->setRange( prop, valMin, valMax );
	propManagers.editIntSlider->setValue( prop, val );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

QtProperty * QPropertyTree::AddIntSpinBox( QtProperty * parent, const char * tooltip, const char * name, const int val, const int valMin, const int valMax ) {
	QtProperty * prop = propManagers.editIntSpinBox->addProperty( name );
	prop->setToolTip( tooltip ? tooltip : "" );
	propManagers.editIntSpinBox->setRange( prop, valMin, valMax );
	propManagers.editIntSpinBox->setValue( prop, val );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

QtProperty * QPropertyTree::AddGroup( QtProperty * parent, const char * tooltip, const char * name ) {
	QtProperty * prop = propManagers.editGroup->addProperty( name );
	prop->setToolTip( tooltip ? tooltip : "" );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

QtProperty * QPropertyTree::AddString( QtProperty * parent, const char * tooltip, const char * name, const char * val ) {
	QtProperty * prop = propManagers.editString->addProperty( name );
	prop->setToolTip( tooltip ? tooltip : "" );
	propManagers.editString->setValue( prop, val );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

QtProperty * QPropertyTree::AddVariant( QtProperty * parent, const char * tooltip, const char * name, const int propType, const QVariant & val ) {
	QtProperty * prop = propManagers.editVariant->addProperty( propType, name );
	prop->setToolTip( tooltip ? tooltip : "" );
	propManagers.editVariant->setValue( prop, val );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

QtProperty * QPropertyTree::AddKeySequence( QtProperty * parent, const char * tooltip, const char * name, const QKeySequence & val ) {
	QtProperty * prop = propManagers.editKeySequence->addProperty( name );
	prop->setToolTip( tooltip ? tooltip : "" );
	propManagers.editKeySequence->setValue( prop, val );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

//////////////////////////////////////////////////////////////////////////
