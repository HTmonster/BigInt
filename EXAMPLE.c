#include"Bint.h"

/*************************************
 ������Զ����Լ����λ��
 #define MAX_BIT_LEN  XXX
 ************************************/

int main(){
	BINT_t *op1,*op2,*rst;
	
	while((op1=BintGet("��һ��������"))==NULL);
	while((op2=BintGet("�ڶ���������"))==NULL); 
	
	rst=Bint_ADD(op1,op2);
	printf("�ӷ������"); 
	BintShow(rst);
	rst=Bint_SUB(op1,op2);
	printf("���������");
	BintShow(rst);
	rst=Bint_MUL(op1,op2);
	printf("�˷������");
	BintShow(rst);
	rst=Bint_DIV(op1,op2);
	printf("���������");
	BintShow(rst);
	rst=Bint_MOD(op1,op2);
	printf("ģ�����");
	BintShow(rst);
	
	return 0;
}