#include "train.h"
#include<algorithm>

using namespace std;


double activate(double num)
{
    return 1/(1+pow(M_E,-num));
}

//微分函数的导数
double diffActivate(double num)
{
    double magic1 = pow(M_E,-num);
    double magic2 = pow(1+magic1,2);
    double sum = magic1/magic2;
    return sum;
}

double outputDelta(double ao,double zo,double t)
{
    return (ao-t)* diffActivate(zo);
}

double filterDelta(vector<double> &output_deltas,vector<double> &weights,double k,double zFk,int outputunit_num)
{
    if(k==0)
    {
        return 0;
    }
    //outputunit的数量为3
    double res=0;
    for(int i=0;i<outputunit_num;i++)
    {
        //cout<<res+output_deltas[i]*weights[i]<<endl;
        res=res+output_deltas[i]*weights[i];
    }
    return res* diffActivate(zFk);
}

Matrix* convolution(Filter& f,const Matrix &img)
{
    Matrix* ret=new Matrix(img.row-f.size+1,img.column-f.size+1);
    int i=0,j=0;
    do {
        j=0;
        do {
            //??????这个地方有过修改！！！
            //??????这个地方有过修改！！！
            //??????这个地方有过修改！！！
            ret->m[j][i] = f.similarity(img)+f.bias;
            j++;
        } while (f.movRight());
        i++;
        f.top_left.first=0;
        f.bottom_right.first=f.size-1;
    }while(f.movDown());
    //ret->Print();
    return ret;
}

//最大值池化,先默认为4 所以filter_size是其边长，是2
Matrix* pooling(const Matrix &conv,Matrix* filter_deltas,int filter_size)
{
    int pool_size = conv.row/filter_size;
    Matrix* pool=new Matrix(pool_size,pool_size);
    for(int i=0;i<pool_size;i++)
    {
        for(int j=0;j<pool_size;j++)
        {
            double max=conv.m[pool_size*i][pool_size*j];
            pair<int,int> pos[2][2]={{{pool_size*i,pool_size*j},{pool_size*i+1,pool_size*j}},{{pool_size*i,pool_size*j+1},{pool_size*i+1,pool_size*j+1}}};
            int max_x=pool_size*i,max_y=pool_size*j;
            if(max<conv.m[pool_size*i+1][pool_size*j])
            {
                max=conv.m[pool_size*i+1][pool_size*j];
                //当前的值已经不可能是最大值了，直接令他为0
                filter_deltas->m[max_x][max_y]=0;
                max_x=pool_size*i+1;
                max_y=pool_size*j;
            }else{
                filter_deltas->m[pool_size*i+1][pool_size*j]=0;
            }
            if(max<conv.m[pool_size*i][pool_size*j+1])
            {
                max=conv.m[pool_size*i][pool_size*j+1];
                filter_deltas->m[max_x][max_y]=0;
                max_x=pool_size*i;
                max_y=pool_size*j+1;
            }
            else{
                filter_deltas->m[pool_size*i][pool_size*j+1]=0;
            }
            if(max<conv.m[pool_size*i+1][pool_size*j+1])
            {
                max=conv.m[pool_size*i+1][pool_size*j+1];
                filter_deltas->m[max_x][max_y]=0;
                max_x=pool_size*i+1;
                max_y=pool_size*j+1;
            }else{
                filter_deltas->m[pool_size*i+1][pool_size*j+1]=0;
            }
            pool->m[i][j] = max;
            //将所有为最大值的单元格对应的filter_deltas都置为一
            for(int z=0;z<2;z++)
            {
                for(int v=0;v<2;v++)
                {
                    if(conv.m[pos[z][v].first][pos[z][v].second]==max)
                    {
                        filter_deltas->m[pos[z][v].first][pos[z][v].second]=1;
                    }
                }
            }
        }
    }
    return pool;
}

