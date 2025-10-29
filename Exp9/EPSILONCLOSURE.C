#include<stdio.h>
int n,sc,i,j,k,t,l;
char symbol[10];
int trans[20][20][20],closure[20];
int getindex(char c)
{
    for(l=0;l<sc;l++)
    {
        if(symbol[l]==c)
        {
            return l+1;
        }
    }
    return -1;
}
void findclosure(int state,int closure[])
{
    if(closure[state])return;
    closure[state]=1;
    for(int p=0;p<n;p++)
    {
        if(trans[state][0][p])
        {
            findclosure(p,closure);
        }
    }
}
void printclosure()
{
    printf("\nEpsilon Closures Table:\n");
    printf("State | Epsilon Closure\n");
    printf("-----------------------\n");
    for (int i = 0; i < n; i++) {
      int closure[20] = {0};
      printf("%c :{",'A'+i);
      findclosure(i,closure);
      int first=1;
      for(k=0;k<n;k++)
      {
          if(closure[k])
          {
              if(!first)
                printf(",");
              printf("%c",'A'+k);
              first=0;
          }
      }
      printf("}\n");
    }
}
int main()
{
    printf("enter the number of states:");
    scanf("%d",&n);
    printf("enter the number of symbols:");
    scanf("%d",&sc);
    printf("enter the symbols without space:");
    scanf("%s",symbol);
    for(i=0;i<n;i++)
    {
        for(j=0;j<=sc;j++)
        {
            for(k=0;k<n;k++)
            {
                trans[i][j][k]=0;
            }
        }
    }
    printf("enter number of transitions:");
    scanf("%d",&t);
    printf("enter the transitions : (A e B)");
    for(i=0;i<t;i++)
    {
        char from,to,arrow[3],sym;
        scanf(" %c %c %c", &from, &sym, &to);
        int fromindex=from-'A';
        int toindex=to-'A';
        int symindex=(sym=='e')?0:getindex(sym);
        trans[fromindex][symindex][toindex]=1;
    }
    printclosure();
return 0;

}


/*


enter the number of states:4
enter the number of symbols:2
enter the symbols without space:ab
enter number of transitions:5
enter the transitions : (A e B)A e B
A e C
B b C
B a B
C e D

Epsilon Closures Table:
State | Epsilon Closure
-----------------------
A :{A,B,C,D}
B :{B}
C :{C,D}
D :{D}

*/