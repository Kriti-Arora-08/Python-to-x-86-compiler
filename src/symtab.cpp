#ifndef SYM_CPP
#define SYM_CPP
#include<bits/stdc++.h>

#include "./node.hpp"
#include "./symtab.hpp"
using namespace std;

//add __name__ into global symbol table by default and set its value to "__main__"

set <string> classes;
set <string> functions;
map <string,struct symtab*> class_tab ;
map <string,int> type_comp;

ofstream fsymout("./out/sym.txt", std::ios::out | std::ios::trunc);
struct symtab* root_tab;

void fill_type_comp(){

  type_comp["empty"]=1;
  type_comp["int"]=2;
  type_comp["float"]=2;
  type_comp["number"]=2;
  type_comp["bool"]=3;
  type_comp["str"]=4;

}

void print_symtab(struct symtab * table){
  fsymout<<"TABLE :"<<table->name<<"||Scope:"<<table->scope<<endl;
  if(table->scope == 1 || table->scope == 3){
    //fsymout<<"Return type:"<<table->return_type<<endl;
  }
  auto it = table->entries.begin();
  // fsymout<<"b--\n";

  for(;it!=table->entries.end();it++){
    // fsymout<<"a\n";
    print_syment(it->second);
    // fsymout<<"s\n";
  }

  // fsymout<<"dd\n";
  int j = table->children.size();
  for(int i=0;i<table->children.size();i++){
    // fsymout<<"de\n";
    print_symtab(table->children[i]);
  }




}

void print_syment(struct syment * entry){
  // fsymout<<"entry\n";
  if(entry==NULL){
    // fsymout<<"YES\n";
    return;
  }
  fsymout<<entry->name<<"\t"<<entry->type<<"\t"<<entry->token<<"\t"<<entry->lineno<<endl;
  // fsymout<<"entry close\n";
}



int check_args(struct node * root_sym , struct symtab * cur ,int num_para  ){
        
        int cur_para=num_para;
        if(root_sym->name=="argument"){
          return num_para-1;
        }else{
          for(int i=0;i<root_sym->children.size();i++){
            cur_para=check_args(root_sym->children[i],cur,cur_para);
          }

        }

        return cur_para;
        
}

void atomex(struct node * atom_ex,struct symtab * cur){

  atom_ex->tab = cur;
// fsymout<<"atomex1\n";
  if(atom_ex->children[0]->children[0]->name=="[" && atom_ex->children[0]->children.size()>2){
    struct node * atom_c = atom_ex->children[0]->children[1];
    atom_com(atom_c,cur);
  }  
// fsymout<<"atomex2\n";
  if(atom_ex->children[0]->children[0]->name!="number" &&atom_ex->children[0]->children[0]->name!="float" && atom_ex->children[0]->children[0]->name!="strings" && atom_ex->children[0]->children[0]->name!="[" && atom_ex->children[0]->children[0]->name!="(" && atom_ex->children[0]->children[0]->name!="True" && atom_ex->children[0]->children[0]->name!="False"){
    //atomex a declared variable
    // fsymout<<"atomex3\n";

    struct node * atom = atom_ex->children[0];
    struct node * trailer_child = atom_ex->children[1]->children[0];

    if(trailer_child->name!="empty"){
      //there is a non empty trailer
      // fsymout<<"atomex4\n";
      if(trailer_child->name=="func_call"){
        //check whether atom refers to any functions
        //if yes, then also check the parameters are correct and their types are correct

        if(atom->children[0]->name=="print" ||atom->children[0]->name=="len" ){
          trav(trailer_child,cur);
          atom_ex->type="None";
          return;
        }

        if(functions.count(atom->children[0]->name)==1 ){
          // struct symtab * func_tab = root_tab->entries[atom->children[0]->name];
          struct symtab * func_tab;
          int found=0;
          for(int i=0;i<root_tab->children.size();i++){
            func_tab=root_tab->children[i];
            if(func_tab->name==atom->children[0]->name){
              found=1;
              break;
            }
          }

          if(!found){
            //error
            cout<<"line:"<<atom_ex->lineno<<"-ERROR:function "<<atom->children[0]->name<<" does not exist"<<endl;
            exit(1);
          }

          if(func_tab==NULL){
            //error
            cout<<"line:"<<atom_ex->lineno<<"-ERROR:function "<<atom->children[0]->name<<" does not exist"<<endl;
            exit(2);
          }

          atom_ex->type = func_tab->return_type;

          int num_para = func_tab->para_ind;
          int given_para = check_args(trailer_child, func_tab,num_para);

          if(given_para>0){
            cout<<"line:"<<atom_ex->lineno<<"-ERROR:Number of parameters is less\n"<<endl;
            exit(90);
          }

          if(given_para<0){
            cout<<"line:"<<atom_ex->lineno<<"-ERROR:Number of parameters is more\n"<<endl;
            exit(92);
          }
          
          trav(trailer_child->children[1],cur);
          
        }else{
          //no such function exist
          //can probably be a constructor for the class
          if(classes.count(atom->children[0]->name)==1){
            //a class exists---> it is class constructor
            atom_ex->type = atom->children[0]->name;
            return;
          }else{
            //error
            cout<<"line:"<<atom_ex->lineno<<"-ERROR:function"<<atom->children[0]->name<<" does not exist"<<endl;
            exit(2);
          }
        }

      }
      
      if(trailer_child->name=="methodcall"){
        //check for the class that atom object belongs to and whether the method exists and then check the parameters 
        
        // struct node * atom = atom_ex->children[0];
        // struct node * trailer_child = atom_ex->children[1]->children[0];

        struct syment * var;
        bool found = false;
        string name = atom->children[0]->name;
        struct symtab * temp =cur;

        if(cur->scope==2 || cur->scope ==3 || cur->scope ==1 || cur->scope==0 ){
          if(temp->entries[name]!=NULL){
            var = temp->entries[name];
            found=true;
          }

          if(!found){
            //error---the variable is not declared
            cout<<"line:"<<atom_ex->lineno<<"-ERROR:the variable "<<name<<" is not declared\n";
            exit(101);
          } 

        }else{
          
          while(!found){
            if(temp->entries[name]!=NULL){
              var = temp->entries[name];
              found=true;
            }else{
              temp = temp->parent;
              if(temp==NULL){break;}
              if(temp->scope!=4){break;}
            }
          }

          if(!found && temp!=NULL){
            if(temp->entries[name]!=NULL){
              found=true;
              var = temp->entries[name];
            }
          }



          if(!found){
            //error---the variable is not declared
            cout<<"line:"<<atom_ex->lineno<<"-ERROR:the variable "<<name<<" is not declared\n";
            exit(101);
          }
        }
        //var points to the entry of symbol table

        string type = var->type;
        //cout<<type<<endl;
        temp = class_tab[type];
        if(temp==NULL){
          // cout<<"YES"<<endl;
          cout<<"ERROR: Wrong method call\n";
          exit(404);
        }
        struct symtab * temp2;
        string call_name = atom_ex->children[1]->children[0]->children[1]->name;

        for(int i=0;i<temp->children.size();i++){
          temp2= temp->children[i];
          if(temp2->name == type){
            //this is the method call
            break;
          }

        }

        if(temp2->name!=call_name){
          //error
          //no such method exists
          cout<<"line:"<<atom_ex->lineno<<"-ERROR:"<<atom_ex->children[1]->children[0]->children[1]->name<<"- no such method exists\n";
          exit(69);
        }

        //temp2 points to the method symtab

        int num_para = temp2->para_ind;
        num_para--;
        int left = check_args(trailer_child, cur,num_para);
        if(left!=0){
          //error
          if(left>0){
            //less number of args than expected
            cout<<"ERR:less numbers of arguments\n";
            exit(343);
          }else{
            //more args than expected
            cout<<"More args than expected\n";
            exit(404);
          }
        }
      }

      if(trailer_child -> name == "array_acc"){
        //check that subcriptlist simplifies to a valid number
        trav(trailer_child->children[1], cur );

      }

      if(trailer_child->name == "attribute"){ 
        //check whether this attribute belongs to the class that atom object belongs to
        string name = atom->children[0]->name;
        struct syment * ent = entry_find(name,cur);
        string class_name = ent->type;

        struct symtab * class_symtab = class_tab[class_name];

        auto it = class_symtab->attributes.begin();

        for(;it!=class_symtab->attributes.end();it++){
          if(it->first == trailer_child->children[1]->name){
            atom_ex->type = it->second;
            return;
          }
        }

        cout<<"line:"<<atom_ex->lineno<<" ERROR:the attribute is not found\n";
        exit(404);


      }


    }else{
      //trailer_child -> empty
      //a normal variable should exist with that name
      /////////////////////////////

      struct syment * var = entry_find(atom->children[0]->name,cur);

      if(var==NULL){
        cout<<"line:"<<atom_ex->lineno<<"ERROR:variable not declared\n";
        exit(404);
      }else{
        atom_ex->type=var->type;
      }

      return;

      /*
      int scope = cur->scope;

      if(scope==1){
        //normal function scope
        //everything should  be declared in this scope
        if(cur->entries[atom->children[0]->name]==NULL){
          //use of undeclared variable
          //error
          cout<<"line:"<<atom->lineno<<"-ERROR:variable "<<atom->children[0]->name<<" not declared in this function def\n";
          exit(43);
        }else{
          struct syment * var = cur->entries[atom->children[0]->name];
          atom_ex->type = var->type;
        }
      }else if(scope==0){
        if(cur->entries[atom->children[0]->name]==NULL){
          //use of undeclared variable
          //error
          cout<<"line:"<<atom->lineno<<"-ERROR:variable "<<atom->children[0]->name<<" not declared\n";
          exit(43);
        }else{
          struct syment * var = cur->entries[atom->children[0]->name];
          atom_ex->type = var->type;
        }
      }else if(scope==4){
        //scope is if block or for block
        //search till parent block
        struct symtab * temp = cur;
        int found=0;

        while(temp!=NULL && temp->scope==4 && !found){
          if(temp->entries[atom->children[0]->name]!=NULL){
            found=1;
          }else{
            temp = temp->parent;
          }
        }

        if(!found){
          if(temp==NULL){
            cout<<"line:"<<atom->lineno<<"-ERROR:variable "<<atom->children[0]->name<<" not declared\n";
            exit(43);
          }else{
            if(temp->entries[atom->children[0]->name]==NULL){
              cout<<"line:"<<atom->lineno<<"-ERROR:variable "<<atom->children[0]->name<<" not declared\n";
              exit(43);
            }
          }
        }
        struct syment * var = cur->entries[atom->children[0]->name];
        atom_ex->type = var->type;
        
      }*/
      //////////////////////////////////////
    }

  }else{
    if(atom_ex->children[0]->children[0]->name=="number" || atom_ex->children[0]->children[0]->name=="float" ){
      atom_ex->type = atom_ex->children[0]->children[0]->name; 
    }else if(atom_ex->children[0]->children[0]->name == "strings"){
      atom_ex->type = "str";
    }else if(atom_ex->children[0]->children[0]->name == "True" || atom_ex->children[0]->children[0]->name == "False"){
      atom_ex->type = "bool";
    }

  }

}

struct syment * entry_find(string name,struct symtab * cur){

  struct syment * var;
  bool found = false;
  // string name = atom->children[0]->name;
  struct symtab * temp =cur;

  if(cur->scope==2 || cur->scope ==3 || cur->scope ==1 || cur->scope==0 ){
    if(temp->entries[name]!=NULL){
      var = temp->entries[name];
      found=true;
    }

    if(!found){
      return NULL;
    } else{
      return var;
    }

  }else{
    
    while(!found){
      if(temp->entries[name]!=NULL){
        var = temp->entries[name];
        found=true;
      }else{
        temp = temp->parent;
        if(temp==NULL){break;}
        if(temp->scope!=4){break;}
      }
    }

    if(!found && temp!=NULL){
      if(temp->entries[name]!=NULL){
        found=true;
        var = temp->entries[name];
      }
    }



    if(!found){
      //error---the variable is not declared
      return NULL;
    }else{
      return var;
    }
  }




}


void tfp(struct node * root_sym/*->name="tfpdef"*/ , struct symtab * cur){
  struct node * atom = root_sym->children[0];

  //cout<<"tfpdef\n";

  if(cur->scope == 1){
    //normal function
    //cout<<"norm func\n";
    if(root_sym->children.size()==1){
      //error as it refers to self keyword
      cout<<"line:"<<root_sym->lineno<<"ERROR:Self is a reserved keyword for class methods only\n";
      exit(39);
    }

    //cout<<"b\n";
    struct node * colon = root_sym->children[1];
    
    struct syment * ent = new syment();

    ent->name=atom->name;
    ent->token="parameter";
    ent->lineno=root_sym->lineno;
    // //check whether same variable name exists or not
    // if(cur->entries[ent->name]!=NULL){
    //   //error
    // }

    

    if(colon->children[0]->name == "empty"){
      //error
      cout<<"line:"<<root_sym->lineno<<"ERROR:no type for argument\n";
      exit(47);
    }else{
      string type = colon->children[1]->name;
      if(type =="dtype"){
        ent->type = colon->children[1]->children[0]->name;
      }else{
        //check whether such a class exists
        if(classes.count(colon->children[1]->children[0]->name)==1){
          ent->type = colon->children[1]->children[0]->name;
        }else{
          //error--no such class type exists
          cout<<"line:"<<root_sym->lineno<<"ERROR:No type exists for the argument\n";
          exit(63);
        }

      }
    }
    
    cur->para_ind++;

    if(cur->entries[ent->name]!=NULL){
      //error---already a defined parameter
      cout<<"line:"<<root_sym->lineno<<"-ERROR:Parameter name repeated in function definition\n";
      exit(34);
    }

    cur->entries[ent->name]=ent;

    cur->parameters[ent->name]=ent->type;


  }else if(cur->scope == 3){
    //class method
    //cout<<"class_method\n";
    int a = cur->para_ind;
    //cout<<"b"<<a<<endl;
    if(cur->para_ind == 0 ){
      //this should be self 
      //cout<<"a\n";
      if(root_sym->children.size()>1){
        //but it is not
        //error
        cout<<"line:"<<root_sym->lineno<<"ERROR:first arg should be self\n";
        exit(24);
      }else{
        cur->para_ind ++ ;
      }
      
      

    }else{

      cur->para_ind++;
      struct syment * ent = new syment();

      ent->name=atom->name;

      //check whether same variable name exists or not
      if(cur->entries[ent->name]!=NULL){
        //error
        cout<<"line:"<<root_sym->lineno<<"-ERROR:variable "<<ent->name<<" is already another parameter\n";
        exit(77);
      }
      struct node * colon = root_sym->children[1];
      //cout<<"col\n";
      if(colon->children[0]->name == "empty"/*no type declared*/){
        //error
        cout<<"line:"<<root_sym->lineno<<"-ERROR:variable "<<ent->name<<" does not have a data type declared\n";
        exit(77);
      }else{
        string type = colon->children[1]->name;
        if(type =="dtype"){
          ent->type = colon->children[1]->children[0]->name;
        }else{
          //check whether such a class exists
          if(classes.count(colon->children[1]->children[0]->name) == 1){
            ent->type = colon->children[1]->children[0]->name;
          }else{
            //error--no such class type exists
            cout<<"line:"<<root_sym->lineno<<"-ERROR:variable "<<ent->name<<" does not have a defined type\n";
            exit(77);
          }

        }

        ent->token = "parameter";
        ent->lineno = root_sym->lineno;
        cur->entries[ent->name]=ent;
      }

    }
  }
}


void decl (struct node * root_sym , struct symtab * cur ){
  if(root_sym->children[0]->name=="class_attr"){
    //it must be of the form self.xyz
    struct node * class_attr = root_sym->children[0];
    //first check that cur is a class scope
    if(cur->scope== 0 || cur->scope == 1 || cur->scope==2 || cur->scope==4 /*current scope is global or of a function*/){
      //error
      cout<<"line:"<<root_sym->lineno<<"ERROR:Wrong declaration\n";
      exit(42);
    }

    //check that name is self
    //cout<<"x\n";
    struct syment * ent = new syment();

    ent->name = class_attr->children[1]->children[1]->name;
    ent->attr = true;
    ent->lineno = root_sym->lineno;
    ent->token = "attribute";

    string type = root_sym->children[1]->children[1]->name;
    root_sym->type = type;
    if(type=="dtype"){
      ent->type = root_sym->children[1]->children[1]->children[0]->name;
    }else{
      //it is a class object
      //check if that class exists
      if(classes.count(root_sym->children[1]->children[1]->children[0]->name)==1){
        ent->type = root_sym->children[1]->children[1]->children[0]->name;
        ent->class_obj = true;
      }else{
        //error
        cout<<"line:"<<root_sym->lineno<<" ERROR:No such class exists\n";
        exit(37);
      }

    }
    //check if it is already declared
    if(cur->entries[ent->name]!=NULL && cur->entries[ent->name]->attr == true){
      //error
      cout<<"line:"<<root_sym->lineno<<" ERROR:attribute already declared\n";
      exit(41);
    }

    cur->entries[ent->name]=ent;
    cur->parent->attributes[ent->name]=ent->type;


  // }else if(root_sym->children[0]->name=="class_attr"){
  //   struct node * class_attr = root_sym->children[0];
  //   struct node * annass = root_sym->children[1];

  //   //check that current scope is class method

  }else{
    //simple declaration
    //cout<<"asasa\n";
    string name = root_sym->children[0]->name;
    if(cur->entries[name]!=NULL){
      //error---variable already declared in the scope
      
      cout<<"line:"<<root_sym->lineno<<" ERROR:variable "<<name<<" already declared in the scope"<<endl;
      exit(0);
    }

    //check if the type is a class ... if yes check that class exists or not
    string type_var = root_sym->children[1]->children[1]->name;
    

    if(type_var == "class"){
      if(classes.count(root_sym->children[1]->children[1]->children[0]->name)==0){
        //error
        // cout<<"here\n";
        cout<<"line:"<<root_sym->lineno<<" ERROR:The class type does not exist\n";

        exit(53);


      }
    }

    struct syment * temp = new syment();
    temp->name=name;
    temp->token = "NAME";
    temp->lineno=root_sym->lineno;
    temp->type=root_sym->children[1]->children[1]->children[0]->name;
    
    cur->entries[name]=temp;


  }
}

void class_def(struct node * root_sym, struct symtab * cur){
  //root_sym points to a node whose name is classdef
  //symtab is set

  cur->name = root_sym->children[1]->name;

  if(classes.count(cur->name)==1){
    //error
    cout<<"line:"<<root_sym->lineno<<" ERROR:class already defined"<<endl;
    exit(10);
  }

  int n_child = root_sym->children.size();
  if(n_child==4){
    //no inheritance
    //cout<<"b\n";
    classes.insert(cur->name);
    // cout<<"class inseted "<<cur->name<<endl;
    trav(root_sym->children[3],cur);
  }else if(n_child == 6){
    //no inheritance but brackets present
    classes.insert(cur->name);
    trav(root_sym->children[5],cur);
  }else if(n_child == 7){
    //inheritance

    classes.insert(cur->name);
    string inhclass = root_sym->children[3]->name;

    if(classes.count(inhclass)==0){
      //error---no such class to inherit from
      cout<<"line:"<<root_sym->lineno<<"-ERROR:no class named "<<inhclass<<" exists for inheritance\n";
      exit(90);
    }

    cur->inher_class = class_tab[inhclass];
    cur->attributes = cur->inher_class->attributes;
    trav(root_sym->children[6],cur);
    
  }else{
    //error
    cout<<"line:"<<root_sym->lineno<<" ERROR\n";
    exit(20);
  }


}

void funcdef(struct node * root_sym,struct symtab * cur){

  struct symtab * func = new symtab(); 
  functions.insert(func->name);
  func ->parent = cur;
  func -> name = root_sym->children[1]->name;
  func->para_ind = 0;
  if(cur->scope == 0){
    //normal function
    func->scope = 1;
    if(functions.count(func->name)==1){
      //error
      cout<<"line:"<<root_sym->lineno<<"ERROR:The function already exist\n";
      exit(22);
    }
    functions.insert(func->name);
    root_tab->children.push_back(func);
  }else if(cur->scope == 2){
    //class method
    func->scope = 3;
    //check that class method does not exist already
    for(int i=0;i<cur->children.size();i++){
      if(cur->children[i]->name==func->name){
        //error
        cout<<"line:"<<root_sym->lineno<<"-ERROR:Class method already defined"<<endl;
        exit(21);
      }
    }

  }else{
    //error
    cout<<"line:"<<root_sym->lineno<<" ERR:function defined in wrong scope"<<endl;
    exit(1000);
  }

  int j = root_sym->children.size();

  for(int i=0;i<j;i++){
    trav(root_sym->children[i],func);
  }

  cur->children.push_back(func);


}

void atom_com(struct node * root_sym , struct symtab * cur){
      
      //trailer_child -> empty
      //a normal variable should exist with that name
      int scope = cur->scope;
      struct node * atom = root_sym->children[0];
      
      if(root_sym->children.size()>1){
        atom_com(root_sym->children[2],cur);
      }

      if( atom->children[0]->name == "number" || atom->children[0]->name == "float" ){
        root_sym->type = atom->children[0]->name;
        return;
      }else if(atom->children[0]->name == "strings"){
        root_sym->type = "str";
        return;
      }

      struct syment * var = entry_find(atom->children[0]->name,cur);

      if(var==NULL){
        cout<<"line:"<<root_sym->lineno<<" ERROR: variable "<<atom->children[0]->name<<" not declared\n";
        exit(404);
      }else{
        root_sym->type = var->type;
      }

      return;
    
    

}



//funcdef
//classdef
//decl_stmt
//parameters---tfpdef
//

void trav(struct node * root_sym,struct symtab * cur){

  if(root_sym->name =="atom_expr"){

    atomex(root_sym,cur);
  }else if(root_sym->name == "atom_comma"){
    
    atom_com(root_sym,cur);

    if(root_sym->children.size()>1){
      atom_com(root_sym->children[2],cur);
    }
    
  }else if(root_sym->name == "decl_stmt"){
    //two cases
    //1-second child is attribute
    //2-second child is annassign
    // cout<<"Enter decl stmt"<<endl;
    decl(root_sym,cur);

  }else if(root_sym->name == "funcdef"){
    //check whether class method or normal function
    //cout<<"funcdef\n";

    funcdef(root_sym,cur);

    return ;
  }else if(root_sym->name == "arrow_test"){
    if(root_sym->children.size()==2){
      cur->return_type =root_sym->children[0]->name; 
    }
  }else if(root_sym->name == "tfpdef"){
    tfp(root_sym,cur);
  
  }else if(root_sym->name =="classdef"){
    //need to create a new scope-> new symtab
    //cout<<"a"<<endl;
    struct symtab * newtab = new symtab();

    newtab->parent=cur;
    newtab->scope = 2;//class_scope
    cur->children.push_back(newtab);
    class_def(root_sym,newtab);
    class_tab[newtab->name]=newtab;
    
  }else if(root_sym->name == "if_stmt"){
    struct symtab * if_tab = new symtab();
    int cur_scope = cur->scope;

    if(cur_scope == 2){
      //error
      //if stmt in class
      cout<<"line:"<<root_sym->lineno<<"-ERROR:if block in class scope\n";
      exit(23);
    }

    if_tab->name="if_block";
    if_tab->scope = 4;
    if_tab ->parent = cur;
    // cout<<"d\n";
    for(int i=0;i<root_sym->children.size();i++){
      // cout<<"if "<<i<<endl;
      trav(root_sym->children[i],if_tab);
    }
    cur->children.push_back(if_tab);
    // cout<<"a\n";
  
  }else if(root_sym->name == "elif_block"){
    if(root_sym->children[0]->name=="empty"){
      return;
    }
    struct symtab * elif_tab = new symtab();
    elif_tab->name = "elif_block";
    elif_tab->parent = cur->parent;
    cur->parent->children.push_back(elif_tab);
    elif_tab->scope = 4;

    for(int i=0;i<root_sym->children.size();i++){
      trav(root_sym->children[i],elif_tab);
    }

    // cout<<"b\n";

  }else if(root_sym->name == "else_block"){
    if(root_sym->children[0]->name=="empty"){
      return;
    }
    struct symtab * elif_tab = new symtab();
    elif_tab->name = "else_block";
    elif_tab->parent = cur->parent;
    cur->parent->children.push_back(elif_tab);
    elif_tab->scope = 4;

    for(int i=0;i<root_sym->children.size();i++){
      trav(root_sym->children[i],elif_tab);
    }

    // cout<<"c\n";
  }else {
    // cout<<"enter_useless_node:"<<root_sym->name<<endl;
    for(int i=0;i<root_sym->children.size();i++){
      // cout<<"---"<<i<<endl;
      trav(root_sym->children[i],cur);
    }
  }
}

////////////////////////////////////////

string check_type(struct node * root_sym){

  if(root_sym->name == "empty"){
    return "empty";
  }

  if(root_sym->name=="atom_expr"){
    return root_sym->type;
  }
  
  if(root_sym->name=="term"){
    string type = check_type(root_sym->children[0]);
    int a = type_comp[type];
    int b = type_comp[check_type(root_sym->children[1])];
    
    if(b==1 || a==b){
      return type;
    }else{
      cout<<"line:"<<root_sym->lineno<<" ERROR: Type error-Incompatible operation\n";
      exit(404);
    }

  }

  if(root_sym->name == "op_term"){
    if(root_sym->children.size()==1 ){
      string type = check_type(root_sym->children[0]);
      return type;
    }else{
      string type1 = check_type(root_sym->children[0]);
      string type2 = check_type(root_sym->children[2]);

      if(type_comp[type1]==1){
        return type2;
      }else{
        int a=type_comp[type1];
        int b = type_comp[type2];
        if((a==3||b==3)||(a!=b)){
          cout<<"line:"<<root_sym->lineno<<" ERROR: Type error-Incompatible operation\n";
          exit(404);
        }else{
          return type1;
        }
      }
      
      
      // return type;
    }


  }

  if(root_sym->name == "arith_expr"){
    string type1 = check_type(root_sym->children[0]);
    string type2 = check_type(root_sym->children[1]);

    int a=type_comp[type1];
    int b = type_comp[type2];
    // cout<<root_sym->lineno<<" "<<a<<" "<<b<<endl;
    if(b==1){
      return type1;
    }else{
      if(a!=b || a==3 || b==3){
        cout<<"line:"<<root_sym->lineno<<" ERROR: Type error-Incompatible operation\n";
          exit(404);
      }
    }


  }

  if(root_sym->name == "and_test"){
    string a = check_type(root_sym->children[0]);
    string b = check_type(root_sym->children[1]);

    if(b=="empty"){
      return a;
    }

    if(b=="bool" && a=="bool"){
      return "bool";
    }
    else{
      cout<<"ERROR: and test incompatible with operands\n";
      exit(23);
    }

  }

  if(root_sym->name == "op_testlist_star_expr"){
    return check_type(root_sym->children[1]);
  }

  
  if(root_sym->children.size()>0){
    vector<string> res(root_sym->children.size());
    for(int i=0;i<root_sym->children.size();i++){
      res[i]=check_type(root_sym->children[i]);
    }

    return res[0];
  }

  return "empty";
}



////////////////////////////////////////





#endif