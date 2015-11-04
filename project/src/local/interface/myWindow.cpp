#include "myWindow.hpp"

#include "myWidgetGL.hpp"
#include "../../lib/common/error_handling.hpp"
#include "ui_mainwindow.h"

#include <iostream>

myWindow::myWindow(QWidget *parent)
    :QMainWindow(parent),ui(new Ui::MainWindow)
{
    try
    {
        //Setup window layout
        ui->setupUi(this);

        //Create openGL context
        QGLFormat qglFormat;
        qglFormat.setVersion(1,2);

        //Create OpenGL Widget renderer
        glWidget=new myWidgetGL(qglFormat);
        QGridLayout *layout = new QGridLayout;
        glWidget->setLayout(layout);

        //Initialization of some parameters
        glWidget->Bille(ui->Bille->isChecked());
        glWidget->Cube(ui->Cube_2->isChecked());
        glWidget->Chat(ui->Chat_2->isChecked());
        glWidget->get_value_K1(ui->K1_value->value());
        glWidget->get_value_K2(ui->K2_value->value());
        glWidget->get_value_K3(ui->K3_value->value());
        glWidget->get_value_KWind(ui->K_wind_value->value());
        glWidget->get_uWind_x_value(ui->x_value->value());
        glWidget->get_uWind_y_value(ui->y_value->value());
        glWidget->get_rayon_value(ui->rayon_size->value());

        //Add the OpenGL Widget into the layout
        ui->layout_scene->addWidget(glWidget);
    }
    catch(cpe::exception_cpe const& e)
    {
        std::cout<<std::endl<<e.report_exception()<<std::endl;
    }

    //Connect slot and signals
    //definiton connect SCENE
    connect(ui->wireframe,SIGNAL(clicked()),this,SLOT(action_wireframe())); //Affichage de la scene en version triangle
    connect(ui->reset,SIGNAL(clicked()),this,SLOT(action_reset()));
    connect(ui->quit,SIGNAL(clicked()),this,SLOT(action_quit()));

    //Object parameters
    connect(ui->Bille,SIGNAL(clicked()),this,SLOT(action_bille())); // Affichage de la sphere
    connect(ui->Cube_2, SIGNAL(clicked()),this, SLOT(action_cube()));
    connect(ui->rayon_size,SIGNAL(valueChanged(int)),this,SLOT(get_rayon_size()));
    connect(ui->Chat_2, SIGNAL(clicked()), this, SLOT(action_chat()));


    //definition connect PARAMETRES
    connect(ui->K1_value,SIGNAL(valueChanged(int)),this,SLOT(get_K1_value()));
    connect(ui->K2_value,SIGNAL(valueChanged(int)),this,SLOT(get_K2_value()));
    connect(ui->K3_value,SIGNAL(valueChanged(double)),this,SLOT(get_K3_value()));
    connect(ui->K_wind_value,SIGNAL(valueChanged(double)),this,SLOT(get_Kwind_value()));

    connect(ui->x_value,SIGNAL(valueChanged(int)),this,SLOT(get_uWind_x()));
    connect(ui->y_value,SIGNAL(valueChanged(int)),this,SLOT(get_uWind_y()));




}

myWindow::~myWindow()
{}

void myWindow::action_quit()
{
    close();
}

void myWindow::action_reset()
{
    //std::cout<<"test1"<<std::endl;
    bool const state_reset=ui->reset->isChecked();
    glWidget->reset(state_reset);
}

void myWindow::action_wireframe()
{
    bool const state_wireframe=ui->wireframe->isChecked();
    glWidget->wireframe(state_wireframe);
}

void myWindow::action_bille()
{
    bool const state_bille=ui->Bille->isChecked();
    ui->Cube_2->setCheckState(Qt::Unchecked);
    ui->Chat_2->setCheckState(Qt::Unchecked);
    glWidget->Cube(false);
    glWidget->Chat(false);
    glWidget->Bille(state_bille);
}

void myWindow::action_chat()
{
    bool const state_chat=ui->Chat_2->isChecked();
    ui->Cube_2->setCheckState(Qt::Unchecked);
    ui->Bille->setCheckState(Qt::Unchecked);
    glWidget->Cube(false);
    glWidget->Bille(false);
    glWidget->Chat(state_chat);
}

void myWindow::action_cube()
{
    bool const state_cube=ui->Cube_2->isChecked();
    ui->Bille->setCheckState(Qt::Unchecked);
    ui->Chat_2->setCheckState(Qt::Unchecked);
    glWidget->Chat(false);
    glWidget->Bille(false);
    glWidget->Cube(state_cube);
}

void myWindow::get_K1_value()
{
     int val_K1 = ui->K1_value->value();
     //std::cout<<val_K1<<std::endl;
     glWidget->get_value_K1(val_K1);
}

void myWindow::get_K2_value()
{
     int val_K2 = ui->K2_value->value();
     //std::cout<<val_K2<<std::endl;
     glWidget->get_value_K2(val_K2);
}

void myWindow::get_K3_value()
{
     float val_K3 = ui->K3_value->value();
     //std::cout<<val_K3<<std::endl;
     glWidget->get_value_K3(val_K3);
}

void myWindow::get_Kwind_value()
{
     float val_KWind = ui->K_wind_value->value();
     //std::cout<<val_KWind<<std::endl;
     glWidget->get_value_KWind(val_KWind);
}


void myWindow::get_uWind_x()
{
    int uWind_x = ui->x_value->value();
    //std::cout<<uWind_x<<std::endl;
    glWidget->get_uWind_x_value(uWind_x);
}

void myWindow::get_uWind_y()
{
    int uWind_y = ui->y_value->value();
    //std::cout<<uWind_y<<std::endl;
    glWidget->get_uWind_y_value(uWind_y);
}

void myWindow::get_rayon_size()
{
     int val_rayon = ui->rayon_size->value();
     //std::cout<<val_rayon<<std::endl;
     glWidget->get_rayon_value(val_rayon);
}
