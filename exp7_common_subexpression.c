#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int i,j,k;
typedef struct{
char oper;
char op1[5];
char op2[5];
char result[5];
}instruction;
int issame(instruction *a,instruction *b)
{
    if(a->oper!=b->oper)
    {
        return 0;
    }
    if(a->oper=='+' ||a->oper=='*')
    {
        return((strcmp(a->op1,b->op1)==0&&strcmp(a->op2,b->op2)==0)||(strcmp(a->op2,b->op1)==0&&strcmp(a->op1,b->op2)==0));
    }
    else
    {
        return(strcmp(a->op1,b->op1)==0&&strcmp(a->op2,b->op2)==0);
    }
}
int redefined(instruction instr[],int i,int j)
{
    for(k=i;k<j;k++)
    {
        if(instr[k].oper=='=')
        {
            if(strcmp(instr[k].op1, instr[j].op1) == 0 || strcmp(instr[k].op1, instr[j].op2) == 0)
            {
                return 1;
            }
        }
    }
    return 0;
}
void common_sub(instruction instr[],int n)
{
    for(i=0;i<n;i++)
    {
        if(instr[i].oper=='0')
            continue;
        for(j=i+1;j<n;j++)
        {
            if(instr[j].oper=='0')
            continue;
            if(issame(&instr[i],&instr[j])&&!redefined(instr,i,j))
            {
                for(k=j+1;k<n;k++)
                {
                    if(strcmp(instr[k].op1,instr[j].result)==0)
                    {
                        strcpy(instr[k].op1,instr[i].result);
                    }
                    if(strcmp(instr[k].op2,instr[j].result)==0)
                    {
                        strcpy(instr[k].op2,instr[i].result);
                    }
                }
                instr[j].oper='0';
            }
        }
    }
}
void printins(instruction instr[],int n)
{
    for (int i = 0; i < n; i++) {
        if (instr[i].oper != '0') {
            printf("\n%c %s %s %s", instr[i].oper, instr[i].op1, instr[i].op2, instr[i].result);
        }
    }
}
void main()
{
    instruction instr[100];
    int n;
    printf("enter the number of instructions:");
    scanf("%d",&n);
    for(i=0;i<n;i++)
    {
      scanf(" %c %s %s %s",&instr[i].oper,instr[i].op1,instr[i].op2,instr[i].result);
    }
    common_sub(instr,n);
    printins(instr,n);
}

/* output


enter the number of instructions:6
* a b t1
= a # 5
* a b t2
+ c t1 t3
* a b t4
- t3 t4 t5

* a b t1
= a # 5
* a b t2
+ c t1 t3
- t3 t2 t5


*/