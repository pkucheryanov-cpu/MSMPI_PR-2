#include <iostream>
#include <iomanip>
#include <mpi.h>

//using namespace std;
//
//int main(int argc, char** argv)
//{
//    int mynode, totalnodes;
//    int sum, startval, endval, accum;
//    MPI_Status status;
//    double start_time = 0.0, end_time = 0.0, total_time = 0.0;
//
//    // Ініціалізація середовища MPI
//    MPI_Init(&argc, &argv);
//    MPI_Comm_size(MPI_COMM_WORLD, &totalnodes);
//    MPI_Comm_rank(MPI_COMM_WORLD, &mynode);
//
//    // Синхронізація всіх процесів перед стартом замірів часу
//    MPI_Barrier(MPI_COMM_WORLD);
//    start_time = MPI_Wtime();
//
//    // Декомпозиція діапазону та локальне сумування
//    sum = 0;
//    startval = 1000 * mynode / totalnodes + 1;
//    endval = 1000 * (mynode + 1) / totalnodes;
//
//    for (int i = startval; i <= endval; i = i + 1)
//    {
//        sum = sum + i;
//    }
//
//    // Передача локальних сум від підлеглих процесів до вузла 0
//    if (mynode != 0)
//    {
//        MPI_Send(&sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
//    }
//    // Вузол 0 збирає часткові результати
//    else
//    {
//        for (int j = 1; j < totalnodes; j = j + 1)
//        {
//            MPI_Recv(&accum, 1, MPI_INT, j, 1, MPI_COMM_WORLD, &status);
//            sum = sum + accum;
//        }
//    }
//
//    // Фіксація завершення паралельного сегмента
//    end_time = MPI_Wtime();
//    total_time = end_time - start_time;
//
//    // Нульовий вузол виводить результат та витрачений час
//    if (mynode == 0)
//    {
//        cout << "The sum from 1 to 1000 is: " << sum << endl;
//        cout << "Nodes: " << totalnodes
//            << " | Execution time: " << fixed << setprecision(8)
//            << total_time << " seconds" << endl;
//    }
//
//    MPI_Finalize();
//    return 0;
//}

#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    int mynode, totalnodes;
    const int datasize = 5; // Розмір буфера масиву
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &totalnodes);
    MPI_Comm_rank(MPI_COMM_WORLD, &mynode);

    if (totalnodes < 2)
    {
        if (mynode == 0)
        {
            cout << "At least 2 processes are required to demonstrate the exchange!" << endl;
        }
        MPI_Finalize();
        return 0;
    }

    vector<int> buffer(datasize);

    // Підлеглі процеси заповнюють буфер та відправляють на Node 0
    if (mynode != 0)
    {
        for (int i = 0; i < datasize; ++i)
        {
            buffer[i] = mynode * 10 + i;
        }
        MPI_Send(buffer.data(), datasize, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    // Головний процес приймає масиви від кожного підлеглого вузла
    else
    {
        cout << "Node 0: Start receiving arrays from nodes" << endl;

        for (int sender = 1; sender < totalnodes; ++sender)
        {
            MPI_Recv(buffer.data(), datasize, MPI_INT, sender, 0, MPI_COMM_WORLD, &status);

            cout << "Received from Node " << sender << ": [ ";
            for (int i = 0; i < datasize; ++i)
            {
                cout << buffer[i] << " ";
            }
            cout << "]" << endl;
        }

        cout << "Node 0: Data collection from all nodes has been successfully completed" << endl;
    }

    MPI_Finalize();
    return 0;
}