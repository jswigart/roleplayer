#include <QVarLengthArray>

#include "tiletools.h"
#include "dialog_mapproperties.h"

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

QtProperty * QPropertyTree::AddBool( QtProperty * parent, const QString & tooltip, const QString & name, const bool val ) {
	QtProperty * prop = propManagers.editBool->addProperty( name );
	prop->setToolTip( tooltip );
	propManagers.editBool->setValue( prop, val );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

QtProperty * QPropertyTree::AddColor( QtProperty * parent, const QString & tooltip, const QString & name, const QColor & val ) {
	QtProperty * prop = propManagers.editColor->addProperty( name );
	prop->setToolTip( tooltip );
	propManagers.editColor->setValue( prop, val );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

QtProperty * QPropertyTree::AddDate( QtProperty * parent, const QString & tooltip, const QString & name, const QDate & val ) {
	QtProperty * prop = propManagers.editDate->addProperty( name );
	prop->setToolTip( tooltip );
	propManagers.editDate->setValue( prop, val );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

QtProperty * QPropertyTree::AddDateTime( QtProperty * parent, const QString & tooltip, const QString & name, const QDateTime & val ) {
	QtProperty * prop = propManagers.editDateTime->addProperty( name );
	prop->setToolTip( tooltip );
	propManagers.editDateTime->setValue( prop, val );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

QtProperty * QPropertyTree::AddFloat( QtProperty * parent, const QString & tooltip, const QString & name, const float val, const float valMin, const float valMax ) {
	QtProperty * prop = propManagers.editDouble->addProperty( name );
	prop->setToolTip( tooltip );
	propManagers.editDouble->setRange( prop, valMin, valMax );
	propManagers.editDouble->setValue( prop, val );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

QtProperty * QPropertyTree::AddIntSlider( QtProperty * parent, const QString & tooltip, const QString & name, const int val, const int valMin, const int valMax ) {
	QtProperty * prop = propManagers.editIntSlider->addProperty( name );
	prop->setToolTip( tooltip );
	propManagers.editIntSpinBox->setRange( prop, valMin, valMax );
	propManagers.editIntSlider->setValue( prop, val );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

QtProperty * QPropertyTree::AddIntSpinBox( QtProperty * parent, const QString & tooltip, const QString & name, const int val, const int valMin, const int valMax ) {
	QtProperty * prop = propManagers.editIntSpinBox->addProperty( name );
	prop->setToolTip( tooltip );
	propManagers.editIntSpinBox->setRange( prop, valMin, valMax );
	propManagers.editIntSpinBox->setValue( prop, val );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

QtProperty * QPropertyTree::AddGroup( QtProperty * parent, const QString & tooltip, const QString & name ) {
	QtProperty * prop = propManagers.editGroup->addProperty( name );
	prop->setToolTip( tooltip );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

QtProperty * QPropertyTree::AddString( QtProperty * parent, const QString & tooltip, const QString & name, const char * val ) {
	QtProperty * prop = propManagers.editString->addProperty( name );
	prop->setToolTip( tooltip );
	propManagers.editString->setValue( prop, val );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

QtProperty * QPropertyTree::AddVariant( QtProperty * parent, const QString & tooltip, const QString & name, const int propType, const QVariant & val ) {
	QtProperty * prop = propManagers.editVariant->addProperty( propType, name );
	prop->setToolTip( tooltip );
	propManagers.editVariant->setValue( prop, val );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

QtProperty * QPropertyTree::AddKeySequence( QtProperty * parent, const QString & tooltip, const QString & name, const QKeySequence & val ) {
	QtProperty * prop = propManagers.editKeySequence->addProperty( name );
	prop->setToolTip( tooltip );
	propManagers.editKeySequence->setValue( prop, val );
	if ( parent != NULL ) {
		parent->addSubProperty( prop );
	} else {
		addProperty( prop );
	}
	return prop;
}

//////////////////////////////////////////////////////////////////////////