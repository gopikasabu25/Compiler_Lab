#include <stdio.h>
#include <stdlib.h>
#define MAX_STATES 26
#define MAX_SYMBOLS 10
#define MAX_DFA_STATES 1024

int n, symbol_count;
char states[MAX_STATES];
char symbols[MAX_SYMBOLS];
int transitions[MAX_STATES][MAX_SYMBOLS + 1][MAX_STATES]; // 0 = epsilon
int initial_state;
int final_states[MAX_STATES];

int get_state_index(char s) {
    for (int i = 0; i < n; i++)
    {
    if (states[i] == s)
     return i;
    }
    return -1;
}

int get_symbol_index(char s) {
    if (s == 'e') return 0; // epsilon
    for (int i = 0; i < symbol_count; i++)
        if (symbols[i] == s) return i + 1;
    return -1;
}

// Recursive epsilon closure
void epsilon_closure(int state, int closure[]) {
    if (closure[state]) return;
    closure[state] = 1;
    for (int i = 0; i < n; i++)
        if (transitions[state][0][i]) epsilon_closure(i, closure);
}

// Apply epsilon closure to a set
void epsilon_closure_set(int set[], int closure[]) {
    for (int i = 0; i < n; i++) closure[i] = 0;
    for (int i = 0; i < n; i++)
        if (set[i]) epsilon_closure(i, closure);
}

// Move on symbol
void move(int set[], int sym, int result[]) {
    for (int i = 0; i < n; i++) result[i] = 0;
    for (int i = 0; i < n; i++)
        if (set[i])
            for (int j = 0; j < n; j++)
                if (transitions[i][sym][j]) result[j] = 1;
}

// Check equality of sets
int sets_equal(int a[], int b[]) {
    for (int i = 0; i < n; i++) if (a[i] != b[i]) return 0;
    return 1;
}

// Print set
void print_set(int set[]) {
    printf("{");
    int first = 1;
    for (int i = 0; i < n; i++)
        if (set[i]) { if (!first) printf(","); printf("%c", states[i]); first = 0; }
    printf("}");
}

int main() {
    printf("Number of states: "); scanf("%d", &n);
    printf("States: ");
    for (int i = 0; i < n; i++)
        scanf(" %c", &states[i]);

    printf("Number of input symbols (excluding epsilon): ");
    scanf("%d", &symbol_count);
    printf("Symbols: ");
    for (int i = 0; i < symbol_count; i++)
        scanf(" %c", &symbols[i]);

    char c;
    printf("Initial state: "); scanf(" %c", &c);
    initial_state = get_state_index(c);

    int fcount;
    printf("Number of final states: "); scanf("%d", &fcount);
    printf("enter final state:");
    for (int i = 0; i < fcount; i++) {
        scanf(" %c", &c);
        final_states[get_state_index(c)] = 1;
    }
    // Initialize transitions
    for (int i = 0; i < n; i++)
        for (int j = 0; j <= symbol_count; j++)
            for (int k = 0; k < n; k++)
                transitions[i][j][k] = 0;

    int t;
    printf("Number of transitions: "); scanf("%d", &t);
    printf("Enter transitions (A e B), use 'e' for epsilon:\n");
    for (int i = 0; i < t; i++) {
        char from, sym, to;
        scanf(" %c %c %c", &from, &sym, &to);
        transitions[get_state_index(from)][get_symbol_index(sym)][get_state_index(to)] = 1;
    }

    // NFA -> DFA conversion
    int dfa_states[MAX_DFA_STATES][MAX_STATES] = {0};
    int processed = 0;
    int dfa_final[MAX_DFA_STATES] = {0};
    int dfa_trans[MAX_DFA_STATES][MAX_SYMBOLS] = {0};

    epsilon_closure(initial_state, dfa_states[0]);
    int dfa_count = 1;

    while (processed < dfa_count) {
        // Check final state
        for (int i = 0; i < n; i++)
            if (dfa_states[processed][i] && final_states[i]) dfa_final[processed] = 1;

        for (int s = 0; s < symbol_count; s++) {
            int move_result[MAX_STATES] = {0};
            move(dfa_states[processed], s+1, move_result);

            int closure_result[MAX_STATES] = {0};
            epsilon_closure_set(move_result, closure_result);
             int empty_set[MAX_STATES] = {0};
if (!sets_equal(closure_result, empty_set)) {
                int found = -1;
                for (int i = 0; i < dfa_count; i++)
                    if (sets_equal(dfa_states[i], closure_result)) { found = i; break; }
                if (found == -1) {
                    for (int i = 0; i < n; i++) dfa_states[dfa_count][i] = closure_result[i];
                    found = dfa_count++;
                }
                dfa_trans[processed][s] = found;
            }
        }
        processed++;
    }

    // Print DFA
    printf("\nDFA States and Transitions:\n");
    for (int i = 0; i < dfa_count; i++) {
        printf("S%d ", i);
        print_set(dfa_states[i]);
        if (dfa_final[i]) printf(" (Final)");
        printf("\n");
    }

    printf("\nDFA Transition Table:\nState");
    for (int i = 0; i < symbol_count; i++) printf("\t%c", symbols[i]);
    printf("\n");

    for (int i = 0; i < dfa_count; i++) {
        printf("S%d", i);
        for (int s = 0; s < symbol_count; s++)
            printf("\tS%d", dfa_trans[i][s]);
        printf("\n");
    }

    return 0;
}

/*

Number of states: 11
States: ABCDEFGHIJK
Number of input symbols (excluding epsilon): 2
Symbols: ab
Initial state: A
Number of final states: 1
enter final state:K
Number of transitions: 13
Enter transitions (A e B), use 'e' for epsilon:
A e B
A e H
B e C
B e E
C a D
E b F
D e G
F e G
G e B
H a I
I b J
J b K
G e H

DFA States and Transitions:
S0 {A,B,C,E,H}
S1 {B,C,D,E,G,H,I}
S2 {B,C,E,F,G,H}
S3 {B,C,E,F,G,H,J}
S4 {B,C,E,F,G,H,K} (Final)

DFA Transition Table:
State   a       b
S0      S1      S2
S1      S1      S3
S2      S1      S2
S3      S1      S4
S4      S1      S2


*/