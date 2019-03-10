#ifndef CNF_SOLVER_H
#define CNF_SOLVER_H
#define STLEN 80



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
typedef struct clause* ClauseSet;
/*
 struct clauseSet
 {
 //int numOfLiteral; // number of literals in this clause set
 //int numOfClause; // number of clause in this clause set
 struct clause *firstClause; // point to the first clause of the set
 };
 */

/* read a cnf file, and store the data in the clauseSet data structure, return a
 pointer pointed to the new clauseSet */
ClauseSet readCnf(char *);
struct literal * createLiteral(int);
struct clause * createClause(struct literal *);
void traverseClauseSet(ClauseSet);

#endif

