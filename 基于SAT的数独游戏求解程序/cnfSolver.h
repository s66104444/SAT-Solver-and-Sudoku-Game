#ifndef CNF_SOLVER_H
#define CNF_SOLVER_H
#define STLEN 80

// +++++++++++++++++++++++++++++++++++Data Structure related++++++++++++++++++++++++++++++++++++++
struct clause;
struct literal;
struct clauseSet;
/* the definition of a clause */
struct clause
{
    struct clause *nextClause;
    struct literal *firstLiteral;
};
/* the definition of a literal */
struct literal
{
    int theLiteral;
    struct literal *nextLiteral;
};
/* the definition of a clause set (aka, a CNF formula)*/
struct clauseSet
{
    int numOfLiteral; // number of literals in this clause set
    int numOfClause; // number of clause in this clause set
    struct clause *firstClause; // point to the first clause of the set
};

// data structure related++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* read a cnf file, and store the data in the clauseSet data structure, return a
 pointer pointed to the new clauseSet */
struct clauseSet readCnfCreateClauseSet(char *);
struct literal * createLiteral(int);
struct clause * createClause(struct literal *);
void traverseClauseSet(struct clauseSet);
int findUnitLiteral(struct clauseSet);


// +++++++++++++++++++++++++++++++++++DPLL related++++++++++++++++++++++++++++++++++++++++++++++
/* use DPLL algorithm to solve a SAT problem */
struct result;
struct result
{
    int satOrNot; // 0 means unsatisfiable, 1 means satistiable
    int noOfLiteral;
    int * arrHead; //
};

struct backTrackStackNode;
struct backTrackStackNode
{
    struct clauseSet * data;
    struct backTrackStackNode * next;
};


struct result DPLL(struct clauseSet);

struct clauseSet unitPropagation(struct clauseSet, int);



#endif

