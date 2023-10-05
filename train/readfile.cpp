#include "train.h"
#include <fstream>
#include <iostream>
using namespace std;

string readLine(string file_name,int row_num,int row_size)
{
    fstream file;
    char line[1024];
    file.open(file_name,ios::in);
    if(!file.is_open())
    {
        cerr<<"fail to open file!"<<endl;
    }
    for(int i=0;i<row_num;i++)
    {
        file.getline(line,row_size);
    }
    string ret(line);
    return ret;
}

Matrix conv2img(string data,int row,int col)
{
    Matrix img(row,col);
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            img.m[i][j]= data[i*col+j]-'0';
        }
    }
    return img;
}


