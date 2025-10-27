#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct Var {
    char name[20];
    char dtype[10];
    float value;
    int isConstant; // 1 = constant, 0 = not constant
} symbolTable[50];

int varCount = 0;

// Check if a variable already exists
int isDeclared(char *name) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0)
            return 1;
    }
    return 0;
}

// Get numeric or constant variable value
float getValue(char *name, int *found) {
    if (isdigit(name[0]) || name[0] == '.') {
        *found = 1;
        return atof(name);
    }
    for (int i = 0; i < varCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0 && symbolTable[i].isConstant) {
            *found = 1;
            return symbolTable[i].value;
        }
    }
    *found = 0;
    return 0;
}

// Insert or update variable
void updateVar(char *name, char *dtype, float val, int isConst) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0) {
            symbolTable[i].value = val;
            symbolTable[i].isConstant = isConst;
            if (*dtype) strcpy(symbolTable[i].dtype, dtype);
            return;
        }
    }
    strcpy(symbolTable[varCount].name, name);
    strcpy(symbolTable[varCount].dtype, dtype);
    symbolTable[varCount].value = val;
    symbolTable[varCount].isConstant = isConst;
    varCount++;
}

// Perform arithmetic
float calcResult(char op, float a, float b) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return (b != 0) ? a / b : 0;
        default: return 0;
    }
}

// Print variable with correct type (skip redeclaration)
void printVar(char *dtype, char *var, float val, int redeclared) {
    if (strcmp(dtype, "int") == 0) {
        if (redeclared)
            printf("%s = %d;\n", var, (int)val);
        else
            printf("%s %s = %d;\n", dtype, var, (int)val);
    } else {
        if (redeclared)
            printf("%s = %.2f;\n", var, val);
        else
            printf("%s %s = %.2f;\n", dtype, var, val);
    }
}

// Handle assignment with operator (+,-,*,/)
void handleOpAssign(char *dtype, char *var, char *op1, char *op2, char op) {
    int k1, k2;
    float v1 = getValue(op1, &k1);
    float v2 = getValue(op2, &k2);
    int redeclared = isDeclared(var);

    if (k1 && k2) {
        float res = calcResult(op, v1, v2);
        updateVar(var, dtype, res, 1);
        printVar(dtype, var, res, redeclared);
    } else {
        updateVar(var, dtype, 0, 0);
        if (redeclared)
            printf("%s = %s %c %s;\n", var, op1, op, op2);
        else
            printf("%s %s = %s %c %s;\n", dtype, var, op1, op, op2);
    }
}

// Handle simple assignment (=)
void handleSimpleAssign(char *dtype, char *var, char *rhs) {
    int known;
    float v1 = getValue(rhs, &known);
    int redeclared = isDeclared(var);

    if (known) {
        updateVar(var, dtype, v1, 1);
        printVar(dtype, var, v1, redeclared);
    } else {
        updateVar(var, dtype, 0, 0);
        if (redeclared)
            printf("%s = %s;\n", var, rhs);
        else
            printf("%s %s = %s;\n", dtype, var, rhs);
    }
}

int main() {
    FILE *fp = fopen("code.c", "r");
    if (!fp) {
        printf("Cannot open code.c file!\n");
        return 1;
    }

    char line[200], dtype[20], var[20], op1[20], op2[20], op;
    while (fgets(line, sizeof(line), fp)) {
        // Skip unneeded lines
        if (strstr(line, "#include") || strstr(line, "main") ||
            strstr(line, "{") || strstr(line, "}") ||
            strstr(line, "printf") || strstr(line, "return")) {
            printf("%s", line);
            continue;
        }

        // Clean up line
        line[strcspn(line, ";\n")] = '\0';
        if (!*line) continue;

        int matched;
        char dtypeFound[10] = "";

        // Case 1: type var = op1 op op2;
        matched = sscanf(line, "%s %s = %s %c %s", dtype, var, op1, &op, op2);
        if (matched == 5) { handleOpAssign(dtype, var, op1, op2, op); continue; }

        // Case 2: var = op1 op op2;
        matched = sscanf(line, "%s = %s %c %s", var, op1, &op, op2);
        if (matched == 4) {
            for (int i = 0; i < varCount; i++)
                if (strcmp(symbolTable[i].name, var) == 0)
                    strcpy(dtypeFound, symbolTable[i].dtype);
            handleOpAssign(dtypeFound, var, op1, op2, op);
            continue;
        }

        // Case 3: type var = value;
        matched = sscanf(line, "%s %s = %s", dtype, var, op1);
        if (matched == 3) { handleSimpleAssign(dtype, var, op1); continue; }

        // Case 4: var = value;
        matched = sscanf(line, "%s = %s", var, op1);
        if (matched == 2) {
            for (int i = 0; i < varCount; i++)
                if (strcmp(symbolTable[i].name, var) == 0)
                    strcpy(dtypeFound, symbolTable[i].dtype);
            handleSimpleAssign(dtypeFound, var, op1);
            continue;
        }

        // Case 5: type var;
        matched = sscanf(line, "%s %s", dtype, var);
        if (matched == 2 && strchr(var, '=') == NULL) {
            if (!isDeclared(var)) {
                updateVar(var, dtype, 0, 0);
                printf("%s %s;\n", dtype, var);
            }
            continue;
        }

        // Otherwise print as is
        printf("%s\n", line);
    }

    fclose(fp);

    // Final output
    printf("\n--- Final Variable Status (Constant Propagation) ---\n");
    for (int i = 0; i < varCount; i++) {
        struct Var v = symbolTable[i];
        if (v.isConstant)
            printf("%s = %g (%s constant)\n", v.name, v.value, v.dtype);
        else
            printf("%s = (non-constant)\n", v.name);
    }

    return 0;
}
