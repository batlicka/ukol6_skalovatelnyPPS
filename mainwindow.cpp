#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_clicked()
{
    int barrier, posun, sum =0;
    int N = ui->lineEditN->text().toInt();
    int NumThread = ui->lineEditNumThreads->text().toInt();
    omp_set_num_threads(NumThread);

    int *X = new int[N];
    int *S = new int[N];
    int *Z = new int[N];

    //testovaci data ze skript
    X[0]=2;
    X[1]=1;
    X[2]=3;

    X[3]=1;
    X[4]=2;
    X[5]=0;

    X[6]=4;
    X[7]=2;
    X[8]=3;

    X[9]=5;
    X[10]=0;
    X[11]=3;

    X[12]=1;
    X[13]=4;
    X[14]=2;

    #pragma omp parallel
    {
        #pragma omp for ordered schedule(dynamic)
            for(int i=0; i<N;i++){
                #pragma omp ordered
    //                X[i]=i;
                Z[i]=X[i];
            }
    }
    int CellsForOneProcessors = static_cast<int>(floor(N/NumThread));

    //sekvenční prefix. součet
    int NulSum = 0;

    for(int i =0; i<=N-1;i++){
        sum = sum + X[i];
        S[i]= sum;
        NulSum++;
        Z[i]=S[i];
        if(NulSum==CellsForOneProcessors && i<CellsForOneProcessors*NumThread-1){
            Z[i]=sum;
            NulSum=0;
            sum=0;
        }
    }

    //cast z neskalovatelneho
    barrier = static_cast<int>(ceil(log(NumThread) / log(2)));
//    for (int j = 0; j < barrier;j++)
//    {
//       posun = static_cast<int>(pow(2,j));
//       #pragma omp parallel shared(S,Z)
//        {
//        #pragma omp for ordered schedule(dynamic)
//            for (int i = posun; i < N; i++)
//            {
//                #pragma omp ordered
//                Z[i] =Z[i]+ S[i - posun];
//            }
//        #pragma omp for ordered schedule(dynamic)
//            for (int i = posun; i < N; i++)
//            {
//                #pragma omp ordered
//                S[i] = Z[i];
//            }
//        }
//    }


    QString output= "";
    for(int i=0; i<=N-1;i++){
        output =output + "|"+ QString::number(S[i]);
    }

    ui->lineEditRes->setText(output);




}
