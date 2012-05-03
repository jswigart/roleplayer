#ifndef TILETOOLS_H
#define TILETOOLS_H

#include <QtDesigner/QDesignerExportWidget>
#include <QtDesigner/QDesignerCustomWidgetInterface>
#include <QtDeclarative/QDeclarativeView>
#include <QLayout>
#include <QStyle>

#include "qtpropertybrowser/qttreepropertybrowser.h"
#include "qtpropertybrowser/qtpropertymanager.h"
#include "qtpropertybrowser/qtvariantproperty.h"
#include "qtpropertybrowser/qteditorfactory.h"

class QGraphicsTileViewPlugin : public QObject, public QDesignerCustomWidgetInterface
{
	Q_OBJECT
	Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
	QGraphicsTileViewPlugin(QObject *parent = 0);

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

class QDESIGNER_WIDGET_EXPORT QGraphicsTileView : public QGraphicsView/*QDeclarativeView*/ {
public:
	Q_OBJECT
	Q_PROPERTY(bool drawGrid READ getDrawGrid WRITE setDrawGrid DESIGNABLE true NOTIFY gridEnabledChanged);
	Q_PROPERTY(int cellSize READ getGridSize WRITE setGridSize DESIGNABLE gridEnabled NOTIFY gridSizeChanged);
public:
	bool gridEnabled() const { return gridRender; }
	int getGridSize() const { return gridSize; }
	void setGridSize( int size ) { gridSize = size; }

	bool getDrawGrid() const { return gridRender; }
	void setDrawGrid( bool b ) { gridRender = b; }

	QGraphicsTileView( QWidget *parent = 0 );
	QGraphicsTileView(QGraphicsScene *scene, QWidget *parent = 0);
	//QGraphicsTileView( const QUrl &source, QWidget *parent );
	~QGraphicsTileView();
protected:
	void drawBackground ( QPainter * painter, const QRectF & rect );
	void drawForeground ( QPainter * painter, const QRectF & rect );
Q_SIGNALS:
	void gridEnabledChanged(bool);
	void gridSizeChanged(float);
public Q_SLOTS:
	void redrawView();
private:
	int			gridSize;
	bool		gridRender;

	bool		initialized;

	void		init();
};

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
public:
	Q_OBJECT	
public:
	QtProperty * AddBool( QtProperty * parent, const char * tooltip, const char * name, const bool val );
	QtProperty * AddColor( QtProperty * parent, const char * tooltip, const char * name, const QColor & val );
	QtProperty * AddDate( QtProperty * parent, const char * tooltip, const char * name, const QDate & val );
	QtProperty * AddDateTime( QtProperty * parent, const char * tooltip, const char * name, const QDateTime & val );
	QtProperty * AddFloat( QtProperty * parent, const char * tooltip, const char * name, const float val, const float valMin = FLT_MIN, const float valMax = FLT_MAX );
	QtProperty * AddIntSlider( QtProperty * parent, const char * tooltip, const char * name, const int val, const int valMin = INT_MIN, const int valMax = INT_MAX );
	QtProperty * AddIntSpinBox( QtProperty * parent, const char * tooltip, const char * name, const int val, const int valMin = INT_MIN, const int valMax = INT_MAX );
	QtProperty * AddGroup( QtProperty * parent, const char * tooltip, const char * name );
	QtProperty * AddString( QtProperty * parent, const char * tooltip, const char * name, const char * val );
	QtProperty * AddVariant( QtProperty * parent, const char * tooltip, const char * name, const int propType, const QVariant & val );
	QtProperty * AddKeySequence( QtProperty * parent, const char * tooltip, const char * name, const QKeySequence & val );

	QPropertyTree( QWidget *parent = 0 );
	~QPropertyTree();
private:

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
};

//////////////////////////////////////////////////////////////////////////

#endif // TILETOOLS_H
