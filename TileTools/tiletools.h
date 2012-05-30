#ifndef TILETOOLS_H
#define TILETOOLS_H

#include <QDeclarativeExtensionPlugin>
#include <QtDesigner/QDesignerExportWidget>
#include <QtDesigner/QDesignerCustomWidgetInterface>
#include <QtDeclarative/QDeclarativeView>
#include <QLayout>
#include <QStyle>
#include <QGraphicsObject>

#include "qtpropertybrowser/qttreepropertybrowser.h"
#include "qtpropertybrowser/qtpropertymanager.h"
#include "qtpropertybrowser/qtvariantproperty.h"
#include "qtpropertybrowser/qteditorfactory.h"

//////////////////////////////////////////////////////////////////////////

class QPropertyTreePlugin : public QObject, public QDesignerCustomWidgetInterface
{
	Q_OBJECT
	Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
	QPropertyTreePlugin(QObject *parent = 0);

	bool isContainer() const;
	bool isInitialized() const;
	QIcon icon() const;
	QString domXml() const;
	QString group() const;
	QString includeFile() const;
	QString name() const;
	QString toolTip() const;
	QString whatsThis() const;
	QWidget *createWidget(QWidget *parent);
	void initialize(QDesignerFormEditorInterface *core);

private:
	bool initialized;
};

class QDESIGNER_WIDGET_EXPORT QPropertyTree : public QtTreePropertyBrowser {
	Q_OBJECT
public:
	struct propManagers_t {
		QtBoolPropertyManager			* editBool;
		QtColorPropertyManager			* editColor;
		QtDatePropertyManager			* editDate;
		QtDateTimePropertyManager		* editDateTime;
		QtDoublePropertyManager			* editDouble;
		QtEnumPropertyManager			* editEnum;
		QtFlagPropertyManager			* editFlag;
		QtFontPropertyManager			* editFont;
		QtGroupPropertyManager			* editGroup;
		QtIntPropertyManager			* editIntSlider;
		QtIntPropertyManager			* editIntSpinBox;
		QtPointPropertyManager			* editPoint;
		QtRectPropertyManager			* editRect;
		QtSizePropertyManager			* editSize;
		QtSizePolicyPropertyManager		* editSizePolicy;
		QtStringPropertyManager			* editString;
		QtTimePropertyManager			* editTime;
		QtVariantPropertyManager		* editVariant;
		QtKeySequencePropertyManager	* editKeySequence;
	} propManagers;
public:
	QtProperty * AddBool( QtProperty * parent, const QString & tooltip, const QString & name, const bool val );
	QtProperty * AddColor( QtProperty * parent, const QString & tooltip, const QString & name, const QColor & val );
	QtProperty * AddDate( QtProperty * parent, const QString & tooltip, const QString & name, const QDate & val );
	QtProperty * AddDateTime( QtProperty * parent, const QString & tooltip, const QString & name, const QDateTime & val );
	QtProperty * AddFloat( QtProperty * parent, const QString & tooltip, const QString & name, const float val, const float valMin = FLT_MIN, const float valMax = FLT_MAX );
	QtProperty * AddIntSlider( QtProperty * parent, const QString & tooltip, const QString & name, const int val, const int valMin = INT_MIN, const int valMax = INT_MAX );
	QtProperty * AddIntSpinBox( QtProperty * parent, const QString & tooltip, const QString & name, const int val, const int valMin = INT_MIN, const int valMax = INT_MAX );
	QtProperty * AddGroup( QtProperty * parent, const QString & tooltip, const QString & name );
	QtProperty * AddString( QtProperty * parent, const QString & tooltip, const QString & name, const char * val );
	QtProperty * AddVariant( QtProperty * parent, const QString & tooltip, const QString & name, const int propType, const QVariant & val );
	QtProperty * AddKeySequence( QtProperty * parent, const QString & tooltip, const QString & name, const QKeySequence & val );

	QPropertyTree( QWidget *parent = 0 );
	~QPropertyTree();
private:
};

//////////////////////////////////////////////////////////////////////////



#endif // TILETOOLS_H
