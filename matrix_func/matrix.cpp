#include "matrix.h"
#include <iostream>

using  namespace std;

Matrix::Matrix(int row,int column)
{
    this->row=row;
    this->column=column;
    this->m=(double**)malloc(row*sizeof(double*));
    if(this->m== nullptr)
    {
        cerr<<"err ! mem is not enough"<<endl;
        exit(-1);
    }
    for(int i=0;i<row;i++)
    {
        this->m[i]=(double *) malloc(column*sizeof(double));
        if(this->m== nullptr)
        {
            cerr<<"err ! mem is not enough"<<endl;
            exit(-1);
        }
    }
}

Matrix::~Matrix()
{
    if(this->m!= nullptr)
    {
        int temprow=this->row,tempcolumn=this->column;
        destroy();
        cout<<"free finish "<<temprow<<" "<<tempcolumn<<endl;
    }
}

void Matrix::destroy() const
{
    for(int i=0;i<row;i++)
    {
        free(this->m[i]);
    }
    free(m);
}

void Matrix::Print() const
{
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            std::cout<<this->m[i][j]<<" ";
        }
        cout<<endl;
    }
}

void Matrix::transpose()
{
    double **new_m;
    new_m=(double**)malloc(column*sizeof(double*));
    if(this->m== nullptr)
    {
        cerr<<"err in transpose!"<<endl;
        exit(-1);
    }
    for(int i=0;i<column;i++)
    {
        new_m[i]=(double *) malloc(row*sizeof(double));
        if(this->m== nullptr)
        {
            cerr<<"err in transpose!"<<endl;
            exit(-1);
        }
    }

    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            new_m[j][i]=this->m[i][j];
        }
    }
    destroy();
    int temp;
    temp=this->column;
    this->column=this->row;
    this->row=temp;
    this->m=new_m;
}

void Matrix::sub(const Matrix&m2) const
{
    if(m2.row!=this->row||m2.column!=this->column)
    {
        cerr<<"err in sub!"<<endl;
        return;
    }
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            this->m[i][j]=this->m[i][j]-m2.m[i][j];
        }
    }
}

void Matrix::add(const Matrix&m2) const
{
    if(m2.row!=this->row||m2.column!=this->column)
    {
        cerr<<"err in add!"<<endl;
        return;
    }
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            this->m[i][j]=this->m[i][j]+m2.m[i][j];
        }
    }
}

void Matrix::add(double num) const
{
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            this->m[i][j]=this->m[i][j]+num;
        }
    }
}

void Matrix::multiply(double num) const
{
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            this->m[i][j]=(this->m[i][j])*num;
        }
    }
}

void Matrix::multiply(const Matrix &m2)
{
    //mij*mjk => mik
    if(m2.row!=this->column)
    {
        cerr<<"err in multiply,columns and rows don't match!"<<endl;
        return ;
    }
    else
    {
        double **new_m;
        new_m=(double**)malloc(column*sizeof(double*));
        if(this->m== nullptr)
        {
            cerr<<"err in transpose!"<<endl;
            exit(-1);
        }
        for(int i=0;i<column;i++)
        {
            new_m[i]=(double *) malloc(row*sizeof(double));
            if(this->m== nullptr)
            {
                cerr<<"err in transpose!"<<endl;
                exit(-1);
            }
        }
        for(int i=0;i<this->row;i++)
        {
            for(int j=0;j<m2.column;j++)
            {
                double sum=0;
                for(int k=0;k<m2.row;k++)
                {
                    sum=sum+this->m[i][k]*m2.m[k][j];

                }
                new_m[i][j]=sum;
                //cout<<i<<" "<<j<<" "<<new_m.m[i][j]<<endl;
            }
        }
        this->destroy();
        this->m=new_m;
        this->column=m2.column;
    }
}

void Matrix::hadamard(const Matrix&m2) const
{
    if(m2.row!=this->row||m2.column!=this->column)
    {
        cerr<<"err in hadamard!"<<endl;
        return;
    }
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            this->m[i][j]=this->m[i][j]*m2.m[i][j];
        }
    }
}

Matrix::Matrix(){
    this->m= nullptr;
}

//return the reference in order to implement chain operation
Matrix& Matrix::operator+(const Matrix &m2) {
    this->add(m2);
    return *this;
}
Matrix& Matrix::operator-(const Matrix &m2) {
    this->sub(m2);
    return *this;
}

Matrix& Matrix::operator*(const Matrix &m2) {
    this->multiply(m2);
    return *this;
}

Matrix& Matrix::operator*(double num) {
    this->multiply(num);
    return *this;
}