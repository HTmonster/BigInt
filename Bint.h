# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# include<math.h>
# include <assert.h>

/******************************************************************
 ���� ʹ��char��ʾһ��ʮ�������йص�һЩ�궨�� 
*******************************************************************/

#define ASCII_NUM_9 57
#define ASCII_NUM_0 48
#define POS  1
#define NEG  0
#define  Fix(n) ((n)-ASCII_NUM_0)
#define NOT_NUM 0x88

/******************************************************************
 һ�����������λ�� 
*******************************************************************/
#ifndef MAX_BIT_LEN
#define MAX_BIT_LEN 500
#endif

/******************************************************************
  ���ݽṹ���� 
*******************************************************************/
typedef char bitDec;

typedef struct intSave{
	bitDec  numer[MAX_BIT_LEN];//ʮ�������� 
	char  sign;//����λ 
	unsigned long long len; //���� 
}BINT_t;

/******************************************************************
 �������� 
*******************************************************************/
//����  
BINT_t* Bint_input(char hint[]);
void Bint_output(BINT_t* Bint); 

//��ʹ�ö��� 
#define BintGet(c)  (Bint_input((c))) 
#define BintShow(b)  (Bint_output((b)))

//�޷��żӼ� 
BINT_t* Bint_Unsigned_ADD(BINT_t* op1,BINT_t* op2);
BINT_t* Bint_Unsigned_SUB(BINT_t* op1,BINT_t* op2);
//�з��żӼ� 
BINT_t* Bint_ADD(BINT_t* op1,BINT_t* op2);
BINT_t* Bint_SUB(BINT_t* op1,BINT_t* op2); 
//�˷� ���� 
BINT_t* Bint_MUL(BINT_t* op1,BINT_t* op2); 
BINT_t* Bint_DIV(BINT_t* op1,BINT_t* op2);
//ģ��
BINT_t* Bint_MOD(BINT_t* op1,BINT_t* op2);

//////////////////////////////////���庯��//////////////////////////////////////
//               ���� �޷��żӼ� �з��żӼ� �˳� ģ��                        // 
/////////////////////////////////////////////////////////////////////////////// 

/******************************************************
* Des: ���û�������һ���������ṹ 
* @Argu:�ַ��� [��ʾ��Ϣ] 
* Returns:ָ��  �������ṹ 
*******************************************************/
BINT_t* Bint_input(char hint[]){
    //����Ļ��� 
	char input[MAX_BIT_LEN+1]; 
	
	//�����ṹ���� 
	BINT_t* ret= (BINT_t*)malloc(sizeof(BINT_t));
	
	//���� 
	printf("[%s] ������һ��������(%dλ����)\n",hint,MAX_BIT_LEN);
	scanf("%s",input);
	
	//�α�  ���㳤�� 
	char* p =input;
	
	/*�����ж�*/
	switch(*p){
		case '-':ret->sign=NEG;p++;break;
		case '+':ret->sign=POS;p++;break; 
		default: ret->sign=POS;
	}
 	/*���㳤�Ȳ���ֵ*/ 
	int len_cnt=0;
	char begin_flag=1; 
	while(*p!='\0'){
		if (*p>ASCII_NUM_9||*p<ASCII_NUM_0){
			printf(":::������� ������\n");
			free(ret);
			return NULL; 
		}else if(*p==ASCII_NUM_0&&begin_flag) {
			p++;
			continue;
		}else{
			begin_flag=0;
			ret->numer[len_cnt]=Fix(*p);
			p++; 
			len_cnt++; 
		}
	}
	//ȫΪ0
	if(*p=='\0'&&begin_flag==1) {
		ret->numer[0]=0;
		ret->len=len_cnt=1;
	}
	
	//��lenλ 
	ret->len=len_cnt; 
	
	//����ʣ���Ϊ0x88 -120 	
	memset(&(ret->numer[len_cnt]),NOT_NUM,MAX_BIT_LEN-len_cnt); 
	
#ifdef _DEBUG 
	for(int i=0;i<ret->len;i++){
		printf("%d ",ret->numer[i]);}
	printf("\nlen:%d",ret->len);
	printf(" sign:%d\n",ret->sign);
#endif
	
	return ret;
} 


/******************************************************
* Des: ��BINTת��Ϊ������ʽ��� 
* @Argu:BINT_t* Ҫ��������� 
* @Returns:void 
*******************************************************/
void Bint_output(BINT_t* Bint){
	
	if(Bint->sign==NEG){
		printf("-");
	}
	long long i;
	for(i=0;i<Bint->len;i++){
		printf("%d",Bint->numer[i]);
	}
	printf("\n");
}

/******************************************************
* Des: �޷��ŵļӷ� 
* @Argu:BINT_t* ����
*       BINT_t* ���� 
* @Returns: BINT* ��Ӻ�Ľ�� sign Ĭ��Ϊ��  
*******************************************************/
BINT_t* Bint_Unsigned_ADD(BINT_t* op1,BINT_t* op2){
	unsigned long long maxLen,rstLen,gap;
	BINT_t * rst,*L,*S;
	bitDec * output; 
	
	assert(op1!=NULL&&op2!=NULL); 
	
	//�����󳤶�  ��� 
	maxLen=(op1->len >op2->len)?op1->len:op2->len;
	gap=abs(op1->len - op2->len);
	
	//������ָ�� 
	L=maxLen==op1->len?op1:op2;
	S=maxLen==op1->len?op2:op1;

	//��� �ṹ 
	rst=(BINT_t*)malloc(sizeof(BINT_t));
	
	
	//����������� ��һλ��Ϊ��λԤ�� 
	rstLen=maxLen+1;
	output=(bitDec*)malloc(sizeof(bitDec)*rstLen); 
	
	//��λ ��Ϊ��λԤ�� 
	output[0]=0; 
	
	//�ǽ�λ��� 
	long long i;
	for(i=0;i<gap;i++){
		output[i+1]=L->numer[i];
	} 
	for(i=gap;i<maxLen;i++){
		//rst[i]=l[i]+s[i-gap] 
		output[i+1]=L->numer[i]+S->numer[i-gap];
	} 
#ifdef _DEBUG	
	printf("output: ");
	for(i=0;i<maxLen+1;i++){
		printf("%d ",output[i]);} 
	printf("\n");
#endif
	
	//ѭ����λ���� 
	for(i=maxLen;i>0;i--){
		if(output[i]>=10){
			output[i]-=10;
			output[i-1]+=1;
		}
	} 

	//��λ0�ж� 
	if(output[0]==0){
		output++;
		rstLen--;
	}
	
	//�����ֵ 
	memcpy(rst->numer,output,rstLen);
	memset(&(rst->numer[rstLen]),NOT_NUM,MAX_BIT_LEN-rstLen);
	rst->sign=POS;//����λΪ�� 
	rst->len=rstLen; 
	
	//�ͷžֲ���Դ 
	free(output);

#ifdef _DEBUG
	printf("�޷��żӷ������\n");
	for(i=0;i<rst->len;i++){
		printf("%d ",rst->numer[i]);}
	printf("\nsign:%d len:%d\n",rst->sign,rst->len);
#endif
	return rst;
} 

/******************************************************
* Des: �޷��� ���� 
* @Argu:BINT_t* op1������ op2���� 
* @Returns:BINT_t ��Ӻ�Ľ��  ����λ���ݽ������ 
*******************************************************/
BINT_t* Bint_Unsigned_SUB(BINT_t* op1,BINT_t* op2){
	unsigned long long maxLen=0,gap,Zero_cnt=0;
	BINT_t * rst,*L,*S;
	bitDec * output; 
	
	assert(op1!=NULL&&op2!=NULL);  
	
	//�����󳤶�  ��� 
	maxLen=(op1->len >op2->len)?op1->len:op2->len;
	gap=abs(op1->len - op2->len);
	
	//������ָ�� 
	L=maxLen==op1->len?op1:op2;
	S=maxLen==op1->len?op2:op1;

	//��� �ṹ 
	rst=(BINT_t*)malloc(sizeof(BINT_t));
	
	//����������� 
	output=(bitDec*)malloc(sizeof(bitDec)*(maxLen));
	
 	long long i,j;
	for(i=0;i<gap;i++){
		output[i]=L->numer[i];
	} 
	for(i=gap;i<maxLen;i++){
		//rst[i]=l[i]-s[i-gap] 
		output[i]=L->numer[i]-S->numer[i-gap];
	} 
	
	//ѭ����λ���� 
	for(i=maxLen-1;i>0;i--){
		if(output[i]<0){
			output[i]+=10;
			output[i-1]-=1;
		}
	} 
	
	
	//��λ0ͳ�� 
	for(i=0;output[i]==0&&i<maxLen;i++){
		Zero_cnt++;
	}
	//ȫΪ0 
	if(Zero_cnt==maxLen){
		//���0 
		rst->numer[0]=0;
		memset(&(rst->numer[1]),NOT_NUM,MAX_BIT_LEN-1);
		rst->len=1;
	} else{
		for(i=Zero_cnt,j=0;i<maxLen;i++,j++){
			rst->numer[j]=output[i];
		}
		//memcpy(rst->numer,&output[Zero_cnt],maxLen-Zero_cnt);
		memset(&(rst->numer[j]),NOT_NUM,MAX_BIT_LEN-maxLen+Zero_cnt);
		rst->len=maxLen-Zero_cnt;
	}
	
	//����λ��ֵ 
	rst->sign= L==op1? POS:NEG;
	
	//�ͷžֲ���Դ 
	free(output);
	
#ifdef _DEBUG	
	printf("�޷��ż������: \n");
	for(i=0;i<rst->len	;i++){
		printf("%d ",rst->numer[i]);} 
	printf("\nsign:%d len:%d \n",rst->sign,rst->len);
#endif

	return rst; 
} 


 
/******************************************************
* Des: ������ �������ӷ� 
* @Argu: BINT_t* 
*        BINT_t* Ҫ��ӵ����������� 
* @Returns: BINT_t* �Ӻ�Ľ�� 
*******************************************************/
BINT_t* Bint_ADD(BINT_t* op1,BINT_t* op2){
	BINT_t * rst;
	
	assert(op1!=NULL&&op2!=NULL); 
	
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
	
#ifdef _DEBUG	
	printf("�з��żӷ����: \n");
	for(int i=0;i<rst->len	;i++){
		printf("%d ",rst->numer[i]);} 
	printf("\nsign:%d len:%d \n",rst->sign,rst->len);
#endif

	return rst;
}

/******************************************************
* Des: ����������  op1-op2 
* @Argu: BINT_t* ������
*        BINT_t* ���� 
* @Returns:BINT_t* ��� 
*******************************************************/
BINT_t* Bint_SUB(BINT_t* op1,BINT_t* op2){
	BINT_t * rst;
	
	assert(op1!=NULL&&op2!=NULL); 
	
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
#ifdef _DEBUG	
	printf("�з��ż������: \n");
	for(int i=0;i<rst->len	;i++){
		printf("%d ",rst->numer[i]);} 
	printf("\nsign:%d len:%d \n",rst->sign,rst->len);
#endif
	return rst;
}

/******************************************************
* Des: �������˷� 
* @Argu: BINT_t* op1 op2 �������� 
* @Returns:BINT_t* �˷���� 
*******************************************************/
BINT_t* Bint_MUL(BINT_t* op1,BINT_t* op2){
    unsigned long long maxLen,minLen,rstLen;
	BINT_t * rst,*L,*S;
	bitDec * output; 
    long long i,j;
	
	assert(op1!=NULL&&op2!=NULL); 
	
	//���� ��̳��� 
	maxLen=(op1->len >op2->len)?op1->len:op2->len;
	minLen=(op1->len==maxLen)?op2->len:op1->len;
	
	//������� Ԥ��һλ��λ 
	rstLen=maxLen+minLen; 
	
#ifdef _DEBUG
	printf("maxlen:%d\t",maxLen);
	printf("minlen:%d\t",minLen);
	printf("rstlen:%d\n",rstLen);
#endif
	
	//������ָ�� 
	L=maxLen==op1->len?op1:op2;
	S=maxLen==op1->len?op2:op1;

	//��� �ṹ 
	rst=(BINT_t*)malloc(sizeof(BINT_t));
	
	//����������� 
	output=(bitDec*)malloc(sizeof(bitDec)*(rstLen));
	memset(output,0,rstLen);

#ifdef _DEBUG
	printf("output��ʼ��Ϊ:");
	for(int k=0;k<rstLen;k++) printf("%d ",output[k]);
	printf("\n");
#endif 
		
	for(i=minLen-1;i>=0;i--){
		//�ǽ��� 
		for(j=maxLen-1;j>=0;j--){
			output[i+j+1] +=L->numer[j]*S->numer[i];
		}	
#ifdef _DEBUG
	printf("[%d]:",i);
	for(int k=0;k<rstLen;k++) printf("%2d ",output[k]);
	printf("\n");
#endif 
		// ��λ
		for(j=maxLen+i;j>i;j--) {
			output[j-1]+=output[j]/10;
			output[j]=output[j]%10;
		}
#ifdef _DEBUG
	printf("[F]:",i);
	for(int k=0;k<rstLen;k++) printf("%2d ",output[k]);
	printf("\n\n");
#endif 
	}
	
	//Ԥ���׽�λ�ж� 
	if(output[0]==0){
		rstLen--;
		output++; 
	}
	
	//��ֵ��rst 
	memcpy(rst->numer,output,rstLen);
	memset(&(rst->numer[rstLen]),NOT_NUM,MAX_BIT_LEN-rstLen);
	rst->sign=(op1->sign==op2->sign)?POS:NEG;
	rst->len=rstLen;
	
	//�ͷžֲ���Դ 
	free(output);
	
#ifdef _DEBUG	
	printf("�˷����: \n");
	for(int k=0;k<rst->len	;k++){
		printf("%d ",rst->numer[k]);} 
	printf("\nsign:%d len:%d \n",rst->sign,rst->len);
#endif
	
	return rst; 
} 

/******************************************************
* Des: ���������� 
* @Argu: BINT_t* op1[������]   op2[����]   
* @Returns:BINT_t*  ���  ȡ�� 
*******************************************************/
BINT_t* Bint_DIV(BINT_t* op1,BINT_t* op2){
    unsigned long long rstLen,len1,len2;
	BINT_t * rst;
	bitDec * output,*subc; 
    long long i,j,k;
    char s0_flag=0;
    
    assert(op1!=NULL&&op2!=NULL); 
    
    if(op1->len<op2->len){
    	printf(":::��ʾ��������С�ڳ���\n");
    	
    	//����0 
    	rst=(BINT_t *)malloc(sizeof(BINT_t));
		rst->numer[0]=0;
		memset(&(rst->numer[1]),NOT_NUM,MAX_BIT_LEN-1);
		rst->sign=POS;
		rst->len=1;

#ifdef _DEBUG
	printf("ģ������\n");
	for(i=0;i<rst->len;i++){
		printf("%d ",rst->numer[i]);}
	printf("\nsign:%d len:%d\n",rst->sign,rst->len);
#endif
		
		return rst;		 
    }
    
    //������� ͬʱԤ��һ����λ 
	rstLen=op1->len-op2->len+1;
	len1=op1->len;
	len2=op2->len;
	
    //��� �ṹ 
	rst=(BINT_t*)malloc(sizeof(BINT_t));
	
	//����������� 
	output=(bitDec*)malloc(sizeof(bitDec)*(rstLen));
	memset(output,0,rstLen);
	
	//������� 
	subc=(bitDec*)malloc(sizeof(bitDec)*(len2+1));
	memset(subc,0,len2+1);
	

	for(i=0;i<rstLen;i++){
		if(i==0){
		    //��һ��ȡǰ��op1ȡlen2���� subc��λΪ0 
			for(j=0;j<len2;j++){
			   subc[j+1]=op1->numer[j];
			} 
		}
		else{
			//�ڶ��ο�ʼ
			//ǰlen2����λ���ӻ� 
			for(j=0;j<len2;j++){
				subc[j]=subc[j+1]+op2->numer[j];
			} 
			//����һ������ 
			subc[len2]=op1->numer[i+len2-1];	
		} 
#ifdef _DEBUG
	printf("[SUBC]:");
	for(int m=0;m<len2+1;m++)printf("%d ",subc[m]);
	printf("\n");
#endif
		
	    //1-10�Լ� 
		for(j=1;j<=10;j++){
			//���� 
			for(k=0;k<len2;k++){
				subc[k+1]=subc[k+1]-op2->numer[k];
			}

			//С��0�ж�
			s0_flag=0; //�Ƿ�С��0 
			for(k=0;k<len2+1;k++) {
				//��һ��������Ϊ���� ��һ��Ϊ���� 
				if(subc[k]<0){
					s0_flag=1;
					break;
				} 
				//��һ��������Ϊ���� ��һ��Ϊ���� 
				else if(subc[k]>0){
					break; 
				}
			}
#ifdef _DEBUG
	printf("[%d]:",j);
	for(int m=0;m<len2+1;m++)printf("%d ",subc[m]);
	printf("\n");
#endif
			//С��0 дoutput 
			if(s0_flag){
				output[i]=j-1;
				break;
            }
			//���� ��������
			for(k=len2;k>0;k--)	{
				if(subc[k]<0){
					subc[k]+=10;
					subc[k-1]--;
				}
			}
		}
#ifdef _DEBUG
	printf("[output]:");
	for(int m=0;m<rstLen;m++)printf("%d ",output[m]);
	printf("\n");
#endif		 
	}
	
	//�����λ�ж� 
	if(output[0]==0){
		output++;
		rstLen--;
	} 
	
	//д��
	memcpy(rst->numer,output,rstLen);
	memset(&(rst->numer[rstLen]),NOT_NUM,MAX_BIT_LEN-rstLen);
	rst->sign=(op1->sign==op2->sign) ?POS:NEG;
	rst->len=rstLen;
	
	//�ͷžֲ���Դ 
	free(output);
	free(subc);
	
#ifdef _DEBUG
	printf("���������\n");
	for(i=0;i<rst->len;i++){
		printf("%d ",rst->numer[i]);}
	printf("\nsign:%d len:%d\n",rst->sign,rst->len);
#endif
	
	return rst;
} 

/******************************************************
* Des: ������ģ�� 
* @Argu:BINT_t* op1 ������ op2 ���� 
* @Returns:BINT_t* ģ���� 
*******************************************************/
BINT_t* Bint_MOD(BINT_t* op1,BINT_t* op2){
	unsigned long long rstLen,len1,len2,rLen;
	BINT_t * rst;
	bitDec *subc,*p; 
    long long i,j,k;
    char s0_flag=0;
    
    assert(op1!=NULL&&op2!=NULL); 
    
    if(op1->len<op2->len){
    	printf(":::��ʾ��������С�ڳ���\n");
    	
    	//����op1�ĸ�ֵ 
    	rst=(BINT_t *)malloc(sizeof(BINT_t));
		memcpy(rst->numer,op1->numer,MAX_BIT_LEN);
		rst->sign=op1->sign;
		rst->len=op1->len;
		
#ifdef _DEBUG
	printf("ģ������\n");
	for(i=0;i<rst->len;i++){
		printf("%d ",rst->numer[i]);}
	printf("\nsign:%d len:%d\n",rst->sign,rst->len);
#endif
		
		return rst;		 
    }
    
    //�����Ľ������ Ԥ��һ����λ ͬʱҲ��ѭ���ĳ��� 
	rLen=op1->len-op2->len+1;
	len1=op1->len;
	len2=op2->len;
	
	//�������� 
	rstLen=len2+1;
	
    //��� �ṹ 
	rst=(BINT_t*)malloc(sizeof(BINT_t));
	
	//������� ��ģ�໺��  
	subc=(bitDec*)malloc(sizeof(bitDec)*rstLen);
	memset(subc,0,rstLen);
	

	for(i=0;i<rLen;i++){
		if(i==0){
		    //��һ��ȡǰ��op1ȡlen2���� subc��λΪ0 
			for(j=0;j<len2;j++){
			   subc[j+1]=op1->numer[j];
			} 
		}
		else{
			//�ڶ��ο�ʼ
			//ǰlen2����λ���ӻ� 
			for(j=0;j<len2;j++){
				subc[j]=subc[j+1]+op2->numer[j];
			} 
			//����һ������ 
			subc[len2]=op1->numer[i+len2-1];	
		} 
#ifdef _DEBUG
	printf("[SUBC]:");
	for(int m=0;m<len2+1;m++)printf("%d ",subc[m]);
	printf("\n");
#endif
		
	    //1-10�Լ� 
		for(j=1;j<=10;j++){
			//���� 
			for(k=0;k<len2;k++){
				subc[k+1]=subc[k+1]-op2->numer[k];
			}

			//С��0�ж�
			s0_flag=0; //�Ƿ�С��0 
			for(k=0;k<rstLen;k++) {
				//��һ��������Ϊ���� ��һ��Ϊ���� 
				if(subc[k]<0){
					s0_flag=1;
					break;
				} 
				//��һ��������Ϊ���� ��һ��Ϊ���� 
				else if(subc[k]>0){
					break; 
				}
			}
#ifdef _DEBUG
	printf("[%d]:",j);
	for(int m=0;m<rstLen;m++)printf("%d ",subc[m]);
	printf("\n");
#endif
			//С��0 дoutput 
			if(s0_flag){
				break;
            }
			//���� ��������
			for(k=len2;k>0;k--)	{
				if(subc[k]<0){
					subc[k]+=10;
					subc[k-1]--;
				}
			}
		}	 
	}
	
    //�ؼӻ� 
	for(i=0;i<len2;i++){
		subc[i+1]+=op2->numer[i];
	}
	
#ifdef _DEBUG
	printf("[subc]:");
	for(int m=0;m<rstLen;m++)printf("%d ",subc[m]);
	printf("\n");
#endif	
	
	//ģ����λ0�ж� 
	p=subc;
	while(*p==0&&rstLen>0){
		p++;
		rstLen--;
	}
	//����Ϊ0 
	if(rstLen==0){
		//д��0 
		rst->numer[0]=0;
		memset(&(rst->numer[1]),NOT_NUM,MAX_BIT_LEN-1);
		rst->sign=POS;
		rst->len=1;
	} else{
		//����д��
		memcpy(rst->numer,p,rstLen);
		memset(&(rst->numer[rstLen]),NOT_NUM,MAX_BIT_LEN-rstLen);
		rst->sign=(op1->sign==op2->sign) ?POS:NEG;
		rst->len=rstLen;	
	}
	
	
	
	//�ͷžֲ���Դ 
	free(subc);
	
#ifdef _DEBUG
	printf("ģ������\n");
	for(i=0;i<rst->len;i++){
		printf("%d ",rst->numer[i]);}
	printf("\nsign:%d len:%d\n",rst->sign,rst->len);
#endif
	
	return rst;
} 