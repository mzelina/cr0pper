#include "cr0pper.h"
#include "ui_cr0pper.h"
#include "cr0pperscene.h"
#include <QDebug>
#include <QFileDialog>

Cr0pper::Cr0pper(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::Cr0pper),
	m_lastImage(0), m_lastRect(0)
{
	ui->setupUi(this);

	m_scene = new Cr0pperScene();
	ui->image->setScene(m_scene);

	QPen *red = new QPen(QBrush(Qt::red), 5.0);

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

bool Cr0pper::isLeft(int index, QPointF pos) {
	QImage *img = m_images.at(index);
	int yMid = img->height()/2.0;
	int yDelta = img->height()/8.0;
	if ( pos.x() < img->width()/2.0 &&
		 pos.y() > (yMid - yDelta) &&
		 pos.y() < (yMid + yDelta) ) {
		m_currentSide = IMAGE_LEFT;
		return true;
	}
	return false;
}

bool Cr0pper::isRight(int index, QPointF pos) {
	QImage *img = m_images.at(index);
	int yMid = img->height()/2.0;
	int yDelta = img->height()/8.0;
	if ( pos.x() > img->width()/2.0 &&
		 pos.y() > (yMid - yDelta) &&
		 pos.y() < (yMid + yDelta) ) {
		m_currentSide = IMAGE_RIGHT;
		return true;
	}
	return false;
}

bool Cr0pper::isTop(int index, QPointF pos) {
	QImage *img = m_images.at(index);
	int xMid = img->width()/2.0;
	int xDelta = img->width()/8.0;
	if ( pos.y() < img->height()/2.0 &&
		 pos.x() > (xMid - xDelta) &&
		 pos.x() < (xMid + xDelta) ) {
		m_currentSide = IMAGE_TOP;
		return true;
	}
	return false;
}

bool Cr0pper::isBottom(int index, QPointF pos) {
	QImage *img = m_images.at(index);
	int xMid = img->width()/2.0;
	int xDelta = img->width()/8.0;
	if ( pos.y() > img->height()/2.0 &&
		 pos.x() > (xMid - xDelta) &&
		 pos.x() < (xMid + xDelta) ) {
		m_currentSide = IMAGE_BOTTOM;
		return true;
	}
	return false;
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
	int index = ui->imageFiles->currentRow();
	int value;
	m_currentSide = IMAGE_NONE;
	if ( isLeft(index, pos) || isRight(index, pos) ) {
		value = validateX(index, pos.x());
		m_vertical->setX(value);
		m_vertical->show();
	} else if ( isTop(index, pos) || isBottom(index, pos) ){
		value = validateY(index, pos.y());
		m_horizontal->setY(value);
		m_horizontal->show();
	}
}

void Cr0pper::imageMove(QPointF pos) {
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

void Cr0pper::updateStats() {
	int index = ui->imageFiles->currentRow();
	QRect rect = *m_crops.at(index);
	ui->left->setText(QString::number((int)rect.left()));
	ui->top->setText(QString::number((int)rect.top()));
	ui->width->setText(QString::number((int)rect.width()));
	ui->height->setText(QString::number((int)rect.height()));
}

void Cr0pper::imageRelease(QPointF pos) {
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
	int index = ui->imageFiles->currentRow();
	m_lastRect->setRect(*m_crops.at(index));
	updateStats();
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
	m_lastRect = m_scene->addRect(*m_crops.at(index));
}

void Cr0pper::on_scale_valueChanged(double scale)
{
	ui->image->setTransform(QTransform().scale(scale, scale));
}
