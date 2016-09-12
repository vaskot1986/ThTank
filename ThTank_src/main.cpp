#include "thtank.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ThTank *master = new ThTank(0);
    master->show();


    return app.exec();
}
