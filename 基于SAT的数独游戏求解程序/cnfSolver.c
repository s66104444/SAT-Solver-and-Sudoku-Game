#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "cnfSolver.h"
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* read a cnf file, and store the data in the clause set data structure,
 return the new clause set */
struct clauseSet readCnfCreateClauseSet(const char *filePath)
{
    int noOfInfoLines = -1;
    char pBuf[STLEN];
    char cnfBuf[STLEN];
    int noOfClause = 0;
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
            sscanf(buf, "%s %s %d %d", pBuf, cnfBuf, &noOfLiteral, &noOfClause);
        }
        noOfInfoLines++;
    } while (buf[0] == 'p' || buf[0] == 'c');
    printf("\n=======================\n");
    printf("creating the clause set\n");
    //printf("Number of Information Lines = %d\n", noOfInfoLines);
    printf("Number of Literals = %d\n", noOfLiteral);
    printf("Number of Clauses = %d\n", noOfClause);
    printf("=======================\n\n");
    rewind(fp);
    for (; i <= noOfInfoLines; i++)
        fgets(buf, STLEN, fp);
        
        // create the clause set
        struct clauseSet aClauseSet;
    aClauseSet.numOfLiteral = noOfLiteral;
    aClauseSet.numOfClause = noOfClause;
    aClauseSet.firstClause = NULL;
    
    struct literal * firstLiteralPtr = NULL;
    struct literal * currentLiteralPtr = NULL;
    struct literal * lastLiteralPtr = NULL;
    
    //struct clause * firstClausePtr = NULL;
    struct clause * currentClausePtr = NULL;
    struct clause * lastClausePtr = NULL;
    for (i = 1; i <= noOfClause; i++)
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
    printf("\n========================\n");
    printf("begin traversing\nformula information: %d\t%d\n", aClauseSet.numOfLiteral, aClauseSet.numOfClause);
    
    struct clause * currentClausePtr = aClauseSet.firstClause;
    struct literal * currentLiteralPtr = NULL;
    if (currentClausePtr)
        currentLiteralPtr = currentClausePtr->firstLiteral;
    else
        return;
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
    printf("========================\n");
}


/* find an unit literal */
int findUnitLiteral(struct clauseSet aClauseSet, struct result * resultPtr)
{
    struct clause * currentClausePtr = aClauseSet.firstClause;
    if (currentClausePtr == NULL)
    {
        resultPtr->satOrNot = 1;
        return 0;
    }
    
    while (currentClausePtr)
    {
        if ( (currentClausePtr->firstLiteral != NULL) )
        {
            if ( (currentClausePtr->firstLiteral)->nextLiteral == NULL) // have found an unit literal
            {
                return currentClausePtr->firstLiteral->theLiteral;
            }
        }
        else // have found an empty clause
        {
            resultPtr->satOrNot = 0;
            return 0;
        }
        currentClausePtr = currentClausePtr->nextClause;
    }
    return 0;
}


// ++++++++++++++++++++++++++++++++++++++++++DPLL related
struct result DPLL(struct clauseSet currentClauseSet)
{
    // initialize the result
    struct result theResult;
    theResult.satOrNot = -1;
    theResult.noOfLiteral = currentClauseSet.numOfLiteral;
    theResult.arrHead = NULL;
    
    if (currentClauseSet.firstClause == NULL)
    {
        theResult.satOrNot = 1;
        return theResult;
    }
    
    theResult.arrHead = (int *) malloc(theResult.noOfLiteral * sizeof(int));
    int i;
    for (i = 0; i < theResult.noOfLiteral; i++)
    {
        theResult.arrHead[i] = -1;
    }
    // end result initialization
    
    int aUnitLiteral = 0;
    
    // initialize the backtrack stack
    int branchLiteral = currentClauseSet.firstClause->firstLiteral->theLiteral;
    struct backTrackStackNode * aStack = NULL;
    aStack = push(aStack, currentClauseSet, -branchLiteral);
    // end
    
    while (1)
    {
        // branch rule
        if (branchLiteral > 0)
        {
            theResult.arrHead[branchLiteral - 1] = 1;
            currentClauseSet = unitPropagation(currentClauseSet, branchLiteral, &theResult);
        }
        else
        {
            theResult.arrHead[abs(branchLiteral) - 1] = 0;
            currentClauseSet = unitPropagation(currentClauseSet, branchLiteral, &theResult);
        }
        
        if (theResult.satOrNot == 0)
        {
            if (aStack != NULL)
            {
                aStack = pop(aStack, &currentClauseSet, &branchLiteral);
                continue;
            }
        }
        if (theResult.satOrNot == 1)
            return theResult;
        
        
        // unit literal rule
        while ( (aUnitLiteral = findUnitLiteral(currentClauseSet, &theResult)) )
        {
            theResult.arrHead[abs(aUnitLiteral)-1] = aUnitLiteral > 0 ? 1 : 0;
            currentClauseSet = unitPropagation(currentClauseSet, aUnitLiteral, &theResult);
        }
        
        if (theResult.satOrNot == 0)
        {
            if (aStack != NULL)
            {
                aStack = pop(aStack, &currentClauseSet, &branchLiteral);
                continue;
            }
            return theResult;
        }
        if (theResult.satOrNot == 1)
            return theResult;
        
        branchLiteral = currentClauseSet.firstClause->firstLiteral->theLiteral;
        aStack = push(aStack, currentClauseSet, -branchLiteral);
    }
    
    
    return theResult;
}



struct clauseSet unitPropagation(struct clauseSet aClauseSet, int aUnitLiteral, struct result * resultPtr)
{
    struct clause * currentClausePtr = aClauseSet.firstClause;
    if (currentClausePtr == NULL)
    {
        resultPtr->satOrNot = 1;
        return aClauseSet;
    }
    if (currentClausePtr->firstLiteral == NULL)
    {
        resultPtr->satOrNot = 0;
        return aClauseSet;
    }
    resultPtr->satOrNot = -1;
    
    struct clause * lastClausePtr = NULL;
    
    struct literal * lastLiteralPtr = NULL;
    struct literal * currentLiteralPtr = NULL;
    
    while (currentClausePtr)
    {
        currentLiteralPtr = currentClausePtr->firstLiteral;
        lastLiteralPtr = NULL;
        
        while (currentLiteralPtr)
        {
            if ( currentLiteralPtr->theLiteral == aUnitLiteral ) // should delete the whole clause
            {
                if (!lastClausePtr) // current clause is the first clause of the whole clause set
                {
                    aClauseSet.firstClause = currentClausePtr->nextClause;
                    freeClause(currentClausePtr);
                    currentClausePtr = aClauseSet.firstClause;
                    
                    if (aClauseSet.firstClause == NULL) // empty clause set
                    {
                        resultPtr->satOrNot = 1;
                        return aClauseSet;
                    }
                }
                else
                {
                    lastClausePtr->nextClause = currentClausePtr->nextClause;
                    freeClause(currentClausePtr);
                    currentClausePtr = lastClausePtr->nextClause;
                }
                // should start traverse the new current clause
                if (currentClausePtr)
                    currentLiteralPtr = currentClausePtr->firstLiteral;
                else
                    currentLiteralPtr = NULL;
                lastLiteralPtr = NULL;
                continue;
            }
            else if ( currentLiteralPtr->theLiteral == - aUnitLiteral ) // should delete current literal
            {
                if (!lastLiteralPtr) // current literal is the first literal of current clause
                {
                    currentClausePtr->firstLiteral = currentLiteralPtr->nextLiteral;
                    freeLiteral(currentLiteralPtr);
                    currentLiteralPtr = currentClausePtr->firstLiteral;
                    
                    if (currentClausePtr->firstLiteral == NULL) // empty clause
                    {
                        resultPtr->satOrNot = 0;
                        return aClauseSet;
                    }
                }
                else
                {
                    lastLiteralPtr->nextLiteral = currentLiteralPtr->nextLiteral;
                    freeLiteral(currentLiteralPtr);
                    currentLiteralPtr = lastLiteralPtr->nextLiteral;
                }
                continue;
            }
            lastLiteralPtr = currentLiteralPtr;
            if (currentLiteralPtr)
                currentLiteralPtr = currentLiteralPtr->nextLiteral;
        }
        
        lastClausePtr = currentClausePtr;
        if (currentClausePtr)
            currentClausePtr = currentClausePtr->nextClause;
    }
    
    return aClauseSet;
}



//================stack related=====================
/* push */
struct backTrackStackNode * push(struct backTrackStackNode * head, struct clauseSet aClauseSet, int theOtherChoice)
{
    struct backTrackStackNode * nodePtr = (struct backTrackStackNode *) malloc(sizeof(struct backTrackStackNode));
    nodePtr->data.numOfClause = aClauseSet.numOfClause;
    nodePtr->data.numOfLiteral = aClauseSet.numOfLiteral;
    nodePtr->data.firstClause = NULL;
    nodePtr->otherChoice = theOtherChoice;
    
    //struct clause * firstClausePtr = NULL;
    struct clause * currentClausePtr = NULL;
    struct clause * lastClausePtr = NULL;
    
    struct literal * currentLiteralPtr = NULL;
    struct literal * lastLiteralPtr = NULL;
    
    struct clause * originalClausePtr = aClauseSet.firstClause;
    struct literal * originalLiteralPtr = NULL;
    if (originalClausePtr)
        originalLiteralPtr = originalClausePtr->firstLiteral;
    
    while (originalClausePtr)
    {
        currentClausePtr = (struct clause *) malloc(sizeof(struct clause));
        currentClausePtr->firstLiteral = NULL;
        currentClausePtr->nextClause = NULL;
        if ( !((nodePtr->data).firstClause) )
            (nodePtr->data).firstClause = currentClausePtr;
        while (originalLiteralPtr)
        {
            currentLiteralPtr = (struct literal *) malloc(sizeof(struct literal));
            currentLiteralPtr->theLiteral = originalLiteralPtr->theLiteral;
            currentLiteralPtr->nextLiteral = NULL;
            
            if (!(currentClausePtr->firstLiteral))
                currentClausePtr->firstLiteral = currentLiteralPtr;
            
            if (lastLiteralPtr)
                lastLiteralPtr->nextLiteral = currentLiteralPtr;
            
            lastLiteralPtr = currentLiteralPtr;
            originalLiteralPtr = originalLiteralPtr->nextLiteral;
        }
        if (lastClausePtr)
            lastClausePtr->nextClause = currentClausePtr;
        lastClausePtr = currentClausePtr;
        
        originalClausePtr = originalClausePtr->nextClause;
        lastLiteralPtr = NULL;
        if (originalClausePtr)
            originalLiteralPtr = originalClausePtr->firstLiteral;
    }
    
    nodePtr->next = NULL;
    if (!head)
        return nodePtr;
    nodePtr->next = head;
    return nodePtr;
}
/* pop */
struct backTrackStackNode * pop(struct backTrackStackNode * head, struct clauseSet * clauseSetPtr, int * branchLiteral)
{
    if (!head)
        return NULL;
    *clauseSetPtr = head->data;
    *branchLiteral = head->otherChoice;
    struct backTrackStackNode * temp = head->next;
    free(head);
    return temp;
}

void freeLiteral(struct literal * literalPtr)
{
    if (!literalPtr)
        free(literalPtr);
}

void freeClause(struct clause * clausePtr)
{
    struct literal * temp = clausePtr->firstLiteral;
    free(clausePtr);
    while (temp)
    {
        freeLiteral(temp);
        temp = temp->nextLiteral;
    }
}

/* write the result of DPLL to a res file */
void writeToFile(const char * fileName, int satOrNot, int *arrHead, int noOfLiteral, double timeTaken)
{
    FILE *fp;
    char * destination = changeExtension(fileName);
    if ((fp = fopen(destination, "w")) == NULL)
    {
        fprintf(stdout, "Can't open \"%s\" file.\n", destination);
        free(destination);
        exit(EXIT_FAILURE);
    }
    free(destination);
    
    fprintf(fp, "s %d\n", satOrNot);
    
    fprintf(fp, "v");
    int index;
    for (index = 1; index <= noOfLiteral; index++)
    {
        fprintf(fp, " %d ", arrHead[index-1] == 1 ? index : -index);
    }
    
    fprintf(fp, "\nt %f\n", timeTaken);
    
    if (fclose(fp) != 0)
        fprintf(stderr,"Error closing file\n");
}

char * changeExtension(const char *filename)
{
    size_t nameLength = strlen(filename) + 1;
    char * destination = (char *) malloc(nameLength*sizeof(char));
    strcpy(destination, filename);
    char *end = destination + strlen(destination);
    
    while (end > destination && *end != '.') {
        --end;
    }
    
    end++;
    *end = 'r';
    end++;
    *end = 'e';
    end++;
    *end = 's';
    
    return destination;
}

/* check the validation of the result, valid return 1, invalid return 0 */
int checkValidation(struct clauseSet aClauseSet, struct result theResult)
{
    printf("开始验证\n");
    struct result validationResult;
    validationResult.satOrNot = -1;
    validationResult.noOfLiteral = aClauseSet.numOfLiteral;
    validationResult.arrHead = NULL;
    int index;
    int literal;
    /*
     for (index = 1; index <= theResult.noOfLiteral; index++)
     {
     printf("%d\t", theResult.arrHead[index-1]);
     }
     */
    for (index = 1; index <= theResult.noOfLiteral; index++)
    {
        //printf("\nbefore unit propagation\n\n");
        //traverseClauseSet(aClauseSet);
        literal = theResult.arrHead[index-1] == 1 ? index : -index;
        //printf("\nunit literal : %d\n", literal);
        aClauseSet = unitPropagation(aClauseSet, literal, &validationResult);
        //printf("\nafter unit propagation\n\n");
        //traverseClauseSet(aClauseSet);
        //printf("\nvalidation: %d\n", validationResult.satOrNot);
        if (validationResult.satOrNot != -1)
            return validationResult.satOrNot;
    }
    return validationResult.satOrNot;
}

