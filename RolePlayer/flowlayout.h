#ifndef FLOW_LAYOUT_H
#define FLOW_LAYOUT_H

#include <QWidget>
#include <QLayout>
#include <QStyle>

class QFlowLayout : public QLayout
{
public:
	QFlowLayout(QWidget *parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);
	QFlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);
	~QFlowLayout();

	void addItem(QLayoutItem *item);
	int horizontalSpacing() const;
	int verticalSpacing() const;
	Qt::Orientations expandingDirections() const;
	bool hasHeightForWidth() const;
	int heightForWidth(int) const;
	int count() const;
	QLayoutItem *itemAt(int index) const;
	QSize minimumSize() const;
	void setGeometry(const QRect &rect);
	QSize sizeHint() const;
	QLayoutItem *takeAt(int index);

private:
	int doLayout(const QRect &rect, bool testOnly) const;
	int smartSpacing(QStyle::PixelMetric pm) const;

	QList<QLayoutItem *> itemList;
	int m_hSpace;
	int m_vSpace;
};

#endif // FLOW_LAYOUT_H
