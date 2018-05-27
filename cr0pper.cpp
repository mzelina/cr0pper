#include "cr0pper.h"
#include "ui_cr0pper.h"
#include <QDebug>
#include <QFileDialog>

Cr0pper::Cr0pper(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::Cr0pper),
	m_lastImage(0), m_lastRect(0)
{
	ui->setupUi(this);

	m_scene = new QGraphicsScene();
	ui->image->setScene(m_scene);

	m_horizontal = new QGraphicsLineItem(-20000,0,20000,0);
	m_vertical = new QGraphicsLineItem(-20000,0,20000,0);
}

Cr0pper::~Cr0pper()
{
	delete ui;
}

bool Cr0pper::isLeft(QPoint pos) {
	return true;
}

bool Cr0pper::isRight(QPoint pos) {
	return true;
}

void Cr0pper::mousePressEvent(QMouseEvent *event)
{
	int index = ui->imageFiles->currentRow();
	if ( index < 0 ) { return; }

	QPoint loc = event->pos();
	if ( isLeft(loc) || isRight(loc) ) {
		//m_horizontal = loc.x();
	} else {
		//showVertical(loc.y());
	}
}

void Cr0pper::mouseMoveEvent(QMouseEvent *event)
{
	int index = ui->imageFiles->currentRow();
	if ( index < 0 ) { return; }
}

void Cr0pper::mouseReleaseEvent(QMouseEvent *event)
{
	// rubberBand->hide();
	int index = ui->imageFiles->currentRow();
	if ( index < 0 ) { return; }
	// determine selection, for example using QRect::intersects()
	// and QRect::contains().
}

void Cr0pper::on_loadFiles_clicked()
{
	QFileDialog dlg;
	dlg.setFileMode(QFileDialog::ExistingFiles);
	dlg.exec();
	QStringList files = dlg.selectedFiles();
	foreach (QString file, files) {
		ui->imageFiles->addItem(file);
		QImage *image = new QImage(file);
		m_images.append(image);
		m_crops.append(image->rect());
	}
}

void Cr0pper::on_imageFiles_currentRowChanged(int index)
{
//	m_rubberBands.at(index)->setGeometry(QRect(m_origins.at(0), QSize()));
	qDebug() << "on_imageFiles_currentRowChanged" << index;

	if ( m_lastImage ) {
		m_scene->removeItem(m_lastImage);
		m_lastImage = 0;
	}
	if ( m_lastRect ) {
		m_scene->removeItem(m_lastRect);
		m_lastRect = 0;
	}

	QPixmap p = QPixmap::fromImage(*m_images.at(index)); // load pixmap
	// get label dimensions
	int w = ui->image->width();
	int h = ui->image->height();

	ui->startHeight->setNum(m_images.at(index)->height());
	ui->startWidth->setNum(m_images.at(index)->width());

	// set a scaled pixmap to a w x h window keeping its aspect ratio
	m_lastImage = m_scene->addPixmap(p);
	m_lastRect = m_scene->addRect(m_crops.at(index));
}

void Cr0pper::on_scale_valueChanged(double scale)
{
	ui->image->setTransform(QTransform().scale(scale, scale));
}
