电梯基本状态信息：
S1 电梯位置（层）;
S2 电梯状态（运动，停止）;
S3 电梯运行方向（上行，下行）：在电梯状态为运动状态时有效
S4 门开关状态（开，关）;
S5 轿厢有无人员状态（有，无）;
S6 冲顶状态（是，否）;
S7 蹲底状态（是，否）;
S8 电源状态（有，无）;
S9 平层状态（是，否）;

传感器：
双光束传感器：
	两路输入 有先后顺序 来判断上升还是下降
	IN_2 PE1  上平层信号  A
	IN_3 PE2  下平层信号  B
	下降沿跳变表示开始上升或下降
	A先触发表示上升
	B先触发表示下降
	可以得到S1、S2、S3、S9等信息
门闭合传感器：
	一路输入 来判断电梯门打开还是关闭
	IN_1 PE0
	高电平表示门打开
	低电平表示门关闭
	可以得到S4信息
限位开关传感器：
	两路输入 来判断电梯是否冲顶或者是否蹲底
	IN_4 PE3  下限位开关
	IN_5 PE4  上限位开关
	高电平表示未触发，电梯运行正常
	低电平表示冲顶或蹲底
	可以得到S6、S7等信息
	
电梯的八种运行状态
1、平层运行
2、平层静止
3、上升1
4、上升2
5、上升3
6、下降1
7、下降2
8、下降3
定义一个运行状态变量（枚举类型）state
在每次转换运行状态的时候检测当前状态是不是即将要转换的状态的上一个状态。
