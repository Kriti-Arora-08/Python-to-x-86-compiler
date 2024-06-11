#include<bits/stdc++.h>

#ifndef SYM_HPP
#define SYM_HPP

using namespace std;

struct syment{

  string name;
  string type;
  string token;
  int lineno;
  int offset;//??
  unsigned long long size;
  bool attr;//whether the variable is an attribute or not--
  bool class_obj;//whether the variable is class object or not
  bool func_name;//whether the given entry is function or not

  syment(){

    this->attr=false;
    this->class_obj=false;
    this->func_name=false;

  }
  

};

struct symtab{
  int uid;
  vector<struct symtab * > children;
  struct symtab * parent;
  int scope;//0-global , 1 - function , 2 - class , 3 - class method , 4 - if else or for blocks
  string name; // class name (if classdef) , function_name (if function def) , method_name (if method)
  string return_type;//only for class methods and functions 
  int para_ind;//stores the current parameter index when parsing 'tfpdef'
  struct symtab * inher_class;//pointer to symbol table of the inherited class(if any in class definition)
  map<string,string> parameters ; //maps parameters to its types -- only when the symtab represents a function 
  map<string,string> attributes ; //maps attributes to its types -- only when the symbtab represents a class def
  map <string ,struct syment*> entries;


  symtab(){
    //constructor
    this->scope=0;
    this->para_ind=0;
    this->parent=NULL;

  }

};

void atomex(struct node * atom_ex,struct symtab * cur);

void fill_type_comp();


void trav(struct node * root,struct symtab * cur);

void print_symtab(struct symtab * table);

void print_syment(struct syment * entry);

struct syment * entry_find(string name, struct symtab * cur);

string check_type(struct node * root);

void atom_com(struct node * root_sym , struct symtab * cur);


#endif