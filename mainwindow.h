#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtCore>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void printExpression(QString expression);
    void printAnswer(QString answer);
    void pointToError(QString errorMessage, int index);
    void printToBox(QString text);

private slots:
    void clickedSlot(QString bt_text);
    void on_lineEdit_calc_returnPressed();
    void on_clear_bt_clicked();
    void on_equals_bt_clicked();
    void on_clear_fn_button_clicked();
    void on_listWidget_currentItemChanged(QListWidgetItem *current);
    void on_decimal_bt_clicked();
    void on_del_bt_clicked();
    void on_left_bt_clicked();
    void on_enter_fn_button_clicked();

signals:
    void clicked(const QString &text);

private:
    Ui::MainWindow *ui;
    QSignalMapper *signalMapper;

    // Calculator Tab
    void buttonSignals();


    // Function Tab
    void ABLayout(QString explanation);
};

#endif // MAINWINDOW_H


