#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include "../matrix_func/matrix.h"
#ifndef NUMBER_TRAIN_H
#define NUMBER_TRAIN_H

const std::string train_data = "../data/data1.txt";
const std::string init_filter = "../data/init_filter.txt";
const std::string init_output_layer = "../data/init_output_layer.txt";
const std::string true_answer = "../data/true_answer.txt";
const int OUTPUT_UNIT_NUM=3;
const int POOL_NUM=3;
const int FILTER_NUM=3;
const int DATA_SIZE=96;
const int POOL_SIZE = 2;
const int FILTER_SIZE = 3;
const int alpha = 0.2; //步长

class ParamList{
public:
    int param_num;
    std::vector<Matrix*> conv_params;
    std::vector<std::vector<Matrix*>> output_params;
    std::vector<double> conv_bias;
    std::vector<double> output_bias;
};

class Filter{
public:
    int size;
    int border; //size of the convolution layer
    double bias;
    Matrix* w; //weight matrix
    std::pair<int,int> top_left; // position of top-left of the slide window
    std::pair<int,int> bottom_right; // position of bottom-right of the slide window
    Filter(int size, int border,Matrix* params,double bias);
    Filter(int size,int border);
    bool movRight();
    bool movDown();
    double similarity(const Matrix &m2);
};

class OutputUnit{
public:
    int pool_num; //池化层单元的个数
    double bias;
    OutputUnit(int pool_num,int pool_size,std::vector<Matrix*>& params,double bias);
    OutputUnit(const OutputUnit& o2);
    std::vector<Matrix*> weights;
};

std::string readLine(std::string fileName, int row_num,int row_size);
double activate(double num);
Matrix conv2img(std::string data,int row,int col);
Matrix* convolution(Filter& f,const Matrix &img);
Matrix* pooling(const Matrix &conv,Matrix* filter_deltas,int filter_size=2);
double output(OutputUnit &units,std::vector<Matrix*> matrixes);
double totalCost(std::vector<double> &output,std::vector<double> &true_ans);
double outputDelta(double ao,double zo,double t);
double filterDelta(std::vector<double> &output_deltas,std::vector<double> &weights,double k,double zFk,int outputunit_num);
void initParamList(ParamList& paramList);
void printParamList(const ParamList& paramList);
void getParamFromList(Matrix* paramVector,ParamList &list);
Matrix* derivative(ParamList paramList,Matrix &img,std::vector<Matrix *>& filter_deltas,std::vector<double>& output_deltas,std::vector<Matrix*> pools);
#endif //NUMBER_TRAIN_H
