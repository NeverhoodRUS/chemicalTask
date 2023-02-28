#ifndef APPSTRINGS_H
#define APPSTRINGS_H
#include <QString>

namespace Constants {

static const QStringList chemicalElements = {"H","He","Li","Be","B","C","N","O","F","Ne","Na","Mg","Al","Si","P","S","Cl","Ar","K","Ca","Sc","Ti","V","Cr","Mn","Fe","Co","Ni","Cu","Zn","Ga","Ge","As","Se","Br","Kr","Rb","Sr","Y","Zr","Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I","Xe","Cs","Ba","La","Ce","Pr","Nd","Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po","At","Rn","Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am","Cm","Bk","Cf","Es","Fm","Md","No","Lr","Rf","Db","Sg","Bh","Hs","Mt","Ds","Rg","Cn","Nh","Fl","Mc","Lv","Ts","Og"};
static const QStringList finishElementChars = {"[", "]", "(", ")", " "}; ///< Символы обозначающие, что предыдущий элемент завершён
}

namespace ExceptionStrings {

static const std::string incorrectSequence = "неверная последовательность";
static const std::string incorrectElement = "некорректный элемент";
}

#endif // APPSTRINGS_H
