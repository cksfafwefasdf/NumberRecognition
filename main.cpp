#include "matrix_func/matrix.h"
#include "train/train.h"

using namespace std;

const string train_data = "../data/data1.txt";
const string init_filter = "../data/init_filter.txt";
const string init_output_layer = "../data/init_output_layer.txt";
const string true_answer = "../data/true_answer.txt";
const int OUTPUT_UNIT_NUM=3;
const int POOL_NUM=3;
const int FILTER_NUM=3;
const int DATA_SIZE=1;
const int POOL_SIZE = 2;
const int FILTER_SIZE = 3;
const int alpha = 0.2; //步长



void initAnser(vector<vector<double>> &answers);
void initParamList(ParamList& paramList);
void printParamList(ParamList& paramList);

int main()
{
    vector<vector<double>> answers(DATA_SIZE);
    //一共有70个参数
    ParamList paramList;
    double totalC=0;
    initAnser(answers);
    //读取硬盘中的初始化参数
    initParamList(paramList);

    for(int times=1;times<=DATA_SIZE;times++)
    {
        vector<Filter> filters;
        vector<Matrix*> pools;
        vector<Matrix*> conv_inputs;
        vector<OutputUnit> units;
        vector<vector<string>> output_params;
        vector<double> outputs;
        vector<double> zos;
        vector<double> output_deltas(OUTPUT_UNIT_NUM);
        //每个output对应三个pool,一共有3个output，所以是9
        for(int i=0,k=1;i<OUTPUT_UNIT_NUM;i++)
        {
            output_params.push_back(vector<string>());
            for(int j=0;j<POOL_NUM;j++)
            {
                output_params[i].push_back(readLine(init_output_layer,k,100));
                k++;
            }
        }

        Matrix img = conv2img(readLine(train_data,times,100),6,6);

        //为filter的delta预留空间
        int filter_deltas_size = img.row-FILTER_SIZE+1;
        vector<Matrix*> filter_deltas;
        filter_deltas.reserve(FILTER_NUM);
        for(int i=0;i<FILTER_NUM;i++)
        {
            filter_deltas[i]=new Matrix(filter_deltas_size,filter_deltas_size);
        }
        //卷积和池化
        for(int i=1;i<=FILTER_NUM;i++)
        {
            Filter f(3,img.row,paramList.conv_params[i-1],paramList.conv_bias[i-1]); //3*3尺寸的filter
            filters.push_back(f);
            Matrix* conv_input = convolution(filters[i-1],img);
            conv_inputs.push_back(conv_input);
            //此for结束后这个conv会被free
            Matrix conv(conv_input->row,conv_input->column);
            //计算卷积层的输出矩阵
            for(int k=0;k<conv_input->row;k++)
            {
                for(int z=0;z<conv_input->column;z++)
                {
                    conv.m[k][z]= activate(conv_input->m[k][z]);
                }
            }
            //conv_input->Print();
            //在池化的时候顺便就可以把输出层的一些delta算出来了
            Matrix* pool = pooling(conv,filter_deltas[i-1]);
            pools.push_back(pool);
        }

        for(int i=0;i<OUTPUT_UNIT_NUM;i++)
        {
            OutputUnit unit=OutputUnit(POOL_NUM,POOL_SIZE,paramList.output_params[i],paramList.output_bias[i]);
            units.push_back(unit);
            //units[i].weights[0]->Print();
        }
        //cout<<"res-------"<<endl;
        for(int i=0;i<units.size();i++)
        {
            double res = output(units[i],pools);
            //zos里面存的是Zno outputs里面存的是Ano
            zos.push_back(res);
            outputs.push_back(activate(res));
            //cout<<outputs[i]<<endl;
        }

        //计算输出层delta
        for(int i=0;i<OUTPUT_UNIT_NUM;i++)
        {
            double deltatemp=outputDelta(outputs[i],zos[i],answers[times-1][i]);
            output_deltas[i]=deltatemp;
        }

        //计算卷积层delta
        for(int k=0;k<FILTER_NUM;k++)
        {
            vector<double> weights;
            for(int i=0;i<filter_deltas_size;i++)
            {
                for(int j=0;j<filter_deltas_size;j++)
                {
                    //第三个参数处理一下 units[i].weights 可以得到
                    //conv_input 可以得到zFk 有3个filter，有三个conv
                    for(int z=0;z<OUTPUT_UNIT_NUM;z++)
                    {
                        weights.push_back(units[z].weights[k]->m[i/POOL_SIZE][j/POOL_SIZE]);
                    }
                    //第k个filter的i行j列
                    filter_deltas[k]->m[i][j]=filterDelta(output_deltas,weights,filter_deltas[k]->m[i][j],conv_inputs[k]->m[i][j],OUTPUT_UNIT_NUM);
                    weights.clear();
                }
            }
        }

        //计算C和CT
        double res = totalCost(outputs,answers[times-1]);
        totalC=totalC+res;

        filter_deltas[0]->Print();
        cout<<endl;
        filter_deltas[1]->Print();
        cout<<endl;
        filter_deltas[2]->Print();

    }
    cout<<"------------ totalC ----------- "<<endl;
    cout<<totalC<<endl;
    cout<<"------------ totalC ----------- "<<endl;
    return 0;
}

void initAnser(vector<vector<double>> &answers)
{
    for(int i=0;i<DATA_SIZE;i++)
    {
        answers.push_back(vector<double>());
        answers[i].reserve(OUTPUT_UNIT_NUM);
        //最后一个参数随便给的，可以改，反正就是一个缓存
        string buffer=readLine(true_answer,i+1,8*OUTPUT_UNIT_NUM);
        stringstream ss(buffer);
        for(int j=0;j<OUTPUT_UNIT_NUM;j++)
        {
            ss>>answers[i][j];
        }
    }
}

void initParamList(ParamList& paramList)
{
    vector<string> filterInitParams;
    vector<string> outputInitParams;
    paramList.conv_params.reserve(FILTER_NUM);
    for(int i=0;i<FILTER_NUM;i++)
    {
        paramList.conv_params[i]=new Matrix(FILTER_SIZE,FILTER_SIZE);
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
}

void printParamList(ParamList& paramList)
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