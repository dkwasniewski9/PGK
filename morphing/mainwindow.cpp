#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startX1 = ui->frame->x();
    startY1 = ui->frame->y();
    startX2 = ui->frame2->x();
    startY2 = ui->frame2->y();
    startX3 = ui->frame3->x();
    startY3 = ui->frame3->y();
    width = ui->frame->width();
    height = ui->frame->height();
    ilosc_czesci = 0;
    ilosc_klatek = 0;
    index1.first = -1;
    index1.second = -1;
    index2.first = -1;
    index2.second = -1;
    delay_number = 30;
    tab_size = 0;
    img1 = new QImage(width, height, QImage::Format_RGB32);
    img2 = new QImage(width, height, QImage::Format_RGB32);
    img3 = new QImage(width, height, QImage::Format_RGB32);
    obraz1 = new QImage(":/obrazy/1.jpg");
    obraz2 = new QImage(":/obrazy/2.jpg");
    clean();
}


MainWindow::~MainWindow()
{
    delete img1;
    delete img2;
    delete img3;
    delete obraz1;
    delete obraz2;
    for(int i = 0; i < tab_size; ++i){
        delete punkty1[i];
        delete punkty2[i];
    }
    if(tab_size != 0){
        delete punkty1;
        delete punkty2;
    }
    delete ui;
}

void MainWindow::on_exitButton_clicked()
{
    qApp->quit();
}
void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.drawImage(startX1, startY1, *img1);
    p.drawImage(startX2, startY2, *img2);
    p.drawImage(startX3, startY3, *img3);
}

void MainWindow::clean()
{
    *img1 = obraz1 -> copy(0, 0, width, height);
    *img2 = obraz2 -> copy(0, 0, width, height);
    unsigned char *wsk;
    for(int i=0; i<height; i++)
    {
        wsk = img3->scanLine(i);
        for(int j=0; j<width; j++)
        {
            wsk[4*j] = 0;
            wsk[4*j+1] = 0;
            wsk[4*j+2] = 0;
        }
    }
}

void MainWindow::delay(int n){
    QTime dieTime= QTime::currentTime().addMSecs(n);
    while (QTime::currentTime() < dieTime) QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void MainWindow::siatka(int ilosc_czesci)
{
    int ilosc_punktow = ilosc_czesci + 1;
    for(int i = 0; i < tab_size; ++i){
        delete punkty1[i];
        delete punkty2[i];
    }
    punkty1 = new std::pair<int,int>*[ilosc_punktow];
    punkty2 = new std::pair<int,int>*[ilosc_punktow];
    for(int i = 0; i < ilosc_punktow; i++){
        punkty1[i] = new std::pair<int,int>[ilosc_punktow];
        punkty2[i] = new std::pair<int,int>[ilosc_punktow];
        for(int j = 0; j < ilosc_punktow; j++){
            punkty1[i][j].first = width * (i/(double)ilosc_czesci);
            punkty1[i][j].second = height * (j/(double)ilosc_czesci);
            punkty2[i][j].first = width * (i/(double)ilosc_czesci);
            punkty2[i][j].second = height * (j/(double)ilosc_czesci);
        }
    }
    tab_size = ilosc_punktow;
    clean();
    draw_punkty(1);
    draw_punkty(2);
    draw_czesci(1);
    draw_czesci(2);
}
void MainWindow::on_resetButton_clicked()
{
    clean();
    siatka(ilosc_czesci);
    update();
}

void MainWindow::drawPixel(int x, int y, int obraz)
{
    unsigned char *wsk;
    if(obraz == 1 && x > 0 && y > 0 && x < (width - 1) && y < (height - 1)){
        wsk = img1->scanLine(y);
    }
    else if(obraz == 2 && x > 0 && y > 0 && x < (width - 1) && y < (height - 1)){
        wsk = img2->scanLine(y);
    }
    else{
        return;
    }
    wsk[4*x] = 0;
    wsk[4*x+1] = 0;
    wsk[4*x+2] = 255;
}
void MainWindow::draw_punkt(int x, int y, int obraz){
    for(int j = 0; j < 11; ++j){
        for(int k = 0; k < 11; ++k){
            drawPixel(x-5+j, y-5+k, obraz);
        }
    }
    update();
}

void MainWindow::draw_punkty(int obraz)
{
    if(obraz == 1){
        for(int i = 1; i < ilosc_czesci; ++i){
            for(int j = 1; j < ilosc_czesci; ++j){
                draw_punkt(punkty1[i][j].first,punkty1[i][j].second, 1);
            }
        }
    }
    else if(obraz == 2){
        for(int i = 1; i < ilosc_czesci; ++i){
            for(int j = 1; j < ilosc_czesci; ++j){
                draw_punkt(punkty2[i][j].first,punkty2[i][j].second, 2);
            }
        }
    }
}

void MainWindow::draw_czesci(int obraz)
{
    if(obraz == 1){
        for(int i = 0; i < ilosc_czesci; ++i){
            for(int j = 0; j < ilosc_czesci; ++j){
                czesc(punkty1[i][j], punkty1[i + 1][j], punkty1[i + 1][j + 1], 1);
            }
        }
    }
    else if(obraz == 2){
        for(int i = 0; i < ilosc_czesci; ++i){
            for(int j = 0; j < ilosc_czesci; ++j){
                czesc(punkty2[i][j], punkty2[i + 1][j], punkty2[i + 1][j + 1], 2);
            }
        }
    }
    update();
}

void MainWindow::czesc(std::pair<int, int> punkt1, std::pair<int, int> punkt2, std::pair<int, int> punkt3, int obraz)
{
    linia(punkt1.first, punkt1.second, punkt2.first, punkt2.second, obraz);
    linia(punkt2.first, punkt2.second, punkt3.first, punkt3.second, obraz);
    linia(punkt3.first, punkt3.second, punkt1.first, punkt1.second, obraz);
}
void MainWindow::linia(int x1, int y1, int x2, int y2, int obraz){
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
            drawPixel(x,round(y), obraz);
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
            drawPixel(round(x),y, obraz);
        }
    }
    update();
}

void MainWindow::morphing()
{
    QImage *obraz = new QImage(width, height, QImage::Format_RGB32);
    std::vector<std::pair<int, int>> trojkat1, trojkat2;
    klatki.clear();
    for(int n = 0; n <= ilosc_klatek; ++n){
        for(int i = 0; i < ilosc_czesci; ++i){
            for(int j = 0; j < ilosc_czesci; ++j){
                trojkat1.push_back(punkty1[i][j]);
                trojkat1.push_back(punkty1[i + 1][j]);
                trojkat1.push_back(punkty1[i + 1][j + 1]);
                trojkat2.push_back(punkty2[i][j]);
                trojkat2.push_back(punkty2[i + 1][j]);
                trojkat2.push_back(punkty2[i + 1][j + 1]);
                klatka(n, trojkat1, trojkat2, obraz);
                trojkat1.clear();
                trojkat2.clear();
                trojkat1.push_back(punkty1[i][j]);
                trojkat1.push_back(punkty1[i][j + 1]);
                trojkat1.push_back(punkty1[i + 1][j + 1]);
                trojkat2.push_back(punkty2[i][j]);
                trojkat2.push_back(punkty2[i][j + 1]);
                trojkat2.push_back(punkty2[i + 1][j + 1]);
                klatka(n, trojkat1, trojkat2, obraz);
                trojkat1.clear();
                trojkat2.clear();
            }
        }
        ui->progressBar->setValue(n/(double) ilosc_klatek * 100);
        klatki.push_back(obraz -> copy());
    }
    delete obraz;
}

void MainWindow::animacja()
{
    for(int i = 0; i < (int) klatki.size(); ++i){
        *img3 = klatki.at(i).copy();
        repaint();
        delay(delay_number);
    }
}

void MainWindow::klatka(int n, std::vector<std::pair<int, int>> trojkat1, std::vector<std::pair<int, int>> trojkat2, QImage *frame)
{
    double x, y;
    int r1, g1, b1, r2, g2, b2;
    int ymin = height;
    int ymax = 0;
    std::vector <int> przeciecia;
    std::vector<std::pair<int, int>> punkty;
    QImage *PQR;
    QImage *PQR2;
    x = (1 - n/(double)ilosc_klatek) * trojkat1.at(0).first + n/(double)ilosc_klatek * trojkat2.at(0).first;
    y = (1 - n/(double)ilosc_klatek) * trojkat1.at(0).second + n/(double)ilosc_klatek * trojkat2.at(0).second;
    punkty.push_back(std::make_pair(x, y));
    x = (1 - n/(double)ilosc_klatek) * trojkat1.at(1).first + n/(double)ilosc_klatek * trojkat2.at(1).first;
    y = (1 - n/(double)ilosc_klatek) * trojkat1.at(1).second + n/(double)ilosc_klatek * trojkat2.at(1).second;
    punkty.push_back(std::make_pair(x, y));
    x = (1 - n/(double)ilosc_klatek) * trojkat1.at(2).first + n/(double)ilosc_klatek * trojkat2.at(2).first;
    y = (1 - n/(double)ilosc_klatek) * trojkat1.at(2).second + n/(double)ilosc_klatek * trojkat2.at(2).second;
    punkty.push_back(std::make_pair(x, y));
    PQR = new QImage(width, height, QImage::Format_RGB32);
    PQR2 = new QImage(width, height, QImage::Format_RGB32);
    unsigned char *wsk, *wsk2, *wsk3;
    teksturowanie(trojkat1, punkty, obraz1->scanLine(0), PQR->scanLine(0));
    teksturowanie(trojkat2, punkty, obraz2->scanLine(0), PQR2->scanLine(0));
    for(int i = 0; i < (int) punkty.size(); ++i){
        if(punkty.at(i).second < ymin) ymin = punkty.at(i).second;
        if(punkty.at(i).second > ymax) ymax = punkty.at(i).second;
    }
    for(int y = ymin; y <= ymax; ++y){
        for(int i = 0; i < (int) punkty.size() - 1; ++i){
            if(przecina(punkty.at(i).second, punkty.at(i + 1).second, y) == 1){
                przeciecia.push_back(xprzeciecia(punkty.at(i).first, punkty.at(i).second, punkty.at(i + 1).first, punkty.at(i + 1).second, y));
            }
        }
        if(przecina(punkty.front().second, punkty.back().second, y) == 1){
            przeciecia.push_back(xprzeciecia(punkty.front().first, punkty.front().second, punkty.back().first, punkty.back().second, y));
        }
        std::sort(przeciecia.begin(), przeciecia.end());
        while(przeciecia.size() >= 2){
            for(int i = przeciecia.front() + 1; i <= przeciecia.at(1) ; ++i){
                wsk = PQR->scanLine(y);
                wsk2 = PQR2->scanLine(y);
                wsk3 = frame->scanLine(y);
                r1 = wsk[4*i+2];
                g1 = wsk[4*i+1];
                b1 = wsk[4*i];
                r2 = wsk2[4*i+2];
                g2 = wsk2[4*i+1];
                b2 = wsk2[4*i];
                wsk3[4*i] = (1 - n/(double)ilosc_klatek) * b1 + n/(double)ilosc_klatek * b2;
                wsk3[4*i+1] = (1 - n/(double)ilosc_klatek) * g1 + n/(double)ilosc_klatek * g2;
                wsk3[4*i+2] = (1 - n/(double)ilosc_klatek) * r1 + n/(double)ilosc_klatek * r2;
            }
            przeciecia.erase(przeciecia.begin(), przeciecia.begin() + 2);
        }
        przeciecia.clear();
    }
    delete PQR;
    delete PQR2;
}
int MainWindow::przecina(int y1, int y2, int y){
    if((y1 > y && y2 <= y) || (y1 <= y && y2 > y)) return 1;
    else return -1;
}
int MainWindow::xprzeciecia(int x1, int y1, int x2, int y2, int y){
    double a, b, x;
    a = (x2 - x1)/static_cast<double>(y2-y1);
    b = x1 - a * y1;
    x = a * y + b;
    return round(x);
}
double MainWindow::barycentrycznew(int x, int y, std::vector<std::pair<int, int> > punkty)
{
    double licznik, mianownik;
    licznik = (punkty.at(1).first - punkty.at(0).first) * (y - punkty.at(0).second) - (x - punkty.at(0).first) * (punkty.at(1).second - punkty.at(0).second);
    mianownik = (punkty.at(1).first - punkty.at(0).first) * (punkty.at(2).second - punkty.at(0).second) - (punkty.at(2).first - punkty.at(0).first) * (punkty.at(1).second - punkty.at(0).second);
    return licznik/mianownik;
}

double MainWindow::barycentrycznev(int x, int y, std::vector<std::pair<int, int> > punkty)
{
    double licznik, mianownik;
    licznik = (x - punkty.at(0).first) * (punkty.at(2).second - punkty.at(0).second) - (punkty.at(2).first - punkty.at(0).first) * (y - punkty.at(0).second);
    mianownik = (punkty.at(1).first - punkty.at(0).first) * (punkty.at(2).second - punkty.at(0).second) - (punkty.at(2).first - punkty.at(0).first) * (punkty.at(1).second - punkty.at(0).second);
    return licznik/mianownik;
}
double MainWindow::Interpolacja(int c1, int c2, double z)
{
    return (1 - z) * c1 + z * c2;
}

void MainWindow::teksturowanie(std::vector<std::pair<int, int> > punkty1, std::vector<std::pair<int, int>> punkty2, unsigned char *wsk1, unsigned char *wsk2)
{
    double col1, col2, final_color;
    int ymin = height;
    int ymax = 0;
    double u, v, w;
    int xt, yt;
    int x1, y1;
    double a, b;
    std::vector <int> przeciecia;
    for(int i = 0; i < (int) punkty1.size(); ++i){
        if(punkty2.at(i).second < ymin) ymin = punkty2.at(i).second;
        if(punkty2.at(i).second > ymax) ymax = punkty2.at(i).second;
    }
    for(int y = ymin; y <= ymax; ++y){
        for(int i = 0; i < (int) punkty2.size() - 1; ++i){
            if(przecina(punkty2.at(i).second, punkty2.at(i + 1).second, y) == 1){
                przeciecia.push_back(xprzeciecia(punkty2.at(i).first, punkty2.at(i).second, punkty2.at(i + 1).first, punkty2.at(i + 1).second, y));
            }
        }
        if(przecina(punkty2.front().second, punkty2.back().second, y) == 1){
            przeciecia.push_back(xprzeciecia(punkty2.front().first, punkty2.front().second, punkty2.back().first, punkty2.back().second, y));
        }
        std::sort(przeciecia.begin(), przeciecia.end());
        while(przeciecia.size() >= 2){
            for(int i = przeciecia.front() + 1; i <= przeciecia.at(1) ; ++i){
                w = barycentrycznew(i, y, punkty2);
                v = barycentrycznev(i, y, punkty2);
                u = 1 - w - v;
                xt = punkty1.at(0).first * u + v * punkty1.at(1).first + w * punkty1.at(2).first;
                yt = punkty1.at(0).second * u + v * punkty1.at(1).second + w * punkty1.at(2).second;
                a = punkty1.at(0).first * u + v * punkty1.at(1).first + w * punkty1.at(2).first;
                b = punkty1.at(0).second * u + v * punkty1.at(1).second + w * punkty1.at(2).second;
                x1 = xt + 1;
                y1 = yt + 1;
                a-=xt;
                b-=yt;
                col1 = Interpolacja(wsk1[width * 4 * yt + 4 * xt], wsk1[width * 4 * yt + 4 * x1], a);
                col2 = Interpolacja(wsk1[width * 4 * y1 + 4 * xt], wsk1[width * 4 * y1 + 4 * x1], a);
                final_color = Interpolacja(col1, col2, b);
                wsk2[width * 4 * y + 4 * i] = round(final_color);
                col1 = Interpolacja(wsk1[width * 4 * yt + 4 * xt + 1], wsk1[width * 4 * yt + 4 * x1 + 1], a);
                col2 = Interpolacja(wsk1[width * 4 * y1 + 4 * xt + 1], wsk1[width * 4 * y1 + 4 * x1 + 1], a);
                final_color = Interpolacja(col1, col2, b);
                wsk2[width * 4 * y + 4 * i + 1] = round(final_color);
                col1 = Interpolacja(wsk1[width * 4 * yt + 4 * xt + 2], wsk1[width * 4 * yt + 4 * x1 + 2], a);
                col2 = Interpolacja(wsk1[width * 4 * y1 + 4 * xt + 2], wsk1[width * 4 * y1 + 4 * x1 + 2], a);
                final_color = Interpolacja(col1, col2, b);
                wsk2[width * 4 * y + 4 * i + 2] = round(final_color);
            }
            przeciecia.erase(przeciecia.begin(), przeciecia.begin() + 2);
        }
        przeciecia.clear();
    }
    update();
}
void MainWindow::on_czesci_valueChanged(int arg1)
{
    ilosc_czesci = arg1;
    if(ilosc_klatek > 0 && ilosc_czesci > 0){
        ui->generateButton->setEnabled(true);
    }
    else{
        ui->generateButton->setEnabled(false);
    }
    siatka(ilosc_czesci);
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int x,y;
    x = event->pos().x();
    y = event->pos().y();
    int index_x = -1;
    int index_y = -1;
    if(x >= startX1 && y >= startY1 && x < width + startX1 && y < height + startY1){
        x-=startX1;
        y-=startY1;
        for(int i = 1; i < ilosc_czesci + 1; ++i){
            for(int j = 1; j < ilosc_czesci + 1; ++j){
                if(punkty1[i][j].first -5 < x && punkty1[i][j].first + 5 > x && punkty1[i][j].second -5 < y && punkty1[i][j].second +5 > y){
                    index_x = i;
                    index_y = j;
                    break;
                }
            }
        }
        if(index_x != -1){
            index1.first = index_x;
            index1.second = index_y;
            index2.first = -1;
            index2.second = -1;
        }
        else{
            index1.first = -1;
            index1.second = -1;
            index2.first = -1;
            index2.second = -1;
        }
    }
    else if(x >= startX2 && y >= startY2 && x < width + startX2 && y < height + startY2){
        x-=startX2;
        y-=startY2;
        for(int i = 1; i < ilosc_czesci + 1; ++i){
            for(int j = 1; j < ilosc_czesci + 1; ++j){
                if(punkty2[i][j].first -5 < x && punkty2[i][j].first + 5 > x && punkty2[i][j].second -5 < y && punkty2[i][j].second +5 > y){
                    index_x = i;
                    index_y = j;
                    break;
                }
            }
        }
        if(index_x != -1){
            index2.first = index_x;
            index2.second = index_y;
            index1.first = -1;
            index1.second = -1;
        }
        else{
            index1.first = -1;
            index1.second = -1;
            index2.first = -1;
            index2.second = -1;
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int x,y;
    if(index1.first != -1){
        x = event->pos().x() - startX1;
        y = event->pos().y() - startY1;
        if(x >= 0 && y >= 0 && x < width && y < height){
            *img1 = obraz1 -> copy();
            punkty1[index1.first][index1.second].first = x;
            punkty1[index1.first][index1.second].second = y;
            draw_punkty(1);
            draw_czesci(1);
        }
    }
    else if(index2.first != -1){
        x = event->pos().x() - startX2;
        y = event->pos().y() - startY2;
        if(x >= 0 && y >= 0 && x < width && y < height){
            *img2 = obraz2 -> copy();
            punkty2[index2.first][index2.second].first = x;
            punkty2[index2.first][index2.second].second = y;
            draw_punkty(2);
            draw_czesci(2);
        }
    }
}

void MainWindow::on_klatki_valueChanged(int arg1)
{
    ilosc_klatek = arg1;
    if(ilosc_klatek > 0 && ilosc_czesci > 0){
        ui->generateButton->setEnabled(true);
    }
    else{
        ui->generateButton->setEnabled(false);
    }
}
void MainWindow::on_generateButton_clicked()
{
    ui->progressBar->setValue(0);
    ui->startButton->setEnabled(false);
    ui->generateButton->setEnabled(false);
    morphing();
    ui->generateButton->setEnabled(true);
    ui->startButton->setEnabled(true);
}

void MainWindow::on_startButton_clicked()
{
    ui->startButton->setEnabled(false);
    ui->generateButton->setEnabled(false);
    animacja();
    ui->generateButton->setEnabled(true);
    ui->startButton->setEnabled(true);
}

void MainWindow::on_delay_valueChanged(int arg1)
{
    delay_number = arg1;
}
