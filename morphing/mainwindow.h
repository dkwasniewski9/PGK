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
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void on_czesci_valueChanged(int arg1);

    void on_klatki_valueChanged(int arg1);

    void on_generateButton_clicked();

    void on_startButton_clicked();

    void on_delay_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QImage *img1, *img2, *img3;
    QImage *obraz1, *obraz2;
    int width, height;
    int startX1, startY1;
    int startX2, startY2;
    int startX3, startY3;
    std::pair<int, int> index1, index2;
    int ilosc_czesci;
    int ilosc_klatek;
    int delay_number;
    int tab_size;
    void clean();
    std::vector<QImage> klatki;
    std::pair<int, int> **punkty1;
    std::pair<int, int> **punkty2;
    void delay(int n);
    void siatka(int ilosc_kwadratow);
    void drawPixel(int x, int y, int obraz);
    void draw_punkt(int x, int y, int obraz);
    void draw_punkty(int obraz);
    void draw_czesci(int obraz);
    void blending();
    void czesc(std::pair<int, int> punkt1, std::pair<int, int> punkt2, std::pair<int, int> punkt3, int obraz);
    void linia(int x1, int y1, int x2, int y2, int obraz);
    void morphing();
    void animacja();
    void klatka(int n, std::vector<std::pair<int, int>> punkty1, std::vector<std::pair<int, int>> punkty2, QImage *frame);
    void teksturowanie(std::vector<std::pair<int, int>> punkty1, std::vector<std::pair<int, int>> punkty2, unsigned char *wsk1, unsigned char *wsk2);
    int przecina(int y1, int y2, int y);
    int xprzeciecia(int x1, int y1, int x2, int y2, int y);
    double barycentrycznew(int x, int y, std::vector<std::pair<int, int>> punkty);
    double barycentrycznev(int x, int y, std::vector<std::pair<int, int>> punkty);
    double Interpolacja(int c1, int c2, double z);
};
#endif // MAINWINDOW_H
