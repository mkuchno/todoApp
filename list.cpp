#include "list.h"
#include "ui_list.h"

#include <QColor>

list::list(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::list)
{
    ui->setupUi(this);

    //copy font of ui list widget to add strikeout for it in some case
    font = ui->listWidget->font();

    //disable "done element" and "undone/delete element" buttons
    //they cannot be used if there are no elements in list
    ui->doneButton->setDisabled(true);
    ui->deleteButton->setDisabled(true);
}

list::~list()
{
    delete ui;
}

//Add a element to list with "done" condition or without
void list::addElementToList(QString element, bool done){

    //Create new list widget element with text
    //and set his aligment to center
    QListWidgetItem* newEl = new QListWidgetItem(element);
    newEl->setTextAlignment(Qt::AlignCenter);

    //if "done" variable is true set "done" condition on this element
    //it means green and strikeout text
    if(done){

        newEl->setForeground(Qt::green);

        font.setStrikeOut(true);
        newEl->setFont(font);

    }else{

        //if element is not "done" - can be editable
        newEl->setFlags(newEl->flags() | Qt::ItemIsEditable);
    }

    //add this element to the list
    ui->listWidget->addItem(newEl);

    //enable "done" and "delete" functions
    ui->doneButton->setDisabled(false);
    ui->deleteButton->setDisabled(false);
}

//On Click listener for "add new list item - button"
void list::on_addElementButton_clicked()
{
    //if text is empty show warning, else add new element to list
    //and clear "line edit"
    if(ui->elementLineEdit->text() == ""){

        ui->elementLineEdit->setStyleSheet("color: red;");

        //unselected all
        ui->listWidget->clearSelection();

    }else{

        //Create new list widget element with text
        //set this item to be editable
        //and set his aligment to center
        QListWidgetItem* newEl = new QListWidgetItem(ui->elementLineEdit->text());
        newEl->setFlags(newEl->flags() | Qt::ItemIsEditable);
        newEl->setTextAlignment(Qt::AlignCenter);

        ui->listWidget->addItem(newEl);
        ui->elementLineEdit->setText("");

        //unselected all
        ui->listWidget->clearSelection();

        //enable "done element" and "undone/delete element" buttons
        //they can be used after create new element in list
        ui->doneButton->setDisabled(false);
        ui->deleteButton->setDisabled(false);
    }
}

//Enter listener in "Add element" - edit line
void list::on_elementLineEdit_returnPressed()
{
    //do the same as clicking the "Add element" - button
    ui->addElementButton->click();
}

//On Click listener for "done - button"
void list::on_doneButton_clicked()
{
    //if element is clicked and selected (you can see that it is clicked)
    //change font color, which show that element is done and set font to strikeout
    if(ui->listWidget->currentItem() != NULL && ui->listWidget->currentItem()->isSelected()){

        ui->listWidget->currentItem()->setForeground(Qt::green);

        font.setStrikeOut(true);
        ui->listWidget->currentItem()->setFont(font);

        //if element is "done" - can not be editable
        auto elementFlags = ui->listWidget->currentItem()->flags();
        ui->listWidget->currentItem()->setFlags(elementFlags & ~Qt::ItemIsEditable);

        //unselected all
        ui->listWidget->clearSelection();
    }
}

//On Click listener for double click on list element
//do the same as the "done - button"
void list::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    //get last pressed mouse button
    auto mouseButton = qApp->mouseButtons();

    //if last pressed mouse button is right button
    if(mouseButton & Qt::RightButton){

        item->setForeground(Qt::green);

        font.setStrikeOut(true);
        item->setFont(font);

        //if element is "done" - can not be editable
        auto elementFlags = ui->listWidget->currentItem()->flags();
        ui->listWidget->currentItem()->setFlags(elementFlags & ~Qt::ItemIsEditable);

        //unselected all
        item->setSelected(false);
        ui->listWidget->clearSelection();
    }
}

//On Click listener for "delete element - button"
void list::on_deleteButton_clicked()
{
    //if element is clicked and selected (you can see that it is clicked)
    if(ui->listWidget->currentItem() != NULL && ui->listWidget->currentItem()->isSelected()){

        //if element text color is green, change it to black and set strikeout to false
        //else delete element from list
        if(ui->listWidget->currentItem()->foreground() == Qt::green){

            ui->listWidget->currentItem()->setForeground(Qt::black);

            font.setStrikeOut(false);
            ui->listWidget->currentItem()->setFont(font);

            //if element is not "done" - can be editable
            auto elementFlags = ui->listWidget->currentItem()->flags();
            ui->listWidget->currentItem()->setFlags(elementFlags | Qt::ItemIsEditable);

        }else{

            ui->listWidget->takeItem(ui->listWidget->currentRow());

            //unselected all
            ui->listWidget->clearSelection();

            areThereElements();
        }
    }
}

//On Change Text listener for "line edit - new element"
//change text color to default after show warning
void list::on_elementLineEdit_textEdited(const QString)
{
    ui->elementLineEdit->setStyleSheet("");
}

//On Click listener on list item
void list::on_listWidget_itemClicked(QListWidgetItem)
{
    //change text color of "line edit" to default (in case if text is red)
    ui->elementLineEdit->setStyleSheet("");
}

//On item selection changed listener
void list::on_listWidget_itemSelectionChanged()
{
    //get last pressed mouse button
    auto mouseButton = qApp->mouseButtons();

    //if last pressed mouse button is right button
    if(mouseButton & Qt::RightButton){

        //change text color of "line edit" to default (in case if text are red)
        ui->elementLineEdit->setStyleSheet("");
    }
}

//Check for elements and disable "done element" and "undone/delete element" buttons if not
void list::areThereElements(){

    if(ui->listWidget->count() == 0){

        ui->doneButton->setDisabled(true);
        ui->deleteButton->setDisabled(true);

    }else{

        ui->doneButton->setDisabled(false);
        ui->deleteButton->setDisabled(false);
    }
}
