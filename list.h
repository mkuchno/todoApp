#ifndef LIST_H
#define LIST_H

#include <QWidget>
#include <QListWidget>

namespace Ui {
class list;
}

class list : public QWidget
{
    Q_OBJECT

public:
    explicit list(QWidget *parent = nullptr);

    void addElementToList(QString element, bool done);

    ~list();

private slots:
    void on_addElementButton_clicked();

    void on_elementLineEdit_returnPressed();

    void on_doneButton_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_deleteButton_clicked();

    void on_elementLineEdit_textEdited(const QString);

    void on_listWidget_itemClicked(QListWidgetItem);

    void on_listWidget_itemSelectionChanged();

private:
    Ui::list *ui;

    QFont font;

    void areThereElements();
};

#endif // LIST_H
