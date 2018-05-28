#ifndef CROPPER_H
#define CROPPER_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QRubberBand>
#include <QImage>
#include <QGraphicsItem>
#include <QGraphicsScene>

class Cr0pperScene;

namespace Ui {
class Cr0pper;
}

class Cr0pper : public QMainWindow
{
	Q_OBJECT

public:
	explicit Cr0pper(QWidget *parent = 0);
	~Cr0pper();
	enum ImageSide {
		IMAGE_NONE,
		IMAGE_LEFT,
		IMAGE_TOP,
		IMAGE_RIGHT,
		IMAGE_BOTTOM
	};
protected slots:
private slots:
	void on_loadFiles_clicked();
	void on_imageFiles_currentRowChanged(int currentRow);
	void on_scale_valueChanged(double arg1);
	void imageClick(QPointF pos);
	void imageMove(QPointF pos);
	void imageRelease(QPointF pos);
private:
	int validateX(int index, int value);
	int validateY(int index, int value);
	void processCropRectangle(QPointF pos);
	void updateStats();

	Ui::Cr0pper *ui;
	QList<QRect *> m_crops;
	QList<QImage *> m_images;
	QGraphicsItem *m_line;
	Cr0pperScene *m_scene;
	ImageSide m_currentSide;
	QGraphicsPixmapItem *m_lastImage;
	QGraphicsRectItem *m_originalRect;
	QGraphicsRectItem *m_boundsRect;
	QGraphicsRectItem *m_cropRect;
	QGraphicsLineItem *m_horizontal;
	QGraphicsLineItem *m_vertical;
};

#endif // CROPPER_H
