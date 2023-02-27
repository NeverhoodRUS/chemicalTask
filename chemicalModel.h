#ifndef CHEMICALMODEL_H
#define CHEMICALMODEL_H
#include "chemicalValidator.h"
#include <QString>
#include <QRegularExpression>
#include <QVector>
#include <QMap>

class ChemicalModel
{
public:
    explicit ChemicalModel();
    ~ChemicalModel() {
        delete _validator;
    }

    inline void setSplitterRegExp(const QString &pattern)   { _splittRegExp.setPattern(pattern); }

    QMap<QString/*chemical element*/, int/*element's count*/> analyzeString(const QString &inputString);

private:
    void insertElementToMap(QMap<QString/*chemical element*/, int/*element's count*/> &map, const QString nextElement, const QString element) const;
    void changeStatus(const QString el);

    enum StatusParse {
        NO_BRACKETS = 0,
        SQUARE_OPENED,
        SQUARE_OPENED_ROUND_CLOSED,
        SQUARE_OPENED_ROUND_OPENED,
        ROUND_OPENED,
        SQUARE_CLOSED,
        ROUND_CLOSED
    };

private:
    const QString _errorElementPrefix;
    QRegularExpression _splittRegExp;
    ChemicalValidator *_validator;
    uint _status;
};

#endif // CHEMICALMODEL_H
