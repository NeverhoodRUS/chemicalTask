#include <QCoreApplication>
#include <QSharedPointer>
#include <QDebug>
#include "chemicalModel.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSharedPointer<ChemicalModel> model { new ChemicalModel() };
    auto map = model->analyzeString("K[Fe(NO3)2]4фы");
    foreach (auto key, map.keys()) {
        qDebug() << key << " => " << map.value(key);
    }

    return a.exec();
}
