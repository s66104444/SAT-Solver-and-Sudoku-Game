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
struct clauseSet readCnfCreateClauseSet(const char *);
struct literal * createLiteral(int);
struct clause * createClause(struct literal *);
void traverseClauseSet(struct clauseSet);



// +++++++++++++++++++++++++++++++++++DPLL related++++++++++++++++++++++++++++++++++++++++++++++
/* result structure */
struct result;
struct result
{
    int satOrNot; // 0 means unsatisfiable, 1 means satistiable, -1 means undecided
    int noOfLiteral;
    int * arrHead; //
};
/* end result structure */

/* back track related */
struct backTrackStackNode;
struct backTrackStackNode
{
    struct clauseSet data;
    int otherChoice;
    struct backTrackStackNode * next;
};
struct backTrackStackNode * push(struct backTrackStackNode *, struct clauseSet, int);
struct backTrackStackNode * pop(struct backTrackStackNode *, struct clauseSet *, int *);
/* end back track related */


struct result DPLL(struct clauseSet);
struct clauseSet unitPropagation(struct clauseSet, int, struct result *);
int findUnitLiteral(struct clauseSet, struct result *);
void freeLiteral(struct literal *);
void freeClause(struct clause *);

/* write the result of DPLL to a res file */
void writeToFile(const char *, int, int *, int, double);
char * changeExtension(const char *);

/* check the validation of the result, valid return 1, invalid return 0 */
int checkValidation(struct clauseSet aClauseSet, struct result theResult);
#endif

