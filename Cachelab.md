###Cachelab

#### Part A

获取命令行参数和，申请数组空间，解析trace是3个难点。用数组模拟LRN算法，最近使用过的block的tag放在最前面。用位运算从address中截取出tag和组索引，而块偏移不需要使用。

#### Part B

PDF第九页提到了叫做Blocking的技巧，可以有效减少miss次数，另外本地变量最多12个尽量利用起来可以减少两个矩阵之间的conflict miss次数。



b部分模拟的cache是一个32组，每组一行，每行32字节的直接映射cache，而数组元素都定义为int型，每个占4字节，故每行cache可放$32/4=8$个元素。



##### 1）M = 32， N = 32

完全按行序转换时，从A中取数每8个元素miss一次，而向B中存数每一次都会miss。利用Blocking技巧，将矩阵划分成$8×8$的块，则A的miss次数不变，而B的miss次数（当A与B之间没有conflict miss时）降低到与A相同。测试结果如下：

Summary for official submission (func 0): correctness=1 misses=287

TEST_TRANS_RESULTS=1:287

##### 2）M = 64， N = 64

若仍划分成$8×8$的块，则A的miss次数不变，而B全部miss。作分析如下：![](https://raw.githubusercontent.com/miigao/misc/master/cachelabpartb6464-1.png "title")

数字表示该处元素是在第几次miss之后被换入cache中的。即，对于一个$8×8$的块，从A中取数总共造成8次miss。

![](https://raw.githubusercontent.com/miigao/misc/master/cachelabpartb6464-2.png "title")

对于B的访问全部miss。



进一步将$8×8$的块划分为4个$4×4$的块，并按照对A橙红蓝绿的顺序依次访问，对B橙蓝红绿的顺序依次访问可显著降低B的miss次数（当A与B之间没有conflict miss时）至8次，而这样会导致A的miss次数升高至至少16次，因为访问红区时，橙区被换出，而蓝区和橙区在相同的cache里，故也被换出，当接下来再访问蓝区时，需要再重新调入，故miss次数翻倍。

![](https://raw.githubusercontent.com/miigao/misc/master/cachelabpartb6464-3.png "title")

要在降低B的miss次数同时不让A的miss次数升高，有一个比较tricky的办法（应该也是唯一的方法）：

第一步，从A中按行取橙区和蓝区，转置后按列存入B中橙区和蓝区，此时，AB分别miss最少4次，而存入B中蓝区的数据只是暂存在这里。第二步，取出B中蓝区一行待用，取出A中红区一列，转置成一行存入B中蓝区，然后将B中蓝区待用的一行存入B中红区的一行，此操作执行4次，AB分别miss最少4次。第三步，转置绿区，常规操作，miss次数为零，故总共miss次数为AB各8次。

##### 3）M = 61， N = 67

cache映射是斜的，实在看不出规律，故采用瞎猜分块法，当划分成$12×4$的块时，总miss次数为1932次，而规则里要求2000次以下可以得满分，已满足要求。

分析：（无）

##### Conclusion

Cache Lab summary:
                        Points   Max pts      Misses
Csim correctness          27.0        27
Trans perf 32x32           8.0         8         287
Trans perf 64x64           8.0         8        1171
Trans perf 61x67          10.0        10        1932
          Total points    53.0        53