#include "polishnotation.h"
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

char array[ARRAY_POLISH_NUM] = {0};
char arrayTemp[ARRAY_NUM] = {0};
double calcArray[ARRAY_NUM] = {0};

int sp = 0;
int spTemp = 0;
int calcsp = 0;

void
pushChar(char* stack,
         char data,
         int* sp)
{
    if (*sp < ARRAY_NUM)
    {
        stack[(*sp)++] = data;
    }
}

char
popChar(char* stack, int* sp)
{
    if (*sp > 0)
    {
        return stack[--(*sp)];
    }
    else
    {
        return 0;
    }
}

void
pushDouble(double* stack,
           double data,
           int* sp)
{
    if (*sp < ARRAY_NUM)
    {
        stack[(*sp)++] = data;
    }
}

double
popDouble(double* stack, int* sp)
{
    if ( *sp > 0 )
    {
        return stack[--(*sp)];
    }
    else
    {
        return 0;
    }
}

int
calcPriority(char op)
{
    int priority = 0;

    if (op == '/')
    {
        priority = 3;
    }
    else if (op == '*')
    {
        priority = 2;
    }
    else if (op == '+' || op == '-')
    {
        priority = 1;
    }

    return priority;
}

void
pushOperator(char op)
{
    while (true)
    {
        char opTemp = popChar(arrayTemp, &spTemp);

        int priorityA = calcPriority(op);
        int priorityB = calcPriority(opTemp);

        if (priorityA >= priorityB)
        {
            pushChar(arrayTemp, opTemp, &spTemp);
            pushChar(arrayTemp, op, &spTemp);
            break;
        }

        pushChar(array, opTemp, &sp);
        pushChar(array, ',', &sp);
        pushChar(arrayTemp, op, &spTemp);
    }
}

double
calcFormula(char op,
            double figA,
            double figB)
{
    double result = 0;

    if (figA > DOUBLE_MAX || figB > DOUBLE_MAX)
    {
        printf(ERROR_LONG);
        exit(1);
    }

    if (op == '/')
    {
        if (figA == 0)
        {
            printf(CALC_ERROR);
            exit(1);
        }

        result = figB / figA;
    }
    else if (op == '*')
    {
        result = figB * figA;
    }
    else if (op == '+')
    {
        result = figB + figA;
    }
    else if (op == '-')
    {
        result = figB - figA;
    }

    return result;
}

void
toPolishNotation(char* formula)
{
    char temp = 0;

    for (int i = 0; i < ARRAY_NUM; i++)
    {
        if (formula[i] == '(')
        {
            pushChar(arrayTemp, formula[i], &spTemp);
        }
        else if (formula[i] == ')')
        {
            while ((temp = popChar(arrayTemp, &spTemp)) != '(')
            {
                pushChar(array, temp, &sp);
                pushChar(array, ',', &sp);
            }
        }
        else if (isdigit(formula[i]))
        {
            pushChar(array, formula[i], &sp);

            while (true)
            {
                if (!isdigit(formula[i+1]))
                {
                    pushChar(array, ',', &sp);
                    break;
                }
                pushChar(array, formula[i+1], &sp);
                i++;
            }
        }
        else if (formula[i] == '+' || formula[i] == '-' ||
                 formula[i] == '*' || formula[i] == '/')
        {
            pushOperator(formula[i]);
        }
    }

    while (spTemp != 0)
    {
        pushChar(array, popChar(arrayTemp, &spTemp), &sp);
        pushChar(array, ',', &sp);
    }
}

double
calcPolishNotation(char* polishFormula)
{
    double calcResult = 0;

    for (int i = 0; i < ARRAY_POLISH_NUM; i++)
    {
        if (isdigit(polishFormula[i]))
        {
            char temp[ARRAY_NUM] = {0};
            temp[0] = polishFormula[i];

            int j = 1;
            while (true)
            {
                if (!isdigit(polishFormula[i+1]))
                {
                    pushDouble(calcArray, atof(temp), &calcsp);
                    break;
                }
                temp[j++] = polishFormula[i+1];
                i++;
            }
        }

        if (polishFormula[i] == '+' || polishFormula[i] == '-' ||
            polishFormula[i] == '*' || polishFormula[i] == '/')
        {
            double figA = popDouble(calcArray, &calcsp);
            double figB = popDouble(calcArray, &calcsp);
            double result = calcFormula(polishFormula[i], figA, figB);

            if (result > DOUBLE_MAX)
            {
                printf(ERROR_LONG);
                exit(1);
            }
            pushDouble(calcArray, result, &calcsp);
        }
    }

    calcResult = round(calcArray[0]);

    return calcResult;
}
