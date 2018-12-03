#include"Bint.h"

/*************************************
 这里可以定义自己的最长位数
 #define MAX_BIT_LEN  XXX
 ************************************/

int main(){
	BINT_t *op1,*op2,*rst;
	
	while((op1=BintGet("第一个操作数"))==NULL);
	while((op2=BintGet("第二个操作数"))==NULL); 
	
	rst=Bint_ADD(op1,op2);
	printf("加法结果："); 
	BintShow(rst);
	rst=Bint_SUB(op1,op2);
	printf("减法结果：");
	BintShow(rst);
	rst=Bint_MUL(op1,op2);
	printf("乘法结果：");
	BintShow(rst);
	rst=Bint_DIV(op1,op2);
	printf("除法结果：");
	BintShow(rst);
	rst=Bint_MOD(op1,op2);
	printf("模结果：");
	BintShow(rst);
	
	return 0;
}