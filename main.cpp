#include "matrix_func/matrix.h"
#include "train/train.h"

using namespace std;


void initAnswer(vector<vector<double>> &answers);
void onDestroy(vector<Matrix*> pools,vector<Matrix*> conv_inputs,vector<Matrix*> filter_deltas,Matrix* differ);

int main()
{
    fstream output_params;
    fstream output_cost;
    output_params.open(model_params,ios::out);
    output_cost.open(cost_records,ios::out);

    vector<vector<double>> answers(DATA_SIZE);
    //一共有70个参数
    ParamList paramList;
    initAnswer(answers);
    //读取硬盘中的初始化参数
    initParamList(paramList);
    Matrix* paramVector =new Matrix(1,paramList.param_num);
    getParamFromList(paramVector,paramList);

    for(int training_times=0;training_times<TRAINING_TIMES;training_times++)
    {
        //总的损失值（对于96个数据而言的）
        double totalC=0;
        Matrix* gradient= new Matrix(1,paramList.param_num);

        //初始化总的偏导数
        for(int i=0;i<paramList.param_num;i++)
        {
            gradient->m[0][i]=0;
        }
        //total_differ->Print();

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
                //pool中存放的是zp
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
                //zos里面存的是Zno outputs里面存的是Ano,zos是输出层的输出值
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

            //开始优化
            //pools[0]->Print();
            //求出每一个图片的偏导数
            Matrix* differ = derivative(paramList,img,filter_deltas,output_deltas,pools);
            //differ->Print();
            gradient->add(*differ);
            //释放相应的内存
            onDestroy(pools,conv_inputs,filter_deltas,differ);
        }

        //根据梯度更新参数
        //梯度乘以步长,并且是负梯度方向，所以要记得加负号
        gradient->multiply(-alpha);
        paramVector->add(*gradient);
        putParamInList(paramList,paramVector); //将梯度重新存回去
        //printParamList(paramList);

        cout<<"------------ totalC ----------- "<<endl;
        cout<<totalC<<endl;
        output_cost<<"the "<<training_times+1<<" times training , the cost is "<<totalC<<"\n";
        cout<<"------------ totalC ----------- "<<endl;
        free(gradient);
    }
    for(int i=0;i<paramVector->column;i++)
    {
        output_params<<paramVector->m[0][i]<<" ";
    }

    //关闭打开文件
    output_params.close();
    output_cost.close();
    return 0;
}



void initAnswer(vector<vector<double>> &answers)
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

void onDestroy(vector<Matrix*> pools,vector<Matrix*> conv_inputs,vector<Matrix*> filter_deltas,Matrix* differ)
{
    for(Matrix* pool: pools)
        free(pool);
    for(Matrix* input:conv_inputs)
        free(input);
    for(Matrix* delta:filter_deltas)
        free(delta);
    free(differ);
}