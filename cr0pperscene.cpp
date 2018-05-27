#include "cr0pperscene.h"
#include <QDebug>

Cr0pperScene::Cr0pperScene(QObject *parent) :
	QGraphicsScene(parent)
{

}

void Cr0pperScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	emit mouseDown(event->scenePos());
}

void Cr0pperScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	emit mouseMove(event->scenePos());
}

void Cr0pperScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	emit mouseUp(event->scenePos());
}
