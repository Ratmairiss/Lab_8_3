
#include "MainHeader.h"

int main(int argc, char* argv[])
{
	QApplication a(argc,argv);

	QFile file(":/new/prefix1/style.qss");  
	file.open(QFile::ReadOnly);
	a.setStyleSheet(file.readAll());  
	file.close();

	PhoneDirectory app;
	app.show();

	return a.exec();
}