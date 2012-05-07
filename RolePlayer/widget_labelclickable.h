#ifndef WIDGET_LABEL_CLICKABLE
#define WIDGET_SELECTABLELABEL

#include <QLabel>

class QLabelClickable : public QLabel
{
	Q_OBJECT
public:
	void setOverlay( const QPixmap & pic );
	const QPixmap & getOverlay() const { return overlay; }

	explicit QLabelClickable(QWidget *parent=0, Qt::WindowFlags f=0);
	explicit QLabelClickable(const QString &text, QWidget *parent=0, Qt::WindowFlags f=0);
	~QLabelClickable();
protected:
	void mousePressEvent ( QMouseEvent * event );

	void paintEvent ( QPaintEvent * event );
private:
	QPixmap		overlay;

Q_SIGNALS:
	void clicked( QLabelClickable * label );
};

#endif // FLOW_LAYOUT_H
