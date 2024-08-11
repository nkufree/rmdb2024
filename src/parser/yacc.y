%{
#include "ast.h"
#include "yacc.tab.h"
#include <iostream>
#include <memory>

int yylex(YYSTYPE *yylval, YYLTYPE *yylloc);

void yyerror(YYLTYPE *locp, const char* s) {
    std::cerr << "Parser Error at line " << locp->first_line << " column " << locp->first_column << ": " << s << std::endl;
}

using namespace ast;
%}

// request a pure (reentrant) parser
%define api.pure full
// enable location in error handler
%locations
// enable verbose syntax error message
%define parse.error verbose

// keywords
%token SHOW TABLES CREATE TABLE DROP DESC INSERT INTO VALUES DELETE FROM ASC ORDER GROUP BY HAVING STATIC_CHECKPOINT LOAD OUTPUT_FILE OFF
WHERE UPDATE SET SELECT MAX MIN SUM COUNT AS INT CHAR FLOAT INDEX AND JOIN EXIT HELP TXN_BEGIN TXN_COMMIT TXN_ABORT TXN_ROLLBACK ORDER_BY ENABLE_NESTLOOP ENABLE_SORTMERGE ON
// non-keywords
%token IN LEQ NEQ GEQ T_EOF

// type-specific tokens
%token <sv_str> IDENTIFIER VALUE_STRING PATH_LIKE
%token <sv_int> VALUE_INT
%token <sv_float> VALUE_FLOAT
%token <sv_bool> VALUE_BOOL

// specify types for non-terminal symbol
%type <sv_node> stmt dbStmt ddl dml txnStmt setStmt SelStmt
%type <sv_field> field
%type <sv_fields> fieldList
%type <sv_type_len> type
%type <sv_comp_op> op
%type <sv_expr> expr
%type <sv_val> value
%type <sv_vals> valueList oneValue
%type <sv_vals_list> valuesList
%type <sv_str> tbName colName alias fileName
%type <sv_strs> tableList colNameList
%type <sv_col> col colEach
%type <sv_cols> colList selector
%type <sv_set_clause> setClause
%type <sv_set_clauses> setClauses
%type <sv_cond> condition
%type <sv_conds> whereClause optWhereClause opOnClause
%type <sv_orderby>  order_clause opt_order_clause
%type <sv_groupby>  opt_group_clause
%type <sv_orderby_dir> opt_asc_desc
%type <sv_aggr_type> opt_aggregate
%type <sv_setKnobType> set_knob_type

%%
start:
        stmt ';'
    {
        parse_tree = $1;
        YYACCEPT;
    }
    |   HELP
    {
        parse_tree = std::make_shared<Help>();
        YYACCEPT;
    }
    |   EXIT
    {
        parse_tree = nullptr;
        YYACCEPT;
    }
    |   T_EOF
    {
        parse_tree = nullptr;
        YYACCEPT;
    }
    |   SET OUTPUT_FILE OFF
    {
        parse_tree = std::make_shared<SetOutputFile>();
        YYACCEPT;
    }
    ;

stmt:
        dbStmt
    |   ddl
    |   dml
    |   txnStmt
    |   setStmt
    ;

txnStmt:
        TXN_BEGIN
    {
        $$ = std::make_shared<TxnBegin>();
    }
    |   TXN_COMMIT
    {
        $$ = std::make_shared<TxnCommit>();
    }
    |   TXN_ABORT
    {
        $$ = std::make_shared<TxnAbort>();
    }
    | TXN_ROLLBACK
    {
        $$ = std::make_shared<TxnRollback>();
    }
    ;

dbStmt:
        SHOW TABLES
    {
        $$ = std::make_shared<ShowTables>();
    }
    |   SHOW INDEX FROM tbName
    {
        $$ = std::make_shared<ShowIndex>($4);
    }
    |   CREATE STATIC_CHECKPOINT
    {
        $$ = std::make_shared<CreateStaticCheckpoint>();
    }
    |   LOAD fileName INTO tbName
    {
        $$ = std::make_shared<LoadStmt>($2, $4);
    }
    ;

setStmt:
        SET set_knob_type '=' VALUE_BOOL
    {
        $$ = std::make_shared<SetStmt>($2, $4);
    }
    ;

ddl:
        CREATE TABLE tbName '(' fieldList ')'
    {
        $$ = std::make_shared<CreateTable>($3, $5);
    }
    |   DROP TABLE tbName
    {
        $$ = std::make_shared<DropTable>($3);
    }
    |   DESC tbName
    {
        $$ = std::make_shared<DescTable>($2);
    }
    |   CREATE INDEX tbName '(' colNameList ')'
    {
        $$ = std::make_shared<CreateIndex>($3, $5);
    }
    |   DROP INDEX tbName '(' colNameList ')'
    {
        $$ = std::make_shared<DropIndex>($3, $5);
    }
    ;

dml:
        INSERT INTO tbName VALUES valuesList
    {
        $$ = std::make_shared<InsertStmt>($3, $5);
    }
    |   DELETE FROM tbName optWhereClause
    {
        $$ = std::make_shared<DeleteStmt>($3, $4);
    }
    |   UPDATE tbName SET setClauses optWhereClause
    {
        $$ = std::make_shared<UpdateStmt>($2, $4, $5);
    }
    |   SelStmt
    {
        $$ = $1;
    }
    ;

SelStmt:
        SELECT selector FROM tableList opOnClause optWhereClause opt_order_clause opt_group_clause
    {
        $$ = std::make_shared<SelectStmt>($2, $4, $5, $6, $7, $8);
    }
    ;

oneValue:
    '(' valueList ')'
    {
        $$ = $2;
    }
    ;

valuesList:
        oneValue
    {
        $$ = std::vector<std::vector<std::shared_ptr<Value>>>{$1};
    }
    |   valuesList ',' oneValue
    {
        $$.push_back($3);
    }
    ;

fieldList:
        field
    {
        $$ = std::vector<std::shared_ptr<Field>>{$1};
    }
    |   fieldList ',' field
    {
        $$.push_back($3);
    }
    ;

colNameList:
        colName
    {
        $$ = std::vector<std::string>{$1};
    }
    | colNameList ',' colName
    {
        $$.push_back($3);
    }
    ;

field:
        colName type
    {
        $$ = std::make_shared<ColDef>($1, $2);
    }
    ;

type:
        INT
    {
        $$ = std::make_shared<TypeLen>(SV_TYPE_INT, sizeof(int));
    }
    |   CHAR '(' VALUE_INT ')'
    {
        $$ = std::make_shared<TypeLen>(SV_TYPE_STRING, $3);
    }
    |   FLOAT
    {
        $$ = std::make_shared<TypeLen>(SV_TYPE_FLOAT, sizeof(float));
    }
    ;

valueList:
        value
    {
        $$ = std::vector<std::shared_ptr<Value>>{$1};
    }
    |   valueList ',' value
    {
        $$.push_back($3);
    }
    ;

value:
        VALUE_INT
    {
        $$ = std::make_shared<IntLit>($1);
    }
    |   VALUE_FLOAT
    {
        $$ = std::make_shared<FloatLit>($1);
    }
    |   VALUE_STRING
    {
        $$ = std::make_shared<StringLit>($1);
    }
    |   VALUE_BOOL
    {
        $$ = std::make_shared<BoolLit>($1);
    }
    ;

condition:
        col op expr
    {
        $$ = std::make_shared<BinaryExpr>($1, $2, $3);
    }
    |  col IN '(' SelStmt ')'
    {
        $$ = std::make_shared<BinaryExpr>($1, SV_OP_IN, std::static_pointer_cast<Expr>(std::make_shared<SubQueryStmt>(std::static_pointer_cast<SelectStmt>($4))));
    }
    | col IN '(' valueList ')'
    {
        $$ = std::make_shared<BinaryExpr>($1, SV_OP_IN, std::static_pointer_cast<Expr>(std::make_shared<ValueList>($4)));
    }
    ;


opOnClause:
        /* epsilon */ { /* ignore*/ }
    |
        ON whereClause
    {
        $$ = $2;
    }
    ;

optWhereClause:
        /* epsilon */ { /* ignore*/ }
    |   WHERE whereClause
    {
        $$ = $2;
    }
    ;

whereClause:
        condition 
    {
        $$ = std::vector<std::shared_ptr<BinaryExpr>>{$1};
    }
    |   whereClause AND condition
    {
        $$.push_back($3);
    }
    ;


col:
        tbName '.' colName
    {
        $$ = std::make_shared<Col>($1, $3);
    }
    |   colName
    {
        $$ = std::make_shared<Col>("", $1);
    }
    // 这里是别名、聚合函数的处理
    // |   tbName '.' colName AS alias
    // {
    //     $$ = std::make_shared<Col>($1, $3, $5);
    // }
    // |   colName AS alias
    // {
    //     $$ = std::make_shared<Col>("", $1, $3);
    // }
    |   opt_aggregate '(' tbName '.' colName ')'
    {
        $$ = std::make_shared<Col>($3, $5);
        $$->aggr_type = $1;
    }
    // |   opt_aggregate '(' tbName '.' colName ')' AS alias
    // {
    //     $$ = std::make_shared<Col>($3, $5, $8);
    //     $$->aggr_type = $1;
    // }
    |   opt_aggregate '(' colName ')'
    {
        $$ = std::make_shared<Col>("", $3);
        $$->aggr_type = $1;
    }
    |   opt_aggregate '(' '*' ')'
    {
        $$ = std::make_shared<Col>("", "*");
        $$->aggr_type = $1;
    }
    // |   opt_aggregate '(' colName ')' AS alias
    // {
    //     $$ = std::make_shared<Col>("", $3, $6);
    //     $$->aggr_type = $1;
    // }
    // |   opt_aggregate '(' '*' ')' AS alias
    // {
    //     $$ = std::make_shared<Col>("", "*", $6);
    //     $$->aggr_type = $1;
    // }
    ;


colList:
        colEach
    {
        $$ = std::vector<std::shared_ptr<Col>>{$1};
    }
    |   colList ',' colEach
    {
        $$.push_back($3);
    }
    ;

colEach:
        col
    {
        $$ = $1;
    }
    |   col AS alias
    {
        $1->alias = $3;
        $$ = $1;
    }
    ;

op:
        '='
    {
        $$ = SV_OP_EQ;
    }
    |   '<'
    {
        $$ = SV_OP_LT;
    }
    |   '>'
    {
        $$ = SV_OP_GT;
    }
    |   NEQ
    {
        $$ = SV_OP_NE;
    }
    |   LEQ
    {
        $$ = SV_OP_LE;
    }
    |   GEQ
    {
        $$ = SV_OP_GE;
    }
    ;

expr:
        value
    {
        $$ = std::static_pointer_cast<Expr>($1);
    }
    |   col
    {
        $$ = std::static_pointer_cast<Expr>($1);
    }
    |   '(' SelStmt ')'
    {
        $$ = std::static_pointer_cast<Expr>(std::make_shared<SubQueryStmt>(std::static_pointer_cast<SelectStmt>($2)));
    }
    ;

setClauses:
        setClause
    {
        $$ = std::vector<std::shared_ptr<SetClause>>{$1};
    }
    |   setClauses ',' setClause
    {
        $$.push_back($3);
    }
    ;

setClause:
        colName '=' value
    {
        $$ = std::make_shared<SetClause>($1, $3);
    }
    ;

selector:
        '*'
    {
        $$ = {};
    }
    |   colList
    ;

tableList:
        tbName
    {
        $$ = std::vector<std::string>{$1};
    }
    |   tableList ',' tbName
    {
        $$.push_back($3);
    }
    |   tableList JOIN tbName
    {
        $$.push_back($3);
    }
    ;

opt_order_clause:
    ORDER BY order_clause      
    { 
        $$ = $3; 
    }
    |   /* epsilon */ { /* ignore*/ }
    ;
    
opt_group_clause:
    GROUP BY colList
    { 
        auto group_by = std::make_shared<GroupBy>($3);
        $$ = group_by;
    }
    | GROUP BY colList HAVING whereClause
    {
        auto group_by = std::make_shared<GroupBy>($3, $5);
        $$ = group_by;
    }
    |   /* epsilon */ { /* ignore*/ }
    ;

order_clause:
      col  opt_asc_desc 
    { 
        $$ = std::make_shared<OrderBy>($1, $2);
    }
    ;   

opt_asc_desc:
    ASC          { $$ = OrderBy_ASC;     }
    |  DESC      { $$ = OrderBy_DESC;    }
    |       { $$ = OrderBy_DEFAULT; }
    ;    

opt_aggregate:
    MAX { $$ = AGGR_TYPE_MAX; }
    | MIN { $$ = AGGR_TYPE_MIN; }
    | SUM { $$ = AGGR_TYPE_SUM; }
    | COUNT { $$ = AGGR_TYPE_COUNT; }

set_knob_type:
    ENABLE_NESTLOOP { $$ = EnableNestLoop; }
    |   ENABLE_SORTMERGE { $$ = EnableSortMerge; }
    ;

tbName: IDENTIFIER;

colName: IDENTIFIER;

fileName: PATH_LIKE;

alias: IDENTIFIER;
%%
