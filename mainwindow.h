#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QRubberBand>
#include <QImage>
#include <QGraphicsItem>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
protected slots:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
private slots:
	void on_loadFiles_clicked();
	void on_imageFiles_currentRowChanged(int currentRow);
	void on_scale_valueChanged(double arg1);

private:
	Ui::MainWindow *ui;
	QList<QRect> m_crops;
	QList<QImage *> m_images;
	QGraphicsItem *m_line;
	QGraphicsScene *m_scene;
	bool isLeft(QPoint pos);
	bool isRight(QPoint pos);

	QGraphicsItem *m_lastImage;
	QGraphicsItem *m_lastRect;
};

#endif // MAINWINDOW_H
