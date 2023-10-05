#include "train.h"
#include<algorithm>

using namespace std;


double activate(double num)
{
    return 1/(1+pow(M_E,-num));
}

Matrix convolution(Filter& f,const Matrix &img)
{
    Matrix ret(img.row-f.size+1,img.column-f.size+1);
    int i=0,j=0;
    do {
        j=0;
        do {
            //??????这个地方有过修改！！！
            //??????这个地方有过修改！！！
            //??????这个地方有过修改！！！
            //??????这个地方有过修改！！！
            //??????这个地方有过修改！！！
            //??????这个地方有过修改！！！
            //??????这个地方有过修改！！！
            ret.m[j][i] = activate(f.similarity(img)+f.bias);
            j++;
        } while (f.movRight());
        i++;
        f.top_left.first=0;
        f.bottom_right.first=f.size-1;
    }while(f.movDown());
    //ret.Print();
    return ret;
}

//最大值池化,先默认为4 所以filter_size是其边长，是2
//最大值池化,先默认为4 所以filter_size是其边长，是2
//最大值池化,先默认为4 所以filter_size是其边长，是2
Matrix* pooling(const Matrix &conv,int filter_size)
{
    int pool_size = conv.row/filter_size;
    Matrix* pool=new Matrix(pool_size,pool_size);
    for(int i=0;i<pool_size;i++)
    {
        for(int j=0;j<pool_size;j++)
        {
            pool->m[i][j] = max({conv.m[pool_size*i][pool_size*j],conv.m[pool_size*i+1][pool_size*j],conv.m[pool_size*i][pool_size*j+1],conv.m[pool_size*i+1][pool_size*j+1]});
        }
    }
    return pool;
}

