#ifndef WIDGET_VIEW
#define WIDGET_VIEW

#include <QDeclarativeView>

class QGameView : public QDeclarativeView
{
	Q_OBJECT
public:

	explicit QGameView(QWidget *parent = 0);
	explicit QGameView(const QUrl &source, QWidget *parent = 0);
	~QGameView();
protected:
	void keyPressEvent( QKeyEvent * keyEvent );
	void keyReleaseEvent( QKeyEvent * keyEvent );
	void wheelEvent( QWheelEvent * event );
private:
};

#endif // WIDGET_VIEW
