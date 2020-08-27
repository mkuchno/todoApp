#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QLayout>
#include <QFile>
#include <QTextStream>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //disable the option to hide the toolbar
    ui->toolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    //create and show "line edit" with "push button" in toolbar

    title = new QLineEdit(ui->toolBar);
    title->setPlaceholderText("change title of the list");
    title->setMaxLength(24);

    QFont font = title->font();
    font.setPointSize(10);
    title->setFont(font);

    setTitle = new QPushButton(ui->toolBar);
    setTitle->setText("Change");

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(title);
    layout->addWidget(setTitle);

    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    widget->setFixedWidth(300);

    QAction *action = ui->toolBar->addWidget(widget);
    action->setVisible(true);

    //Create spacer
    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacer->setVisible(true);

    //Show spacer and help action in toolbar
    ui->toolBar->addWidget(spacer);
    ui->toolBar->addAction(ui->actionHelp);

    //create connect to enable change of todo list title - after clicked "setTitle" button
    QObject::connect(setTitle, SIGNAL (clicked()), this, SLOT (setTitleClicked()));

    //create connect to enable change of todo list title - after clicked "Enter"
    QObject::connect(title, SIGNAL (returnPressed()), this, SLOT (setTitleEnter()));

    //create connect to catch when user edit "line edit"
    QObject::connect(title, SIGNAL (textEdited(QString)), this, SLOT (titleEditChanged()));

    //read data from file (last session)
    readData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//close event - function
void MainWindow::closeEvent(QCloseEvent *event){

    //save data to file (the last app condition)
    saveData();

    //execute close window
    event->accept();
}

void MainWindow::readData(){

    //-----------------
    // Read data
    //-----------------

    //read from the file line by line
    //first line means title of tab
    //lines below means elements of tab (if element contains \t before it means it is "done")
    //if line is blank, it means end of file or start new data block of next tab

    //-----------------

    QFile file("data.txt");
    file.open(QFile::ReadOnly | QFile::Text);

    QTextStream in(&file);

    //disable "change title form" and "delete list action"
    //they cannot be used if there are no lists
    if(file.size() == 0){

        title->setDisabled(true);
        setTitle->setDisabled(true);
        ui->actionDelete->setDisabled(true);
    }

    //variable distinguishing title from item
    bool name = true;

    QString tmpEl;

    //while - file pointer isnt in end of file
    while(!in.atEnd()){

        //if it is title
        //else if it is element
        if(name){

            //create new list, set title and save pointer to list
            listPointer = new list;
            ui->tabWidget->addTab(listPointer, in.readLine());

            name = false;

        }else{

            tmpEl = in.readLine();

            //if line is not blank
            //else if is blank
            if(tmpEl.length() != 0){

                //if element contains \t before it means it is "done"
                //else it is not "done"
                if(tmpEl.at(0) == "\t"){

                    //remove \t from element
                    //and add it to the list with "done" condition
                    tmpEl.remove(0,1);
                    listPointer->addElementToList(tmpEl, true);

                }else{

                    //add element to the list without "done" condition
                    listPointer->addElementToList(tmpEl, false);
                }

            }else{

                name = true;
            }
        }
    }

    file.close();
}

void MainWindow::saveData(){

    //-----------------
    // Save data
    //-----------------

    //save data to file line by line (any of tabs have data block, data blocks are separated with blank line)
    //first line of data block is title of tab
    //lines below are the elements of tab (if element contains \t before it means it is "done")

    //-----------------

    QFile file("data.txt");
    file.open(QFile::WriteOnly | QFile::Text);

    QTextStream out(&file);

    //Create empty pointer to QListWidget (list)
    QListWidget *elements;

    //for loop - for current tabs
    for(int i = 0; i < ui->tabWidget->count(); i++){

        //Find QListWidget child of our current tab
        //and save pointer to this
        elements = ui->tabWidget->widget(i)->findChild<QListWidget *>();

        //save title of tab in file
        out << ui->tabWidget->tabText(i) << "\n";

        //for loop - for elements of tab
        for(int j = 0; j < elements->count(); j++){

            //if element is not "done"
            //else if is "done"
            if(elements->item(j)->font().strikeOut() != true)

                //add element to file
                out << elements->item(j)->text() << "\n";

            else

                //add element to file with \t before
                out << "\t" << elements->item(j)->text() << "\n";
        }

        //if last element of tab is added, end this data block with blank line
        //but not if it is a last tab
        if(i != ui->tabWidget->count() - 1) out << "\n";
    }

    file.close();
}

//On Click listener for "Add new list action"
void MainWindow::on_actionNew_triggered()
{
    QMessageBox::Button clicked;
    clicked = QMessageBox::question(this,
                                    "New",
                                    "Are you sure to create new todo list?",
                                    QMessageBox::Yes | QMessageBox::No);

    //condition if user clicked "Yes" on MessageBox
    if(clicked == QMessageBox::Yes){

       //add a new tab to tabWidget and make it active
       list *newTab = new list;
       ui->tabWidget->addTab(newTab, tr("New"));
       ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(newTab));

       //change text color of "line edit" to default (in case if text are red)
       title->setStyleSheet("");

       //enable "change title form" and "delete list action"
       //they can be used after create new list
       title->setDisabled(false);
       setTitle->setDisabled(false);
       ui->actionDelete->setDisabled(false);
    }
}

//On Click listener for "Delete list action"
void MainWindow::on_actionDelete_triggered()
{
    if(ui->tabWidget->currentWidget() != NULL){

        QMessageBox::Button clicked;
        clicked = QMessageBox::question(this,
                                        "Delete",
                                        "Are you sure to delete this todo list?",
                                        QMessageBox::Yes | QMessageBox::No);

        //condition if user clicked "Yes" on MessageBox
        if(clicked == QMessageBox::Yes){

           //delete current tab
           ui->tabWidget->removeTab(ui->tabWidget->currentIndex());

           //change text color of "line edit" to default (in case if text are red)
           title->setStyleSheet("");

           //if any tab exist
           //get the pointer to line edit in tab next to the deleted one, and set text color to black
           if(ui->tabWidget->currentIndex() != -1){

               QLineEdit *tmp;
               tmp = ui->tabWidget->widget(ui->tabWidget->currentIndex())->findChild<QLineEdit *>();
               tmp->setStyleSheet("");
           }

           areThereLists();
        }
    }
}

//On Click listener for "Open help window action"
void MainWindow::on_actionHelp_triggered()
{
    QMessageBox help;

    help.setWindowTitle("Help");
    help.setTextFormat(Qt::RichText);

    help.setText("<h3>Shortcuts:</h3>"
                 "<ul>"
                 "<li>add new list: <strong style='color: grey;'>Ctrl+N</strong></li>"
                 "<li>delete current list: <strong style='color: grey;'>Ctrl+D</strong></li>"
                 "<li>open a help window: <strong style='color: grey;'>Ctrl+H</strong></li>"
                 "</ul>"
                 "<h3>Buttons Description:</h3>"
                 "<ol>"
                 "<li><h4 style='color: green;'>Done Button:</h4></li>"
                 "<ul>"
                 "<li>select item and click this button to mark done on it</li>"
                 "<li>you can click twice on item with right mouse button to do the same</li>"
                 "</ul>"
                 "<li><h4 style='color: red;'>Not Done or Delete Button:</h4></li>"
                 "<ul>"
                 "<li>if selected item is done, you can change it to not done</li>"
                 "<li>or you can delete an item if it's not done</li>"
                 "</ul>"
                 "</ol>"
                 "<h3>Hidden Functionality:</h3>"
                 "<ul>"
                 "<li>in the edit fields, to enter an item or change the title, you can click enter</li>"
                 "<li>you can change position of tabs and theirs elements with drag and drop</li>"
                 "<li>click twice with left mouse button on not done element to edit him</li>"
                 "<li>click twice with left or right mouse button on list title to automatically copy it to change title form</li>"
                 "</ul>");

    help.exec();
}

//On Click listener for "Set title button"
void MainWindow::setTitleClicked(){

    if(ui->tabWidget->currentWidget() != NULL){

        //if text is empty show warning, else change title of current tab
        if(title->text() == ""){

            //warning is change text color to red
            title->setStyleSheet("color: red;");

        }else{

            ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), title->text());

            //clean line edit - "set title"
            title->setText("");
        }
    }
}

//Enter listener in "line edit - set title"
void MainWindow::setTitleEnter(){

    //do the same as clicking the "Set title button"
    setTitle->click();
}

//Change text listener for "line edit - set title"
//change text color to default after show warning
void MainWindow::titleEditChanged(){

    title->setStyleSheet("");
}

//tab name click listener
void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    //get last pressed mouse button
    auto mouseButton = qApp->mouseButtons();

    //if last pressed mouse button is left button
    if(mouseButton & Qt::LeftButton){

        //change text color to default after show warning in line edit "set title"
        title->setStyleSheet("");
    }

    //get the pointer to line edit in tab and set text color to black
    QLineEdit *tmp;
    tmp = ui->tabWidget->widget(index)->findChild<QLineEdit *>();
    tmp->setStyleSheet("");
}

//tab name double click listener
//copy title of tab to "line edit - set title"
void MainWindow::on_tabWidget_tabBarDoubleClicked(int index)
{
    //change text color to default after show warning in line edit "set title"
    title->setStyleSheet("");

    QString toCopy = ui->tabWidget->tabText(index);
    title->setText(toCopy);
}

//Check for lists and disable "set title form" and "delete list action" if not
void MainWindow::areThereLists(){

    if(ui->tabWidget->currentWidget() == NULL){

        title->setDisabled(true);
        setTitle->setDisabled(true);
        ui->actionDelete->setDisabled(true);
    }

    else{

        title->setDisabled(false);
        setTitle->setDisabled(false);
        ui->actionDelete->setDisabled(false);
    }
}
