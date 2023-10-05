#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include "../matrix_func/matrix.h"
#ifndef NUMBER_TRAIN_H
#define NUMBER_TRAIN_H

class Filter{
public:
    int size;
    int border; //size of the convolution layer
    double bias;
    Matrix* w; //weight matrix
    std::pair<int,int> top_left; // position of top-left of the slide window
    std::pair<int,int> bottom_right; // position of bottom-right of the slide window
    Filter(int size,int border,std::string param);
    Filter(int size,int border);
    bool movRight();
    bool movDown();
    double similarity(const Matrix &m2);
};

class OutputUnit{
public:
    int pool_num; //池化层单元的个数
    double bias;
    OutputUnit(int pool_num,int pool_size,std::vector<std::string>& params);
    std::vector<Matrix*> weights;
};

std::string readLine(std::string fileName, int i, int i1);
double activate(double num);
Matrix conv2img(std::string data,int row,int col);
Matrix convolution(Filter& f,const Matrix &img);
Matrix* pooling(const Matrix &conv,int filter_size=2);
double output(OutputUnit &units,std::vector<Matrix*> matrixes);
double totalCost(std::vector<double> &output,std::vector<double> &true_ans);

#endif //NUMBER_TRAIN_H
