#include "navigatormainwindow.h"
#include "my_utilties.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setAppTheme(&a);
    NavigatorMainWindow w;
    w.show();

    return a.exec();
}
