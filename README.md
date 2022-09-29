# broadband-with-GEP

### 介绍
使用基因表达式编程组成的投票器完成二分类任务

### 样本数据
训练集和验证集数据格式相同
均为同一行中最后一个数值为分类结果(0或1)，其余为特征变量

### 使用说明
已配置为Cmake工程，编译任务包括以下两部分
- #### 静态库编译 
    编译后会在lib目录中生成静态库
    使用时分别将include目录下的头文件和lib下的静态库添加到待引入工程的包含目录和库目录即可

- #### 运行示例程序
    文件夹内会生成名为GeneExpressionProgramming的可执行文件
    ###### 调用示例代码如下：
   ```c++ 
    MultiGEP myMultiGEP;
	myMultiGEP
		.setClassifierCount(1000)
		.setTargetAccuracy(0.8)
		.setCalculator({'+', '-', '*', '/'})
		.setFeatures(10)
		.setThreshold(5.0)
		.setIndividualNum(100)
		.setGeneNum(5)
		.setConnetor('+')
		.allowSingleGene(true)
		.setMutationRate(0.044)
		.setEvaluationSetPath("./data/evaluation.txt")
		.setSaveModelPath("./data/save_model")
		.setTrainSetPath("./data/train.txt")
		.loadModel("./data/save_model")
		.train();
		myMultiGEP.evaluation();
    ```


### 代码结构
- Gene:基因类
- Indivadual:个体类
- Population:种群类
- Parameter:参数类
- DataSource:管理样本数据和文件的读写
- MultiGEP:统一调用类

