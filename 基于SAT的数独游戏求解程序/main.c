#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "cnfSolver.h"

int main(int argc, char const *argv[])
{
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
     end stack test */
     
     
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
    
    //const char * fileName = "/Users/zhugod/Desktop/课设/综合程序设计任务及指导学生包/SAT测试备选算例/基准算例/性能测试/sud00009.cnf";
    //const char * fileName = "/Users/zhugod/Desktop/课设/综合程序设计任务及指导学生包/SAT测试备选算例/基准算例/性能测试/ais10.cnf";
    //const char * fileName = "/Users/zhugod/Desktop/课设/综合程序设计任务及指导学生包/SAT测试备选算例/基准算例/功能测试/unsat-5cnf-30.cnf";
    //const char * fileName = "/Users/zhugod/Desktop/课设/综合程序设计任务及指导学生包/SAT测试备选算例/基准算例/功能测试/sat-20.cnf";
    /*         DPLL test     */
    //fileName = "/Users/zhugod/Desktop/课设/综合程序设计任务及指导学生包/SAT测试备选算例/满足算例/S/problem1-20.cnf";
    
    //const char *fileName = "/Users/zhugod/Desktop/课设/综合程序设计任务及指导学生包/SAT测试备选算例/满足算例/M/bart17.shuffled-231.cnf";
    //const char *fileName = "/Users/zhugod/Desktop/课设/综合程序设计任务及指导学生包/SAT测试备选算例/满足算例/M/ec-mod2c-rand3bip-sat-250-2.shuffled-as.sat05-2534.cnf";
    //const char *fileName = "/Users/zhugod/Desktop/课设/综合程序设计任务及指导学生包/SAT测试备选算例/满足算例/M/m-mod2c-rand3bip-sat-220-3.shuffled-as.sat05-2490-311.cnf";
    const char *fileName = "/Users/zhugod/Desktop/课设/综合程序设计任务及指导学生包/SAT测试备选算例/满足算例/M/problem12-200.cnf";
    //const char *fileName = "/Users/zhugod/Desktop/课设/综合程序设计任务及指导学生包/SAT测试备选算例/满足算例/M/sud00009.cnf";
    //const char * fileName = "validationTest.cnf";
    
    struct clauseSet currentClauseSet = readCnfCreateClauseSet(fileName);
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
    t = clock() - t;
    double timeTaken = 1000*((double)t)/CLOCKS_PER_SEC; // in milliseconds
    printf("\nDPLL 耗时: %f ms\n", timeTaken);
    
    printf("\nSAT or Not: %d\n", theResult.satOrNot);
    /*
    int index;
    for (index=0; index<theResult.noOfLiteral; index++)
        printf("\n%d : %d\n", index+1, theResult.arrHead[index]);
    */
    /*         end DPLL test        */
    
    /* write to file test */
    if (theResult.satOrNot == 1)
        writeToFile(fileName, theResult.satOrNot, theResult.arrHead, theResult.noOfLiteral, timeTaken);
    /* end write to file test */
    
    /* validation function test */
    currentClauseSet = readCnfCreateClauseSet(fileName);
    printf("\n验证结果: %d\n\n", checkValidation(currentClauseSet, theResult));
    /* end validation function test */
    
    return 0;
}
