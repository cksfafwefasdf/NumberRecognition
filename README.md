# NumberRecognition
# 目录结构
NumberRecognition
├── CMakeLists.txt
├── data
│   ├── data1.txt
│   ├── data2.txt
│   ├── init_filter.txt 初始化Filter的数据
│   ├── init_output_layer.txt 初始化输出层的数据
│   └── true_answer.txt 正确的结果
├── main.cpp
├── matrix_func 矩阵类和操作
│   ├── matrix.cpp
│   └── matrix.h
├── output_data 输出数据
│   ├── cost 记录损失值
│   │   └── cost.txt 
│   └── model 记录训练完以后的参数
│       └── params.txt
├── README.md
├── train 训练相关的文件
│   ├── filter.cpp 过滤器
│   ├── output.cpp 输出层单元
│   ├── param_list.cpp 参数列表的一些操作
│   ├── readfile.cpp 读取文件相关的操作
│   ├── train.cpp  训练相关的操作
│   └── train.h 
└── tree

##### 只能识别数字 1 2 3
##### 图像尺寸 6X6
##### 激活函数：sigmoid函数
##### 采用最大池化
##### 只有一层隐藏层（一个卷积层 一个池化层）
##### 一个输出层
##### 一个输入层
