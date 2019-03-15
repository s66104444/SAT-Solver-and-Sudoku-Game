#include<stdio.h>
#include <stdlib.h>
#include "cnfSolver.h"

int main(int argc, char const *argv[])
{
    //struct clauseSet currentClauseSet = readCnfCreateClauseSet("/Users/zhugod/Desktop/课设/综合程序设计任务及指导学生包/SAT测试备选算例/满足算例/S/7cnf20_90000_90000_7.shuffled-20.cnf");
    //struct clauseSet currentClauseSet = readCnfCreateClauseSet("test.cnf");
    //struct clauseSet currentClauseSet = readCnfCreateClauseSet("test_findUnitLiteral.cnf");
    
    /*    stack test
    struct clauseSet oneClauseSet = readCnfCreateClauseSet("test_stack_1.cnf");
    struct clauseSet otherClauseSet = readCnfCreateClauseSet("test_stack_2.cnf");
    struct backTrackStackNode * aStack = NULL;
    aStack = push(aStack, oneClauseSet);
    aStack = push(aStack, otherClauseSet);
    
    struct clauseSet testClauseSet;
    aStack = pop(aStack, &testClauseSet);
    traverseClauseSet(testClauseSet);
    aStack = pop(aStack, &testClauseSet);
    traverseClauseSet(testClauseSet);
    */
     
     
    /* traverse and findUnitLiteral test
    traverseClauseSet(currentClauseSet);
    printf("单子句%d\n", findUnitLiteral(currentClauseSet));
    */
    
    
    /*        test unitPropagation
    struct clauseSet currentClauseSet = readCnfCreateClauseSet("test_propagation.cnf");
    traverseClauseSet(currentClauseSet);
    struct result theResult;
    theResult.satOrNot = -1;
    theResult.noOfLiteral = currentClauseSet.numOfLiteral;
    theResult.arrHead = NULL;
    
    if (currentClauseSet.firstClause == NULL)
    {
        theResult.satOrNot = 1;
        return theResult.satOrNot;
    }
    theResult.arrHead = (int *) malloc(theResult.noOfLiteral * sizeof(int));
    int i;
    for (i = 0; i < theResult.noOfLiteral; i++)
        theResult.arrHead[i] = -1;
    
    int aUnitLiteral = 0;
    // unit literal rule
    while ( (aUnitLiteral = findUnitLiteral(currentClauseSet, &theResult)) )
    {
        
        theResult.arrHead[abs(aUnitLiteral)-1] = aUnitLiteral > 0 ? 1 : 0;
        currentClauseSet = unitPropagation(currentClauseSet, aUnitLiteral, &theResult);
    }
    printf("\nsatOrNot: %d\n", theResult.satOrNot);
    int index;
    for (index=0; index<theResult.noOfLiteral; index++)
        printf("\n%d\n", theResult.arrHead[index]);
          end of test unitPropagation       */
    
    
    
    /*         DPLL test     */
    //struct clauseSet currentClauseSet = readCnfCreateClauseSet("/Users/zhugod/Desktop/课设/综合程序设计任务及指导学生包/SAT测试备选算例/满足算例/S/problem1-20.cnf");
    struct clauseSet currentClauseSet = readCnfCreateClauseSet("/Users/zhugod/Desktop/课设/综合程序设计任务及指导学生包/SAT测试备选算例/基准算例/性能测试/ais10.cnf");
    //struct clauseSet currentClauseSet = readCnfCreateClauseSet("/Users/zhugod/Desktop/课设/综合程序设计任务及指导学生包/SAT测试备选算例/满足算例/L/ec-iso-ukn009.shuffled-as.sat05-3632-1584.cnf");
    //struct clauseSet currentClauseSet = readCnfCreateClauseSet("test_DPLL.cnf");
    traverseClauseSet(currentClauseSet);
    
    // theResult
    struct result theResult;
    theResult.satOrNot = -1;
    theResult.noOfLiteral = currentClauseSet.numOfLiteral;
    theResult.arrHead = NULL;
    if (currentClauseSet.firstClause == NULL)
    {
        theResult.satOrNot = 1;
        return theResult.satOrNot;
    }
    theResult.arrHead = (int *) malloc(theResult.noOfLiteral * sizeof(int));
    int i;
    for (i = 0; i < theResult.noOfLiteral; i++)
        theResult.arrHead[i] = -1;
    // end theResult
    
    theResult = DPLL(currentClauseSet);
    
    printf("\nsatOrNot: %d\n", theResult.satOrNot);
    int index;
    for (index=0; index<theResult.noOfLiteral; index++)
        printf("\n%d : %d\n", index+1, theResult.arrHead[index]);
    
    /*         end DPLL test        */
    
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
    printf("\n=======================\n");
    printf("creating the clause set\n");
    printf("noOfInfoLines = %d\n", noOfInfoLines);
    printf("noOfLiteral = %d\n", noOfLiteral);
    printf("noOfClauses = %d\n", noOfClauses);
    printf("=======================\n\n");
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
    
    // initialize the backtrack stack
    struct backTrackStackNode * aStack = NULL;
        
    int aUnitLiteral = 0;
    // unit rule
    while ( (aUnitLiteral = findUnitLiteral(currentClauseSet, &theResult)) )
    {
        if (theResult.satOrNot != -1)
            return theResult;
        
        theResult.arrHead[abs(aUnitLiteral)-1] = aUnitLiteral > 0 ? 1 : 0;
        currentClauseSet = unitPropagation(currentClauseSet, aUnitLiteral, &theResult);
            
        if (theResult.satOrNot != -1)
            return theResult;
    }
    
    aStack = push(aStack, currentClauseSet);
    
    int branchLiteral = currentClauseSet.firstClause->firstLiteral->theLiteral;
    
    //struct clause * choosenClause = NULL;
    //struct literal * choosenLiteral = NULL;
    
    struct branchStackNode * branchStack = NULL;
    branchStack = branchStackPush(branchStack, -branchLiteral);
    //int theOtherChoice = 0;
    
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
                aStack = pop(aStack, &currentClauseSet);
                branchStack = branchStackPop(branchStack, &branchLiteral);
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
                aStack = pop(aStack, &currentClauseSet);
                branchStack = branchStackPop(branchStack, &branchLiteral);
                continue;
            }
        }
        if (theResult.satOrNot == 1)
            return theResult;
        
        
        aStack = push(aStack, currentClauseSet);
        branchLiteral = currentClauseSet.firstClause->firstLiteral->theLiteral;
        branchStack = branchStackPush(branchStack, - branchLiteral);
        
    }
    
    
    return theResult;
}



struct clauseSet unitPropagation(struct clauseSet currentClauseSet, int aUnitLiteral, struct result * resultPtr)
{
    resultPtr->satOrNot = -1;
    
    struct clause * currentClausePtr = currentClauseSet.firstClause;
    if (currentClausePtr == NULL)
    {
        resultPtr->satOrNot = 1;
        return currentClauseSet;
    }
    if (currentClausePtr->firstLiteral == NULL)
    {
        resultPtr->satOrNot = 0;
        return currentClauseSet;
    }
    
    struct clause * lastClausePtr = NULL;
    
    struct literal * lastLiteralPtr = NULL;
    struct literal * currentLiteralPtr = currentClausePtr->firstLiteral;
    
    while (currentClausePtr)
    {
        if (currentClausePtr->firstLiteral == NULL)
        {
            resultPtr->satOrNot = 0;
            return currentClauseSet;
        }
        currentLiteralPtr = currentClausePtr->firstLiteral;
        lastLiteralPtr = NULL;
        
        while (currentLiteralPtr)
        {
            if ( currentLiteralPtr->theLiteral == aUnitLiteral ) // should delete the whole clause
            {
                if (!lastClausePtr) // current clause is the first clause of the whole clause set
                {
                    currentClauseSet.firstClause = currentClausePtr->nextClause;
                    freeClause(currentClausePtr);
                    currentClausePtr = currentClauseSet.firstClause;
                    
                    if (currentClauseSet.firstClause == NULL) // empty clause set
                    {
                        resultPtr->satOrNot = 1;
                        return currentClauseSet;
                    }
                }
                else
                {
                    lastClausePtr->nextClause = currentClausePtr->nextClause;
                    freeClause(currentClausePtr);
                    currentClausePtr = lastClausePtr->nextClause;
                }
                
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
                        return currentClauseSet;
                    }
                }
                else
                {
                    lastLiteralPtr->nextLiteral = currentLiteralPtr->nextLiteral;
                    freeLiteral(currentLiteralPtr);
                    currentLiteralPtr = lastLiteralPtr->nextLiteral;
                }
                
            }
            lastLiteralPtr = currentLiteralPtr;
            if (currentLiteralPtr)
                currentLiteralPtr = currentLiteralPtr->nextLiteral;
        }
        
        lastClausePtr = currentClausePtr;
        if (currentClausePtr)
            currentClausePtr = currentClausePtr->nextClause;
    }
    
    return currentClauseSet;
}

 

//================stack related=====================
/* push */
struct backTrackStackNode * push(struct backTrackStackNode * head, struct clauseSet aClauseSet)
{
    struct backTrackStackNode * nodePtr = (struct backTrackStackNode *) malloc(sizeof(struct backTrackStackNode));
    nodePtr->data.numOfClause = aClauseSet.numOfClause;
    nodePtr->data.numOfLiteral = aClauseSet.numOfLiteral;
    nodePtr->data.firstClause = NULL;
    
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
struct backTrackStackNode * pop(struct backTrackStackNode * head, struct clauseSet * clauseSetPtr)
{
    if (!head)
        return NULL;
    *clauseSetPtr = head->data;
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


/* branch stack */
struct branchStackNode * branchStackPush(struct branchStackNode * head, int theOtherChoice)
{
    struct branchStackNode * newNodePtr = (struct branchStackNode *) malloc(sizeof(struct branchStackNode));
    newNodePtr->theOtherChoice = theOtherChoice;
    newNodePtr->next = NULL;
    if (!head)
        return newNodePtr;
    newNodePtr->next = head;
    return newNodePtr;
}
struct branchStackNode * branchStackPop(struct branchStackNode * head, int * otherChoicePtr)
{
    if (!head)
        return NULL;
    *otherChoicePtr = head->theOtherChoice;
    struct branchStackNode * temp = head->next;
    free(head);
    return temp;
}
