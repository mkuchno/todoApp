#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <list.h>

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();

    void on_actionDelete_triggered();

    void on_actionHelp_triggered();

    void setTitleClicked();

    void setTitleEnter();

    void titleEditChanged();

    void on_tabWidget_tabBarClicked(int index);

    void on_tabWidget_tabBarDoubleClicked(int index);

private:
    Ui::MainWindow *ui;

    //Variables

    QLineEdit *title;
    QPushButton *setTitle;

    list *listPointer;

    //Functions, events

    void closeEvent(QCloseEvent *event);

    void readData();
    void saveData();

    void areThereLists();
};

#endif // MAINWINDOW_H
