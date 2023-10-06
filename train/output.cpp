#include "train.h"
using namespace std;

//poolsize是每个pool的边长
OutputUnit::OutputUnit(int pool_num,int pool_size,std::vector<Matrix*>& params,double bias)
{
    for(int k=0;k<pool_num;k++)
    {
        Matrix* new_m = new Matrix(pool_size,pool_size);
        this->weights.push_back(new_m);
        for(int i=0;i<pool_size;i++)
        {
            for(int j=0;j<pool_size;j++)
            {
                new_m->m[i][j]=params[k]->m[i][j];
            }
        }
        this->bias=bias;
    }
}

OutputUnit::OutputUnit(const OutputUnit& o2)
{
    this->bias=o2.bias;
    this->pool_num=o2.pool_num;
    for(int k=0;k<pool_num;k++)
    {
        Matrix* new_m = new Matrix(o2.weights[0]->row,o2.weights[0]->row);
        this->weights.push_back(new_m);
        for(int i=0;i<new_m->row;i++)
        {
            for(int j=0;j<new_m->row;j++)
            {
                new_m->m[i][j]=o2.weights[k]->m[i][j];
            }
        }
    }
}

double output(OutputUnit &unit,std::vector<Matrix*> pools)
{
    //一定要用拷贝构造函数来构造，如果直接赋值，由于unit里面用了Matrix*，会出现浅拷贝，拷贝过去的是Matrix*，不是一个新的Matrix
    OutputUnit tempunit(unit);
    for(int i=0;i<pools.size();i++)
    {
        //由于pool的值在后几次运算中还要被用到，所以此处不要用pool来乘
        tempunit.weights[i]->hadamard(*pools[i]);
    }

    double sum=0;
    for(int k=0;k<pools.size();k++)
    {
        for(int i=0;i<pools[k]->row;i++)
        {
            for(int j=0;j<pools[k]->column;j++)
            {
                sum=sum+tempunit.weights[k]->m[i][j];
            }
        }
    }
    sum=sum+tempunit.bias;
    //先返回zn，到外面再处理成an
    //sum=activate(sum);
    return sum;
}

double totalCost(std::vector<double> &outputs,std::vector<double> &true_ans)
{
    double res=0;
    for(int i=0;i<outputs.size();i++)
    {
        res=res+(true_ans[i]- outputs[i])*(true_ans[i]- outputs[i]);
        //cout<<"----res "<<true_ans[i]<<"  "<<outputs[i]<<endl;
    }
    res=res*0.5;
    return res;
}