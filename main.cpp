#include "cr0pper.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Cr0pper w;
	w.show();

	return a.exec();
}
