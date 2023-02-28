#include "chemicalModel.h"
#include "appstrings.h"

ChemicalModel::ChemicalModel() :
    _errorElementPrefix("Error "),
    _validator(new ChemicalValidator()),
    _closedStatuses{SQUARE_OPENED_ROUND_CLOSED, SQUARE_CLOSED, ROUND_CLOSED}
{
}

QMap<QString, int> ChemicalModel::analyzeString(const QString &inputString)
{
    if(!_validator->isValid(inputString))
        throw std::invalid_argument(ExceptionStrings::incorrectSequence);
    QMap<QString/*chemical element*/, int/*element's count*/> resultMap;
    QString element;
    QMap<QString, int> squareElems; //Elements inside square brackets
    QMap<QString, int> roundElems; //Elements inside round brackets

    _status = NO_BRACKETS;

    for(int i(0); i < inputString.size(); ++i)
    {
        const QString itEl = inputString.at(i);
        changeStatus(itEl);
        //Если элемент не является элементом обозначающим, что предыдущий элемент завершён и статус не является статусом,
        // при котором происходит подсчёт кол-ва элементов внутри скобок, и элемент не является числом элемента
        if((Constants::finishElementChars.contains(itEl) && !_closedStatuses.contains(_status)) || itEl.toInt())
            continue;
        element.append(itEl);
        //Следующий элемент итерации
        const QString &nextElement = (i < inputString.size()-1)? QString{inputString.at(i+1)} : "";
        //флаг обозначающий завершённость элемента
        const bool elementCompiled = Constants::finishElementChars.contains(nextElement) || nextElement.isUpper()
                || nextElement.isEmpty() || nextElement.toInt() > 0;
        if(!elementCompiled)
            continue;
        //Если элемент некорректен
        if(!Constants::chemicalElements.contains(element) && !_closedStatuses.contains(_status)) {
            resultMap.insert(_errorElementPrefix + element, 1);
            element.clear();
            continue;
        }
        //Действия согласно статусам хода итерации
        switch (_status) {
        //Если элемент не находится внутри скобок
        case NO_BRACKETS: {
            insertElementToMap(resultMap, nextElement, element);
            element.clear();
            break;
        }
        //Если элемент находится внутри квадратных скобок
        case SQUARE_OPENED: {
            insertElementToMap(squareElems, nextElement, element);
            element.clear();
            break;
        }
        //Если элемент находится внутри круглых скобок
        case SQUARE_OPENED_ROUND_OPENED:
        case ROUND_OPENED: {
            insertElementToMap(roundElems, nextElement, element);
            element.clear();
            break;
        }
        //Если квадрытные скобки закрылись
        case SQUARE_CLOSED: {
            squareClosed(nextElement, squareElems, resultMap);
            element.clear();
            break;
        }
        //Если круглые скобки закрылись
        case SQUARE_OPENED_ROUND_CLOSED:
        case ROUND_CLOSED: {
            roundClosed(nextElement, roundElems, squareElems, resultMap);
            element.clear();
            break;
        }
        default:
            break;
        }
    }
    return resultMap;
}

void ChemicalModel::insertElementToMap(QMap<QString, int> &map, const QString nextElement, const QString element) const
{
    int count = 0;
    if((nextElement.isUpper() && !nextElement.toInt()) || nextElement.isEmpty() || nextElement == "[" || nextElement == "]"
            || nextElement == "(" || nextElement == ")" || nextElement == " ")
        count = 1;
    else
        count = nextElement.toInt();
    if(count > 0)
    {
        if(map.contains(element))
            map[element] = map.value(element) + count;
        else
            map.insert(element, count);
    }
    else
        throw std::invalid_argument(ExceptionStrings::incorrectSequence + " " + element.toStdString() + nextElement.toStdString());
}

void ChemicalModel::changeStatus(const QString el)
{
    if(el == "[")
        _status = SQUARE_OPENED;
    else if(el == "]")
        _status = SQUARE_CLOSED;
    else if(el == "(")
    {
        if(_status == SQUARE_OPENED)
            _status = SQUARE_OPENED_ROUND_OPENED;
        else
            _status = ROUND_OPENED;
    }
    else if(el == ")")
    {
        if(_status == SQUARE_OPENED_ROUND_OPENED)
            _status = SQUARE_OPENED_ROUND_CLOSED;
        else
            _status = ROUND_CLOSED;
    }
    else {
        if((_status == SQUARE_CLOSED || _status == ROUND_CLOSED) && el.toInt() == 0)
            _status = NO_BRACKETS;
    }
}

void ChemicalModel::squareClosed(const QString nextEl, QMap<QString, int> &squareElems, QMap<QString, int> &resultMap) const
{
    //множитель квадратных скобок
    const int squareMultiplier = nextEl.toInt();
    //перебор всех элементов внутри квадратных скобок
    for(const QString &squareEl: squareElems.keys())
    {
        //кол-во итерируемого элемента
        const int value = squareElems.value(squareEl);
        if(value > 0 && squareMultiplier > 0)
            squareElems[squareEl] = value * squareMultiplier;
        else if(value == 0 || squareMultiplier == 0)
            squareElems[squareEl] = value > 0 ? value : squareMultiplier;
        //если результирующий массив имеет данный элемент
        if(resultMap.contains(squareEl))
            resultMap[squareEl] = resultMap[squareEl] + squareElems[squareEl];
        else
            resultMap.insert(squareEl, squareElems[squareEl]);
    }
    squareElems.clear();
}

void ChemicalModel::roundClosed(const QString nextEl, QMap<QString, int> &roundElems, QMap<QString, int> &squareElems, QMap<QString, int> &resultMap) const
{
    //множитель круглых скобок
    const int roundMultiplier = nextEl.toInt();
    for(const QString &roundEl: roundElems.keys())
    {
        //кол-во итерируемого элемента
        const int value = roundElems.value(roundEl);
        if(value > 0 && roundMultiplier > 0)
            roundElems[roundEl] = value * roundMultiplier;
        else if(value == 0 || roundMultiplier == 0)
            roundElems[roundEl] = value > 0 ? value : roundMultiplier;
        //Если круглые скобки не находятся внутри квадратных
        if(!squareElems.isEmpty())
        {
            if(squareElems.contains(roundEl))
                squareElems[roundEl] = squareElems[roundEl] + roundElems[roundEl];
            else
                squareElems.insert(roundEl, roundElems[roundEl]);
        }
        else
        {
            if(resultMap.contains(roundEl))
                resultMap[roundEl] = resultMap[roundEl] + roundElems[roundEl];
            else
                resultMap.insert(roundEl, roundElems[roundEl]);
        }
    }
    roundElems.clear();
}
