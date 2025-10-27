#include <stdio.h>

#define MAX_STATES 26
#define MAX_SYMBOLS 10

int n; // number of states
int symbol_count; // number of input symbols
char symbols[MAX_SYMBOLS]; // single-character symbols
int transitions[MAX_STATES][MAX_SYMBOLS]; // DFA transitions
int final_states[MAX_STATES]; // 1 if final, 0 otherwise
int distinguishable[MAX_STATES][MAX_STATES];

// Map symbol to index
int symbol_to_index(char sym) {
    for (int i = 0; i < symbol_count; i++)
        if (symbols[i] == sym) return i;
    return -1;
}

// Mark distinguishable pairs using table-filling method
void mark_distinguishable() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            distinguishable[i][j] = 0;

    // Initially mark pairs with one final and one non-final
    for (int i = 0; i < n; i++)
        for (int j = i+1; j < n; j++)
            if ((final_states[i] && !final_states[j]) || (!final_states[i] && final_states[j]))
                distinguishable[i][j] = 1;

    int changed;
    do {
        changed = 0;
        for (int i = 0; i < n; i++) {
            for (int j = i+1; j < n; j++) {
                if (distinguishable[i][j]) continue;

                for (int s = 0; s < symbol_count; s++) {
                    int t1 = transitions[i][s];
                    int t2 = transitions[j][s];
                    if (t1 == -1 || t2 == -1) continue;

                    int x = t1 < t2 ? t1 : t2;
                    int y = t1 > t2 ? t1 : t2;

                    if (distinguishable[x][y]) {
                        distinguishable[i][j] = 1;
                        changed = 1;
                        break;
                    }
                }
            }
        }
    } while (changed);
}

// Print minimized DFA
void print_minimized_dfa() {
    int group[MAX_STATES];
    for (int i = 0; i < MAX_STATES; i++) group[i] = -1;
    int group_count = 0;

    // Assign groups
    for (int i = 0; i < n; i++) {
        if (group[i] == -1) {
            group[i] = group_count;
            for (int j = i+1; j < n; j++)
                if (!distinguishable[i][j]) group[j] = group_count;
            group_count++;
        }
    }

    // Print groups
    printf("\nMinimized DFA states (groups):\n");
    for (int g = 0; g < group_count; g++) {
        printf("%c = {", 'A'+g);
        int first = 1;
        for (int i = 0; i < n; i++) {
            if (group[i] == g) {
                if (!first) printf(", ");
                printf("%c", 'A'+i);
                first = 0;
            }
        }
        printf("}\n");
    }

    // Print transition table
    printf("\nTransition table:\nState");
    for (int s = 0; s < symbol_count; s++) printf("\t%c", symbols[s]);
    printf("\n");

    for (int g = 0; g < group_count; g++) {
        printf("%c", 'A'+g);
        for (int s = 0; s < symbol_count; s++) {
            int dest = -1;
            for (int i = 0; i < n; i++)
                if (group[i] == g) { dest = group[transitions[i][s]]; break; }
            printf("\t%c", 'A'+dest);
        }
        printf("\n");
    }

    // Print final states
    printf("\nFinal states:\n");
    for (int g = 0; g < group_count; g++)
        for (int i = 0; i < n; i++)
            if (group[i] == g && final_states[i]) { printf("%c\n", 'A'+g); break; }
}

int main() {
    printf("Enter number of states (max 26): ");
    scanf("%d", &n);

    printf("Enter number of input symbols: ");
    scanf("%d", &symbol_count);

    printf("Enter input symbols (single characters, e.g., 01 or ab): ");
    scanf("%s", symbols);

    // Initialize transitions and final states
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < symbol_count; j++)
            transitions[i][j] = -1;
        final_states[i] = 0;
    }

    // Read transitions
    int t;
    printf("Enter number of transitions: ");
    scanf("%d", &t);
    printf("Enter transitions in format: A+0->B\n");
    for (int i = 0; i < t; i++) {
    char from,to,arrow[3],sym;
        scanf(" %c %c %c", &from, &sym, &to);
    int from_index = from - 'A';
    int to_index = to - 'A';
    int sym_index = symbol_to_index(sym);
    transitions[from_index][sym_index] = to_index;
}
    // Read final states
    int fcount;
    printf("Enter number of final states: ");
    scanf("%d", &fcount);
    if (fcount > 0) {
        printf("Enter final states (A-Z):\n");
        for (int i = 0; i < fcount; i++) {
            char f;
            scanf(" %c", &f);
            final_states[f - 'A'] = 1;
        }
    }

    mark_distinguishable();
    print_minimized_dfa();
    return 0;
}

/*


Enter number of states (max 26): 5
Enter number of input symbols: 2
Enter input symbols (single characters, e.g., 01 or ab): ab
Enter number of transitions: 10
Enter transitions in format: A+0->B
A a B
A b C
B a D
C a B
C b C
D a B
E a B
E b C
B b D
D b E
Enter number of final states: 1
Enter final states (A-Z):
E

Minimized DFA states (groups):
A = {A, C}
B = {B}
C = {D}
D = {E}

Transition table:
State   a       b
A       B       A
B       C       C
C       B       D
D       B       A

Final states:
D

*/