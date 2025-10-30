#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main()
{
    FILE *fp;
    fp=fopen("qua.txt","r");
    char oper,op1[10],op2[10],res[10];
    char opr[10];
    int i;
    while(!(feof(fp)))
    {
        fscanf(fp,"%c %s %s %s\n",&oper,op1,op2,res);
        switch(oper)
        {
            case '+':strcpy(opr,"ADD");
            i=1;
            break;
            case '-':strcpy(opr,"SUB");
            i=1;
            break;
            case '*':strcpy(opr,"MUL");
            i=0;
            break;
            case '/':strcpy(opr,"DIV");
            i=0;
            break;
        }
        if(strlen(op1)==2)
        {
            if(op1[1]=='1')
                printf("MOV AX,BX\n");
            else if(op1[1]=='2')
                printf("MOV AX,CX\n");
            else if(op1[1]=='3')
                printf("MOV AX,DX\n");
        }
        else
        {
            printf("MOV AX,%s\n",op1);
        }
        if(strlen(op2)==2)
        {
            if(i==1)
            {
            if(op2[1]=='1')
                printf("%s AX,BX\n",opr);
            else if(op2[1]=='2')
                printf("%s AX,CX\n",opr);
            else if(op2[1]=='3')
                printf("%s AX,DX\n",opr);
            }
            else
            {
              if(op2[1]=='1')
                printf("%s BX\n",opr);
            else if(op2[1]=='2')
                printf("%s CX\n",opr);
            else if(op2[1]=='3')
                printf("%s DX\n",opr);
            }
        }
        else
        {
            printf("%s AX,%s\n",opr,op2);
        }
        if(strlen(res)==2)
        {
            if(res[1]=='1')
                printf("MOV BX,AX\n");
            else if(res[1]=='2')
                printf("MOV CX,AX\n");
            else if(res[1]=='3')
                printf("MOV DX,AX\n");
        }
    }
}

/*

output

MOV AX,a
ADD AX,b
MOV BX,AX
MOV AX,c
ADD AX,d
MOV CX,AX
MOV AX,BX
MUL CX
MOV DX,AX

*/