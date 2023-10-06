# NumberRecognition
# 目录结构

NumberRecognition <br>
├── CMakeLists.txt <br>
├── data<br>
│   ├── data1.txt<br>
│   ├── data2.txt<br>
│   ├── init_filter.txt 初始化Filter的数据<br>
│   ├── init_output_layer.txt 初始化输出层的数据<br>
│   └── true_answer.txt 正确的结果<br>
├── main.cpp<br>
├── matrix_func 矩阵类和操作<br>
│   ├── matrix.cpp<br>
│   └── matrix.h<br>
├── output_data 输出数据<br>
│   ├── cost 记录损失值<br>
│   │   └── cost.txt <br>
│   └── model 记录训练完以后的参数<br>
│       └── params.txt<br>
├── README.md<br>
├── train 训练相关的文件<br>
│   ├── filter.cpp 过滤器<br>
│   ├── output.cpp 输出层单元<br>
│   ├── param_list.cpp 参数列表的一些操作<br>
│   ├── readfile.cpp 读取文件相关的操作<br>
│   ├── train.cpp  训练相关的操作<br>
│   └── train.h <br>
└── tree<br>

##### 只能识别数字 1 2 3
##### 图像尺寸 6X6
##### 激活函数：sigmoid函数
##### 采用最大池化
##### 只有一层隐藏层（一个卷积层 一个池化层）
##### 一个输出层
##### 一个输入层
##### 开发环境：CLion
