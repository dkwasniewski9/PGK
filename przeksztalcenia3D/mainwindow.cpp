#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startX = ui->frame->x();
    startY = ui->frame->y();
    width = ui->frame->width();
    height = ui->frame->height();
    img = new QImage(width, height, QImage::Format_RGB32);
    szescian();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exitButton_clicked()
{
    qApp->quit();
}
void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.drawImage(startX, startY, *img);
}

void MainWindow::on_resetButton_clicked()
{
    ui->przesuniecie_x->setValue(0);
    ui->przesuniecie_y->setValue(0);
    ui->przesuniecie_z->setValue(0);
    przesunieciex = 0;
    przesunieciey = 0;
    przesunieciez = 0;
    ui->obroty_x->setValue(0);
    ui->obroty_y->setValue(0);
    ui->obroty_z->setValue(0);
    obrotyx = 0;
    obrotyy = 0;
    obrotyz = 0;
    ui->skalowanie_x->setValue(100);
    ui->skalowanie_y->setValue(100);
    ui->skalowanie_z->setValue(100);
    skalowaniex = 1;
    skalowaniey = 1;
    skalowaniez = 1;
    szescian();
}
void MainWindow::clean()
{
    unsigned char *wsk;
    for(int i=0; i<height; i++)
    {
        wsk = img->scanLine(i);
        for(int j=0; j<width; j++)
        {
            wsk[4*j] = 0;
            wsk[4*j+1] = 0;
            wsk[4*j+2] = 0;
        }
    }
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 4; ++j){
            wynik[i][j] = 0;
            skalowanie[i][j] = 0;
            obrot_x[i][j] = 0;
            obrot_y[i][j] = 0;
            obrot_z[i][j] = 0;
            przesuniecie[i][j] = 0;
        }
    }
    wynik[0][0] = 1;
    wynik[1][1] = 1;
    wynik[2][2] = 1;
    wynik[3][3] = 1;
    skalowanie[3][3] = 1;
    obrot_x[0][0] = 1;
    obrot_x[3][3] = 1;
    obrot_y[1][1] = 1;
    obrot_y[3][3] = 1;
    obrot_z[2][2] = 1;
    obrot_z[3][3] = 1;
    przesuniecie[0][0] = 1;
    przesuniecie[1][1] = 1;
    przesuniecie[2][2] = 1;
    przesuniecie[3][3] = 1;
}
void MainWindow::szescian()
{
    clean();
    punkty.clear();
    przesuniecia();
    skalowania();
    obroty();
    mnozenie(przesuniecie);
    mnozenie(skalowanie);
    mnozenie(obrot_x);
    mnozenie(obrot_y);
    mnozenie(obrot_z);
    rzutowanie(A[0],A[1], A[2], A[3]);
    rzutowanie(B[0],B[1], B[2], B[3]);
    rzutowanie(C[0],C[1], C[2], C[3]);
    rzutowanie(D[0],D[1], D[2], D[3]);
    rzutowanie(E[0],E[1], E[2], E[3]);
    rzutowanie(F[0],F[1], F[2], F[3]);
    rzutowanie(G[0],G[1], G[2], G[3]);
    rzutowanie(H[0],H[1], H[2], H[3]);
    rysuj();
    update();
}
void MainWindow::mnozenie(double macierz[4][4])
{
    double pom[4][4];
    double x;
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 4; ++j){
            x = 0;
            for(int k = 0; k < 4; k++){
                x += wynik[i][k] * macierz[k][j];
            }
            pom[i][j] = x;
        }
    }
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 4; ++j){
            wynik[i][j] = pom[i][j];
        }
    }
}
void MainWindow::rzutowanie(int x, int y, int z, int w)
{
    int x1, y1, z1, wynikx, wyniky;
    x1 = x * wynik[0][0] +  y * wynik[0][1] + z * wynik[0][2] + w * wynik[0][3];
    y1 = x * wynik[1][0] +  y * wynik[1][1] + z * wynik[1][2] + w * wynik[1][3];
    z1 = x * wynik[2][0] +  y * wynik[2][1] + z * wynik[2][2] + w * wynik[2][3];
    wynikx = x1*d/(z1 + d);
    wyniky = y1*d/(z1 + d);
    punkty.push_back(std::make_pair(250-wynikx, 250-wyniky));
}
void MainWindow::drawPixel(int x, int y)
{
    unsigned char *wsk;

    if(x>=0 && y>=0 && x<width && y<height)
    {
        wsk = img->scanLine(y);
        wsk[4*x] = 0;
        wsk[4*x+1] = 255;
        wsk[4*x+2] = 255;
    }

}
void MainWindow::draw_punkt(int x, int y){
    for(int j = 0; j < 11; ++j){
        for(int k = 0; k < 11; ++k){
            drawPixel(x-5+j, y-5+k);
        }
    }
    update();
}
void MainWindow::linia(int x1, int y1, int x2, int y2){
    double y;
    double x;
    double a, b;
    if(x1 > x2){
        std::swap(x1,x2);
        std::swap(y1,y2);
    }

    if(abs(x1-x2) > abs(y1 - y2)){
        a = (y2 - y1)/static_cast<double>(x2-x1);
        b = y1 - a * x1;
        for(int x = x1; x<=x2; ++x){
            y = a * x + b;
            drawPixel(x,round(y));
        }
    }
    else{
        if(y1 > y2){
            std::swap(x1,x2);
            std::swap(y1,y2);
        }
        a = (x2 - x1)/static_cast<double>(y2-y1);
        b = x1 - a * y1;
        for(int y = y1; y<=y2; ++y){
            x = a * y + b;
            drawPixel(round(x),y);
        }
    }
    update();
}

void MainWindow::przesuniecia()
{
    przesuniecie[0][3] = przesunieciex;
    przesuniecie[1][3] = przesunieciey;
    przesuniecie[2][3] = przesunieciez;

}

void MainWindow::obroty()
{
    obrot_x[1][1] = cos(obrotyx);
    obrot_x[1][2] = - sin(obrotyx);
    obrot_x[2][1] = sin(obrotyx);
    obrot_x[2][2] = cos(obrotyx);

    obrot_y[0][0] = cos(obrotyy);
    obrot_y[0][2] = sin(obrotyy);
    obrot_y[2][0] = - sin(obrotyy);
    obrot_y[2][2] = cos(obrotyy);

    obrot_z[0][0] = cos(obrotyz);
    obrot_z[0][1] = - sin(obrotyz);
    obrot_z[1][0] = sin(obrotyz);
    obrot_z[1][1] = cos(obrotyz);
}

void MainWindow::skalowania()
{
    skalowanie[0][0] = skalowaniex;
    skalowanie[1][1] = skalowaniey;
    skalowanie[2][2] = skalowaniez;
}

void MainWindow::rysuj()
{
    for(int i = 0; i < 3; ++i){
        linia(punkty.at(i).first, punkty.at(i).second, punkty.at(i+1).first, punkty.at(i+1).second);
    }
    linia(punkty.at(3).first, punkty.at(3).second, punkty.at(0).first, punkty.at(0).second);
    for(int i = 4; i < 7; ++i){
        linia(punkty.at(i).first, punkty.at(i).second, punkty.at(i+1).first, punkty.at(i+1).second);
    }
    linia(punkty.at(7).first, punkty.at(7).second, punkty.at(4).first, punkty.at(4).second);
    for(int i = 0; i < 4; ++i){
        linia(punkty.at(i).first, punkty.at(i).second, punkty.at(i+4).first, punkty.at(i+4).second);
    }
}

void MainWindow::on_przesuniecie_x_valueChanged(int value)
{
    przesunieciex = value;
    szescian();
}
void MainWindow::on_przesuniecie_y_valueChanged(int value)
{
    przesunieciey = value;
    szescian();
}
void MainWindow::on_przesuniecie_z_valueChanged(int value)
{
    przesunieciez = value;
    szescian();
}
void MainWindow::on_obroty_x_valueChanged(int value)
{
    obrotyx = value / 180.0 * M_PI;
    szescian();
}
void MainWindow::on_obroty_y_valueChanged(int value)
{
    obrotyy = value / 180.0 * M_PI;
    szescian();
}
void MainWindow::on_obroty_z_valueChanged(int value)
{
    obrotyz = value / 180.0 * M_PI;
    szescian();
}
void MainWindow::on_skalowanie_x_valueChanged(int value)
{
    skalowaniex = value / 100.0;
    szescian();
}
void MainWindow::on_skalowanie_y_valueChanged(int value)
{
    skalowaniey = value / 100.0;
    szescian();
}
void MainWindow::on_skalowanie_z_valueChanged(int value)
{
    skalowaniez = value / 100.0;
    szescian();
}
