#include <iostream>
#include "matrix_func/matrix.h"
using namespace std;

Matrix test(Matrix &m2)
{
    Matrix m = m2;
    return m;
}

int main() {
    Matrix m(2,3);
    Matrix m2(3,2);

    for(int i=0;i<3;i++){
        for(int j=0;j<2;j++){
            m2.m[i][j]=-1.5;
        }
    }

    for(int i=0;i<2;i++){
        for(int j=0;j<3;j++){
            m.m[i][j]=3.5;
        }
    }
    m=m*m2;
    m.hadamard(m);
    m.Print();
//    cout<<endl;
//    m3.transpose();
//    m3.Print();
    return 0;
}

