%{
#include <bits/stdc++.h>
#include "node.hpp"
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define YYDEBUG 1
using namespace std;

extern FILE* yyin;
extern int yylex();
extern int yylineno;
extern int yyparse();
void yyerror(string msg);
void error(string msg);
unsigned long long c=0;
bool flag = false;
unsigned long long get_id() {
    c++;
    return c;
}
%}

%define parse.error verbose

%union {
    struct node* n;
    char* val;
}

%token COMPOUND_STATEMENT_elif COMPOUND_STATEMENT_else COMPOUND_STATEMENT_for COMPOUND_STATEMENT_if COMPOUND_STATEMENT_while DELIMITER_c_brackets DELIMITER_c_paranthesis DELIMITER_colon DELIMITER_comma DELIMITER_dot DELIMITER_o_brackets DELIMITER_o_paranthesis DELIMITER_pointer DELIMITER_semicolon FLOW_STATEMENT_break FLOW_STATEMENT_continue FLOW_STATEMENT_return KEYWORD_and KEYWORD_class KEYWORD_define KEYWORD_false KEYWORD_in KEYWORD_is KEYWORD_none KEYWORD_global KEYWORD_not KEYWORD_or KEYWORD_true OPERATOR_add OPERATOR_addequal OPERATOR_and OPERATOR_andequal OPERATOR_bitwisenot OPERATOR_bitwiseor OPERATOR_bitwiseorequal OPERATOR_divide OPERATOR_divideequal OPERATOR_doublemultequal OPERATOR_equal OPERATOR_equality OPERATOR_floordivision OPERATOR_floordivisionequal OPERATOR_greaterthan OPERATOR_greaterthanequal OPERATOR_leftshift OPERATOR_leftshiftequal OPERATOR_lessthan OPERATOR_lessthanequal OPERATOR_minus OPERATOR_minusequal OPERATOR_modulo OPERATOR_moduloequal OPERATOR_notequal OPERATOR_rightshift OPERATOR_rightshiftequal OPERATOR_starequal OPERATOR_xor OPERATOR_xorequal POINTER_double_star POINTER_star SYMBOL_decorator INDENT DEDENT ENDMARKER NEWLINE RANGE

%type<n> and_expr and_test annassign arglist argument arith_expr arrow_test atom atom_expr augassign break_stmt c_argument classdef colon_test comma_expr_star_expr comma_names comma_subscript comma_test comma_test_star comp_for comp_if comp_iter comp_op comparison compound_stmt continue_stmt elif_block else_block equal_test expr expr_star_expr expr_stmt exprlist factor flow_stmt for_stmt funcdef global_stmt if_else if_stmt newlines next_arglist not_test op_small_stmt op_testlist_star_expr or_test parameters power return_stmt shift_expr simple_stmt single_input sliceop small_stmt star_expr start stmt stmts strings subscript subscriptlist suite term test test_star_expr testlist testlist_comp testlist_star_expr tfpdef trailer while_stmt xor_expr decl_stmt assign_stmt number attribute methodcall array_acc func_call type_class class_attr method_call range_func atom_comma sub_term div_term

%right OPERATOR_minus

%token<val> NAME NUMBER STRING DTYPE SELF FLOAT
%%

start:
    single_input ENDMARKER {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "start";

                                $$->children.push_back($1); 
                                $1->parent = $$;

                                // struct node* temp = new node(get_id());
                                // temp->name = "EOF";
                                // temp->terminal = true;
                                // temp->parent = $$;
                                // $$->children.push_back(temp);  

                                root=$$;
                            }

newlines:
    newlines NEWLINE        {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "newlines";

                                $$->children.push_back($1); 
                                $1->parent = $$;

                                struct node* temp = new node(get_id());
                                temp->name = "NEWLINE";
                                temp->terminal = true;
                                temp->parent = $$;
                                $$->children.push_back(temp);  
                            }
    | %empty                {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "newlines";

                                struct node* temp = new node(get_id());
                                temp->name = "empty";
                                temp->terminal = true;
                                temp->parent = $$;
                                $$->children.push_back(temp);  
                            }

single_input: 
    newlines stmts           {

                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "single_input";

                                $$->children.push_back($1); 
                                $1->parent = $$; 
                                $$->children.push_back($2);
                                $2->parent = $$;
                            }
    | newlines 
stmts: 
    stmts stmt              {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "stmts";

                                $$->children.push_back($1); 
                                $1->parent = $$;

                                $$->children.push_back($2); 
                                $2->parent = $$;                                
                            }
    | stmt                  {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "stmts";

                                $$->children.push_back($1); 
                                $1->parent = $$; 
                            }

stmt: 
    simple_stmt newlines    {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "stmt";

                                $$->children.push_back($1); 
                                $1->parent = $$;

                                $$->children.push_back($2); 
                                $2->parent = $$;                                
                            }
    | compound_stmt newlines {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "stmt";

                                $$->children.push_back($1); 
                                $1->parent = $$;

                                $$->children.push_back($2); 
                                $2->parent = $$;                                
                            }

simple_stmt: 
      small_stmt op_small_stmt DELIMITER_semicolon NEWLINE {
                                                                $$ = new node(get_id());$$->lineno=yylineno;
                                                                $$->name = "simple_stmt";

                                                                $$->children.push_back($1); 
                                                                $1->parent = $$;

                                                                $$->children.push_back($2); 
                                                                $2->parent = $$;   

                                                                struct node* temp1 = new node(get_id());
                                                                temp1->name = ";";
                                                                temp1->terminal = true;
                                                                temp1->parent = $$;
                                                                $$->children.push_back(temp1); 

                                                                struct node* temp2 = new node(get_id());
                                                                temp2->name = "NEWLINE";
                                                                temp2->terminal = true;
                                                                temp2->parent = $$;
                                                                $$->children.push_back(temp2);                             
                                                            }
    | small_stmt op_small_stmt NEWLINE                      {
                                                                $$ = new node(get_id());$$->lineno=yylineno;
                                                                $$->name = "simple_stmt";

                                                                $$->children.push_back($1); 
                                                                $1->parent = $$;

                                                                $$->children.push_back($2); 
                                                                $2->parent = $$;   

                                                                struct node* temp1 = new node(get_id());
                                                                temp1->name = "NEWLINE";
                                                                temp1->terminal = true;
                                                                temp1->parent = $$;
                                                                $$->children.push_back(temp1);                            
                                                            }

op_small_stmt:
      op_small_stmt DELIMITER_semicolon small_stmt  {
                                                        $$ = new node(get_id());$$->lineno=yylineno;
                                                        $$->name = "op_small_stmt";

                                                        $$->children.push_back($1); 
                                                        $1->parent = $$;

                                                        struct node* temp1 = new node(get_id());
                                                        temp1->name = ";";
                                                        temp1->terminal = true;
                                                        temp1->parent = $$;
                                                        $$->children.push_back(temp1); 

                                                        $$->children.push_back($3); 
                                                        $3->parent = $$;            
                                                    }
    | %empty                {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "op_small_stmt";
                                struct node* temp = new node(get_id());
                                temp->name = "empty";
                                temp->terminal = true;
                                temp->parent = $$;
                                $$->children.push_back(temp);  
                            }

small_stmt: 
      expr_stmt             {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "small_stmt";

                                $$->children.push_back($1); 
                                $1->parent = $$; 
                            }
    | flow_stmt             {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "small_stmt";

                                $$->children.push_back($1); 
                                $1->parent = $$; 
                            }
    | global_stmt           {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "small_stmt";

                                $$->children.push_back($1); 
                                $1->parent = $$; 
                            }

compound_stmt: 
      if_stmt               {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "compound_stmt";

                                $$->children.push_back($1); 
                                $1->parent = $$; 
                            }
    | while_stmt            {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "compound_stmt";

                                $$->children.push_back($1); 
                                $1->parent = $$; 
                            }
    | for_stmt              {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "compound_stmt";

                                $$->children.push_back($1); 
                                $1->parent = $$; 
                            }
    | funcdef               {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "compound_stmt";

                                $$->children.push_back($1); 
                                $1->parent = $$; 
                            }
    | classdef              {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "compound_stmt";

                                $$->children.push_back($1); 
                                $1->parent = $$; 
                            }

funcdef: 
    KEYWORD_define NAME parameters arrow_test DELIMITER_colon suite {
                                                                        $$ = new node(get_id());$$->lineno=yylineno;
                                                                        $$->name = "funcdef";

                                                                        struct node* temp = new node(get_id());
                                                                        temp->name = "DEFINE";
                                                                        temp->terminal = true;
                                                                        temp->parent = $$;
                                                                        $$->children.push_back(temp); 

                                                                        struct node* temp1 = new node(get_id());
                                                                        temp1->name = $2;
                                                                        temp1->terminal = true;
                                                                        temp1->parent = $$;
                                                                        $$->children.push_back(temp1); 

                                                                        $$->children.push_back($3); 
                                                                        $3->parent = $$; 

                                                                        $$->children.push_back($4); 
                                                                        $4->parent = $$; 

                                                                        struct node* temp3 = new node(get_id());
                                                                        temp3->name = ":";
                                                                        temp3->terminal = true;
                                                                        temp3->parent = $$;
                                                                        $$->children.push_back(temp3); 

                                                                        $$->children.push_back($6); 
                                                                        $6->parent = $$; 

                                                                    }

arrow_test: 
    DELIMITER_pointer DTYPE  {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "arrow_test";

                                struct node* temp = new node(get_id());
                                temp->name = "->";
                                temp->terminal = true;
                                temp->parent = $$;
                                $$->children.push_back(temp);  
                                struct node* temp2 = new node(get_id());
                                temp->name = $2;
                                temp->terminal = true;
                                temp->parent = $$;
                                $$->children.push_back(temp2);
                                 
                                 
                                }
    | %empty {
        $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "arrow_test";
                                struct node* temp = new node(get_id());
                                temp->name = "empty";
                                temp->terminal = true;
                                temp->parent = $$;
                                $$->children.push_back(temp);
    }

parameters: 
    DELIMITER_o_paranthesis next_arglist DELIMITER_c_paranthesis {
                                                                            $$ = new node(get_id());$$->lineno=yylineno;
                                                                            $$->name = "parameters";

                                                                            struct node* temp = new node(get_id());
                                                                            temp->name = "(";
                                                                            temp->terminal = true;
                                                                            temp->parent = $$;
                                                                            $$->children.push_back(temp); 

                                                                            $$->children.push_back($2); 
                                                                            $2->parent = $$; 

                                                                            struct node* temp1 = new node(get_id());
                                                                            temp1->name = ")";
                                                                            temp1->terminal = true;
                                                                            temp1->parent = $$;
                                                                            $$->children.push_back(temp1); 
                                                                        }
    | DELIMITER_o_paranthesis DELIMITER_c_paranthesis {
                                                                            $$ = new node(get_id());$$->lineno=yylineno;
                                                                            $$->name = "parameters";

                                                                            struct node* temp = new node(get_id());
                                                                            temp->name = "(";
                                                                            temp->terminal = true;
                                                                            temp->parent = $$;
                                                                            $$->children.push_back(temp); 

                                                                            struct node* temp1 = new node(get_id());
                                                                            temp1->name = ")";
                                                                            temp1->terminal = true;
                                                                            temp1->parent = $$;
                                                                            $$->children.push_back(temp1); 
                                                                        }

next_arglist: 
    equal_test     {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "next_arglist";
                                $$->children.push_back($1); 
                                $1->parent = $$; 
                            }
    | next_arglist DELIMITER_comma equal_test  {
                                                        $$ = new node(get_id());$$->lineno=yylineno;
                                                        $$->name = "next_arglist";

                                                        $$->children.push_back($1); 
                                                        $1->parent = $$; 

                                                        struct node* temp = new node(get_id());
                                                        temp->name = "COMMA";
                                                        temp->terminal = true;
                                                        temp->parent = $$;
                                                        $$->children.push_back(temp); 

                                                        $$->children.push_back($3); 
                                                        $3->parent = $$; 
                                                    }
    

tfpdef: NAME colon_test {
                            $$ = new node(get_id());$$->lineno=yylineno;
                            $$->name = "tfpdef";

                            struct node* temp = new node(get_id());
                            temp->name = $1;
                            temp->terminal = true;
                            temp->parent = $$;
                            $$->children.push_back(temp); 

                            $$->children.push_back($2); 
                            $2->parent = $$; 
                        }
        |SELF {             $$ = new node(get_id());$$->lineno=yylineno;
                            $$->name = "tfpdef";

                            struct node* temp = new node(get_id());
                            temp->name = $1;
                            temp->terminal = true;
                            temp->parent = $$;
                            $$->children.push_back(temp); }

colon_test:
    DELIMITER_colon type_class    {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "colon_test";

                                struct node* temp = new node(get_id());
                                temp->name = ":";
                                temp->terminal = true;
                                temp->parent = $$;
                                $$->children.push_back(temp); 
                                
                                $$->children.push_back($2); 
                                
                            }
    | %empty                {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "colon_test";
                                struct node* temp = new node(get_id());
                                temp->name = "empty";
                                temp->terminal = true;
                                temp->parent = $$;
                                $$->children.push_back(temp);  
                            }

type_class: DTYPE{              $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name ="dtype";

                               
                                struct node * temp2 = new node(get_id());
                                temp2->name = $1;
                                temp2->terminal =true;
                                temp2->parent=$$;
                                $$->children.push_back(temp2);

}
| NAME {                        $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name ="class";

                               
                                struct node * temp2 = new node(get_id());
                                temp2->name = $1;
                                temp2->terminal =true;
                                temp2->parent=$$;
                                $$->children.push_back(temp2);

}


equal_test:
    tfpdef OPERATOR_equal test     {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "equal_test";

                                $$->children.push_back($1); 
                                $1->parent = $$; 

                                struct node* temp = new node(get_id());
                                temp->name = "=";
                                temp->terminal = true;
                                temp->parent = $$;
                                $$->children.push_back(temp); 

                                $$->children.push_back($3); 
                                $3->parent = $$; 
                            }
    | tfpdef                {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "equal_test";
                                $$->children.push_back($1); 
                                $1->parent = $$;   
                            }

global_stmt: KEYWORD_global comma_names {
                                                $$ = new node(get_id());$$->lineno=yylineno;
                                                $$->name = "global_stmt";

                                                struct node* temp1 = new node(get_id());
                                                temp1->name = "GLOBAL";
                                                temp1->terminal = true;
                                                temp1->parent = $$;
                                                $$->children.push_back(temp1); 

                                                $$->children.push_back($2); 
                                                $2->parent = $$; 
                                            }

expr_stmt:
    testlist {
         $$ = new node(get_id());$$->lineno=yylineno;
                                            $$->name = "expr_stmt";

                                            $$->children.push_back($1); 
                                            $1->parent = $$;

    } 
    |testlist_star_expr augassign testlist   {
                                            $$ = new node(get_id());$$->lineno=yylineno;
                                            $$->name = "expr_stmt";

                                            $$->children.push_back($1); 
                                            $1->parent = $$;  

                                            $$->children.push_back($2); 
                                            $2->parent = $$;  

                                            $$->children.push_back($3); 
                                            $3->parent = $$; 
                                        } 
    | assign_stmt  {
                                            $$ = new node(get_id());$$->lineno=yylineno;
                                            $$->name = "expr_stmt";

                                            $$->children.push_back($1); 
                                            $1->parent = $$;  

                                            // $$->children.push_back($2); 
                                            // $2->parent = $$;  

                                        }
    
    | decl_stmt {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "expr_stmt";

        $$->children.push_back($1); 
        $1->parent = $$;
    } 

method_call : NAME DELIMITER_dot NAME func_call {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "method_call";

        struct node* temp = new node(get_id());
        temp->name = $1;
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);

        struct node* temp2 = new node(get_id());
        temp2->name = ".";
        temp2->terminal = true;
        temp2->parent = $$;
        $$->children.push_back(temp2);

        struct node* temp3 = new node(get_id());
        temp3->name = $3;
        temp3->terminal = true;
        temp3->parent = $$;
        $$->children.push_back(temp3);

        // struct node* temp4 = new node(get_id());
        // temp4->name = "(";
        // temp4->terminal = true;
        // temp4->parent = $$;
        // $$->children.push_back(temp4);

        $$->children.push_back($4);

        // struct node* temp5 = new node(get_id());
        // temp5->name = ")";
        // temp5->terminal = true;
        // temp5->parent = $$;
        // $$->children.push_back(temp5);

}

assign_stmt : 
        class_attr op_testlist_star_expr{
                                            $$ = new node(get_id());$$->lineno=yylineno;
                                            $$->name = "assign_stmt";

                                            $$->children.push_back($1); 
                                            $1->parent = $$;  

                                            $$->children.push_back($2); 
                                            $2->parent = $$;  
        }
        | atom_expr op_testlist_star_expr  {
                                            $$ = new node(get_id());$$->lineno=yylineno;
                                            $$->name = "assign_stmt";

                                            $$->children.push_back($1); 
                                            $1->parent = $$;  

                                            $$->children.push_back($2); 
                                            $2->parent = $$;  

                                        }    
        | decl_stmt op_testlist_star_expr   {
                                            $$ = new node(get_id());$$->lineno=yylineno;
                                            $$->name = "assign_stmt";

                                            $$->children.push_back($1); 
                                            $1->parent = $$;  

                                            $$->children.push_back($2); 
                                            $2->parent = $$; 
        }

decl_stmt : NAME annassign {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "decl_stmt";

        struct node* temp = new node(get_id());
        temp->name = $1;
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);

        $$->children.push_back($2);
}
    /*| NAME attribute annassign {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "decl_stmt";

        struct node* temp = new node(get_id());
        temp->name = $1;
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);

        $$->children.push_back($2);
        $$->children.push_back($3);
    }*/
    | class_attr annassign {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "decl_stmt";
        $$->children.push_back($1);
        $$->children.push_back($2);
    }

class_attr : SELF attribute {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "class_attr";

        struct node* temp = new node(get_id());
        temp->name = $1;
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        $$->children.push_back($2);
}

annassign: DELIMITER_colon type_class  {
                                            $$ = new node(get_id());$$->lineno=yylineno;
                                            $$->name = "annassign";

                                            struct node* temp = new node(get_id());
                                            temp->name = ":";
                                            temp->terminal = true;
                                            temp->parent = $$;
                                            $$->children.push_back(temp);

                                            $$->children.push_back($2);  

                                            
                                        } 
        

op_testlist_star_expr:
       OPERATOR_equal test {
                                            $$ = new node(get_id());$$->lineno=yylineno;
                                            $$->name = "op_testlist_star_expr";

                                            // $$->children.push_back($1); 
                                            // $1->parent = $$; 

                                            struct node* temp = new node(get_id());
                                            temp->name = "=";
                                            temp->terminal = true;
                                            temp->parent = $$;
                                            $$->children.push_back(temp); 

                                            $$->children.push_back($2); 
                                            $2->parent = $$; 
                                        }


testlist_star_expr: 
      test_star_expr comma_test_star DELIMITER_comma {
                                            $$ = new node(get_id());$$->lineno=yylineno;
                                            $$->name = "testlist_star_expr";

                                            $$->children.push_back($1); 
                                            $1->parent = $$; 

                                            $$->children.push_back($2); 
                                            $2->parent = $$; 

                                            struct node* temp = new node(get_id());
                                            temp->name = "COMMA";
                                            temp->terminal = true;
                                            temp->parent = $$;
                                            $$->children.push_back(temp); 
                                        }
    | test_star_expr comma_test_star   {
                                            $$ = new node(get_id());$$->lineno=yylineno;
                                            $$->name = "testlist_star_expr";

                                            $$->children.push_back($1); 
                                            $1->parent = $$; 

                                            $$->children.push_back($2); 
                                            $2->parent = $$; 
                                        }
    | %empty                            {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "testlist_star_expr";
                                struct node* temp = new node(get_id());
                                temp->name = "empty";
                                temp->terminal = true;
                                temp->parent = $$;
                                $$->children.push_back(temp);  
                            }

augassign: 
      OPERATOR_addequal     {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "augassign";
                                struct node* temp = new node(get_id());
                                temp->name = "+=";
                                temp->terminal = true;
                                temp->parent = $$;
                                $$->children.push_back(temp);  
                            }
    | OPERATOR_minusequal   {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "augassign";
                                struct node* temp = new node(get_id());
                                temp->name = "-=";
                                temp->terminal = true;
                                temp->parent = $$;
                                $$->children.push_back(temp);  
                            }
    | OPERATOR_starequal    {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "augassign";
                                struct node* temp = new node(get_id());
                                temp->name = "*=";
                                temp->terminal = true;
                                temp->parent = $$;
                                $$->children.push_back(temp);  
                            }
    | OPERATOR_divideequal  {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "augassign";
                                struct node* temp = new node(get_id());
                                temp->name = "/=";
                                temp->terminal = true;
                                temp->parent = $$;
                                $$->children.push_back(temp);  
                            }
    | OPERATOR_moduloequal  {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "augassign";
                                struct node* temp = new node(get_id());
                                temp->name = "MODULO_EQUALS";
                                temp->terminal = true;
                                temp->parent = $$;
                                $$->children.push_back(temp);  
                            }
    | OPERATOR_andequal     {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "augassign";
                                struct node* temp = new node(get_id());
                                temp->name = "&=";
                                temp->terminal = true;
                                temp->parent = $$;
                                $$->children.push_back(temp);  
                            }
    | OPERATOR_bitwiseorequal {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "augassign";
                                struct node* temp = new node(get_id());
                                temp->name = "|=";
                                temp->terminal = true;
                                temp->parent = $$;
                                $$->children.push_back(temp);  
                            }
    | OPERATOR_xorequal     {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "augassign";
                                struct node* temp = new node(get_id());
                                temp->name = "^=";
                                temp->terminal = true;
                                temp->parent = $$;
                                $$->children.push_back(temp);  
                            }
    | OPERATOR_leftshiftequal   {
                                    $$ = new node(get_id());$$->lineno=yylineno;
                                    $$->name = "augassign";
                                    struct node* temp = new node(get_id());
                                    temp->name = "<<=";
                                    temp->terminal = true;
                                    temp->parent = $$;
                                    $$->children.push_back(temp);  
                                }
    | OPERATOR_rightshiftequal {
                                    $$ = new node(get_id());$$->lineno=yylineno;
                                    $$->name = "augassign";
                                    struct node* temp = new node(get_id());
                                    temp->name = ">>=";
                                    temp->terminal = true;
                                    temp->parent = $$;
                                    $$->children.push_back(temp);  
                                }
    | OPERATOR_doublemultequal {
                                    $$ = new node(get_id());$$->lineno=yylineno;
                                    $$->name = "augassign";
                                    struct node* temp = new node(get_id());
                                    temp->name = "**=";
                                    temp->terminal = true;
                                    temp->parent = $$;
                                    $$->children.push_back(temp);  
                                }
    | OPERATOR_floordivisionequal   {
                                        $$ = new node(get_id());$$->lineno=yylineno;
                                        $$->name = "augassign";
                                        struct node* temp = new node(get_id());
                                        temp->name = "//=";
                                        temp->terminal = true;
                                        temp->parent = $$;
                                        $$->children.push_back(temp);  
                                    }

flow_stmt: 
      break_stmt            {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "flow_stmt";
                                $$->children.push_back($1); 
                                $1->parent = $$; 
                            }
    | continue_stmt         {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "flow_stmt";
                                $$->children.push_back($1); 
                                $1->parent = $$; 
                            }
    | return_stmt           {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "flow_stmt";
                                $$->children.push_back($1); 
                                $1->parent = $$; 
                            }

break_stmt: FLOW_STATEMENT_break        {
                                            $$ = new node(get_id());$$->lineno=yylineno;
                                            $$->name = "break_stmt";
                                            struct node* temp = new node(get_id());
                                            temp->name = "BREAK";
                                            temp->terminal = true;
                                            temp->parent = $$;
                                            $$->children.push_back(temp);  
                                        }

continue_stmt: FLOW_STATEMENT_continue  {
                                            $$ = new node(get_id());$$->lineno=yylineno;
                                            $$->name = "continue_stmt";
                                            struct node* temp = new node(get_id());
                                            temp->name = "CONTINUE";
                                            temp->terminal = true;
                                            temp->parent = $$;
                                            $$->children.push_back(temp);  
                                        }

return_stmt: 
      FLOW_STATEMENT_return             {
                                            $$ = new node(get_id());$$->lineno=yylineno;
                                            $$->name = "return_stmt";
                                            struct node* temp = new node(get_id());
                                            temp->name = "RETURN";
                                            temp->terminal = true;
                                            temp->parent = $$;
                                            $$->children.push_back(temp);  
                                        }
    | FLOW_STATEMENT_return testlist    {
                                            $$ = new node(get_id());$$->lineno=yylineno;
                                            $$->name = "return_stmt";

                                            struct node* temp = new node(get_id());
                                            temp->name = "RETURN";
                                            temp->terminal = true;
                                            temp->parent = $$;
                                            $$->children.push_back(temp); 

                                            $$->children.push_back($2); 
                                            $2->parent = $$; 
                                        }


comma_names: 
    comma_names DELIMITER_comma NAME    {
                                            $$ = new node(get_id());$$->lineno=yylineno;
                                            $$->name = "comma_names";

                                            $$->children.push_back($1); 
                                            $1->parent = $$; 

                                            struct node* temp = new node(get_id());
                                            temp->name = "COMMA";
                                            temp->terminal = true;
                                            temp->parent = $$;
                                            $$->children.push_back(temp); 

                                            struct node* temp1 = new node(get_id());
                                            temp1->name = $3;
                                            temp1->terminal = true;
                                            temp1->parent = $$;
                                            $$->children.push_back(temp1); 
                                        }
    | NAME                {
                                $$ = new node(get_id());$$->lineno=yylineno;
                                $$->name = "comma_names";
                                struct node* temp = new node(get_id());
                                temp->name = $1;
                                temp->terminal = true;
                                temp->parent = $$;
                                $$->children.push_back(temp);  
                            }

if_stmt: 
     COMPOUND_STATEMENT_if test DELIMITER_colon suite elif_block 
     {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "if_stmt";

        struct node* temp = new node(get_id());
        temp->name = "if";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);

        $$->children.push_back($2); 
        $2->parent = $$;

        struct node* temp1 = new node(get_id());
        temp1->name = "colon";
        temp1->terminal = true;
        temp1->parent = $$;
        $$->children.push_back(temp1);

        $$->children.push_back($4); 
        $4->parent = $$;
        $$->children.push_back($5); 
        $5->parent = $$;
     }

elif_block: 
     COMPOUND_STATEMENT_elif test DELIMITER_colon suite elif_block{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "elif_block";

        // $$->children.push_back($1); 
        // $1->parent = $$;

        struct node* temp = new node(get_id());
        temp->name = "elif";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);

        $$->children.push_back($2); 
        $2->parent = $$;

        struct node* temp1 = new node(get_id());
        temp1->name = "colon";
        temp1->terminal = true;
        temp1->parent = $$;
        $$->children.push_back(temp1);

        $$->children.push_back($4); 
        $4->parent = $$;
        $$->children.push_back($5); 
        $5->parent = $$;

        }
    | else_block {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "elif_block";
        $$->children.push_back($1); 
        $1->parent = $$;
    }

else_block:
    COMPOUND_STATEMENT_else DELIMITER_colon suite{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "else_block";

        struct node* temp = new node(get_id());
        temp->name = "else";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        
        struct node* temp1 = new node(get_id());
        temp1->name = "colon";
        temp1->terminal = true;
        temp1->parent = $$;
        $$->children.push_back(temp1);

        $$->children.push_back($3); 
        $3->parent = $$;
    }
    | %empty {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "else_block";
        struct node* temp = new node(get_id());
        temp->name = "empty";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
    }

while_stmt: COMPOUND_STATEMENT_while test DELIMITER_colon suite {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "while_stmt";

        struct node* temp = new node(get_id());
        temp->name = "while";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);

        $$->children.push_back($2); 
        $2->parent = $$;

        struct node* temp1 = new node(get_id());
        temp1->name = "colon";
        temp1->terminal = true;
        temp1->parent = $$;
        $$->children.push_back(temp1);

        $$->children.push_back($4); 
        $4->parent = $$;
}

range_func: RANGE DELIMITER_o_paranthesis number DELIMITER_c_paranthesis {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "range_func";
        
        struct node* temp = new node(get_id());
        temp->name = "range";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);

        $$->children.push_back($3); 
        $3->parent = $$;


}
  | RANGE DELIMITER_o_paranthesis number DELIMITER_comma number DELIMITER_c_paranthesis {
      $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "range_func";

        struct node* temp = new node(get_id());
        temp->name = "range";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);

        $$->children.push_back($3); 
        $3->parent = $$;
        $$->children.push_back($5); 
        $5->parent = $$;
  }

for_stmt: COMPOUND_STATEMENT_for atom_expr KEYWORD_in range_func DELIMITER_colon suite{
    $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "for_stmt";

        struct node* temp = new node(get_id());
        temp->name = "for";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);

        $$->children.push_back($2); 
        $2->parent = $$;

        struct node* temp1 = new node(get_id());
        temp1->name = "in";
        temp1->terminal = true;
        temp1->parent = $$;
        $$->children.push_back(temp1);

        $$->children.push_back($4); 
        $4->parent = $$;

        struct node* temp2 = new node(get_id());
        temp2->name = "colon";
        temp2->terminal = true;
        temp2->parent = $$;
        $$->children.push_back(temp2);

        $$->children.push_back($6); 
        $6->parent = $$;
}

suite: simple_stmt NEWLINE{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "suite";

        $$->children.push_back($1); 
        $1->parent = $$;

        struct node* temp = new node(get_id());
        temp->name = "newline";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);

}
    | newlines INDENT stmts DEDENT{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "suite";

        $$->children.push_back($1); 
        $1->parent = $$;

        struct node* temp = new node(get_id());
        temp->name = "indent";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);

        $$->children.push_back($3); 
        $3->parent = $$;

        struct node* temp1 = new node(get_id());
        temp1->name = "dedent";
        temp1->terminal = true;
        temp1->parent = $$;
        $$->children.push_back(temp1);
    }


test: if_else{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "test";

        $$->children.push_back($1); 
        $1->parent = $$;
        
}
| or_test  {
    $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "test";

        $$->children.push_back($1); 
        $1->parent = $$;
        
}

if_else:
    or_test COMPOUND_STATEMENT_if or_test COMPOUND_STATEMENT_else test{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "if_else";

        $$->children.push_back($1); 
        $1->parent = $$;

        struct node* temp = new node(get_id());
        temp->name = "if";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);

        $$->children.push_back($3); 
        $3->parent = $$;

        struct node* temp1 = new node(get_id());
        temp1->name = "else";
        temp1->terminal = true;
        temp1->parent = $$;
        $$->children.push_back(temp1);

        $$->children.push_back($5); 
        $5->parent = $$;
    }
    // | %empty {
    //     $$ = new node(get_id());$$->lineno=yylineno;
    //     $$->name = "if_else";
    //     struct node* temp = new node(get_id());
    //     temp->name = "empty";
    //     temp->terminal = true;
    //     temp->parent = $$;
    //     $$->children.push_back(temp);
    // }


// test_nocond: or_test {
//     $$ = new node(get_id());$$->lineno=yylineno;
//         $$->name = "test_nocond";
//         $$->children.push_back($1); 
//         $1->parent = $$;
// }

or_test: and_test {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "or_test";

        $$->children.push_back($1); 
        $1->parent = $$;
}
| and_test KEYWORD_or or_test  {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "or_test";

        $$->children.push_back($1); 
        $1->parent = $$;

        struct node* temp = new node(get_id());
        temp->name = "or";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);

        $$->children.push_back($3); 
        $3->parent = $$;
}

// op_and_test: 
//     op_and_test KEYWORD_or and_test{
//         $$ = new node(get_id());$$->lineno=yylineno;
//         $$->name = "op_and_test";

//         $$->children.push_back($1); 
//         $1->parent = $$;

//         struct node* temp = new node(get_id());
//         temp->name = "or";
//         temp->terminal = true;
//         temp->parent = $$;
//         $$->children.push_back(temp);

//         $$->children.push_back($3); 
//         $3->parent = $$;
//     }
//     | %empty {
//         $$ = new node(get_id());$$->lineno=yylineno;
//         $$->name = "op_and_test";
//         struct node* temp = new node(get_id());
//         temp->name = "empty";
//         temp->terminal = true;
//         temp->parent = $$;
//         $$->children.push_back(temp);
//     }

/*and_test: not_test op_not_test{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "and_test";

        $$->children.push_back($1); 
        $1->parent = $$;
        $$->children.push_back($2); 
        $2->parent = $$;
}*/

and_test : not_test {
    $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "and_test";

        $$->children.push_back($1); 
        $1->parent = $$;
}
| not_test KEYWORD_and and_test {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "and_test";

        $$->children.push_back($1); 
        $1->parent = $$;
        struct node* temp = new node(get_id());
        temp->name = "and";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        $$->children.push_back($3); 
        $3->parent = $$;
}


// op_not_test:
//     op_not_test KEYWORD_and not_test{
//         $$ = new node(get_id());$$->lineno=yylineno;
//         $$->name = "op_not_test";

//         $$->children.push_back($1); 
//         $1->parent = $$;
//         struct node* temp = new node(get_id());
//         temp->name = "and";
//         temp->terminal = true;
//         temp->parent = $$;
//         $$->children.push_back(temp);
//         $$->children.push_back($3); 
//         $3->parent = $$;
//     }
//     | %empty {
//         $$ = new node(get_id());$$->lineno=yylineno;
//         $$->name = "op_not_test";
//         struct node* temp = new node(get_id());
//         temp->name = "empty";
//         temp->terminal = true;
//         temp->parent = $$;
//         $$->children.push_back(temp);
//     }

not_test: KEYWORD_not not_test{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "not_test";

        struct node* temp = new node(get_id());
        temp->name = "not";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);

        $$->children.push_back($2); 
        $2->parent = $$;
} | comparison{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "not_test";
        $$->children.push_back($1); 
        $1->parent = $$;
}
/*
comparison: expr comp_op_expr{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "comparison";

        $$->children.push_back($1); 
        $1->parent = $$;
        $$->children.push_back($2); 
        $2->parent = $$;
}*/

comparison: expr {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "comparison";

        $$->children.push_back($1); 
        $1->parent = $$;
        
}
| expr comp_op expr {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "comparison";

        $$->children.push_back($1); 
        $1->parent = $$;
        $$->children.push_back($2); 
        $2->parent = $$;
        $$->children.push_back($3); 
        $3->parent = $$;
}


// comp_op_expr:
//     comp_op_expr comp_op expr{
//         $$ = new node(get_id());$$->lineno=yylineno;
//         $$->name = "comp_op_expr";

//         $$->children.push_back($1);
//         $1->parent = $$;
//         $$->children.push_back($2); 
//         $2->parent = $$;
//         $$->children.push_back($3);
//         $3->parent = $$;
//     }
//     | %empty {
//         $$ = new node(get_id());$$->lineno=yylineno;
//         $$->name = "comp_op_expr";
//         struct node* temp = new node(get_id());
//         temp->name = "empty";
//         temp->terminal = true;
//         temp->parent = $$;
//         $$->children.push_back(temp);
//     }

comp_op: 
      OPERATOR_lessthan{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "comp_op";
        struct node* temp = new node(get_id());
        temp->name = "<";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
      }
    | OPERATOR_greaterthan{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "comp_op";
        struct node* temp = new node(get_id());
        temp->name = ">";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
      }
    | OPERATOR_equality{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "comp_op";
        struct node* temp = new node(get_id());
        temp->name = "==";
        temp->terminal = true;
        temp->opet = true;
        temp->parent = $$;
        $$->children.push_back(temp);
      }
    | OPERATOR_greaterthanequal{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "comp_op";
        struct node* temp = new node(get_id());
        temp->name = ">=";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
      }
    | OPERATOR_lessthanequal{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "comp_op";
        struct node* temp = new node(get_id());
        temp->name = "<=";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
      }
    | OPERATOR_notequal{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "comp_op";
        struct node* temp = new node(get_id());
        temp->name = "!=";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
      }
    | KEYWORD_in{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "comp_op";
        struct node* temp = new node(get_id());
        // temp->name = "KEYWORD(in)";
        temp->name = "in";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
      }
    | KEYWORD_not KEYWORD_in{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "comp_op";
        struct node* temp = new node(get_id());
        temp->name = "not";
        // temp->name = "KEYWORD(not)";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        struct node* temp1 = new node(get_id());
        temp1->name = "KEYWORD(in)";
        temp1->terminal = true;
        temp1->parent = $$;
        $$->children.push_back(temp1);
      }
    | KEYWORD_is{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "comp_op";
        struct node* temp = new node(get_id());
        temp->name = "is";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
      }
    | KEYWORD_is KEYWORD_not{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "comp_op";
        struct node* temp = new node(get_id());
        temp->name = "is";
        // temp->name = "KEYWORD(is)";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        struct node* temp1 = new node(get_id());
        // temp1->name = "KEYWORD(not)";
        temp1->name = "not";
        temp1->terminal = true;
        temp1->parent = $$;
        $$->children.push_back(temp1);
      }

star_expr: POINTER_star expr{
    $$ = new node(get_id());$$->lineno=yylineno;
    $$->name = "star_expr";
    struct node* temp = new node(get_id());
        temp->name = "star";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
       $$->children.push_back($2); 
        $2->parent = $$ ;
}

expr: xor_expr {
    $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "expr";
     $$->children.push_back($1); 
        $1->parent = $$;
}
| xor_expr OPERATOR_bitwiseor expr {
  $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "expr";

        $$->children.push_back($1); 
        $1->parent = $$;

        struct node* temp = new node(get_id());
        temp->name = "|";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);

        $$->children.push_back($3); 
        $3->parent = $$;
}

// op_xor_expr:
//     op_xor_expr OPERATOR_bitwiseor xor_expr{
//         $$ = new node(get_id());$$->lineno=yylineno;
//         $$->name = "op_xor_expr";
//         $$->children.push_back($1); 
//         $1->parent = $$;
//         struct node* temp = new node(get_id());
//         temp->name = "bitwiseor";
//         temp->terminal = true;
//         temp->parent = $$;
//         $$->children.push_back(temp);

//         $$->children.push_back($3); 
//         $3->parent = $$;
//     }
//     | %empty {
//         $$ = new node(get_id());$$->lineno=yylineno;
//         $$->name = "op_xor_expr";
//         struct node* temp = new node(get_id());
//         temp->name = "empty";
//         temp->terminal = true;
//         temp->parent = $$;
//         $$->children.push_back(temp);
//     }

xor_expr: and_expr {
    $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "xor_expr";
        $$->children.push_back($1); 
        $1->parent = $$;
        
}
| and_expr OPERATOR_xor xor_expr {
    $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "xor_expr";

        $$->children.push_back($1); 
        $1->parent = $$;

        struct node* temp = new node(get_id());
        temp->name = "xor";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);

        $$->children.push_back($3); 
        $3->parent = $$;
}

// op_and_expr:
//       op_and_expr OPERATOR_xor and_expr{
//         $$ = new node(get_id());$$->lineno=yylineno;
//         $$->name = "op_and_expr";
//         $$->children.push_back($1); 
//         $1->parent = $$;
//         struct node* temp = new node(get_id());
//         temp->name = "xor";
//         temp->terminal = true;
//         temp->parent = $$;
//         $$->children.push_back(temp);
//         $$->children.push_back($3); 
//         $3->parent = $$;
//       }
//     | %empty {
//         $$ = new node(get_id());$$->lineno=yylineno;
//         $$->name = "op_and_expr";
//         struct node* temp = new node(get_id());
//         temp->name = "empty";
//         temp->terminal = true;
//         temp->parent = $$;
//         $$->children.push_back(temp);
//     }

/*and_expr: shift_expr op_shift_expr{
    $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "and_expr";
        $$->children.push_back($1); 
        $1->parent = $$;
        $$->children.push_back($2); 
        $2->parent = $$;
}*/

and_expr: shift_expr{
    $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "and_expr";
        $$->children.push_back($1); 
        $1->parent = $$;
        
}

| and_expr OPERATOR_and shift_expr {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "and_expr";
        $$->children.push_back($1); 
        $1->parent = $$;
        struct node* temp = new node(get_id());
        temp->name = "&";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        $$->children.push_back($3); 
        $3->parent = $$;   
}





/*
op_shift_expr:
    op_shift_expr OPERATOR_and shift_expr{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "op_shift_expr";
        $$->children.push_back($1); 
        $1->parent = $$;
        struct node* temp = new node(get_id());
        temp->name = "and";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        $$->children.push_back($3); 
        $3->parent = $$;
    }
    | %empty {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "op_shift_expr";
        struct node* temp = new node(get_id());
        temp->name = "empty";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
    }
    */
/*
shift_expr: arith_expr op_arith_expr{
    $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "shift_expr";
        $$->children.push_back($1); 
        $1->parent = $$;
        $$->children.push_back($2); 
        $2->parent = $$;
}

op_arith_expr:
      op_arith_expr OPERATOR_rightshift arith_expr{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "op_arith_expr";
        $$->children.push_back($1); 
        $1->parent = $$;
        struct node* temp = new node(get_id());
        temp->name = "rightshift";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        $$->children.push_back($3); 
        $3->parent = $$;
      }
    | op_arith_expr OPERATOR_leftshift arith_expr{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "op_arith_expr";
        $$->children.push_back($1); 
        $1->parent = $$;
        struct node* temp = new node(get_id());
        temp->name = "leftshift";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        $$->children.push_back($3); 
        $3->parent = $$;
    }
    | %empty {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "op_arith_expr";
        struct node* temp = new node(get_id());
        temp->name = "empty";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
    }

*/


shift_expr : arith_expr {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "shift_expr";
        $$->children.push_back($1); 
        $1->parent = $$;

}
| shift_expr OPERATOR_leftshift arith_expr {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "shift_expr";
        $$->children.push_back($1); 
        $1->parent = $$;
        struct node* temp = new node(get_id());
        temp->name = "<<";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        $$->children.push_back($3); 
        $3->parent = $$;
}
| shift_expr OPERATOR_rightshift arith_expr {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "shift_expr";
        $$->children.push_back($1); 
        $1->parent = $$;
        struct node* temp = new node(get_id());
        temp->name = ">>";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        $$->children.push_back($3); 
        $3->parent = $$;
}



/*arith_expr: subterm OPERATOR_add arith_expr | subterm + */
arith_expr: sub_term OPERATOR_add arith_expr{
    $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "arith_expr";
        $$->children.push_back($1); 
        $1->parent = $$;
        struct node* temp = new node(get_id());
        temp->name = "+";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        $$->children.push_back($3); 
        $3->parent = $$;
}

| sub_term {
    $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "arith_expr";
        $$->children.push_back($1); 
        $1->parent = $$;
        
}

//sub_term : term | sub_term - term
sub_term: term{
    $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "sub_term";
        $$->children.push_back($1); 
        $1->parent = $$;
}
| sub_term OPERATOR_minus term {
    $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "sub_term";
        $$->children.push_back($1); 
        $1->parent = $$;
        struct node* temp = new node(get_id());
        temp->name = "-";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        $$->children.push_back($3); 
        $3->parent = $$;
}





/*term: factor op_factor{
    $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "term";
    $$->children.push_back($1); 
        $1->parent = $$;
        $$->children.push_back($2); 
        $2->parent = $$;
}

op_factor:
      op_factor POINTER_star factor{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "op_factor";
        $$->children.push_back($1); 
        $1->parent = $$;
        struct node* temp = new node(get_id());
        temp->name = "star";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        $$->children.push_back($3); 
        $3->parent = $$;
      }
    | op_factor SYMBOL_decorator factor{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "op_factor";
        $$->children.push_back($1); 
        $1->parent = $$;
        struct node* temp = new node(get_id());
        temp->name = "decorator";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        $$->children.push_back($3); 
        $3->parent = $$;
      }
    | op_factor OPERATOR_divide factor{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "op_factor";
        $$->children.push_back($1); 
        $1->parent = $$;
        struct node* temp = new node(get_id());
        temp->name = "divide";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        $$->children.push_back($3); 
        $3->parent = $$;
      }
    | op_factor OPERATOR_floordivision factor{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "op_factor";
        $$->children.push_back($1); 
        $1->parent = $$;
        struct node* temp = new node(get_id());
        temp->name = "floordivision";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        $$->children.push_back($3); 
        $3->parent = $$;
      }
    | op_factor OPERATOR_modulo factor{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "op_factor";
        $$->children.push_back($1); 
        $1->parent = $$;
        struct node* temp = new node(get_id());
        temp->name = "modulo";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        $$->children.push_back($3); 
        $3->parent = $$;
      }
    | %empty {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "op_factor";
        struct node* temp = new node(get_id());
        temp->name = "empty";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
    }
    */

// div_term :  factor | div_term (division |modulo | floordiv) factor     

div_term : factor{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "div_term";
        $$->children.push_back($1); 
        $1->parent = $$;
}
| div_term OPERATOR_modulo factor {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "div_term";
        $$->children.push_back($1); 
        $1->parent = $$;
        struct node* temp = new node(get_id());
        temp->name = "%%";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        $$->children.push_back($3); 
        $3->parent = $$;
}
| div_term OPERATOR_floordivision factor {
    $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "div_term";
        $$->children.push_back($1); 
        $1->parent = $$;
        struct node* temp = new node(get_id());
        temp->name = "//";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        $$->children.push_back($3); 
        $3->parent = $$;
}
| div_term OPERATOR_divide factor {
    $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "div_term";
        $$->children.push_back($1); 
        $1->parent = $$;
        struct node* temp = new node(get_id());
        temp->name = "/";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        $$->children.push_back($3); 
        $3->parent = $$;
}

// term: factor | factor * div_term

term : div_term {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "term";
        $$->children.push_back($1); 
        $1->parent = $$;
}
| div_term POINTER_star term {
    $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "term";
        $$->children.push_back($1); 
        $1->parent = $$;
        struct node* temp = new node(get_id());
        temp->name = "*";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);
        $$->children.push_back($3); 
        $3->parent = $$;
}


factor: 
      OPERATOR_add factor{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "factor";

        struct node* temp = new node(get_id());
        temp->name = "+";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);

        $$->children.push_back($2); 
        $2->parent = $$;
      }
    | OPERATOR_minus factor{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "factor";

        struct node* temp = new node(get_id());
        temp->name = "-";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);

        $$->children.push_back($2); 
        $2->parent = $$;
      }
    | OPERATOR_bitwisenot factor{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "factor";

        struct node* temp = new node(get_id());
        temp->name = "~";
        temp->terminal = true;
        temp->parent = $$;
        $$->children.push_back(temp);

        $$->children.push_back($2); 
        $2->parent = $$;
      }
    | power{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$->name = "factor";
        $$->children.push_back($1); 
        $1->parent = $$;
      }


power: atom_expr POINTER_double_star factor {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "power";
    $$->children.push_back($1);
    $1->parent = $$;
    struct node * temp1=new node(get_id());
        temp1->name = "**";
        temp1->terminal=true;
        $$->children.push_back(temp1);
        temp1->parent = $$;

        $$->children.push_back($3);
        $3->parent = $$;}
    | atom_expr {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "power";
     $$->children.push_back($1);}

// double_factor: 
//     {$$ = new node(get_id());$$->lineno=yylineno;
//     $$ -> name = "double_factor";
//       struct node * temp1=new node(get_id());
//         temp1->name = "**";
//         temp1->terminal=true;
//         $$->children.push_back(temp1);
//         $$->children.push_back($2);}

atom_expr: 
    atom trailer {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "atom_expr";
     $$->children.push_back($1);
     $1->parent = $$;
     $$->children.push_back($2);
     $2->parent = $$;}

atom_comma : atom {
    $$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "atom_comma";
    $$->children.push_back($1);
    
    }
    | atom DELIMITER_comma atom_comma {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$ -> name = "atom_comma";
        struct node * temp1=new node(get_id());
        temp1->name = ",";
        temp1->terminal=true;
        $$->children.push_back($1);
        $$->children.push_back(temp1);
        $$->children.push_back($3);

    }
    

atom: 
    DELIMITER_o_paranthesis testlist_comp DELIMITER_c_paranthesis {$$ = new node(get_id());$$->lineno=yylineno;
        $$ -> name = "atom";
        struct node * temp1=new node(get_id()),*temp2=new node(get_id());
        temp1->name = "(";temp2->name = ")"; 
        $$->children.push_back(temp1);
        $$->children.push_back($2);
        $$->children.push_back(temp2);}
    /*| DELIMITER_o_brackets testlist_comp DELIMITER_c_brackets {$$ = new node(get_id());*/
    | DELIMITER_o_brackets atom_comma DELIMITER_c_brackets {$$ = new node(get_id());
    $$->lineno=yylineno;
        $$ -> name = "atom";
        struct node * temp1=new node(get_id()),*temp2=new node(get_id());
        temp1->name = "[";temp2->name = "]"; 
        $$->children.push_back(temp1);
        $$->children.push_back($2);
        $$->children.push_back(temp2);}
    | DELIMITER_o_paranthesis DELIMITER_c_paranthesis {$$ = new node(get_id());$$->lineno=yylineno;
        $$ -> name = "atom";
        struct node * temp1=new node(get_id()),*temp2=new node(get_id());
        temp1->name = "(";temp2->name = ")"; 
        $$->children.push_back(temp1);
        $$->children.push_back(temp2);}
    | DELIMITER_o_brackets DELIMITER_c_brackets  {$$ = new node(get_id());$$->lineno=yylineno;
        $$ -> name = "atom";
        struct node * temp1=new node(get_id()),*temp2=new node(get_id());
        temp1->name = "[";temp2->name = "]"; 
        $$->children.push_back(temp1);
        $$->children.push_back(temp2);}

    | NAME {$$ = new node(get_id());$$->lineno=yylineno;
        $$ -> name = "atom";
        struct node * temp1=new node(get_id());
        temp1->name =$1;
        temp1->terminal = true; 
        $$->children.push_back(temp1);}
    | number {$$ = new node(get_id());$$->lineno=yylineno;
        $$ -> name = "atom";
        $$->children.push_back($1);}
    | strings {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "atom";
    $$->children.push_back($1);}
    | KEYWORD_none {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "atom";
    struct node * temp1=new node(get_id());
    temp1->name ="None";temp1->terminal = true;
     $$->children.push_back(temp1);
     temp1->terminal = true;}
    | KEYWORD_true {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "atom";
    struct node * temp1=new node(get_id());
    temp1->name ="True";
    temp1->terminal = true; 
    $$->children.push_back(temp1);}
    | KEYWORD_false {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "atom";
    struct node * temp1=new node(get_id());
    temp1->name ="False";
    temp1->terminal = true; 
    $$->children.push_back(temp1);}

number: NUMBER 
        {$$ = new node(get_id());$$->lineno=yylineno;
        $$ -> name = "number";
        struct node * temp1=new node(get_id());
        temp1->name =$1;
        temp1->terminal = true; 
        $$->children.push_back(temp1);
        }
      | FLOAT {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$ -> name = "float";
        struct node * temp1=new node(get_id());
        temp1->name =$1;
        temp1->terminal = true; 
        $$->children.push_back(temp1);
      }

strings:
    STRING strings {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "strings";
    struct node * temp1=new node(get_id());
    temp1->name =$1;
    temp1->terminal = true;
    $$->children.push_back(temp1);
    $$->children.push_back($2);}
    | STRING {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "strings";
    struct node * temp1=new node(get_id());
    temp1->name =$1;
    temp1->terminal = true; 
    $$->children.push_back(temp1);}

testlist_comp:
    test_star_expr comp_for {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "testlist_comp";
    $$->children.push_back($1);
    $$->children.push_back($2);}
    | testlist_star_expr {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "testlist_comp";
    $$->children.push_back($1);}

test_star_expr: 
    test {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "test_star_expr";
    $$->children.push_back($1);
    $1->parent = $$;}
    | star_expr {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "test_star_expr";
    $$->children.push_back($1);$1->parent = $$;}

comma_test_star: 
    comma_test_star DELIMITER_comma test_star_expr {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "test_star_expr";
    struct node * temp1=new node(get_id());
    temp1->name=",";
    $$->children.push_back($1);
    $$->children.push_back(temp1);
    $$->children.push_back($3);}
    | %empty {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "test_star_expr";
    
    struct node * temp1=new node(get_id());
    temp1->name="empty";
    temp1->terminal=true;
    $$->children.push_back(temp1);}

trailer:
    attribute{
        $$ = new node(get_id());$$->lineno=yylineno;
        $$ -> name = "trailer";
        $$->children.push_back($1);
    }
    |methodcall {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$ -> name = "trailer";
        $$->children.push_back($1);
    }
    |func_call {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$ -> name = "trailer";
        $$->children.push_back($1);
        $1->parent = $$;
    }
    |array_acc {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$ -> name = "trailer";
        $$->children.push_back($1);
    }
    
    | %empty {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$ -> name = "trailer";
        
        struct node * temp1=new node(get_id());
        temp1->name="empty";
        temp1->terminal=true;
        $$->children.push_back(temp1);
    }

func_call : DELIMITER_o_paranthesis DELIMITER_c_paranthesis{
            $$ = new node(get_id());$$->lineno=yylineno;
            $$ -> name = "func_call";
           
            struct node * temp3=new node(get_id()),*temp4=new node(get_id());
            temp3->name = "(";
            temp4->name = ")";
            temp3->terminal=true;
            temp4->terminal=true; 
            $$->children.push_back(temp3);
            $$->children.push_back(temp4);
        }
    |DELIMITER_o_paranthesis arglist DELIMITER_c_paranthesis{
            $$ = new node(get_id());$$->lineno=yylineno;
            $$ -> name = "func_call";
            
           
            struct node * temp3=new node(get_id()),*temp4=new node(get_id());
            temp3->name = "(";
            temp4->name = ")";
            temp3->terminal=true;
            temp4->terminal=true; 
            $$->children.push_back(temp3);
            $$->children.push_back($2);
            $$->children.push_back(temp4);
    }

methodcall: DELIMITER_dot NAME DELIMITER_o_paranthesis DELIMITER_c_paranthesis{
            $$ = new node(get_id());$$->lineno=yylineno;
            $$ -> name = "methodcall";
            struct node * temp1=new node(get_id()),*temp2=new node(get_id());
            temp1->name = ".";
            temp2->name = $2;
            temp1->terminal=true;
            temp2->terminal=true; 
            $$->children.push_back(temp1) ; 
            $$->children.push_back(temp2);
            struct node * temp3=new node(get_id()),*temp4=new node(get_id());
            temp3->name = "(";
            temp4->name = ")";
            temp3->terminal=true;
            temp4->terminal=true; 
            $$->children.push_back(temp3);
           
            $$->children.push_back(temp4);
    }
    |  DELIMITER_dot NAME DELIMITER_o_paranthesis arglist DELIMITER_c_paranthesis{
            $$ = new node(get_id());$$->lineno=yylineno;
            $$ -> name = "methodcall";
            struct node * temp1=new node(get_id()),*temp2=new node(get_id());
            temp1->name = ".";
            temp2->name = $2;
            temp1->terminal=true;
            temp2->terminal=true; 
            struct node * temp3=new node(get_id()),*temp4=new node(get_id());
            $$->children.push_back(temp1) ; 
            $$->children.push_back(temp2);
            temp3->name = "(";
            temp4->name = ")";
            temp3->terminal=true;
            temp4->terminal=true; 
            $$->children.push_back(temp3);
            $$->children.push_back($4);
            $$->children.push_back(temp4);
    }

array_acc: DELIMITER_o_brackets subscriptlist DELIMITER_c_brackets {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$ -> name = "array_acc";
        struct node * temp1=new node(get_id()),*temp2=new node(get_id());
        temp1->name = "[";
        temp2->name = "]";
        temp1->terminal=true;
        temp2->terminal=true; 
        $$->children.push_back(temp1);
        $$->children.push_back($2);
        $$->children.push_back(temp2);
}

attribute: DELIMITER_dot NAME {
        $$ = new node(get_id());$$->lineno=yylineno;
        $$ -> name = "attribute";
        struct node * temp1=new node(get_id()),*temp2=new node(get_id());
        temp1->name = ".";
        temp2->name = $2;
        temp1->terminal=true;
        temp2->terminal=true; 
        $$->children.push_back(temp1) ; 
        $$->children.push_back(temp2);

}

subscriptlist: 
    subscript comma_subscript {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "subscriptlist";
    $$->children.push_back($1);
    $$->children.push_back($2);}
    | subscript comma_subscript DELIMITER_comma  {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "subscriptlist";
    $$->children.push_back($1);
    $$->children.push_back($2);
    struct node * temp1=new node(get_id());
    temp1->name = ",";temp1->terminal=true;
    $$->children.push_back(temp1);}

comma_subscript:
    comma_subscript DELIMITER_comma subscript {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "comma_subscirpt";
    struct node * temp1=new node(get_id());
    temp1->name=",";temp1->terminal=true;
    $$->children.push_back($1);
    $$->children.push_back(temp1);
    $$->children.push_back($3);}
    | %empty {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "comma_subsciptlist";
    struct node * temp1=new node(get_id());
    temp1->name="empty";
    temp1->terminal=true;
    $$->children.push_back(temp1);}

subscript: 
    test {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "subscipt";
    $$->children.push_back($1);$1->parent = $$;}
    | test DELIMITER_colon test sliceop  {$$ = new node(get_id());$$->lineno=yylineno;
        $$ -> name = "subscipt";
        struct node * temp1=new node(get_id());
        temp1->name = ":";
        temp1->terminal=true;
        $$->children.push_back($1);
        $1->parent = $$; 
        $$->children.push_back(temp1);
        $$->children.push_back($3);
        $3->parent = $$;
        $$->children.push_back($4);}
    | DELIMITER_colon test sliceop {$$ = new node(get_id());$$->lineno=yylineno;
        $$ -> name = "subscipt";
        struct node * temp1=new node(get_id());
        temp1->name = ":"; 
        temp1->terminal=true; 
        $$->children.push_back(temp1);
        $$->children.push_back($2);
        $2->parent = $$;
        $$->children.push_back($3);}

    | test DELIMITER_colon test {$$ = new node(get_id());$$->lineno=yylineno;
        $$ -> name = "subscipt";
        struct node * temp1=new node(get_id());
        temp1->name = ":"; 
        temp1->terminal=true;
        $$->children.push_back($1); 
        $1->parent = $$;
        $$->children.push_back(temp1);
        $$->children.push_back($3);
        $3->parent = $$;}

    | test DELIMITER_colon sliceop {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "subscipt";
    struct node * temp1=new node(get_id());
    temp1->name = ":";  temp1->terminal= true; 
    $$->children.push_back($1); 
    $1->parent = $$;
    $$->children.push_back(temp1);
    $$->children.push_back($3);}
    | test DELIMITER_colon {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "subscipt";
    struct node * temp1=new node(get_id());
    temp1->name = ":";  
    temp1->terminal= true; 
    $$->children.push_back($1); 
    $1->parent = $$;
    $$->children.push_back(temp1);}
    | DELIMITER_colon sliceop {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "subscipt";
    struct node * temp1=new node(get_id());
    temp1->name = ":";  
    temp1->terminal= true; 
    $$->children.push_back(temp1);
    $$->children.push_back($2);}

    | DELIMITER_colon test {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "subscipt";
    struct node * temp1=new node(get_id());
    temp1->name = ":";  
    temp1->terminal= true;  
    $$->children.push_back(temp1);
    $$->children.push_back($2);
    $2->parent = $$;
    }

    | DELIMITER_colon{ $$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "subscipt";
    struct node * temp1=new node(get_id());
    temp1->name = ":";  
    temp1->terminal= true; 
    $$->children.push_back(temp1);}

sliceop: 
    DELIMITER_colon test {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "sliceop";
    struct node * temp1=new node(get_id());
    temp1->name = ":";  
    temp1->terminal= true; 
    $$->children.push_back(temp1);
    $$->children.push_back($2);
    $2->parent = $$;}
    | DELIMITER_colon  {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "sliceop";
    struct node * temp1=new node(get_id());
    temp1->name = ":";  
    temp1->terminal= true; 
    $$->children.push_back(temp1);}

exprlist: 
    expr_star_expr comma_expr_star_expr {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "exprlist";
    $$->children.push_back($1);
    $$->children.push_back($2);}
    | expr_star_expr comma_expr_star_expr DELIMITER_comma {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "testlist_comp";
    $$->children.push_back($1);
    $$->children.push_back($2);
    struct node * temp1=new node(get_id());
    temp1->name = ",";  
    temp1->terminal= true; 
    $$->children.push_back(temp1);}

expr_star_expr:
    expr {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "expr_star_expr";
    $$->children.push_back($1);}
    | star_expr {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "expr_star_expr";
    $$->children.push_back($1);}

comma_expr_star_expr:
    comma_expr_star_expr DELIMITER_comma expr_star_expr {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "comma_expr_start_expr";
    struct node * temp1=new node(get_id());
    temp1->name = ",";  
    temp1->terminal= true; 
    $$->children.push_back($1); 
    $$->children.push_back(temp1);
    $$->children.push_back($3);}

    | %empty {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "comma_expr_start_expr";
    struct node * temp1=new node(get_id());
    temp1->name = "empty";  
    temp1->terminal= true; 
    $$->children.push_back(temp1);}

testlist: test comma_test {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "testlist";
    $$->children.push_back($1);
    $1->parent = $$;
    $$->children.push_back($2);}
    | test comma_test DELIMITER_comma {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "testlist";
    $$->children.push_back($1);
    $1->parent = $$;
    $$->children.push_back($2);
    struct node * temp1=new node(get_id());
    temp1->name = ",";  
    temp1->terminal= true; 
    $$->children.push_back(temp1);}
    
  |  method_call {
         $$ = new node(get_id());$$->lineno=yylineno;
                                            $$->name = "expr_stmt";

                                            $$->children.push_back($1); 
                                            $1->parent = $$;

    } 

comma_test: 
    comma_test DELIMITER_comma test {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "comma_test";
    struct node * temp1=new node(get_id());
    temp1->name = ",";  temp1->terminal= true; 
    $$->children.push_back($1); 
    $$->children.push_back(temp1);
    $$->children.push_back($3);
    $3->parent = $$;}
    | %empty {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "comma_test";
    struct node * temp1=new node(get_id());
    temp1->name = "empty";  
    temp1->terminal= true; 
    $$->children.push_back(temp1);}

classdef: 
    KEYWORD_class NAME DELIMITER_colon suite {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "classdef";
    struct node * temp1=new node(get_id()),*temp2=new node(get_id()),*temp5=new node(get_id());
    temp1->name = "class";  
    temp1->terminal= true;
    temp2->name = $2;  
    temp2->terminal= true;
    temp5->name = ":";  
    temp5->terminal= true; 
    $$->children.push_back(temp1);
    $$->children.push_back(temp2); 
    $$->children.push_back(temp5);
    $$->children.push_back($4);}

    | KEYWORD_class NAME DELIMITER_o_paranthesis DELIMITER_c_paranthesis DELIMITER_colon suite {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "classdef";
    struct node * temp1=new node(get_id()),*temp2=new node(get_id()),*temp3=new node(get_id()),*temp4=new node(get_id()),*temp5=new node(get_id());
    temp1->name = "class";  
    temp1->terminal= true;
    temp2->name = $2;  
    temp2->terminal= true;
    temp3->name = "(";  
    temp3->terminal= true;
    temp4->name = ")";  
    temp4->terminal= true;
    temp5->name = ":";  
    temp5->terminal= true; 
    $$->children.push_back(temp1);
    $$->children.push_back(temp2);
    $$->children.push_back(temp3);
    $$->children.push_back(temp4); 
    $$->children.push_back(temp5);
    $$->children.push_back($6);}

    | KEYWORD_class NAME DELIMITER_o_paranthesis NAME DELIMITER_c_paranthesis DELIMITER_colon suite {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "classdef";
    struct node * temp1=new node(get_id()),*temp2=new node(get_id()),*temp3=new node(get_id()),*temp4=new node(get_id()),*temp5=new node(get_id()), *temp6 = new node(get_id());
    temp1->name = "class";  
    temp1->terminal= true;
    temp2->name = $2;  
    temp2->terminal= true;
    temp3->name = "(";  
    temp3->terminal= true;
    temp4->name = ")";  
    temp4->terminal= true;
    temp5->name = ":";  
    temp5->terminal= true; 
    temp6->name = $4;
    temp6->terminal = true;
    $$->children.push_back(temp1);
    $$->children.push_back(temp2);
    $$->children.push_back(temp3);
    $$->children.push_back(temp6);
    $$->children.push_back(temp4); 
    $$->children.push_back(temp5);
    $$->children.push_back($7);}

arglist: c_argument DELIMITER_comma  {$$ = new node(get_id());$$->lineno=yylineno;
                                $$ -> name = "arglist";
                                $$->children.push_back($1);
                                struct node * temp1=new node(get_id());
                                temp1->name = ",";  
                                temp1->terminal= true; 
                                $$->children.push_back(temp1);
                                }
    | c_argument  {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "arglist";
    $$->children.push_back($1);}

c_argument:
     c_argument DELIMITER_comma argument {$$ = new node(get_id());$$->lineno=yylineno;
     $$ -> name = "c_argument";
     struct node * temp1=new node(get_id());
     temp1->name = ",";  
     temp1->terminal= true; 
     $$->children.push_back($1); 
     $$->children.push_back(temp1);
     $$->children.push_back($3);}

    | argument  {
                    $$ = new node(get_id());
                    $$->lineno=yylineno;
                    $$ -> name = "c_argument"; 

                    $$->children.push_back($1);
                    $1->parent = $$;
                }

argument:  
    test  {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "argument"; 
    $$->children.push_back($1);
    $1->parent = $$;}
          
comp_iter: 
    comp_for {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "comp_iter"; 
    $$->children.push_back($1);}
    | comp_if {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "comp_iter"; 
    $$->children.push_back($1);}
    | %empty {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "comp_iter";
    struct node * temp1=new node(get_id());
    temp1->name = "empty";  
    temp1->terminal= true; 
    $$->children.push_back(temp1);}

comp_for: 
    COMPOUND_STATEMENT_for exprlist KEYWORD_in or_test comp_iter {$$ = new node(get_id());$$->lineno=yylineno;$$ -> name = "comp_for";struct node * temp1=new node(get_id()),*temp2=new node(get_id());
    temp1->name = "for";  
    temp1->terminal= true;
    temp2->name = "in";  
    temp2->terminal= true; 
    $$->children.push_back(temp1);
    $$->children.push_back($2);
    $$->children.push_back(temp2);
    $$->children.push_back($4); 
    $$->children.push_back($5);}

comp_if: 
    COMPOUND_STATEMENT_if or_test comp_iter  {$$ = new node(get_id());$$->lineno=yylineno;
    $$ -> name = "comp_if";
    struct node * temp1=new node(get_id());
    temp1->name = "if";  
    temp1->terminal= true; 
    $$->children.push_back(temp1);
    $$->children.push_back($2);
    $$->children.push_back($3);}

%%

int yywrap() {
    return 1;
}

void yyerror(string msg) {
    flag = true;
	cout << RED << "\nError at line " << yylineno << RESET<<endl;
}

void error(string msg) {
	cout << RED << "\nError in question at line " << RESET<<endl;
}