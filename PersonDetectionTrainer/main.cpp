#include "trainermainwindow.h"
#include <QApplication>
#include "my_utilties.h"
int main(int argc, char *argv[])
{
    myTimer.start();
    QApplication a(argc, argv);
    setAppTheme(&a);
    TrainerMainWindow w;
    w.show();

    return a.exec();
}
