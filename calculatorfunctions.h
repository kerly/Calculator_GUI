#ifndef CALCULATORFUNCTIONS_H
#define CALCULATORFUNCTIONS_H

#include <mainwindow.h>
#include <QString>
#include <QChar>

class CalculatorFunctions
{
public:
    CalculatorFunctions(MainWindow *mainWindow);
    ~CalculatorFunctions();

    QString enterPressed(QString lineText);

private:
    MainWindow *_mainWindow;
    QString evaluateExpression(QString text);
    QString singleOperation(QString text, int index);
    void highestSubstring(QString text, int *startPos, int *endPos);
    int currentIndex(QString ex, int index);
    int numOfDigitsBefore(QString text, int index);
    int numOfDigitsAfter(QString text, int index);
    int indexOfHighestPrecedence(QString text);
    int tierValue(QChar ch);
    QString computeOnOperator(double a, double b, int index, QChar op);
    QString correctWhitespace(QString ex);
    QString formatExpression(QString ex);
    QString normalizeNumbers(QString ex);
    QString removeParenthesis(QString ex);
    QString removeDecimals(QString ex);
    QString addMultipliers(QString ex);
    bool isWhitespace(QChar ch);
    bool isClosedPar(QChar ch);
    bool isOpenPar(QChar ch);
    bool isChar(QChar ch);
    bool isNum(QChar ch);
    bool isOp(QChar ch);
    bool isNegativeSign(QString ex, int index);
    bool isDecimal(QString ex, int index);
    bool numRelated(int identifier);
    bool opRelated(int identifier);
    bool anyWhitespace(QString ex);
    bool complexExpression(QString ex, int index);
    bool ensureCorrectness(QString ex);
    bool correctParenthesis(QString ex);
};

#endif // CALCULATORFUNCTIONS_H
