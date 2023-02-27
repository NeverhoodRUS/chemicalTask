#include "chemicalModel.h"
#include "appstrings.h"
#include <QDebug>

ChemicalModel::ChemicalModel() :
    _errorElementPrefix("Error "),
    _validator(new ChemicalValidator())
{
    _splittRegExp.setPattern("[A-Z][^A-Z]*");
}

QMap<QString, int> ChemicalModel::analyzeString(const QString &inputString)
{
    if(!_validator->isValid(inputString))
        throw std::invalid_argument(ExceptionStrings::incorrectSequence);
    QMap<QString/*chemical element*/, int/*element's count*/> resultMap;
    QString element;
    QMap<QString, int> squareElems;
    QMap<QString, int> roundElems;

    _status = NO_BRACKETS;

    for(int i(0); i < inputString.size(); ++i)
    {
        const QString itEl = inputString.at(i);
        if(Constants::finishElementChars.contains(itEl) || itEl.toInt() > 0)
            continue;
        element.append(itEl);
        const QString &nextElement = (i < inputString.size()-1)? QString{inputString.at(i+1)} : "";
        const bool elementCompiled = Constants::finishElementChars.contains(nextElement) || nextElement.isUpper()
                || nextElement.isEmpty() || nextElement.toInt() > 0;
        if(!elementCompiled)
            continue;
        if(!Constants::chemicalElements.contains(element))
            throw std::invalid_argument(ExceptionStrings::incorrectElement + " " + element.toStdString());
        changeStatus(itEl);

        switch (_status) {
        case NO_BRACKETS:
        {
            insertElementToMap(resultMap, nextElement, element);
            element.clear();
            break;
        }
        case SQUARE_OPENED:
        {
            insertElementToMap(squareElems, nextElement, element);
            element.clear();
            break;
        }
        case SQUARE_OPENED_ROUND_OPENED:
        case ROUND_OPENED:
        {
            insertElementToMap(roundElems, nextElement, element);
            element.clear();
            break;
        }
        case SQUARE_CLOSED: {
            const int squareMultiplier = nextElement.toInt();
            for(const QString &squareEl: squareElems.keys())
            {
                const int value = squareElems.value(squareEl);
                if(value > 0 && squareMultiplier > 0)
                    squareElems[squareEl] = value * squareMultiplier;
                else if(value == 0 || squareMultiplier == 0)
                    squareElems[squareEl] = value > 0 ? value : squareMultiplier;

                if(resultMap.contains(squareEl))
                    resultMap[squareEl] = resultMap[squareEl] + squareElems[squareEl];
                else
                    resultMap.insert(squareEl, squareMultiplier);
            }
            squareElems.clear();
            element.clear();
            break;
        }
        case SQUARE_OPENED_ROUND_CLOSED:
        case ROUND_CLOSED:
        {
            const int roundMultiplier = nextElement.toInt();
            for(const QString &roundEl: roundElems.keys())
            {
                const int value = roundElems.value(roundEl);
                if(value > 0 && roundMultiplier > 0)
                    roundElems[roundEl] = value * roundMultiplier;
                else if(value == 0 || roundMultiplier == 0)
                    roundElems[roundEl] = value > 0 ? value : roundMultiplier;

                if(!squareElems.isEmpty())
                {
                    if(squareElems.contains(roundEl))
                        squareElems[roundEl] = squareElems[roundEl] + roundElems[roundEl];
                    else
                        squareElems.insert(roundEl, roundMultiplier);
                }
                else
                {
                    if(resultMap.contains(roundEl))
                        resultMap[roundEl] = resultMap[roundEl] + roundElems[roundEl];
                    else
                        resultMap.insert(roundEl, roundMultiplier);
                }
            }
            roundElems.clear();
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
        if(_status == SQUARE_CLOSED || _status == ROUND_CLOSED)
            _status = NO_BRACKETS;
    }
}
