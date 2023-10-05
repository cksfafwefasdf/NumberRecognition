#include "train.h"

using namespace std;

bool Filter::movDown()
{
    if(this->bottom_right.second>=border-1)
    {
        //std::cerr<<"out of border ! "<<endl;
        return false;
    }
    this->bottom_right.second++;
    this->top_left.second++;
    return true;
}
bool Filter::movRight()
{
    if(this->bottom_right.first>=border-1)
    {
        //std::cerr<<"out of border ! "<<endl;
        return false;
    }
    this->top_left.first++;
    this->bottom_right.first++;
    return true;
}
Filter::Filter(int size, int border,string param) {
    this->border=border;
    this->size=size;
    this->top_left.first=this->top_left.second=0;
    this->bottom_right.first=this->bottom_right.second=size-1;

    //一定要用static，要不然会被提前释放！！
    static Matrix m(size,size);
    this->w=&m;

    stringstream ss(param);
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            ss>>this->w->m[i][j];
        }
    }
    ss>>this->bias;
}

Filter::Filter(int size,int border){
    this->border=border;
    this->size=size;
    this->top_left.first=this->top_left.second=0;
    this->bottom_right.first=this->bottom_right.second=size-1;

    //一定要用static，要不然会被提前释放！！
    static Matrix m(size,size);
    this->w=&m;
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            this->w->m[i][j]=0;
        }
    }
    this->bias=0;
}


double Filter::similarity(const Matrix &m2)
{
    double ret=0;
    for(int i=this->top_left.first;i<=this->bottom_right.first;i++)
    {
        for(int j=this->top_left.second;j<=this->bottom_right.second;j++)
        {
            ret=ret+m2.m[i][j]*this->w->m[i-this->top_left.first][j-this->top_left.second];
            //cout<<"check "<<i<<" "<<j<<" "<<i-this->top_left.first<<" "<<j-this->top_left.second<<endl;
        }
    }
    return ret;
}
