%{
#include <bits/stdc++.h>
#include <vector>
#include "parser.tab.h"
using namespace std;
#define YY_DECL int yylex()
// extern void yyerror();
vector<int> vec={0};
int lno=1;
string tb;

int wht_sp=0;
int state=0;

void count_wht(string str){
    int count=0;
    for(int i=0;i<str.size();i++){
        if(str[i]=='\n') {
        //   cout<<"nl ";
        }
        if(str[i]==' '){
        //   cout<<"sp ";
          count++;
        }
        else if(str[i]=='\t'){
        //   cout<<"tb ";
          count+=2;
        }
    }
    wht_sp=count;
    return;
}

%}

%x comment2
%x textblock
%x s1
%option noyywrap
%option yylineno


ind [\t ]
alpha [a-zA-Z_0-9]
name [a-zA-Z_]{alpha}*
dig [0-9]
integ {dig}+
flo {integ}+"."{integ}+ 
STRING_LEX ("[^\"\n\r]*")|('[^\'\n\r]*')
STR ("\""[^\"\n\r]*"\"")|("\'"[^\'\n\r]*"\'")
SPACE [ \t\f]
LT \n|\r|\r\n
%%

%{
    // cout<<"OK"<<endl;
    if(state==1 || state==2){
        // cout<<"OK@"<<endl;
        int n=vec.size();
        // cout<<wht_sp<<endl;
        if(vec[n-1]<wht_sp){
            // cout<<"B1"<<endl;
            if(state==2){
                // cout<<"ERROR"<<endl;
            // yyerror();    
            cerr << "Illegal Indentation " << yylineno << '\n';    
            exit(0);    
            }
            vec.push_back(wht_sp);
            state=0;
            // cout<<"ind"<<endl;
            return INDENT;
        }
        else if(vec[n-1]==wht_sp){
            // cout<<"B2"<<endl;
            state=0;
        }
        else {
            // cout<<"B3"<<endl;
            // cout<<"ded"<<endl;
            vec.resize(n-1);
            state=2;
            return DEDENT;
        }
    }
    if(state==3) {yyterminate();}
%}

"#"  { BEGIN comment2; }
<comment2>[^\n\r\r\n]*
<comment2>\n { BEGIN 0; }

"\'\'\'"{SPACE}*{LT} { 
    BEGIN textblock; 
    tb = "";    // we do not include the starting quotes
}
<textblock>[^\"\\]* { tb += yytext; }
<textblock>\\ { 
    cerr << "Illegal \\ inside text block found at line: " << yylineno << '\n';
    BEGIN 0;
    return 0;
}
<textblock>\"{1,2} {
    tb += (yyleng == 1 ? "\\\"" : "\\\"\\\"") ; 
}
<textblock><<EOF>> { 
    cerr << "Unfinished textblock. Aborting!" << '\n';
    BEGIN 0;
    return 0; 
}

<textblock>"\""+"\"\"" { 
    yylval.val = strdup(yytext);
    tb = "";
    BEGIN 0; 
    return (STRING);
}

"range" {return RANGE;}
"self" {yylval.val = strdup(yytext);return SELF;}
"None"|"int"|"float"|"bool"|"list[int]"|"list[bool]"|"list[float]"|"str" {yylval.val = strdup(yytext); return DTYPE;}
"*=" {return OPERATOR_starequal;}
"**=" {return OPERATOR_doublemultequal;}
"**" {return POINTER_double_star;}
"*" {return POINTER_star;}
{STR} {yylval.val = strdup(yytext);return STRING;}
"!=" {return OPERATOR_notequal;}
"%=" {return OPERATOR_moduloequal;}
"%" {return OPERATOR_modulo;}
"&=" {return OPERATOR_andequal;}
"&" {return OPERATOR_and;}
"(" {return DELIMITER_o_paranthesis;}
")" {return DELIMITER_c_paranthesis;}
"=" {return OPERATOR_equal;}
"+=" {return OPERATOR_addequal;}
"+" {return OPERATOR_add;}
"," {return DELIMITER_comma;}
"-=" {return OPERATOR_minusequal;}
"-" {return OPERATOR_minus;}
"->" {return DELIMITER_pointer;}
"." {return DELIMITER_dot;}
"/" {return OPERATOR_divide;}
"//" {return OPERATOR_floordivision;}
"//=" {return OPERATOR_floordivisionequal;}
"/=" {return OPERATOR_divideequal;}
":" {return DELIMITER_colon;}
";" {return DELIMITER_semicolon;}
"<" {return OPERATOR_lessthan;}
"<<" {return OPERATOR_leftshift;}
"<<=" {return OPERATOR_leftshiftequal;}
"<=" {return OPERATOR_lessthanequal;}
"==" {return OPERATOR_equality;}
">" {return OPERATOR_greaterthan;}
">=" {return OPERATOR_greaterthanequal;}
">>" {return OPERATOR_rightshift;}
">>=" {return OPERATOR_rightshiftequal;}
"@" {return SYMBOL_decorator;}
"False" {return KEYWORD_false;}
"True" {return KEYWORD_true;}
"[" {return DELIMITER_o_brackets;}
"]" {return DELIMITER_c_brackets;}
"^" {return OPERATOR_xor;}
"^=" {return OPERATOR_xorequal;}
"and" {return KEYWORD_and;}
"break" {return FLOW_STATEMENT_break;}
"class" {return KEYWORD_class;}
"continue" {return FLOW_STATEMENT_continue;}
"def" {return KEYWORD_define;}
"elif" {return COMPOUND_STATEMENT_elif;}
"else" {return COMPOUND_STATEMENT_else;}
"for" {return COMPOUND_STATEMENT_for;}
"if" {return COMPOUND_STATEMENT_if;}
"in" {return KEYWORD_in;}
"is" {return KEYWORD_is;}
"global" {return KEYWORD_global;}
"not" {return KEYWORD_not;}
"or" {return KEYWORD_or;}
"return " {return FLOW_STATEMENT_return;}
"while" {return COMPOUND_STATEMENT_while;}
"|" {return OPERATOR_bitwiseor;}
"|=" {return OPERATOR_bitwiseorequal;}
"~" {return OPERATOR_bitwisenot;}
"\\\n" {}
"\n"{ind}* {//cout<<"nl"<<endl;
                string str=yytext;
                count_wht(str);
                state=1;lno++;
                // cout<<"\n"<<"line:"<<lno<<" ";
                // cout<<"\n";
                return NEWLINE;
            }

<<EOF>> {state=3;return ENDMARKER;}

{flo}  {yylval.val = strdup(yytext);return FLOAT ;}
{integ} {yylval.val = strdup(yytext);return NUMBER ;}

{name} {yylval.val = strdup(yytext);return NAME;}

.   {}
%%