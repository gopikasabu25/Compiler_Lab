#include <stdio.h>
#include <string.h>
#include <ctype.h>

char prod[10][10], first[10][10], follow[10][10], nt[10];
int n, ntc = 0;

int idx(char c)
{
    for (int i = 0; i < ntc; i++)
        if (nt[i] == c)
            return i;
    return -1;
}
void add(char *s, char c)
{
    if (!strchr(s, c))
    {
        int l = strlen(s);
        s[l] = c;
        s[l + 1] = '\0';
    }
}

void first_calc(char c)
{
    int ci = idx(c);
    if (ci == -1 || first[ci][0])
        return;
    for (int i = 0; i < n; i++)
        if (prod[i][0] == c)
        {
            if (prod[i][3] == c)
                continue;
            if (prod[i][3] == '#')
            {
                add(first[ci], '#');
                continue;
            }
            int all_eps = 1;
            for (int j = 3; j < strlen(prod[i]); j++)
            {
                char sym = prod[i][j];
                int eps = 0;
                if (!isupper(sym)) // terminal
                {
                    add(first[ci], sym);
                    all_eps = 0;
                    break;
                }
                first_calc(sym);
                int si = idx(sym);
                for (int k = 0; k < strlen(first[si]); k++)
                    if (first[si][k] == '#')
                        eps = 1;
                    else
                        add(first[ci], first[si][k]);
                if (!eps)
                {
                    all_eps = 0;
                    break;
                }
            }
            if (all_eps)
                add(first[ci], '#');
        }
}

void follow_calc(char c)
{
    int ci = idx(c);
    if (ci == -1)
        return;
    if (c == nt[0] && !strchr(follow[ci], '$'))
        add(follow[ci], '$');
    for (int i = 0; i < n; i++)
        for (int j = 3; j < strlen(prod[i]); j++)
            if (prod[i][j] == c)
            {
                int k = j + 1, done = 0;
                while (!done)
                {
                    if (k < strlen(prod[i]))
                    {
                        char ns = prod[i][k];
                        int eps = 0;
                        if (!isupper(ns))
                        {
                            add(follow[ci], ns);
                            done = 1;
                        }
                        else
                        {
                            int ni = idx(ns);
                            for (int l = 0; l < strlen(first[ni]); l++)
                                if (first[ni][l] == '#')
                                    eps = 1;
                                else
                                    add(follow[ci], first[ni][l]);
                        }
                        if (!eps)
                            done = 1;
                        k++;
                    }
                    else
                    {
                        if (prod[i][0] != c)
                        {
                            follow_calc(prod[i][0]);
                            int hi = idx(prod[i][0]);
                            for (int l = 0; l < strlen(follow[hi]); l++)
                                add(follow[ci], follow[hi][l]);
                        }
                        done = 1;
                    }
                }
            }
}

void print_set(char *s)
{
    printf("{ ");
    for (int i = 0; i < strlen(s); i++)
        printf("%c%s", s[i], (i < strlen(s) - 1) ? ", " : "");
    printf(" }");
}

int main()
{
    printf("Enter number of productions: ");
    scanf("%d", &n);
    printf("Enter productions (e.g. S->AB or B->#):\n");
    for (int i = 0; i < n; i++)
        scanf("%s", prod[i]);
    for (int i = 0; i < n; i++)
        if (!strchr(nt, prod[i][0]))
            nt[ntc++] = prod[i][0];
    for (int i = 0; i < ntc; i++)
        first_calc(nt[i]);
    for (int i = 0; i < ntc; i++)
        follow_calc(nt[i]);
    printf("\n--- RESULTS ---\n");
    for (int i = 0; i < ntc; i++)
    {
        printf("FIRST(%c) = ", nt[i]);
        print_set(first[i]);
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < ntc; i++)
    {
        printf("FOLLOW(%c) = ", nt[i]);
        print_set(follow[i]);
        printf("\n");
    }
}