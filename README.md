# BigInt大整数运算库

#### 文件构成结构

```c
---|
   |-Bint.h 自己写的大整数运算库
   |-EXAMPLE.c 使用示例
```



#### 前言

1. 这个版本为第一个版本，未有太多优化。大多采用类似手算的算法

 	2. 这个算法内存占用过大。自定义的结构体数字表示采用定长的方法，后期将优化采用动态长度的方法，减少内存占用。

### Bint.h头文件具体解析

****

#### 数据结构表示

数字的存储是以十进制为基础的，所以把一个char的大小定义为一个十进制存储bitDec。

而一个数字的表示采用基于bitDec的数组，数组长度为MAX_BIT_LEN(ps:内存占用大，后边要改为动态数组)。sign表示符号位，len表示有效长度。

```c
typedef char bitDec; 

typedef struct intSave{
	bitDec  numer[MAX_BIT_LEN];//十进制数字 
	char  sign;//符号位 
	unsigned long long len; //长度 
}BINT_t;
```

#### 一些宏定义

```c
#define ASCII_NUM_9 57 //ascill码的9 用于输入修正
#define ASCII_NUM_0 48 //同上
#define POS  1
#define NEG  0        //符号位
#define  Fix(n) ((n)-ASCII_NUM_0) //把一个字符的数字修正为具体的数字
#define NOT_NUM 0x88  //数字数组中不是数字的填充 随意起的可以自定义

/******************************************************************
 一个大整数最长的位数 使用者可以自定义
*******************************************************************/
#ifndef MAX_BIT_LEN
#define MAX_BIT_LEN 500
#endif
```

#### 与用户的交互 输入 输出

数字的输入采用用户直接输入，可以允许开头带符号（+，-）或不带符号。输出也是输出一个字符串，当有负号的时候输出负号，正数不输出符号。

具体的函数定义如下

```c
BINT_t* Bint_input(char hint[]);//获得大整数 其中的hint用于给用户提示信息，如果用户输入有错则返回NULL
void Bint_output(BINT_t* Bint); //以字符的形式输出大整数

//简化使用定义 
#define BintGet(c)  (Bint_input((c))) 
#define BintShow(b)  (Bint_output((b))//函数的重定义 用于方便书写
```

其中函数的主要代码如下：

```c
printf("[%s] 请输入一个大整数(%d位以内)\n",hint,MAX_BIT_LEN);
scanf("%s",input);  //输出hint提示信息等 获得用户输入的字符串 存于input中

char* p =input;
/*符号判断*/
switch(*p){
    case '-':ret->sign=NEG;p++;break;
    case '+':ret->sign=POS;p++;break; 
    default: ret->sign=POS;
}
/*计算长度并赋值 其中开头的0不计入 输入非数字为输入错误 返回NULL*/ 
int len_cnt=0;
char begin_flag=1; 
while(*p!='\0'){
    if (*p>ASCII_NUM_9||*p<ASCII_NUM_0){
        printf(":::输入错误 非数字\n");
        free(ret);
        return NULL; 
    }else if(*p==ASCII_NUM_0&&begin_flag) {
        p++;
        continue;
    }else{
        begin_flag=0;
        ret->numer[len_cnt]=Fix(*p);//存入 并把字符形式转化为具体的数字
        p++; 
        len_cnt++; 
    }
}
//当用户全部输入0时候 则返回一个0 长度为1
```

#### 无符号的加减法

无符号的加减法都采用先运算 后再进制的方法。

- **无符号加法**

  首先对于数字的位数 结果的长度一般为两个加数长度最长的一位，但是还要预留一位用来防止进位。

  运算先采用尾对齐 然后将每一个相加到具体的位上，这时候先不进行进制。在做完一遍后，再来做进制，从尾数开始 大于10的减去10，并给前一位加一。这样就得到了结果

  ![](E:\Github\BigInt\真加法.png)

  ```c
  //最长与最短指针 
  L=maxLen==op1->len?op1:op2;
  S=maxLen==op1->len?op2:op1;
  
  /*先不进位相加*/
  long long i;
  for(i=0;i<gap;i++){
      output[i+1]=L->numer[i];
      //从头开始 把长的长出的部分复制
  } 
  for(i=gap;i<maxLen;i++){
      //rst[i]=l[i]+s[i-gap] 
      //然后再把剩下的按位相加
      output[i+1]=L->numer[i]+S->numer[i-gap];
  } 
  
  /*进位处理*/ 
  for(i=maxLen;i>0;i--){
      if(output[i]>=10){
          output[i]-=10;
          output[i-1]+=1;
      }
  } 
  ```

- **无符号减法**

  无符号减法与加法同理，区别在于结果最长长度就是两个数最长的长度。还有符号的处理，以及结果中开头会有多个0甚至全为0。

  主要代码如下：

  ```c
  /*先非进制按位减法 先不管谁减谁，先用长的减短的*/
  long long i,j;
  for(i=0;i<gap;i++){
      output[i]=L->numer[i];
  } 
  for(i=gap;i<maxLen;i++){
      //rst[i]=l[i]-s[i-gap] 
      output[i]=L->numer[i]-S->numer[i-gap];
  } 
  
  //循环进位处理 
  for(i=maxLen-1;i>0;i--){
      //当为负数的时候借一位
      if(output[i]<0){
          output[i]+=10;
          output[i-1]-=1;
      }
  } 
  
  //首位0统计 
  for(i=0;output[i]==0&&i<maxLen;i++){
      Zero_cnt++;
  }
  //全为0 
  if(Zero_cnt==maxLen){
      //输出0 
      rst->numer[0]=0;
      memset(&(rst->numer[1]),NOT_NUM,MAX_BIT_LEN-1);
      rst->len=1;
  } else{
      for(i=Zero_cnt,j=0;i<maxLen;i++,j++){
          rst->numer[j]=output[i];
      }
      memset(&(rst->numer[j]),NOT_NUM,MAX_BIT_LEN-maxLen+Zero_cnt);
      rst->len=maxLen-Zero_cnt;
  }
  
  //符号位赋值  当长的为减数是为正否则为负
  rst->sign= L==op1? POS:NEG;
  ```

  #### 有符号的加减法

  有符号的加减法进行一些合并操作后都可以转化为无符号的加减法操作。所以先对有符号的运算做判断，再进行运算调用无符号的加减法就行

  - **有符号加法**

    ```c
    //(+a)+(+b)=a+b
    //(+a)+(-b)=a-b
    //(-a)+(+b)=b-a
    //(-a)+(-b)=-(a+b)
    
    if(op1->sign==POS&&op2->sign==POS){
        rst=Bint_Unsigned_ADD(op1,op2);
    }else if(op1->sign==POS&&op2->sign==NEG){
        rst=Bint_Unsigned_SUB(op1,op2);
    }else if(op1->sign==NEG&&op2->sign==POS){
        rst=Bint_Unsigned_SUB(op2,op1);
    }else if(op1->sign==NEG&&op2->sign==NEG){
        rst=Bint_Unsigned_ADD(op1,op2);
        rst->sign=NEG;
    }
    ```

  - **有符号减法**

    ```c
    //(+a)-(+b)=a-b
    //(+a)-(-b)=a+b
    //(-a)-(+b)=-(a+b)
    //(-a)-(-b)=b-a
    
    if(op1->sign==POS&&op2->sign==POS){
        rst=Bint_Unsigned_SUB(op1,op2);
    }else if(op1->sign==POS&&op2->sign==NEG){
        rst=Bint_Unsigned_ADD(op1,op2);
    }else if(op1->sign==NEG&&op2->sign==POS){
        rst=Bint_Unsigned_ADD(op1,op2);
        rst->sign=NEG;
    }else if(op1->sign==NEG&&op2->sign==NEG){
        rst=Bint_Unsigned_SUB(op2,op1);
    }
    ```

#### 乘法

乘法时候用短的乘数每一位去乘长的乘数，类似加法，先不进行进制，进行一轮后再进行进制。这样一轮一轮最后得到结果。乘法的结果长度为两个乘数长度的和。

![](E:\Github\BigInt\乘法.png)

```c
for(i=minLen-1;i>=0;i--){
    //每一位进行乘法 存入到output对应的位中
    for(j=maxLen-1;j>=0;j--){
        output[i+j+1] +=L->numer[j]*S->numer[i];
    }	
    // 对每一位进行进制处理 把十位移入到前一位中
    for(j=maxLen+i;j>i;j--) {
        output[j-1]+=output[j]/10;
        output[j]=output[j]%10;
    }
}
```

#### 除法

除法可以转换为减法，但是一位一位的减太麻烦。所以从开头进行试着减，减到结果为负未知。因此，需要另外增加一个存储工具，长度为除数的长度+1，用来表示每次被减的数，当结果为负的时候表示，得到结果，然后并从被除数中补充下一位，进行下一轮运算。

![](E:\Github\BigInt\除法.png)

```c
for(i=0;i<rstLen;i++){
    if(i==0){
        //第一次取前从op1取len2个数 subc首位为0 
        for(j=0;j<len2;j++){
            subc[j+1]=op1->numer[j];
        } 
    }
    else{
        //第二次开始
        //前len2个移位并加回 
        for(j=0;j<len2;j++){
            subc[j]=subc[j+1]+op2->numer[j];
        } 
        //补充一个进来 
        subc[len2]=op1->numer[i+len2-1];	
    } 

    //1-10试减 
    for(j=1;j<=10;j++){
        //减法 
        for(k=0;k<len2;k++){
            subc[k+1]=subc[k+1]-op2->numer[k];
        }

        //小于0判断
        s0_flag=0; //是否小于0 
        for(k=0;k<len2+1;k++) {
            //第一个非零数为负数 则一定为负数 
            if(subc[k]<0){
                s0_flag=1;
                break;
            } 
            //第一个非零数为正数 则一定为正数 
            else if(subc[k]>0){
                break; 
            }
        }

        //小于0 写output 
        if(s0_flag){
            output[i]=j-1;
            break;
        }
        //否则 负数进制
        for(k=len2;k>0;k--)	{
            if(subc[k]<0){
                subc[k]+=10;
                subc[k-1]--;
            }
        }
    }
```



#### 模余

模余同于除法，只是将除法的的余数输出即可。