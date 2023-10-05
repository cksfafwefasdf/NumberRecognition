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
const int DATA_SIZE=96;

int main()
{
    vector<vector<double>> answers(DATA_SIZE);
    double totalC=0;
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
    for(int times=1;times<=DATA_SIZE;times++)
    {

        vector<Filter> filters;
        vector<Matrix*> pools;
        vector<OutputUnit> units;
        vector<vector<string>> output_params;
        vector<double> outputs;
        //每个output对应三个pool,一共有3个output，所以是9
        for(int i=0,k=1;i<OUTPUT_UNIT_NUM;i++)
        {
            output_params.push_back(vector<string>());
            for(int j=0;j<POOL_NUM;j++)
            {
                output_params[i].push_back(readLine(init_output_layer,k,100));
                //cout<<output_params[i-1]<<endl;
                k++;
            }
        }

        Matrix img = conv2img(readLine(train_data,times,100),6,6);
        for(int i=1;i<=FILTER_NUM;i++)
        {
            string buffer = readLine(init_filter,i,100);
            Filter f(3,img.row,buffer); //3*3尺寸的filter
            filters.push_back(f);
            Matrix conv = convolution(filters[i-1],img);
            //conv.Print();
            Matrix* pool = pooling(conv);
            pools.push_back(pool);
            //size先默认为2
            //pools.push_back(pool);
            //cout<<"---------------"<<endl;
            //pool.Print();
        }

//    for(int i=0;i<3;i++)
//    {
//        pools[i]->Print();
//        cout<<"---"<<endl;
//    }
        //cout<<"---OutputUnit---"<<endl;
        for(int i=0;i<OUTPUT_UNIT_NUM;i++)
        {
            units.push_back(OutputUnit(POOL_NUM,2,output_params[i]));
            //units[i].weights[0]->Print();
        }
        //cout<<"res-------"<<endl;
        for(int i=0;i<units.size();i++)
        {
            outputs.push_back(output(units[i],pools));
            //cout<<outputs[i]<<endl;
        }
        //现在读的是第33号数据
        double res = totalCost(outputs,answers[times-1]);
        totalC=totalC+res;
        cout<<"-----------times-------------"<<endl;
        cout<<"res "<< res<<endl;
        cout<<"times "<<times<<endl;
        cout<<"-----------times-------------"<<endl;
    }
    cout<<"------------ totalC ----------- "<<endl;
    cout<<totalC<<endl;
    cout<<"------------ totalC ----------- "<<endl;
    return 0;
}

