#include <calculatorfunctions.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <QSignalMapper>

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
const QString UNDER_LINE = "-------------------------";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set the initial fn tab layout
    MainWindow::on_listWidget_currentItemChanged(ui->listWidget->currentItem());
    buttonSignals();

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Calculator Tab Functions
void MainWindow::clickedSlot(QString bt_text)
{
    ui->lineEdit_calc->insert(bt_text);
}

void MainWindow::on_lineEdit_calc_returnPressed()
{
    QString answer;
    QString lineText = ui->lineEdit_calc->text();

    // Take the original expression
    // Print it in the text box
    printToBox(lineText);

    // Create a calculator function Object
    // To handle the result for this expression
    CalculatorFunctions calculatorFunctions(this);
    answer = calculatorFunctions.enterPressed(lineText);

    // Print the final answer
    printAnswer(answer);
}

void MainWindow::on_clear_bt_clicked()
{
    // Clear the line edit
    // IF line edit is already clear
    // THEN clear text box
    if(ui->lineEdit_calc->text() != "")
    {
        ui->lineEdit_calc->clear();
    }
    else
    {
        ui->textBox_calc->clear();
    }
}

void MainWindow::on_del_bt_clicked()
{
    ui->lineEdit_calc->backspace();
}

void MainWindow::on_decimal_bt_clicked()
{
    ui->checkBox_Integer->setChecked(false);
}

void MainWindow::on_equals_bt_clicked()
{
    ui->lineEdit_calc->returnPressed();
}

void MainWindow::on_left_bt_clicked()
{
    // Move the cursor to the end to auto-scroll
    /*
    QCursor c =  ui->lineEdit_calc->cursor();
    c.movePosition();
    ui->textBox_calc->setTextCursor(c);
    */
}

void MainWindow::printToBox(QString text)
{
    ui->textBox_calc->append(text);
}

void MainWindow::printExpression(QString expression)
{
    // Print the given expression in the textbox
    printToBox(FONT_BLACK + HTML_SPACES +  expression + FONT_END);
}

void MainWindow::printAnswer(QString answer)
{
    if(answer == "Error") return;

    // Print the answer in the textbox
    printToBox(FONT_GREEN + BOLD + answer + FONT_END + BOLD_END);
    printToBox(UNDER_LINE);

    // Give the answer to the line edit for use on the next equation
    ui->lineEdit_calc->setText(answer);
}

void MainWindow::pointToError(QString errorMessage, int index)
{
    QString errorPointer;

    // Pad spaces so the error can be pointed to
    for(int i = 0; i < index; i++)
        errorPointer.append(" ");
    // The carrot points to the error
    errorPointer.append("^");

    // Print out the carrot under the error
    printToBox(SPACES + errorPointer);

    // Print out the error message underneath
    QString errMsg = (FONT_RED + ERR + errorMessage + FONT_END);
    printToBox(errMsg);
    printToBox(UNDER_LINE + BR);
}

void MainWindow::buttonSignals()
{
    // Create a list of buttons to iterate through
    QList<QPushButton*> btList;
    btList.append(ui->one_bt);
    btList.append(ui->two_bt);
    btList.append(ui->three_bt);
    btList.append(ui->four_bt);
    btList.append(ui->five_bt);
    btList.append(ui->six_bt);
    btList.append(ui->seven_bt);
    btList.append(ui->eight_bt);
    btList.append(ui->nine_bt);
    btList.append(ui->zero_bt);
    // Operator buttons
    btList.append(ui->decimal_bt);
    btList.append(ui->add_bt);
    btList.append(ui->sub_bt);
    btList.append(ui->multiply_bt);
    btList.append(ui->divide_bt);
    btList.append(ui->open_bt);
    btList.append(ui->close_bt);
    btList.append(ui->carrot_bt);
    btList.append(ui->sqrt_bt);
    btList.append(ui->mod_bt);
    btList.append(ui->percent_bt);
    btList.append(ui->bracket_bt);
    btList.append(ui->sin_bt);
    btList.append(ui->cos_bt);
    btList.append(ui->tan_bt);

    signalMapper = new QSignalMapper(this);
    QPushButton *temp;

    // Loop through the list to connect the signal mapper
    for(int i = 0; i < btList.size(); i++)
    {
        temp = btList.at(i);
        connect(temp, SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(temp, QString(temp->text()));
    }

    // End mapping all the signals to one slot
    connect(signalMapper, SIGNAL(mapped(QString)),this, SIGNAL(clicked(QString)));
    connect(this, SIGNAL(clicked(QString)),this, SLOT(clickedSlot(QString)));
}

// ///////////////////////////////////////////////////////////// From other project


// ///////////////////////////////////////////////////////////// Functions Tab Functions

void MainWindow::on_clear_fn_button_clicked()
{
    // Clear the contents of every editable space in the tab
    ui->lineEdit_1->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();

    ui->lineEdit_fn_ans->clear();
    ui->textBox_fn->clear();
}

void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current)
{

    // Check the text and perform actions accordingly
    if(current->text() == "Default") {
        // Explain the function purpose
        ui->label_fn_exp->setText("Function Explaination");

        // Enable line edits

        // Disable unused line edits
        ui->lineEdit_2->setDisabled(true);
        ui->lineEdit_3->setDisabled(true);
        ui->lineEdit_4->setDisabled(true);
        ui->lineEdit_5->setDisabled(true);
        ui->lineEdit_6->setDisabled(true);

        // Set text labels
        ui->label_1->setText("");
        ui->label_2->setText("");
        ui->label_3->setText("");
        ui->label_4->setText("");
        ui->label_5->setText("");
        ui->label_6->setText("");
    }
    else if(current->text() == "Modular Exponentiation")
    {
        // Explain the function purpose
        ui->label_fn_exp->setText("Modular Exponentiation function: (base ^ exponent) mod m");

        // Enable line edits
        ui->lineEdit_2->setDisabled(false);
        ui->lineEdit_3->setDisabled(false);

        // Disable unused line edits
        ui->lineEdit_4->setDisabled(true);
        ui->lineEdit_5->setDisabled(true);
        ui->lineEdit_6->setDisabled(true);

        // Set text labels
        ui->label_1->setText("Base:");
        ui->label_2->setText("Exponent:");
        ui->label_3->setText("Mod:");
        ui->label_4->setText("");
        ui->label_5->setText("");
        ui->label_6->setText("");

    }
    else if(current->text() == "Greatest Common Factor")
    {
        ABLayout("Greatest Common Factor: Largest integer divisible by a and b");
    }
    else if(current->text() == "Least Common Multiple")
    {
        ABLayout("Least Common Multiple: Smallest integer divisible by a and b");
    }
    else if(current->text() == "Linear Combination")
    {
        ABLayout("Linear Combination: ax + by = c");
    }
    else if(current->text() == "Primitive Root")
    {
        ABLayout("Primitive root: if (a^1, a^2, ... , a^b-1) are distinct mod b");
    }
    else if(current->text() == "Prime Factorization")
    {
        // Explain the function purpose
        ui->label_fn_exp->setText("Prime Factorization: A number broken down into multiples of it's primes");

        // Enable line edits

        // Disable unused line edits
        ui->lineEdit_2->setDisabled(true);
        ui->lineEdit_3->setDisabled(true);
        ui->lineEdit_4->setDisabled(true);
        ui->lineEdit_5->setDisabled(true);
        ui->lineEdit_6->setDisabled(true);

        // Set text labels
        ui->label_1->setText("Number to Factorize:");
        ui->label_2->setText("");
        ui->label_3->setText("");
        ui->label_4->setText("");
        ui->label_5->setText("");
        ui->label_6->setText("");
    }
    else if(current->text() == "Factorial")
    {
        // Explain the function purpose
        ui->label_fn_exp->setText("Factorial Function: n! = n * (n-1) * (n-2) * ... * 2 * 1");

        // Enable line edits

        // Disable unused line edits
        ui->lineEdit_2->setDisabled(true);
        ui->lineEdit_3->setDisabled(true);
        ui->lineEdit_4->setDisabled(true);
        ui->lineEdit_5->setDisabled(true);
        ui->lineEdit_6->setDisabled(true);

        // Set text labels
        ui->label_1->setText("N!:");
        ui->label_2->setText("");
        ui->label_3->setText("");
        ui->label_4->setText("");
        ui->label_5->setText("");
        ui->label_6->setText("");
    }
    else if(current->text() == "Relative Primes (Euler's Totient)")
    {
        // Explain the function purpose
        ui->label_fn_exp->setText("Euler's Totient Function: Expresses the number of relative primes");

        // Enable line edits

        // Disable unused line edits
        ui->lineEdit_2->setDisabled(true);
        ui->lineEdit_3->setDisabled(true);
        ui->lineEdit_4->setDisabled(true);
        ui->lineEdit_5->setDisabled(true);
        ui->lineEdit_6->setDisabled(true);

        // Set text labels
        ui->label_1->setText("N:");
        ui->label_2->setText("");
        ui->label_3->setText("");
        ui->label_4->setText("");
        ui->label_5->setText("");
        ui->label_6->setText("");
    }
    else if(current->text() == "Chinese Remainder Theorem")
    {
        // Explain the function purpose
        ui->label_fn_exp->setText("Chinese Remainder Theorem: Satisfies a mod b = c for all sets");

        // Enable line edits
        ui->lineEdit_2->setDisabled(false);
        ui->lineEdit_3->setDisabled(false);
        ui->lineEdit_4->setDisabled(false);
        ui->lineEdit_5->setDisabled(false);
        ui->lineEdit_6->setDisabled(false);

        // Disable unused line edits

        // Set text labels
        ui->label_1->setText("Number 1:");
        ui->label_2->setText("Number 1 Remainder:");
        ui->label_3->setText("Number 2:");
        ui->label_4->setText("Number 2 Remainder");
        ui->label_5->setText("Number 3");
        ui->label_6->setText("Number 3 Remainder");
    }
    else if(current->text() == "Summation")
    {
        // Explain the function purpose
        ui->label_fn_exp->setText("Summation function: sum = ( i + i+1 + ... + i+n )");

        // Enable line edits
        ui->lineEdit_2->setDisabled(false);
        ui->lineEdit_3->setDisabled(false);

        // Disable unused line edits
        ui->lineEdit_4->setDisabled(true);
        ui->lineEdit_5->setDisabled(true);
        ui->lineEdit_6->setDisabled(true);

        // Set text labels
        ui->label_1->setText("Starting Index:");
        ui->label_2->setText("Ending Index:");
        ui->label_3->setText("Constant Multiple (Optional):");
        ui->label_4->setText("");
        ui->label_5->setText("");
        ui->label_6->setText("");
    }
}

// Default label / line edit config
void MainWindow::ABLayout(QString explanation)
{
    // Explain the function purpose
    ui->label_fn_exp->setText(explanation);

    // Enable line edits
    ui->lineEdit_2->setDisabled(false);

    // Disable unused line edits
    ui->lineEdit_3->setDisabled(true);
    ui->lineEdit_4->setDisabled(true);
    ui->lineEdit_5->setDisabled(true);
    ui->lineEdit_6->setDisabled(true);

    // Set text labels
    ui->label_1->setText("A:");
    ui->label_2->setText("B:");
    ui->label_3->setText("");
    ui->label_4->setText("");
    ui->label_5->setText("");
    ui->label_6->setText("");
}


void MainWindow::on_enter_fn_button_clicked()
{

    if(ui->listWidget->currentItem()->text() == "Modular Exponentiation")
    {
        // Collect values from line edits
        QString converter1 = ui->lineEdit_1->text();
        QString converter2 = ui->lineEdit_2->text();
        QString converter3 = ui->lineEdit_3->text();

        // Print out initial equation
        ui->textBox_fn->insertPlainText(converter1 + " ^ " + converter2 + " mod " + converter3 + "\n");

        // Convert values for computation
        int base = converter1.toInt();
        int exponent = converter2.toInt();
        int mod = converter3.toInt();

        // Begin computation
        ui->textBox_fn->insertPlainText("Backwards binary representation of " + converter2 + ": ");
        int binary = exponent;
        QString bin;
        while(binary)
        {
            if(binary & 1)
                bin.append("1");
            else
                bin.append("0");

            binary >>= 1;
        }
        ui->textBox_fn->insertPlainText(bin + "\n");

        int remainder;
        int oldBase;
        int oldx;
        int x = 1;
        QString update;
        while(exponent != 0)
        {
            remainder = exponent % 2;
            exponent = exponent / 2;

            if(remainder == 1)
            {
                oldBase = base;
                base = (base * base) % mod;
                update.append("1: New base: ( "); // New base = b^2 % m
                update.append(QString::number(oldBase) + " * " + QString::number(oldBase));
                update.append(") mod " + QString::number(mod) + " = " + QString::number(base) + "\n");

                oldx = x;
                x = (x * oldBase) % mod;
                update.append("    Update x: (");
                update.append(QString::number(oldx) + " * " + QString::number(oldBase) + " )");
                update.append(" mod " + QString::number(mod) + "\n");
                update.append(" = " + QString::number(x) + "\n");
            }
            else
            {
                oldBase = base;
                base = (base * base) % mod;
                update.append("0: New base: ( "); // New base = b^2 % m
                update.append(QString::number(oldBase) + " * " + QString::number(oldBase));
                update.append(") mod " + QString::number(mod) + " = " + QString::number(base) + "\n");
            }

            ui->textBox_fn->insertPlainText(update);
            update.clear();
        }

        // Final answer
        ui->textBox_fn->insertPlainText("Final x = " + QString::number(x) + "\n");
        ui->lineEdit_fn_ans->setText(QString::number(x));
    }
    else if(ui->listWidget->currentItem()->text() == "Greatest Common Factor")
    {

    }

    // Move the cursor to the end to auto-scroll
    QTextCursor c =  ui->textBox_fn->textCursor();
    c.movePosition(QTextCursor::End);
    ui->textBox_fn->setTextCursor(c);
}
