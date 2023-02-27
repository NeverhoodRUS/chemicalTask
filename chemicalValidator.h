#ifndef CHEMICALVALIDATOR_H
#define CHEMICALVALIDATOR_H
#include <QRegExp>

class ChemicalValidator
{
public:
    explicit ChemicalValidator() {
        _regexp.setPattern("");
        _regexp.setCaseSensitivity(Qt::CaseSensitivity::CaseSensitive);
    }

    inline void setRegExp(const QRegExp &value) { _regexp = value; }

    bool isValid(const QString &inputStr) const
    {
        if(!_regexp.exactMatch(inputStr) && !_regexp.isEmpty())
            return false;
        return checkBrackets(inputStr);
    }

private:
    QRegExp _regexp;

    bool checkBrackets(const QString &inputStr) const
    {
        int countRoundOpened = 0;
        int countRoundClosed = 0;
        int countSquareOpened = 0;
        int countSquareClosed = 0;
        bool metRound = false;

        foreach(const QString el, inputStr)
        {
            if(el == "(") {
                countRoundOpened ++;
                metRound = true;
            }
            else if(el == ")")
                countRoundClosed ++;
            else if(el == "[") {
                //If `(` bracket placed before `[`
                if(metRound)
                    return false;
                countSquareOpened ++;
            }
            else if(el == "]")
                countSquareClosed ++;
        }
        return (countRoundOpened == countRoundClosed) && (countSquareOpened == countSquareClosed);
    }
};

#endif // CHEMICALVALIDATOR_H
