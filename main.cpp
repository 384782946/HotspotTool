#include "QGraphicsWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGraphicsWindow w;
    w.show();

    return a.exec();
}
