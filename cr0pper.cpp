#include "cr0pper.h"
#include "ui_cr0pper.h"
#include "cr0pperscene.h"
#include <QDebug>
#include <QFileDialog>

#define CROP_THRESHOLD_DIVISOR (8.0)

Cr0pper::Cr0pper(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::Cr0pper),
	m_lastImage(0)
{
	ui->setupUi(this);

	m_scene = new Cr0pperScene();
	ui->image->setScene(m_scene);

	QPen *red = new QPen(QBrush(Qt::red), 5.0);
	QPen *blue = new QPen(QBrush(Qt::red), 1.0, Qt::DashDotDotLine);

	m_horizontal = new QGraphicsLineItem(-20000,0,20000,0);
	m_horizontal->setZValue(1000);
	m_horizontal->hide();
	m_horizontal->setPen(*red);
	m_scene->addItem(m_horizontal);
	m_vertical = new QGraphicsLineItem(0,-20000,0,20000);
	m_vertical->setZValue(1000);
	m_vertical->hide();
	m_vertical->setPen(*red);
	m_scene->addItem(m_vertical);

	m_boundsRect = new QGraphicsRectItem();
	m_boundsRect->setPen(*blue);
	m_scene->addItem(m_boundsRect);

	m_originalRect = new QGraphicsRectItem();
	m_originalRect->setPen(QPen(QBrush(Qt::black), 1.0, Qt::SolidLine));
	m_scene->addItem(m_originalRect);

	m_cropRect = new QGraphicsRectItem();
	m_cropRect->setPen(QPen(QBrush(Qt::black), 1.0, Qt::DashLine));
	m_scene->addItem(m_cropRect);

	connect(m_scene, &Cr0pperScene::mouseDown,
			this, &Cr0pper::imageClick);
	connect(m_scene, &Cr0pperScene::mouseMove,
			this, &Cr0pper::imageMove);
	connect(m_scene, &Cr0pperScene::mouseUp,
			this, &Cr0pper::imageRelease);
}

Cr0pper::~Cr0pper()
{
	delete ui;
}

int Cr0pper::validateX(int index, int value) {
	if ( value < 0 ) { value = 0; }
	else if ( value > m_images.at(index)->width() ) {
		value = m_images.at(index)->width();
	}
	return value;
}

int Cr0pper::validateY(int index, int value) {
	if ( value < 0 ) { value = 0; }
	else if ( value > m_images.at(index)->height() ) {
		value = m_images.at(index)->height();
	}
	return value;
}

void Cr0pper::imageClick(QPointF pos) {
	m_currentSide = (ImageSide) ((int)m_currentSide + 1);
	processCropRectangle(pos);
	switch (m_currentSide) {
	case IMAGE_LEFT:
	case IMAGE_RIGHT:
		m_vertical->show();
		m_vertical->setX(pos.x());
		break;
	case IMAGE_TOP:
	case IMAGE_BOTTOM:
		m_horizontal->show();
		m_vertical->setY(pos.y());
		break;
	}
}

void Cr0pper::processCropRectangle(QPointF pos) {
	int index = ui->imageFiles->currentRow();
	int value;
	switch (m_currentSide) {
	case IMAGE_LEFT:
		value = validateX(index, pos.x());
		m_vertical->setX(value);
		m_crops.at(index)->setLeft(value);
		break;
	case IMAGE_RIGHT:
		value = validateX(index, pos.x());
		m_vertical->setX(value);
		m_crops.at(index)->setRight(value);
		break;
	case IMAGE_TOP:
		value = validateY(index, pos.y());
		m_horizontal->setY(value);
		m_crops.at(index)->setTop(value);
		break;
	case IMAGE_BOTTOM:
		value = validateY(index, pos.y());
		m_horizontal->setY(value);
		m_crops.at(index)->setBottom(value);
		break;
	}
	updateStats();
}

void Cr0pper::imageMove(QPointF pos) {
	processCropRectangle(pos);
}

void Cr0pper::updateStats() {
	int index = ui->imageFiles->currentRow();
	QRect rect = *m_crops.at(index);
	ui->left->setText(QString::number((int)rect.left()));
	ui->top->setText(QString::number((int)rect.top()));
	ui->width->setText(QString::number((int)rect.width()));
	ui->height->setText(QString::number((int)rect.height()));
	m_cropRect->setRect(rect);
}

void Cr0pper::imageRelease(QPointF pos) {
	processCropRectangle(pos);
	switch (m_currentSide) {
	case IMAGE_LEFT:
	case IMAGE_RIGHT:
		m_vertical->hide();
		break;
	case IMAGE_TOP:
	case IMAGE_BOTTOM:
		m_horizontal->hide();
		break;
	}
	if ( IMAGE_BOTTOM == m_currentSide ) {
		m_currentSide = IMAGE_NONE;
		// Load next image
	}
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
		m_crops.append(new QRect(image->rect()));
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

	QImage *img = m_images.at(index);
	QPixmap p = QPixmap::fromImage(*img); // load pixmap
	// get label dimensions
	int w = ui->image->width();
	int h = ui->image->height();

	ui->startHeight->setNum(m_images.at(index)->height());
	ui->startWidth->setNum(m_images.at(index)->width());

	// set a scaled pixmap to a w x h window keeping its aspect ratio
	m_lastImage = m_scene->addPixmap(p);
	m_cropRect->setRect(*m_crops.at(index));
	m_originalRect->setRect(img->rect());
	m_boundsRect->setRect(img->width()/2.0 - img->width()/CROP_THRESHOLD_DIVISOR,
						  img->height()/2.0 - img->height()/CROP_THRESHOLD_DIVISOR,
						  2 * img->width()/CROP_THRESHOLD_DIVISOR,
						  2 * img->height()/CROP_THRESHOLD_DIVISOR);
	m_currentSide = IMAGE_NONE;
}

void Cr0pper::on_scale_valueChanged(double scale)
{
	ui->image->setTransform(QTransform().scale(scale, scale));
}
