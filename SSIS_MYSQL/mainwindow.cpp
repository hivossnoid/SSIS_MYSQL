#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtSql>
#include <QtDebug>
#include <QApplication>
#include <QtSql>
#include <QtSql/QSqlQuery>
#include <QtDebug>
#include <QApplication>
#include <QMessageBox>
#include <QModelIndex>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << QSqlDatabase::drivers(); // List of Available Drivers

    load_database();
    display_students();
    display_courses();

    CoursesCombobox();//for the courses combobox

    ui->Gender->addItem("Male");//for the gender combobox
    ui->Gender->addItem("Female");//for the gender combobox

    ui->YearLevel->addItem("1st Year");//for the year level comboboxes
    ui->YearLevel->addItem("2nd Year");
    ui->YearLevel->addItem("3rd Year");
    ui->YearLevel->addItem("4th Year");

    setCustomHeaders();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CoursesCombobox()
{
    QSqlQuery query("SELECT courseCode FROM courses");

    if (!query.exec()) {
        qDebug() << "Error: Could not execute query.";
        qDebug() << query.lastError().text();
        return;
    }

    while (query.next()) {
        QString name = query.value(0).toString();
        ui->courseID->addItem(name);
    }
}

void MainWindow::clear_line_edits(){
     ui->studentName->clear();
     ui->lastName->clear();
     ui->studentID->clear();

     ui->course_ID->clear();
     ui->courseName->clear();
}

void MainWindow::setCustomHeaders()
{
    QStringList headers;
    headers << "" << "First Name" << "Last Name" << "ID Number" << "Gender"<< "Year Level" << "" << "Course Code";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    QStringList courseheaders;
    courseheaders << "" << "Course Code" << "Course Name";
      ui->tableWidget_2->setHorizontalHeaderLabels(courseheaders);
}

void MainWindow::load_database()
{
    QCoreApplication::addLibraryPath("C:/Qt/6.7.0/mingw_64/plugins");

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    if (!db.isValid()) {
        qDebug() << "FAILED TO LOAD DRIVER" << QSqlDatabase::drivers();
    }

    db.setHostName("localhost");
    db.setDatabaseName("ssis_mysql");
    db.setUserName("root");
    db.setPassword("Antarisks159357");

    if (!db.open()) {
        qDebug() << "Database not connected" << db.lastError();
    } else {
        qDebug() << "Database connected";
    }
}

void MainWindow::display_students()
{
    QSqlQuery query("SELECT * FROM students");

    if (!query.exec()) {
        qDebug() << "Error: Could not execute query.";
        qDebug() << query.lastError().text();
        return;
    }
    int columnCount = query.record().count();
    ui->tableWidget->setColumnCount(columnCount);

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < columnCount; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            ui->tableWidget->setItem(row, col, item);
        }
        ++row;
    }
     ui->tableWidget->setColumnHidden(0, true);
    ui->tableWidget->setColumnHidden(6, true);
}

void MainWindow::add_student()
{
    QString firstname = ui->studentName->text();
    QString lastname = ui->lastName->text();
    QString ID_number = ui->studentID->text();
    QString Gender = ui->Gender->currentText();
    QString YearLevel = ui->YearLevel->currentText();
    QString courseCode = ui->courseID->currentText();

    if (lastname.isEmpty() || firstname.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter all the required fields.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO students(firstname, lastname, IDnumber, Gender, yearLevel, courseCode) "
                  "VALUES (:firstname, :lastname, :IDNumber, :Gender, :yearLevel, :courseCode)");
    query.bindValue(":firstname", firstname);
    query.bindValue(":lastname", lastname);
    query.bindValue(":IDNumber", ID_number);
    query.bindValue(":Gender", Gender);
    query.bindValue(":yearLevel", YearLevel);
    query.bindValue(":courseCode", courseCode);


    if (!query.exec()) {
        qDebug() << "Error: Could not execute query.";
        qDebug() << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Could not add student to the database.");
        return;
    }

    QMessageBox::information(this, "Success", "Student added successfully.");
        ui->studentName->clear();
        ui->LastName->clear();
        ui->studentID->clear();
}

void MainWindow::add_course()
{
    QString courseCode= ui->course_ID->text();
    QString courseName= ui->courseName->text();

    if (courseCode.isEmpty() || courseName.isEmpty())
    {
        QMessageBox::warning(this, "Input Error", "Please enter all the required fields.");
        return;
    }
    QSqlQuery query;
    query.prepare("INSERT INTO courses(courseCode, courseName)"
                  "values (:courseCode, :courseName)");
    query.bindValue(":courseCode", courseCode);
    query.bindValue(":courseName", courseName);


    if (!query.exec()) {
        qDebug() << "Error: Could not execute action.";
        qDebug() << query.lastError().text();
        QMessageBox::critical(this, "Input Error", "Could not add course to the database.");
        return;
    }

    QMessageBox::information(this, "Success", "Course added successfully.");
    ui->courseID->clear();
    ui->courseName->clear();
}

void MainWindow::display_courses()
{
    QSqlQuery query("SELECT * FROM courses");

    if (!query.exec()) {
        qDebug() << "Error: Could not execute query.";
        qDebug() << query.lastError().text();
        return;
    }

    // Get column count
    int columnCount = query.record().count();
    ui->tableWidget_2->setColumnCount(columnCount);

    // Populate rows
    int row = 0;
    while (query.next())
    {
        ui->tableWidget_2->insertRow(row);
        for (int col = 0; col < columnCount; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            ui->tableWidget_2->setItem(row, col, item);
        }
        ++row;
    }
    ui->tableWidget_2->setColumnHidden(0, true);
}

void MainWindow::delete_warning_students()
{
    int ret = QMessageBox::warning(this, tr("Delete Item"),
                                   tr("Are you sure you want to delete student?"),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
        delete_item();
    }
}

void MainWindow::delete_warning_courses()
{
    int ret = QMessageBox::warning(this, tr("Delete Item"),
                                   tr("Are you sure you want to delete course?"),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
        delete_courses();
    }
}


void MainWindow::delete_item()
{
    // Get the selected items
    QList<QTableWidgetSelectionRange> selectedRanges = ui->tableWidget->selectedRanges();

    if (selectedRanges.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select a student to delete.");
        return;
    }
    for (int i = selectedRanges.count() - 1; i >= 0; --i) {
        int row = selectedRanges.at(i).topRow();

        QString id = ui->tableWidget->item(row, 0)->text();
        QSqlQuery query;
        query.prepare("DELETE FROM students WHERE id = :id");

        query.bindValue(":id", id);

        if (!query.exec()) {
            qDebug() << "Error: Could not execute query.";
            qDebug() << query.lastError().text();
            QMessageBox::critical(this, "Delete Error", "Could not delete student from the database.");
            return;
        }

        ui->tableWidget->removeRow(row);
    }
}

void MainWindow::delete_courses()
{
    QList<QTableWidgetSelectionRange> selectedRanges = ui->tableWidget_2->selectedRanges();
    if (selectedRanges.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select a course to delete.");
        return;
    }
    for (int i = selectedRanges.count() - 1; i >= 0; --i) {
        int row = selectedRanges.at(i).topRow();

        QString courseCode = ui->tableWidget_2->item(row, 1)->text();
        QSqlQuery query;
        query.prepare("DELETE FROM courses WHERE courseCode = :courseCode");
             query.prepare("DELETE FROM courses WHERE courseCode = :courseCode");
        query.bindValue(":courseCode", courseCode);

        if (!query.exec()) {
            qDebug() << "Error: Could not delete course.";
            qDebug() << query.lastError().text();
            QMessageBox::critical(this, "Delete Error", "Could not delete course from the database.");
            return;
        }
        ui->tableWidget_2->removeRow(row);
        handle_Deletion(courseCode);
    }
}

void MainWindow::handle_Deletion(const QString courseCode)
{
    qDebug() << courseCode;
    QSqlQuery query;
    query.prepare("UPDATE students SET courseCode = NULL WHERE courseCode = :courseCode");
    query.bindValue(":courseCode", courseCode);

    if (!query.exec()) {
        qDebug() << "Error: Could not update students.";
        qDebug() << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Could not update students.");
    }
    if (!query.exec()) {
        qDebug() << "Error: Could not update students.";
        qDebug() << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Could not update the students.");
    }
}

void MainWindow::searchDatabase(const QString &searchTerm)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM students WHERE firstname LIKE :searchTerm "
                  "OR lastname LIKE :searchTerm "
                  "OR IDnumber LIKE :searchTerm");
    query.bindValue(":searchTerm", "%" + searchTerm + "%");

    if (!query.exec()) {
        qDebug() << "Error: Could not execute search query.";
        qDebug() << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Could not execute search query.");
        return;
    }

    searchresultsTable(query);
}

void MainWindow::searchresultsTable(QSqlQuery &query)
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);

    int columnCount = query.record().count();
    ui->tableWidget->setColumnCount(columnCount);

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        for (int column = 0; column < columnCount; ++column) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(column).toString());
            ui->tableWidget->setItem(row, column, item);
        }
        ++row;
    }
}


void MainWindow::search_courseDatabase(const QString &searchTerm)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM courses WHERE courseCode LIKE :searchTerm "
                  "OR courseName LIKE :searchTerm ");
    query.bindValue(":searchTerm", "%" + searchTerm + "%");

    if (!query.exec()) {
        qDebug() << "Error: Could not execute search query.";
        qDebug() << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Could not execute search query.");
        return;
    }
    searchcourseTable(query);
}

void MainWindow::searchcourseTable(QSqlQuery &query)
{
    ui->tableWidget_2->clear();
    ui->tableWidget_2->setRowCount(0);

    int columnCount = query.record().count();
    ui->tableWidget_2->setColumnCount(columnCount);

    int row = 0;
    while (query.next()) {
        ui->tableWidget_2->insertRow(row);
        for (int column = 0; column < columnCount; ++column) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(column).toString());
            ui->tableWidget_2->setItem(row, column, item);
        }
        ++row;
    }
}

void MainWindow::edit_function_warning()
{
int ret = QMessageBox::warning(this, tr("Edit Item"),
                               tr("Are you sure you want to edit student?"),
                               QMessageBox::Yes | QMessageBox::No,
                               QMessageBox::No);
if (ret == QMessageBox::Yes)
{
    edit_function();
}

}

void MainWindow::edit_function()
{
    QModelIndexList selectedIndexes = ui->tableWidget->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) return;

    int selectedRow = selectedIndexes.first().row();

    QTableWidgetItem *firstname = ui->tableWidget->item(selectedRow, 1);
    QTableWidgetItem *lastname = ui->tableWidget->item(selectedRow, 2);
    QTableWidgetItem *ID_number = ui->tableWidget->item(selectedRow, 3);

    ui->studentName->setText(firstname ? firstname->text() : "");
    ui->lastName->setText(lastname ? lastname->text() : "");
    ui->studentID->setText(ID_number ? ID_number->text() : "");
}

void MainWindow::update_edit()
{
    QModelIndexList selectedIndexes = ui->tableWidget->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "No selection", "No row selected");
        return;
    }
      int selectedRow = selectedIndexes.first().row();
    QString id = ui->tableWidget->item(selectedRow, 0)->text();
            QString firstname = ui->studentName->text();
                QString lastname = ui->lastName->text();
                  QString IDnumber = ui->studentID->text();
                    QString Gender = ui->Gender->currentText();
                        QString yearLevel = ui->YearLevel->currentText();
                            QString courseCode = ui->courseID->currentText();

        updatestudents(id, firstname, lastname, IDnumber, Gender, yearLevel, courseCode);

          QMessageBox::information(this, "Success", "Student updated successfully.");
}

void MainWindow::edit_course_function_warning()
{
    int ret = QMessageBox::warning(this, tr("Edit Item"),
                                   tr("Are you sure you want to edit course?"),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
        edit_course_function();
    }
}

void MainWindow::edit_course_function()
{
    QModelIndexList selectedIndexes = ui->tableWidget_2->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) return;

    int selectedRow = selectedIndexes.first().row();

    QTableWidgetItem *courseCode = ui->tableWidget_2->item(selectedRow, 1);
    QTableWidgetItem *courseName = ui->tableWidget_2->item(selectedRow, 2);

    ui->course_ID->setText(courseCode ? courseCode->text() : "");
    ui->courseName->setText(courseName ? courseName->text() : "");
}


void MainWindow::update_courses()
{
    QModelIndexList selectedIndexes = ui->tableWidget_2->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "No selection", "No row selected");
        return;
    }
    int selectedRow = selectedIndexes.first().row();
    QString courseid = ui->tableWidget_2->item(selectedRow, 0)->text();
    QString courseCode = ui->course_ID->text();
    QString courseName = ui->courseName->text();

    updateCourses(courseid, courseCode, courseName);

    QMessageBox::information(this, "Success", "Courses updated successfully.");
}


void MainWindow::updateCourses(const QString &courseid, const QString &courseCode, const QString &courseName)
{
    QSqlQuery query;
    query.prepare("UPDATE courses SET courseCode = :courseCode, courseName = :courseName WHERE courseid = :courseid");
    query.bindValue(":courseCode", courseCode);
    query.bindValue(":courseName", courseName);
    query.bindValue(":courseid", courseid);

    if (!query.exec()) {
        qDebug() << "Error: Could not update the database.";
        qDebug() << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Could not update the database.");
        return;
    }
    qDebug() << "Course information updated successfully.";
}


void MainWindow::updatestudents(const QString &id, const QString &firstname, const QString &lastname, const QString &IDnumber, const QString &Gender, const QString &yearLevel, const QString &courseCode)
{
    QSqlQuery query;
    query.prepare("UPDATE students SET firstname = :firstname, lastname = :lastname, IDnumber = :IDnumber, Gender = :Gender, yearLevel = :yearLevel, courseCode = :courseCode WHERE id = :id");
    query.bindValue(":firstname", firstname);
    query.bindValue(":lastname", lastname);
    query.bindValue(":IDnumber", IDnumber);
    query.bindValue(":Gender", Gender);
    query.bindValue(":yearLevel", yearLevel);
    query.bindValue(":courseCode", courseCode);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error: Could not update the database.";
        qDebug() << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Could not update the database.");
        return;
    }
    qDebug() << "Student informantion updated successfully.";
}


void MainWindow::on_pushButton_clicked()
{
    delete_warning_students();
        clear_line_edits();
    setCustomHeaders();
}


void MainWindow::on_addStudent_clicked()
{
    add_student();
    ui->tableWidget->clear();
        clear_line_edits();
    display_students();
    setCustomHeaders();
}


void MainWindow::on_pushButton_2_clicked()
{
    delete_warning_courses();
    ui->tableWidget->clear();
        clear_line_edits();
    display_students();
    setCustomHeaders();
    CoursesCombobox();
}


void MainWindow::on_addCourse_clicked()
{
    add_course();
    ui->tableWidget_2->clear();
        clear_line_edits();
    display_courses();
    setCustomHeaders();
    CoursesCombobox();
}


void MainWindow::on_search_Student_clicked()
{
    QString searchTerm = ui->lineEdit->text();
    if (searchTerm.isEmpty()) {
        QMessageBox::warning(this, "Empty Input", "Please enter a search term.");
        return;
    }
    searchDatabase(searchTerm);
    setCustomHeaders();
        clear_line_edits();
}


void MainWindow::on_searchCourse_clicked()
{
    QString searchTerm = ui->lineEdit_2->text();
    if (searchTerm.isEmpty()) {
        QMessageBox::warning(this, "Empty Input", "Please enter a search term.");
        return;
    }
    search_courseDatabase(searchTerm);
    setCustomHeaders();
    clear_line_edits();
}

void MainWindow::on_Edit_clicked()
{
    edit_function_warning();
}

void MainWindow::on_Update_clicked()
{
    update_edit();
    ui->tableWidget->clear();
        clear_line_edits();
            display_students();
                 setCustomHeaders();
}


void MainWindow::on_Edit_course_clicked()
{
    edit_course_function_warning();
}



void MainWindow::on_UpdateCourse_clicked()
{
    update_courses();
    ui->tableWidget_2->clear();
    clear_line_edits();
    display_courses();
    setCustomHeaders();
}

