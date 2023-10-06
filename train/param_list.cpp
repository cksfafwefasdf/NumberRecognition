#include "train.h"

using namespace std;


void initParamList(ParamList& paramList)
{
    vector<string> filterInitParams;
    vector<string> outputInitParams;

    for(int i=0;i<FILTER_NUM;i++)
    {
        paramList.conv_params.push_back(new Matrix(FILTER_SIZE,FILTER_SIZE));
    }
    //vector<vector<Matrix*>> output_params;
    for(int i=0;i<OUTPUT_UNIT_NUM;i++)
    {
        paramList.output_params.push_back(vector<Matrix*>());
        for(int j=0;j<POOL_NUM;j++)
        {
            Matrix *new_m = new Matrix(POOL_SIZE,POOL_SIZE);
            paramList.output_params[i].push_back(new_m);
        }
    }
    //读取卷积层数据
    for(int i=0;i<FILTER_NUM;i++)
    {
        string param=readLine(init_filter,i+1,1000);
        filterInitParams.push_back(param);
    }

    for(int i=0;i<OUTPUT_UNIT_NUM*POOL_NUM;i++)
    {
        string outputParam = readLine(init_output_layer,i+1,1000);
        outputInitParams.push_back(outputParam);
    }
    for(int k=0;k<filterInitParams.size();k++)
    {
        stringstream ss(filterInitParams[k]);
        for(int i=0;i<FILTER_SIZE;i++)
        {
            for(int j=0;j<FILTER_SIZE;j++)
            {
                ss>>paramList.conv_params[k]->m[i][j];
            }
        }
        double x;
        ss>>x;
        paramList.conv_bias.push_back(x);
    }

    bool lock=true;
    for(int k=0;k<OUTPUT_UNIT_NUM;k++)
    {
        for(int n=0;n<POOL_NUM;n++)
        {
            stringstream ss(outputInitParams[k*POOL_NUM+n]);
            for(int i=0;i<POOL_SIZE;i++)
            {
                for(int j=0;j<POOL_SIZE;j++)
                {
                    //第k个o层的第n个pool
                    ss>>paramList.output_params[k][n]->m[i][j];
                }
            }
            double x;
            ss>>x;
            if(lock)
            {
                paramList.output_bias.push_back(x);
                lock= false;
            }
        }
        lock= true;
    }
    //printParamList(paramList);
    int param_num = paramList.output_params.size()
                    *paramList.output_params[0].size()
                    *paramList.output_params[0][0]->row
                    *paramList.output_params[0][0]->column
                    +paramList.conv_params.size()
                     *paramList.conv_params[0]->row
                     *paramList.conv_params[0]->column
                    +paramList.conv_bias.size()
                    +paramList.output_bias.size();
    paramList.param_num=param_num;
}

void printParamList(const ParamList& paramList)
{
    cout<<"------param--------"<<endl;
    for(int i=0;i<paramList.output_params.size();i++)
    {
        for(int j=0;j<paramList.output_params[i].size();j++)
        {
            paramList.output_params[i][j]->Print();
        }
    }

    for(int i=0;i<paramList.conv_params.size();i++)
    {
        paramList.conv_params[i]->Print();
    }
    cout<<"------param--------"<<endl;
    cout<<endl;
    cout<<"------bias--------"<<endl;

    for(int i=0;i<3;i++)
    {
        cout<<paramList.conv_bias[i]<<" ";
    }
    cout<<endl;
    for(int i=0;i<3;i++)
    {
        cout<<paramList.output_bias[i]<<" ";
    }
    cout<<endl;
    cout<<"------bias--------"<<endl;
    cout<<endl;
}

void putParamINList(ParamList& paramList,Matrix* paramVector)
{
    int count=0;
    //先读卷积层的参数
    for(int k=0;k<paramList.conv_params.size();k++)
    {
        for(int i=0;i<paramList.conv_params[k]->row;i++)
        {
            for(int j=0;j<paramList.conv_params[k]->column;j++)
            {
                paramList.conv_params[k]->m[i][j]=paramVector->m[0][count];
                count++;
            }
        }
    }

    for(int i=0;i<3;i++)
    {
        paramList.conv_bias[i]=paramVector->m[0][count];
        count++;
    }

    for(int k=0;k<paramList.output_params.size();k++)
    {
        for(int n=0;n<paramList.output_params[k].size();n++)
        {
            for(int i=0;i<paramList.output_params[k][n]->row;i++)
            {
                for(int j=0;j<paramList.output_params[k][n]->column;j++)
                {
                    paramList.output_params[k][n]->m[i][j]=paramVector->m[0][count];
                    count++;
                }
            }
        }
    }

    for(int i=0;i<3;i++)
    {
        paramList.output_bias[i]=paramVector->m[0][count];
        count++;
    }
}

void getParamFromList(Matrix* paramVector,ParamList &paramList)
{
    int count=0;
    //先读卷积层的参数
    for(int k=0;k<paramList.conv_params.size();k++)
    {
        for(int i=0;i<paramList.conv_params[k]->row;i++)
        {
            for(int j=0;j<paramList.conv_params[k]->column;j++)
            {
                paramVector->m[0][count]=paramList.conv_params[k]->m[i][j];
                count++;
            }
        }
    }

    for(int i=0;i<3;i++)
    {
        paramVector->m[0][count]=paramList.conv_bias[i];
        count++;
    }

    for(int k=0;k<paramList.output_params.size();k++)
    {
        for(int n=0;n<paramList.output_params[k].size();n++)
        {
            for(int i=0;i<paramList.output_params[k][n]->row;i++)
            {
                for(int j=0;j<paramList.output_params[k][n]->column;j++)
                {
                    paramVector->m[0][count]=paramList.output_params[k][n]->m[i][j];
                    count++;
                }
            }
        }
    }

    for(int i=0;i<3;i++)
    {
        paramVector->m[0][count]=paramList.output_bias[i];
        count++;
    }
}