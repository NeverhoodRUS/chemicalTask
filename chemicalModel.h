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

    QMap<QString/*chemical element*/, int/*element's count*/> analyzeString(const QString &inputString);

private:
    /**
     * @brief insertElementToMap добавляет элемент в ассоциативный массив
     * @param map ассоциативный массив элементов
     * @param nextElement следующий элемент
     * @param element элемент итерации
     */
    void insertElementToMap(QMap<QString/*chemical element*/, int/*element's count*/> &map, const QString nextElement, const QString element) const;
    /**
     * @brief changeStatus метод меняющий статус процесса парсинга данных
     * @param el текущий элемент итерации
     */
    void changeStatus(const QString el);
    /**
     * @brief squareClosed метод выполняющий слияние массива элементов в квадратных скобках с результирующим массивом
     * @param nextEl следующий элемент итерации
     * @param squareElems массив элементов внутри квадратных скобок
     * @param resultMap результирующий массив элементов
     */
    void squareClosed(const QString nextEl, QMap<QString, int> &squareElems, QMap<QString, int> &resultMap) const;
    /**
     * @brief roundClosed метод выполняющий слияние массива элементов в круглых скобках с массивом элементов квадратных скобок
     * @param nextEl следующий элемент итерации
     * @param roundElems массив элементов внутри круглых скобок
     * @param squareElems массив элементов внутри квадратных скобок
     * @param resultMap результирующий массив элементов
     */
    void roundClosed(const QString nextEl, QMap<QString, int> &roundElems, QMap<QString, int> &squareElems, QMap<QString, int> &resultMap) const;

    /**
     * @brief The StatusesParse enum статусы итерации
     */
    enum StatusesParse {
        NO_BRACKETS = 0,            ///< Вне скобок
        SQUARE_OPENED,              ///< Внутри квадратных скобок
        SQUARE_OPENED_ROUND_CLOSED, ///< Подстатус для вычисление корректного статуса. Закрылись круглые скобки внутри квадратных.
        SQUARE_OPENED_ROUND_OPENED, ///< Подстатус для вычисление корректного статуса. Открылись круглые скобки внутри квадратных.
        ROUND_OPENED,               ///< Круглые скобки закрылись
        SQUARE_CLOSED,              ///< Квадратные скобки закрылись
        ROUND_CLOSED                ///< Круглые скобки закрылись
    };

private:
    /** * @brief _errorElementPrefix шаблон-префикс ошибочного элемента */
    const QString _errorElementPrefix;
    ChemicalValidator *_validator;
    /** * @brief _status статус процесса парсинга данных */
    uint _status;
    /** * @brief _closedStatuses перечень статусов, обозначающих закрытие скобок */
    const QVector<uint> _closedStatuses;
};

#endif // CHEMICALMODEL_H
