#include "train.h"
using namespace std;

//poolsize是每个pool的边长
OutputUnit::OutputUnit(int pool_num,int pool_size,vector<string>& params)
{
    for(int k=0;k<pool_num;k++)
    {
        Matrix* new_m = new Matrix(pool_size,pool_size);
        this->weights.push_back(new_m);
        stringstream ss(params[k]);
        for(int i=0;i<pool_size;i++)
        {
            for(int j=0;j<pool_size;j++)
            {
                ss>>new_m->m[i][j];
            }
        }
        ss>>this->bias;
    }
}

double output(OutputUnit &unit,std::vector<Matrix*> pools)
{
//    cout<<"---------- pools and unit ---------"<<endl;
//    pools[1]->Print();
//    cout<<endl;
//    unit.weights[1]->Print();
//    cout<<"---------- pools and unit ---------"<<endl;

    for(int i=0;i<pools.size();i++)
    {
        //由于pool的值在后几次运算中还要被用到，所以此处不要用pool来乘
        unit.weights[i]->hadamard(*pools[i]);
    }
//    cout<<"-----pools[1]-----"<<endl;
//    pools[1]->Print();
//    cout<<"-----pools[1]-----"<<endl;
    double sum=0;
    for(int k=0;k<pools.size();k++)
    {
        for(int i=0;i<pools[k]->row;i++)
        {
            for(int j=0;j<pools[k]->column;j++)
            {
                sum=sum+unit.weights[k]->m[i][j];
            }
        }
    }
    sum=sum+unit.bias;
    sum=activate(sum);
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