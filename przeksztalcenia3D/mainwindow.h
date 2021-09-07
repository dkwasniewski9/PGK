    #ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QFileDialog>
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
    void on_exitButton_clicked();
    void paintEvent(QPaintEvent*);
    void on_resetButton_clicked();

    void on_przesuniecie_x_valueChanged(int value);
    void on_przesuniecie_y_valueChanged(int value);
    void on_przesuniecie_z_valueChanged(int value);

    void on_obroty_x_valueChanged(int value);
    void on_obroty_y_valueChanged(int value);
    void on_obroty_z_valueChanged(int value);

    void on_skalowanie_x_valueChanged(int value);
    void on_skalowanie_y_valueChanged(int value);
    void on_skalowanie_z_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QImage *img;
    int width, height;
    int startX, startY;
    std::vector<std::pair<int, int>> punkty;
    void clean();
    void szescian();
    void rzutowanie(int x, int y, int z, int w);
    void drawPixel(int x, int y);
    void draw_punkt(int x, int y);
    void linia(int x1, int y1, int x2, int y2);

    int przesunieciex = 0;
    int przesunieciey = 0;
    int przesunieciez = 0;
    double przesuniecie[4][4];
    void przesuniecia();

    double obrotyx = 0;
    double obrotyy = 0;
    double obrotyz = 0;
    double obrot_x[4][4];
    double obrot_y[4][4];
    double obrot_z[4][4];
    void obroty();

    double skalowaniex = 1;
    double skalowaniey = 1;
    double skalowaniez = 1;
    double skalowanie[4][4];
    void skalowania();

    void mnozenie(double macierz[4][4]);
    double wynik[4][4];
    void rysuj();
    int A[4] = {-50,50,-50,1};
    int B[4] = {50,50,-50,1};
    int C[4] = {50,50,50,1};
    int D[4] = {-50,50,50,1};
    int E[4] = {-50,-50,-50,1};
    int F[4] = {50,-50,-50,1};
    int G[4] = {50,-50,50,1};
    int H[4] = {-50,-50,50,1};
    int d = -500;
};
#endif // MAINWINDOW_H
