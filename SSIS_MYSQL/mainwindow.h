#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void display_students();
    void display_courses();
    void load_database();

    void add_student();
    void add_course();

    void delete_item();
    void delete_warning_students();

    void delete_courses();
    void delete_warning_courses();

    void CoursesCombobox();
    void setCustomHeaders();

    void handle_Deletion(const QString courseid);
    void searchDatabase(const QString &searchTerm);
    void searchresultsTable(QSqlQuery &query);

        void searchcourseTable(QSqlQuery &query);
        void search_courseDatabase(const QString &searchTerm);

        void clear_line_edits();


private slots:
    void on_pushButton_clicked();

    void on_addStudent_clicked();

    void on_pushButton_2_clicked();

    void on_addCourse_clicked();

    void on_search_Student_clicked();

    void on_searchCourse_clicked();


    void on_Edit_course_clicked();

    void edit_function();

    void update_edit();

    void on_Edit_clicked();

    void updatestudents(const QString &id, const QString &firstname, const QString &lastname, const QString &ID_number, const QString &Gender, const QString &YearLevel, const QString &courseCode);

    void on_Update_clicked();

    void edit_function_warning();

    void updateCourses(const QString &courseid, const QString &courseCode, const QString &courseName);
    void update_courses();
    void edit_course_function();
    void edit_course_function_warning();

    void on_UpdateCourse_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
