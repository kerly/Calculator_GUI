#include "calculatorfunctions.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

// To do
/*
 * Add imp for functions like sin(), sqrt()
 *
 */

// Global const strings
const QString SPACE = " ";
const QString SPACES = "    ";
const QString HTML_SPACE = "&#160;";
const QString HTML_SPACES = "&#160;&#160;&#160;&#160;";
const QString FONT_RED = "<font color=\"Red\">";
const QString FONT_BLUE = "<font color=\"Blue\">";
const QString FONT_GREEN = "<font color=\"Green\">";
const QString FONT_BLACK = "<font color=\"Black\">";
const QString FONT_END = "</font>";
const QString BOLD = "<b>";
const QString BOLD_END = "</b>";
const QString BR = "<br>";
const QString ERR = "Error: ";
const QString PI_UNICODE = "\u03C0";
// Global const identifiers
const int WHITESPACE = 1;
const int NUM = 2;
const int DEC = 3;
const int DOT = 4;
const int NEGATIVE_SIGN = 5;
const int CH = 6;
const int OP = 7;
const int OPEN_PAR = 8;
const int CLOSED_PAR = 9;
// Other global constants
const double pi = 3.14159265359;

CalculatorFunctions::CalculatorFunctions(MainWindow *mainWindow)
{
    _mainWindow = mainWindow;
}

CalculatorFunctions::~CalculatorFunctions()
{

}

QString CalculatorFunctions::enterPressed(QString lineText)
{
    QString answer;

    // Format the expression with correct whitespace
    lineText = formatExpression(lineText);

    // Print the initial expression with fixed whitespaces
    _mainWindow->printExpression(lineText);

    // Evaluate the expression if it can be computed
    if(ensureCorrectness(lineText))
        answer = evaluateExpression(lineText);
    else
        return "Error";

    // Send the final answer back to the UI
    return answer;
}

QString CalculatorFunctions::evaluateExpression(QString text)
{

    // Properly format the expression for use / printing
    text = formatExpression(text);

    // Return the index of the operator with the highest precedence
    int index = indexOfHighestPrecedence(text);

    // Computes the value of the numbers before and after operator
    // Returns the string with the new value in place
    if(index != -1)
    {
        text = singleOperation(text, index);
        // Recursively call this function
        // Using the new expression
        return evaluateExpression(text);
    }

    return text;
}

QString CalculatorFunctions::computeOnOperator(double a, double b, int index, QChar op)
{
    double result = 0.0;

    if(op == '+')
        result = a + b;
    else if(op == '-')
        result = a - b;
    else if(op == '^')
        result = pow(a, b);
    else if(op == '*')
        result =  a * b;
    else if(op == '/')
    {
        if(b != 0)
            result = a / b;
        else
        {
            _mainWindow->pointToError("Cannot divide by zero", index);
            return "Error";
        }
    }

    // Return the result in string form
    return QString::number(result, 'g', 15);
}

QString CalculatorFunctions::singleOperation(QString text, int index)
{
    QString result;

    // Return the number of digits before and after the operator
    int before = numOfDigitsBefore(text, index);
    int after = numOfDigitsAfter(text, index);

    // Make substrings of the numbers before and after the operator
    QString numA = text.mid(index - before, before);
    QString numB = text.mid(index + 1, after);

    // Convert the substrings into a useable numeric data type
    double a = numA.toDouble();
    double b = numB.toDouble();

    // Return the value of this individual expression
    if(isOp(text.at(index)))
        result = computeOnOperator(a, b, index, text.at(index));
    else
    {
    }

    if(result == "Error")
        return result;

    // Replace the part of the string that contained
    // Left substring, operator and right substring
    // With the value that was computed
    QString leftSide = text.mid(0, index - before);
    QString rightSide = text.mid(index + 1 + after, text.size() - index + 1 + after);

    if(!leftSide.isEmpty()) leftSide.insert(leftSide.size(), ' ');
    if(!rightSide.isEmpty()) rightSide.insert(0, ' ');

    // Print the expression in parts
    // So the result can be highlighted in blue
    QString textToSend = (HTML_SPACES + leftSide + FONT_BLUE + BOLD + result + FONT_END + BOLD_END + rightSide);
    _mainWindow->printToBox(textToSend);


    // Replace the text string with the new equation
    text = (leftSide + result + rightSide);

    return text;
}

int CalculatorFunctions::currentIndex(QString ex, int index)
{
    QChar ch = ex.at(index);

    // Return an integer identifying the current index
    if(isNegativeSign(ex, index))
        return NEGATIVE_SIGN;
    if(isDecimal(ex, index))
        return DEC;
    if(ch == '.')
        return DOT;
    if(isOp(ch))
        return OP;
    if(isNum(ch))
        return NUM;
    if(isWhitespace(ch))
        return WHITESPACE;
    if(isOpenPar(ch))
        return OPEN_PAR;
    if(isClosedPar(ch))
        return CLOSED_PAR;
    if(isChar(ch))
        return CH;

    // The current index is an unknown character
    return -1;
}

int CalculatorFunctions::indexOfHighestPrecedence(QString text)
{
    int currentTier = -1;
    int highestTier = -1;
    int highestIndex = -1;

    int startPos = 0;
    int endPos = text.size();

    // Find the index's where the expression should be evalulated
    // For instance: if parenthesis are used
    highestSubstring(text, &startPos, &endPos);

    // Loop through the string
    // Searching for the operator with the highest precedence
    for(int i = startPos; i < endPos; i++)
    {

        // If an operator of higher precedence is found
        // Take note of the index
        currentTier = tierValue(text.at(i));
        if(currentTier > highestTier && !isNegativeSign(text, i))
        {
            highestTier = currentTier;
            highestIndex = i;
        }
    }

    // Return the index to the operator with the highest precedence
    return highestIndex;
}

void CalculatorFunctions::highestSubstring(QString text, int *startPos, int *endPos)
{
    int numOfOpen = 0;
    int numOfClose = 0;

    for(int i = 0; i < text.size(); i++)
    {
        if(isOpenPar(text.at(i)))
        {
            *startPos = i + 1;
            numOfOpen++;
        }
        else if(isClosedPar(text.at(i)))
        {
            *endPos = i - 1;
            numOfClose++;
        }

        // A full expression wrapped in parenthesis has been found
        // Return with the index's of this substring
        if(numOfOpen > 0 && numOfOpen == numOfClose)
            return;

    }

}

int CalculatorFunctions::numOfDigitsBefore(QString text, int index)
{
    int digits = 0;

    /*
    for(int i = index - 1; i >= 0; i--)
    {
        currentSpace = currentIndex(text, index);

        if(numRelated(currentSpace))
        {
            numFound = true;
            digits++;
        }
        else if(numFound == false)
            digits++;
        else if(numFound == true && currentSpace == WHITESPACE)
            break;
        else if(numFound == true && currentSpace == CHAR)
            break;
    }
    */

    // Find number of digits to the left of the operator
    int point = index - 1;
    while(point >= 0 && !opRelated(currentIndex(text, point)))
    {
        digits++;
        point--;
    }

    // Check to see if the loop stopped on a negative sign
    // If so, recursively call the function again
    if(point >= 0 && isNegativeSign(text, point))
    {
        digits++;
        digits += numOfDigitsBefore(text, point);
    }

    return digits;
}

int CalculatorFunctions::numOfDigitsAfter(QString text, int index)
{
    int digits = 0;

    /*
    // Find number of digits to the right of the operator
    for(int i = index + 1; i < text.size(); i++)
    {
        currentSpace = currentIndex(text, index);

        if(numRelated(currentSpace))
        {
            digits++;
            numFound = true;
        }
        else if(numFound == false)
            digits++;
        else if(numFound == true && currentSpace == WHITESPACE)
            break;
        else if(numFound == true && currentSpace == CHAR)
            break;
    }
    */

    int point = index + 1;
    // Check the rest of the string
    while(point < text.size() && !opRelated(currentIndex(text, point)))
    {
        digits++;
        point++;
    }

    /*
    // Check to see if the loop stopped on a negative sign
    // If so, recursively call the function again
    if(point < text.size() && isNegativeSign(text, point))
    {
        digits++;
        digits += numOfDigitsAfter(text, point);
    }
    */

    return digits;
}

int CalculatorFunctions::tierValue(QChar ch)
{
    if(ch == '^')
        return 5;
    if(ch == '*' || ch == '/')
        return 4;
    if(ch == '+' || ch == '-')
        return 1;

    // value is not an operator
    return -1;
}

bool CalculatorFunctions::isWhitespace(QChar ch)
{
    if(ch == ' ')
        return true;
    return false;
}

bool CalculatorFunctions::isClosedPar(QChar ch)
{
    if(ch == ')')
        return true;
    return false;
}

bool CalculatorFunctions::isOpenPar(QChar ch)
{
    if(ch == '(')
        return true;
    return false;
}

bool CalculatorFunctions::isNum(QChar ch)
{
    // ASCII value for single digits
    if(ch >= 48 && ch <= 57)
        return true;
    // ex. 4567e-4
    if(ch == 'e')
        return true;
    return false;
}

bool CalculatorFunctions::isOp(QChar ch)
{
    if(ch == '+' || ch == '-' ||
            ch == '*' || ch == '/' ||
            ch == '^' || ch == '%' )
        return true;
    return false;
}

bool CalculatorFunctions::isChar(QChar ch)
{
    if(ch == '+' || ch == '-' ||
            ch == '*' || ch == '/' ||
            ch == '^' || ch == '%' ||
            ch == '(' || ch == ')' )
        return true;
    return false;
}

bool CalculatorFunctions::isNegativeSign(QString ex, int index)
{
    // Check to see if it's even is a hyphen
    if(ex.at(index) != '-')
        return false;

    // Check the contents to the left of the negative sign
    // If a number comes before an operator
    // Then this is not a negative sign
    // It is a subtraction sign, so return false
    int point = index - 1;
    while(point >= 0 && isOp(ex.at(point)) == false)
    {
        if(isNum(ex.at(point)))
            return false;
        point--;
    }

    // Check the contents to the right of the negative sign
    point = index + 1;
    if(point >= ex.size())
        return false;
    while(point < ex.size() && isNum(ex.at(point)) == false)
    {
        if(isOp(ex.at(point)))
            return false;
        point++;
    }

    return true;
}

bool CalculatorFunctions::isDecimal(QString ex, int index)
{
    int point;

    // Check to see if it's even a decimal
    if(ex.at(index) != '.')
        return false;

    // Check the contents to the right of the decimal
    point = index + 1;
    if(point >= ex.size())
        return false;
    while(point < ex.size() && isNum(ex.at(point)) == false)
    {
        if(isChar(ex.at(point)))
            return false;
        point++;
    }

    return true;

}

bool CalculatorFunctions::numRelated(int identifier)
{
    switch(identifier)
    {
    case NUM:
    case DEC:
    case NEGATIVE_SIGN:
        return true;
    default:
        return false;
    }
}

bool CalculatorFunctions::opRelated(int identifier)
{
    switch(identifier)
    {
    case CH:
    case OP:
    case DOT:
    case OPEN_PAR:
    case CLOSED_PAR:
        return true;
    default:
        return false;
    }
}

bool CalculatorFunctions::complexExpression(QString ex, int index)
{
    QString tempSub;

    if(ex.at(index) == 's')
    {

    }
    else if(ex.at(index) == 'c')
    {

    }
    else if(ex.at(index) == 't')
    {

    }

    return true;
}

bool CalculatorFunctions::anyWhitespace(QString ex)
{
    for(int i = 0; i < ex.size(); i++)
        if(ex.at(i) == ' ')
            return true;
    return false;
}

QString CalculatorFunctions::formatExpression(QString ex)
{
    // Remove unnecessary chars from expression
    ex = correctWhitespace(ex);
    ex = removeDecimals(ex);
    ex = addMultipliers(ex);
    ex = removeParenthesis(ex);
    ex = normalizeNumbers(ex);
    ex = correctWhitespace(ex);

    return ex;
}

QString CalculatorFunctions::removeDecimals(QString ex)
{

    for(int i = 0; i < ex.size(); i++)
    {

        // If the decimal is not valid or necessary, delete it
        if(currentIndex(ex, i) == DOT)
        {
            ex.remove(i, 1);
            i--;
        }
    }

    return ex;
}

QString CalculatorFunctions::removeParenthesis(QString ex)
{
    int openPos = -1;
    int closePos = -1;
    int numOfOpen = 0;
    int numOfClose = 0;
    bool anyOperators = false;
    bool emptyParenthesis = true;

    for(int i = 0; i < ex.size(); i++)
    {

        // Check the contents between the parenthesis
        if(isOpenPar(ex.at(i)))
        {
            numOfOpen = 1;
            numOfClose = 0;
            openPos = i;
            int m = i + 1;
            while(m < ex.size() && numOfClose < numOfOpen)
            {
                if(isOp(ex.at(m)) == true && isNegativeSign(ex, m) == false)
                    anyOperators = true;
                else if(isNum(ex.at(m)))
                    emptyParenthesis = false;
                else if(isOpenPar(ex.at(m)))
                {
                    openPos = m;
                    numOfOpen++;
                }
                else if(isClosedPar(ex.at(m)))
                {
                    closePos = m;
                    numOfClose++;
                }
                m++;
            }

            if(emptyParenthesis == true)
            {
                // Remove the parenthesis and everything between them
                ex.remove(openPos, closePos - openPos + 1);
                i--;
            }
            else if(anyOperators == false)
            {
                // Remove the parenthesis but keep the number inside
                ex.remove(closePos, 1);
                ex.remove(openPos, 1);
                i--;
            }

            // Reset the variables for the next loop
            anyOperators = false;
            emptyParenthesis = true;
        }

    }

    return ex;
}

QString CalculatorFunctions::normalizeNumbers(QString ex)
{
    int currentSpace = -1;
    QString tempCopy;
    QString numRep;
    double num;

    for(int i = 0; i < ex.size(); i++)
    {
        // Find the beginning of a number
        currentSpace = currentIndex(ex, i);

        // Check if the current index is any part of a number
        if(numRelated(currentSpace))
        {
            // Copy the number to a temporary string
            int m = i;
            while(m < ex.size() && numRelated(currentIndex(ex, m)))
            {
                tempCopy.append(ex.at(m));
                ex.remove(m, 1);
            }

            // Correctly represent the string in number form
            num = tempCopy.toDouble();
            numRep = QString::number(num, 'g', 15);

            // Replace the old number with the formatted number
            ex.insert(i, numRep);

            // Jump the loop after the inserted number
            i += numRep.size() + 1;

            // Clear the strings for the next loop
            tempCopy.clear();
            numRep.clear();
        }
    }

    return ex;
}

QString CalculatorFunctions::correctWhitespace(QString ex)
{
    int currentSpace = -1;
    int lastSpace = -1;
    int lastNonWhite = -1;

    if(ex.isEmpty())
        return ex;

    // Remove any initial whitespace
    if(isWhitespace(ex.at(0)))
    {
        while(ex.isEmpty() == false && isWhitespace(ex.at(0)))
            ex.remove(0, 1);
    }

    for(int i = 0; i < ex.size(); i++)
    {

        // Check current index
        currentSpace = currentIndex(ex, i);

        // Clear the whitespace between a negative sign and a number
        if(currentSpace == NEGATIVE_SIGN)
        {
            int j = i + 1;
            while(j < ex.size() && isWhitespace(ex.at(j)))
            {
                ex.remove(j, 1);
                // ex.size() is automatically getting resized
                // So no need to increment j
            }
        }
        // Clear the whitespace between a decimal and it's number
        else if(currentSpace == DEC)
        {
            // Left side whitespace removal
            int j = i - 1;
            while(j >= 0 && isWhitespace(ex.at(j)))
            {
                ex.remove(j, 1);
                j--;
                i--;
            }

            // Right side whitespace removal
            j = i + 1;
            while(j < ex.size() && isWhitespace(ex.at(j)))
            {
                ex.remove(j ,1);
                // ex.size() is automatically getting resized
                // So no need to increment j
            }
        }
        // Compare previous and current index's
        // Perform the appropriate action
        else if(lastNonWhite == NEGATIVE_SIGN && currentSpace == WHITESPACE)
        {
            ex.remove(i, 1);
            i--;
        }
        else if(lastSpace == WHITESPACE && currentSpace == WHITESPACE)
        {
            ex.remove(i, 1);
            i--;
        }
        else if(numRelated(lastSpace) && opRelated(currentSpace))
        {
            ex.insert(i, " ");
        }
        else if(opRelated(lastSpace) && numRelated(currentSpace))
        {
            ex.insert(i, " ");
        }
        else if(opRelated(lastSpace) && opRelated(currentSpace))
        {
            ex.insert(i, " ");
        }

        // Save the index for the next iteration
        lastSpace = currentIndex(ex, i);
        if(lastSpace != WHITESPACE)
            lastNonWhite = lastSpace;

    }

    return ex;
}

QString CalculatorFunctions::addMultipliers(QString ex)
{
    // Save a record of the last non-whitespace char group
    int currentSpace = -1;
    int lastSpace = -1;

    for(int i = 0; i < ex.size(); i++)
    {
        currentSpace = currentIndex(ex, i);

        // Insert multiplication where appropriate
        if(lastSpace == CLOSED_PAR && currentSpace == OPEN_PAR)
            ex.insert(i - 1, '*');
        else if(lastSpace == CLOSED_PAR && numRelated(currentSpace))
            ex.insert(i - 1, '*');
        else if(numRelated(lastSpace) && currentSpace == OPEN_PAR)
            ex.insert(i - 1, '*');

        if(currentSpace != WHITESPACE)
            lastSpace = currentSpace;
    }

    return ex;
}

// Check the expression to ensure it can be computed
bool CalculatorFunctions::ensureCorrectness(QString ex)
{
    bool anyNums = false;
    int currentSpace = -1;
    int lastSpace = -1;
    int lastNonWhite = -1;

    // Check the expression for correct parenthesis
    if(correctParenthesis(ex) == false)
        return false;

    // Run through checking for expressions that would result in an error
    for(int i = 0; i < ex.size(); i++)
    {

        currentSpace = currentIndex(ex, i);

        if(currentSpace == -1)
        {
            _mainWindow->pointToError("Unknown symbol", i);
            return false;
        }

        if(currentSpace == NUM)
            anyNums = true;


        // If the last char was an operator
        // and the current char is an operator
        if(lastNonWhite == OP && currentSpace == OP)
        {
            _mainWindow->pointToError("Unexpected operator", i);
            return false;
        }
        // If the last char was a number
        // and the current char is a number
        else if(lastNonWhite == NUM && currentSpace == NUM && lastSpace == WHITESPACE)
        {
            _mainWindow->pointToError("No operator to use", i);
            return false;
        }
        // If the last char was a decimal
        // and the current char is an operator
        else if(lastNonWhite == DEC && currentSpace == CH)
        {
            _mainWindow->pointToError("Unexpected decimal", i);
            return false;
        }

        // Save the index of the loop for the next iteration
        lastSpace = currentSpace;
        if(currentSpace != WHITESPACE)
            lastNonWhite = currentSpace;
    }

    // Check that the last char isn't an operator
    if(lastNonWhite == OP)
    {
        int index = ex.size() - 1;
        while(index >= 0 && !isOp(ex.at(index)))
        {
            index--;
        }

        _mainWindow->pointToError("Unexpected operator", index);
        return false;
    }

    if(anyNums == false)
    {
        _mainWindow->pointToError("Nothing to compute", 0);
        return false;
    }

    return true;
}

bool CalculatorFunctions::correctParenthesis(QString ex)
{
    int numOfOpen = 0;
    int numOfClose = 0;
    int lastPos = 0;

    for(int i = 0; i < ex.size(); i++)
    {
        if(isOpenPar(ex.at(i)))
        {
            numOfOpen++;
            lastPos = i;
        }
        else if(isClosedPar(ex.at(i)))
        {
            numOfClose++;
            lastPos = i;
        }


        if(numOfOpen < numOfClose)
        {
            _mainWindow->pointToError("Unexpected closed parenthesis", i);
            return false;
        }
    }


    // Check that the number of open parenthesis
    // Matches the number of closed parenthesis
    if(numOfOpen != numOfClose)
    {
        _mainWindow->pointToError("Unmatching number of parenthesis", lastPos);
        return false;
    }

    return true;
}
