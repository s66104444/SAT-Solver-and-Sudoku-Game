#include<stdio.h>
#include <stdlib.h>
#include "cnfSolver.h"

int main(int argc, char const *argv[])
{
    //struct clauseSet currentClauseSet = readCnfCreateClauseSet("/Users/zhugod/Desktop/课设/综合程序设计任务及指导学生包/SAT测试备选算例/满足算例/S/7cnf20_90000_90000_7.shuffled-20.cnf");
    //struct clauseSet currentClauseSet = readCnfCreateClauseSet("test.cnf");
    struct clauseSet currentClauseSet = readCnfCreateClauseSet("test_findUnitLiteral.cnf");
    traverseClauseSet(currentClauseSet);
    return 0;
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* read a cnf file, and store the data in the clause set data structure,
 return the new clause set */
struct clauseSet readCnfCreateClauseSet(char *filePath)
{
    int noOfInfoLines = -1;
    char pBuf[STLEN];
    char cnfBuf[STLEN];
    int noOfClauses = 0;
    int noOfLiteral = 0;
    int i = 1;
    int aLiteral = 0;
    char buf[STLEN];
    FILE *fp;
    
    // open the file
    if ((fp = fopen(filePath, "r")) == NULL)
    {
        fprintf(stdout, "Can't open \"%s\" file.\n", filePath);
        exit(EXIT_FAILURE);
    }
    
    // get rid of useless lines, set noOfLiterals and noOfClauses
    do
    {
        fgets(buf, STLEN, fp);
        if (buf[0] == 'p')
        {
            //fputs(buf, stdout);
            sscanf(buf, "%s %s %d %d", pBuf, cnfBuf, &noOfLiteral, &noOfClauses);
        }
        noOfInfoLines++;
    } while (buf[0] == 'p' | buf[0] == 'c');
    printf("formula information\n");
    printf("noOfInfoLines = %d\n", noOfInfoLines);
    printf("noOfLiteral = %d\n", noOfLiteral);
    printf("noOfClauses = %d\n", noOfClauses);
    printf("===================\n");
    rewind(fp);
    for (; i <= noOfInfoLines; i++)
        fgets(buf, STLEN, fp);
    
    // create the clause set
    struct clauseSet aClauseSet;
    aClauseSet.numOfLiteral = noOfLiteral;
    aClauseSet.numOfClause = noOfClauses;
    aClauseSet.firstClause = NULL;
    
    struct literal * firstLiteralPtr = NULL;
    struct literal * currentLiteralPtr = NULL;
    struct literal * lastLiteralPtr = NULL;
    
    //struct clause * firstClausePtr = NULL;
    struct clause * currentClausePtr = NULL;
    struct clause * lastClausePtr = NULL;
    for (i = 1; i <= noOfClauses; i++)
    {
        while (fscanf(fp, "%d", &aLiteral) == 1)
        {
            if (aLiteral == 0 && firstLiteralPtr)
            {
                currentClausePtr = createClause(firstLiteralPtr);
                break;
            }
            
            currentLiteralPtr = createLiteral(aLiteral);
            if (!firstLiteralPtr)
                firstLiteralPtr = currentLiteralPtr;
            if (lastLiteralPtr)
                lastLiteralPtr->nextLiteral = currentLiteralPtr;
            lastLiteralPtr = currentLiteralPtr;
        }
        
        firstLiteralPtr = NULL;
        currentLiteralPtr = NULL;
        lastLiteralPtr = NULL;
        
        if (!(aClauseSet.firstClause))
            aClauseSet.firstClause = currentClausePtr;
        if (lastClausePtr)
            lastClausePtr->nextClause = currentClausePtr;
        lastClausePtr = currentClausePtr;
    }
    
    
    if (fclose(fp) != 0)
        fprintf(stderr, "Error closing file\n");
    return aClauseSet;
}

/* takes an integer as the parameter, returns a pointer pointing to the new
 created literal structure */
struct literal * createLiteral(int aLiteral)
{
    struct literal * literalPtr = (struct literal *) malloc(sizeof(struct literal));
    if (!literalPtr)
        exit(EXIT_FAILURE);
    literalPtr->theLiteral = aLiteral;
    literalPtr->nextLiteral = NULL;
    return literalPtr;
}

/* takes a pointer pointing at the first literal  */
struct clause * createClause(struct literal * firstLiteralPtr)
{
    struct clause * clausePtr = (struct clause *) malloc(sizeof(struct clause));
    if (!clausePtr)
        exit(EXIT_FAILURE);
    clausePtr->nextClause = NULL;
    clausePtr->firstLiteral = firstLiteralPtr;
    return clausePtr;
}

/* traverse the whole clause set, print the formula */
void traverseClauseSet(struct clauseSet aClauseSet)
{
    printf("formula information\n%d\t%d\n===================\nbegin traversing\n", aClauseSet.numOfLiteral, aClauseSet.numOfClause);
    struct clause * currentClausePtr = aClauseSet.firstClause;
    struct literal * currentLiteralPtr = currentClausePtr->firstLiteral;
    while (currentClausePtr)
    {
        while (currentLiteralPtr)
        {
            printf("%d\t", currentLiteralPtr->theLiteral);
            currentLiteralPtr = currentLiteralPtr->nextLiteral;
        }
        printf("\n");
        currentClausePtr = currentClausePtr->nextClause;
        if (currentClausePtr)
            currentLiteralPtr = currentClausePtr->firstLiteral;
    }
}


/* find the unit literal */
int findUnitLiteral(struct clauseSet aClauseSet)
{
    struct clause * currentClausePtr = aClauseSet.firstClause;
    while (currentClausePtr)
    {
        if (currentClausePtr->firstLiteral->nextLiteral == NULL)
            return currentClausePtr->firstLiteral->theLiteral;
        currentClausePtr = currentClausePtr->nextClause;
    }
    return 0;
}


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// DPLL related

struct result DPLL(struct clauseSet currentClauseSet)
{
    struct result theResult;
    theResult.satOrNot = 0;
    theResult.noOfLiteral = currentClauseSet.numOfLiteral;
    theResult.arrHead = NULL;
    
    if (currentClauseSet.firstClause == NULL)
    {
        theResult.satOrNot = 1;
        return theResult;
    }
    theResult.arrHead = (int *) malloc(theResult.noOfLiteral * sizeof(int));
    
    struct backTrackStackNode * aStack = NULL;
    
    int aUnitLiteral = 0;
    while ( (aUnitLiteral = findUnitLiteral(currentClauseSet)) )
    {
        currentClauseSet = unitPropagation(currentClauseSet, aUnitLiteral);
    }
    return theResult;
}

struct clauseSet unitPropagation(struct clauseSet currentClauseSet, int aUnitLiteral)
{
    struct clauseSet resultClauseSet;
    resultClauseSet.firstClause = NULL;
    resultClauseSet.numOfClause = currentClauseSet.numOfClause;
    resultClauseSet.numOfLiteral = currentClauseSet.numOfLiteral;
    
    struct clause * firstClausePtr = currentClauseSet.firstClause;
    struct clause * lastClausePtr = NULL;
    struct clause * currentClausePtr = currentClauseSet.firstClause;
    
    struct literal * lastLiteralPtr = NULL;
    struct literal * currentLiteralPtr = currentClausePtr->firstLiteral;
    
    while (currentClausePtr)
    {
        while (currentLiteralPtr)
        {
            if ( (currentLiteralPtr->theLiteral == aUnitLiteral) & !(currentLiteralPtr->nextLiteral)) // unit clause
            {
                if (firstClausePtr == currentClausePtr)
                {
                    firstClausePtr = currentClausePtr->nextClause;
                    freeClause(currentClausePtr);
                    currentClausePtr = firstClausePtr;
                }
            }
        }
    }
    
    return resultClauseSet;
}
