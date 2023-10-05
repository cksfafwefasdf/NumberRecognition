#include <string>
#include <iostream>
#include "train/train.h"
#include "matrix_func/matrix.h"
using namespace std;

const string fileName = "../data/data1.txt";

int main()
{
    string data = readLine(fileName, 3, 64);
    Matrix m = conv2img(data,6,6);

    return 0;
}

