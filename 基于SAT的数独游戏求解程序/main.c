#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
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
    
    
    const char * fileName = "/Users/zhugod/Desktop/课设/综合程序设计任务及指导学生包/SAT测试备选算例/基准算例/性能测试/ais10.cnf";
    /*         DPLL test     */
    //struct clauseSet currentClauseSet = readCnfCreateClauseSet("/Users/zhugod/Desktop/课设/综合程序设计任务及指导学生包/SAT测试备选算例/满足算例/S/problem1-20.cnf");
    
    //struct clauseSet currentClauseSet = readCnfCreateClauseSet("/Users/zhugod/Desktop/课设/综合程序设计任务及指导学生包/SAT测试备选算例/基准算例/性能测试/ais10.cnf");
    struct clauseSet currentClauseSet = readCnfCreateClauseSet(fileName);
    //struct clauseSet currentClauseSet = readCnfCreateClauseSet("/Users/zhugod/Desktop/课设/综合程序设计任务及指导学生包/SAT测试备选算例/不满足算例/u-homer14.shuffled-300.cnf");
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
    clock_t t;
    t = clock();
    
    
    theResult = DPLL(currentClauseSet);
    
    printf("\nsatOrNot: %d\n", theResult.satOrNot);
    /*
    int index;
    for (index=0; index<theResult.noOfLiteral; index++)
        printf("\n%d : %d\n", index+1, theResult.arrHead[index]);
    */
    t = clock() - t;
    double timeTaken = 1000*((double)t)/CLOCKS_PER_SEC; // in milliseconds
    printf("\nDPLL() took %f milliseconds to execute \n", timeTaken);
    /*         end DPLL test        */
    
    /* write to file test */
    writeToFile(fileName, theResult.satOrNot, theResult.arrHead, theResult.noOfLiteral, timeTaken);
    
    
    /* write to file test */
    
    return 0;
}
