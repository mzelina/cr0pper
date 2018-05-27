#ifndef CR0PPERSCENE_H
#define CR0PPERSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class Cr0pperScene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit Cr0pperScene(QObject *parent = 0);
signals:
	void mouseDown(QPointF);
	void mouseMove(QPointF);
	void mouseUp(QPointF);
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CR0PPERSCENE_H
