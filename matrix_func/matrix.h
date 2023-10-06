//
// Created by cksfafwefasdf on 23-10-4.
//
#include <iostream>
#ifndef NUMBER_MATRIX_H
#define NUMBER_MATRIX_H

class Matrix
{
public:
    int row;
    int column;
    double **m; //pointer of matrix
    Matrix(int row,int column);
    Matrix();
    ~Matrix();
    void sub(const Matrix &m2) const;
    void add(const Matrix &m2) const;
    void add(double num) const;
    void multiply(const Matrix &m2);
    void multiply(double num) const;
    void hadamard(const Matrix &m2) const;
    void transpose();
    void Print() const;
    Matrix& operator+(const Matrix &m2);
    Matrix& operator-(const Matrix &m2);
    Matrix& operator*(const Matrix &m2);
    Matrix& operator*(double num);
private:
    void destroy() const;

};

#endif //NUMBER_MATRIX_H
