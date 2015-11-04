#pragma once

#ifndef MY_WINDOW_HPP
#define MY_WINDOW_HPP


#include <QMainWindow>




//forward declaration
namespace Ui
{
class MainWindow;
}
class myWidgetGL;

/** Declaration of the Window class */
class myWindow: public QMainWindow
{
    Q_OBJECT

public:

    myWindow(QWidget *parent=NULL);
    ~myWindow();



private slots:

    /** Quit the application */
    void action_quit();
    /** Enable the drawing of the meshes */
    void action_reset();
    /** Set the Wireframe mode for the meshes */
    void action_wireframe();

    void action_bille();

    void action_cube();

    void action_chat();

    void get_K1_value();
    void get_K2_value();
    void get_K3_value();
    void get_Kwind_value();


    void get_uWind_x();
    void get_uWind_y();
    void get_rayon_size();

private:

    /** Layout for the Window */
    Ui::MainWindow *ui;
    /** The OpenGL Widget */
    myWidgetGL *glWidget;





};

#endif
