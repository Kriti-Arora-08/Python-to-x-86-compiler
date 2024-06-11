#ifndef TAC_CPP
#define TAC_CPP
#include <bits/stdc++.h>

#include "./node.hpp"
#include "./symtab.hpp"
#include "./tac.hpp"
using namespace std;

ofstream ftacout("./out/tac.txt", std::ios::out | std::ios::trunc);
vector<raoa *> tac_ins;
unsigned long long int bnos = 0;
map<string, string> stirs;
unsigned long long int getB()
{
  bnos++;
  return bnos;
}

int status = 0;
int flagTAC = 1;
string getReg()
{
  status++;
  return "t" + to_string(status);
}

void assgnB(node *root)
{
  for (auto i : root->children)
  {
    assgnB(i);
  }

  if (root->name == "suite")
  {
    root->blck = getB();
  }
  else if (root->name == "else_block" && root->children.size() == 1)
  {
    root->blck = getB();
  }
  else if (root->name == "else_block" && root->children.size() > 1)
  {
    root->blck = root->children[2]->blck;
  }
  else if (root->name == "funcdef")
  {
    root->blck = getB();
  }
  else if (root->name == "if_stmt")
  {
    root->blck = getB();
  }
  else if (root->name == "elif_block" && root->children.size() == 1)
  {
    root->blck = root->children[0]->blck;
  }
  else if (root->name == "elif_block" && root->children.size() > 1)
  {
    root->blck = getB();
  }
  else if (root->name == "while_stmt")
  {
    root->blck = getB();
  }
  else if (root->name == "for_stmt")
  {
    root->blck = getB();
  }
  else if (root->name == "classdef")
  {
    root->blck = getB();
  }

  return;
}

int stats = 0;
string tac_temp = "";
string shift_temp = "";
string arith_temp = "";
string factor_temp = "";

void makeTAC(node *root)
{

  // raoa *temp = new raoa();
  // tac_ins.push_back(temp);
  // ftacout << temp->block << endl;
  // for (auto i : root->children)
  // {
  //   makeTAC(i);
  // }

  root->val = root->name;

  // if (root->name == "funcdef")
  // {
  //     makeTAC(root->children[0]);
  //     makeTAC(root->children[1]);
  //     stats++;
  //     ftacout << "start block " << stats << "@" << root->children[1]->val << endl;
  //     makeTAC(root->children[2]);
  //     makeTAC(root->children[3]);
  //     makeTAC(root->children[4]);
  //     makeTAC(root->children[5]);
  //     ftacout << "end block " << stats << "@" << root->children[1]->val << endl;
  // }
  // else if (root->name == "if_stmt")
  // {
  //     makeTAC(root->children[0]);
  //     makeTAC(root->children[1]);

  //     makeTAC(root->children[2]);
  //     stats++;
  //     ftacout << "if " << root->children[1]->val << " Skip next" << endl;
  //     ftacout << "goto label " << stats << endl;
  //     makeTAC(root->children[3]);
  //     ftacout << "Label" << stats << endl;
  //     makeTAC(root->children[4]);
  //     makeTAC(root->children[5]);
  // }
  // else if (root->name == "elif_block" && root->children.size() > 1)
  // {
  //     makeTAC(root->children[0]);
  //     makeTAC(root->children[1]);
  //     makeTAC(root->children[2]);
  //     makeTAC(root->children[3]);
  //     stats++;
  //     ftacout << "if " << root->children[1]->val << " Skip next" << endl;
  //     ftacout << "goto label " << stats << endl;

  //     makeTAC(root->children[4]);
  //     ftacout << "Label" << stats << endl;
  // }
  // /*

  // */
  // else if (root->name == "else_block" && root->children.size() > 1)
  // {
  //     makeTAC(root->children[0]);
  //     makeTAC(root->children[1]);
  //     stats++;
  //     ftacout << "if " << root->children[1]->val << " Skip next" << endl;
  //     ftacout << "goto Label " << stats << endl;
  //     makeTAC(root->children[2]);
  //     ftacout << "Label " << stats << endl;
  // }
  // else if (root->name == "while_stmt")
  // {
  //     makeTAC(root->children[0]);
  //     stats++;
  //     int ostat = stats;
  //     ftacout << "Label " << stats << endl;
  //     makeTAC(root->children[1]);
  //     makeTAC(root->children[2]);
  //     int nstats = ++stats;
  //     ftacout << "if " << root->children[1]->val << " Skip next" << endl;
  //     ftacout << "goto Label " << nstats << endl;
  //     stats++;
  //     makeTAC(root->children[3]);
  //     ftacout << "goto Label " << ostat << endl;
  //     ftacout << "Label" << nstats << endl;
  //     makeTAC(root->children[4]);
  // }
  if (flagTAC == 1)
  {
    if (root->name == "if_stmt")
    {
      makeTAC(root->children[0]);
      makeTAC(root->children[1]);
      makeTAC(root->children[2]);
      raoa *temp2 = new raoa();
      temp2->result = "";
      temp2->arg1 = root->children[1]->val;
      temp2->arg2 = ".L" + to_string(root->children[4]->blck);
      temp2->op = "JIF";
      tac_ins.push_back(temp2);

      // raoa *temp = new raoa();
      // temp->result = "LABEL";
      // temp->arg1 = "";
      // temp->arg2 = to_string(root->children[3]->blck);
      // temp->op = ".";
      // tac_ins.push_back(temp);
      makeTAC(root->children[3]);

      raoa *temp4 = new raoa();
      temp4->result = "";
      temp4->arg1 = "";
      temp4->arg2 = ".E" + to_string(root->blck);
      temp4->op = "BRANCH";
      tac_ins.push_back(temp4);

      makeTAC(root->children[4]);

      raoa *temp3 = new raoa();
      temp3->result = "";
      temp3->arg1 = "";
      temp3->arg2 = ".E" + to_string(root->blck);
      temp3->op = "LABEL";
      tac_ins.push_back(temp3);
    }
    else if (root->name == "elif_block" && root->children.size() > 1)
    {
      raoa *temp = new raoa();
      temp->result = "";
      temp->arg1 = "";
      temp->arg2 = ".L" + to_string(root->blck);
      temp->op = "LABEL";
      tac_ins.push_back(temp);

      makeTAC(root->children[0]);
      makeTAC(root->children[1]);
      makeTAC(root->children[2]);
      raoa *temp2 = new raoa();
      temp2->result = "";
      temp2->arg1 = root->children[1]->val;
      temp2->arg2 = ".L" + to_string(root->children[4]->blck);
      temp2->op = "JIF";
      tac_ins.push_back(temp2);

      // raoa *temp = new raoa();
      // temp->result = "LABEL";
      // temp->arg1 = "";
      // temp->arg2 = to_string(root->children[3]->blck);
      // temp->op = ".";
      // tac_ins.push_back(temp);
      makeTAC(root->children[3]);

      raoa *temp4 = new raoa();
      temp4->result = "";
      temp4->arg1 = "";
      temp4->arg2 = ".E" + to_string(root->blck);
      temp4->op = "BRANCH";
      tac_ins.push_back(temp4);

      makeTAC(root->children[4]);

      raoa *temp3 = new raoa();
      temp3->result = "";
      temp3->arg1 = "";
      temp3->arg2 = ".E" + to_string(root->blck);
      temp3->op = "LABEL";
      tac_ins.push_back(temp3);
    }
    else if (root->name == "else_block" && root->children.size() > 1)
    {
      raoa *temp = new raoa();
      temp->result = "";
      temp->arg1 = "";
      temp->arg2 = ".L" + to_string(root->blck);
      temp->op = "LABEL";
      tac_ins.push_back(temp);

      makeTAC(root->children[0]);
      makeTAC(root->children[1]);
      makeTAC(root->children[2]);
    }
    else if (root->name == "else_block" && root->children.size() == 1)
    {
      raoa *temp = new raoa();
      temp->result = "";
      temp->arg1 = "";
      temp->arg2 = ".L" + to_string(root->blck);
      temp->op = "LABEL";
      tac_ins.push_back(temp);
    }
    else if (root->name == "for_stmt")
    {
      // raoa *temp = new raoa();
      // temp->result = "LABEL";
      // temp->arg1 = "";
      // temp->arg2 = to_string(root->blck);
      // temp->op = ".";
      // tac_ins.push_back(temp);
      makeTAC(root->children[0]);
      makeTAC(root->children[1]);
      makeTAC(root->children[2]);
      root->children[3]->blck = root->blck;
      makeTAC(root->children[3]);
      makeTAC(root->children[4]);
      makeTAC(root->children[5]);

      raoa *temp2 = new raoa();
      temp2->result = getReg();
      temp2->arg1 = "";
      temp2->arg2 = "1";
      temp2->op = "ASSIGN_INT";
      tac_ins.push_back(temp2);

      raoa *temp1 = new raoa();
      temp1->result = root->children[1]->val;
      temp1->arg1 = root->children[1]->val;
      temp1->arg2 = temp2->result;
      temp1->op = "+";
      tac_ins.push_back(temp1);

      raoa *temp4 = new raoa();
      temp4->result = "";
      temp4->arg1 = "";
      temp4->arg2 = ".L" + to_string(root->blck);
      temp4->op = "BRANCH";
      tac_ins.push_back(temp4);

      raoa *temp = new raoa();
      temp->result = "";
      temp->arg1 = "";
      temp->arg2 = ".E" + to_string(root->blck);
      temp->op = "LABEL";
      tac_ins.push_back(temp);
    }
    else if (root->name == "while_stmt")
    {
      // raoa *temp = new raoa();
      // temp->result = "LABEL";
      // temp->arg1 = "";
      // temp->arg2 = to_string(root->blck);
      // temp->op = ".";
      // tac_ins.push_back(temp);
      makeTAC(root->children[0]);
      raoa *temp = new raoa();
      temp->result = "";
      temp->arg1 = "";
      temp->arg2 = ".L" + to_string(root->blck);
      temp->op = "LABEL";
      tac_ins.push_back(temp);
      makeTAC(root->children[1]);
      raoa *temp1 = new raoa();
      temp1->result = "";
      temp1->arg1 = root->children[1]->val;
      temp1->arg2 = ".E" + to_string(root->blck);
      temp1->op = "JIF";
      tac_ins.push_back(temp1);
      makeTAC(root->children[2]);

      makeTAC(root->children[3]);

      raoa *temp4 = new raoa();
      temp4->result = "";
      temp4->arg1 = "";
      temp4->arg2 = ".L" + to_string(root->blck);
      temp4->op = "BRANCH";
      tac_ins.push_back(temp4);

      raoa *temp3 = new raoa();
      temp3->result = "";
      temp3->arg1 = "";
      temp3->arg2 = ".E" + to_string(root->blck);
      temp3->op = "LABEL";
      tac_ins.push_back(temp3);
    }
    else if (root->name == "funcdef")
    {

      makeTAC(root->children[0]);
      makeTAC(root->children[1]);

      raoa *temp3 = new raoa();
      temp3->result = "";
      temp3->arg1 = "";
      temp3->arg2 = root->children[1]->val;
      temp3->op = "begin_func";
      tac_ins.push_back(temp3);

      raoa *temp = new raoa();
      temp->result = "";
      temp->arg1 = "";
      temp->arg2 = ".L" + to_string(root->blck);
      temp->op = "LABEL";
      tac_ins.push_back(temp);
      makeTAC(root->children[2]);
      makeTAC(root->children[3]);
      makeTAC(root->children[4]);
      makeTAC(root->children[5]);

      raoa *temp0 = new raoa();
      temp0->result = "";
      temp0->arg1 = "";
      temp0->arg2 = root->children[1]->val;
      temp0->op = "end_func";
      tac_ins.push_back(temp0);
    }
    else if (root->name == "classdef" && root->children.size() == 4)
    {

      makeTAC(root->children[0]);
      makeTAC(root->children[1]);

      raoa *temp3 = new raoa();
      temp3->result = "";
      temp3->arg1 = "";
      temp3->arg2 = root->children[1]->val;
      temp3->op = "begin_class";
      tac_ins.push_back(temp3);

      raoa *temp = new raoa();
      temp->result = "";
      temp->arg1 = "";
      temp->arg2 = ".L" + to_string(root->blck);
      temp->op = "LABEL";
      tac_ins.push_back(temp);
      makeTAC(root->children[2]);
      makeTAC(root->children[3]);

      raoa *temp0 = new raoa();
      temp0->result = "";
      temp0->arg1 = "";
      temp0->arg2 = root->children[1]->val;
      temp0->op = "end_class";
      tac_ins.push_back(temp0);
    }
    else if (root->name == "classdef" && root->children.size() == 6)
    {

      makeTAC(root->children[0]);
      makeTAC(root->children[1]);

      raoa *temp3 = new raoa();
      temp3->result = "";
      temp3->arg1 = "";
      temp3->arg2 = root->children[1]->val;
      temp3->op = "begin_class";
      tac_ins.push_back(temp3);

      raoa *temp = new raoa();
      temp->result = "";
      temp->arg1 = "";
      temp->arg2 = ".L" + to_string(root->blck);
      temp->op = "LABEL";
      tac_ins.push_back(temp);
      makeTAC(root->children[2]);
      makeTAC(root->children[3]);
      makeTAC(root->children[4]);
      makeTAC(root->children[5]);

      raoa *temp0 = new raoa();
      temp0->result = "";
      temp0->arg1 = "";
      temp0->arg2 = root->children[1]->val;
      temp0->op = "end_class";
      tac_ins.push_back(temp0);
    }
    else if (root->name == "classdef" && root->children.size() == 7)
    {

      makeTAC(root->children[0]);
      makeTAC(root->children[1]);

      raoa *temp3 = new raoa();
      temp3->result = "";
      temp3->arg1 = "";
      temp3->arg2 = root->children[1]->val;
      temp3->op = "begin_class";
      tac_ins.push_back(temp3);

      raoa *temp = new raoa();
      temp->result = "";
      temp->arg1 = "";
      temp->arg2 = ".L" + to_string(root->blck);
      temp->op = "LABEL";
      tac_ins.push_back(temp);
      makeTAC(root->children[2]);
      makeTAC(root->children[3]);
      makeTAC(root->children[4]);
      makeTAC(root->children[5]);
      raoa *temp5 = new raoa();
      temp5->result = "";
      temp5->arg1 = "";
      temp5->arg2 = root->children[3]->val;
      temp5->op = "inheritance";
      tac_ins.push_back(temp5);
      makeTAC(root->children[6]);

      raoa *temp0 = new raoa();
      temp0->result = "";
      temp0->arg1 = "";
      temp0->arg2 = root->children[1]->val;
      temp0->op = "end_class";
      tac_ins.push_back(temp0);
    }
    else if (root->name == "func_call")
    {
      if (root->children.size() == 2)
      {
        root->val = getReg();
        raoa *temp = new raoa();
        temp->result = root->val;
        temp->arg1 = "";
        temp->arg2 = root->parent->parent->children[0]->val;
        temp->op = "call_func";
        tac_ins.push_back(temp);
      }
      else
      {
        flagTAC = 1;
        makeTAC(root->children[1]);
        flagTAC = 2;
        makeTAC(root->children[1]);
        root->val = getReg();
        raoa *temp = new raoa();
        temp->result = root->val;
        temp->arg1 = "";
        temp->arg2 = root->parent->parent->children[0]->val;
        temp->op = "call_func";
        tac_ins.push_back(temp);
        flagTAC = 3;
        makeTAC(root->children[1]);
        flagTAC = 1;
        // ftacout<<"REGGGI"<<root->val;
      }
    }
    else
    {
      for (auto i : root->children)
      {
        makeTAC(i);
      }
    }
  }
  else if (root->name == "arglist" || root->name == "c_argument")
  {
    for (auto i : root->children)
    {
      makeTAC(i);
    }
  }
  if (root->name == "or_test")
  {
    if (root->children.size() == 1)
      root->val = root->children[0]->val;
    else
    {
      root->val = getReg();
      raoa *temp = new raoa();
      temp->result = root->val;
      temp->arg1 = root->children[0]->val;
      temp->arg2 = root->children[2]->val;
      temp->op = "or";
      tac_ins.push_back(temp);
    }
  }
  else if (root->name == "argument" && flagTAC == 1)
  {
    root->val = root->children[0]->val;
  }
  // else if (root->name == "trailer")
  // {
  //   root->val = root->children[0]->val;
  // }
  else if (root->name == "argument" && flagTAC == 2)
  {
    root->val = root->children[0]->val;
    raoa *temp = new raoa();
    temp->result = "";
    temp->arg1 = "";
    temp->arg2 = root->val;
    temp->op = "PUSHPARAM";
    tac_ins.push_back(temp);
  }
  else if (root->name == "argument" && flagTAC == 3)
  {
    root->val = root->children[0]->val;
    raoa *temp = new raoa();
    temp->result = "";
    temp->arg1 = "";
    temp->arg2 = root->val;
    temp->op = "POPPARAM";
    tac_ins.push_back(temp);
  }
  else if (root->name == "expr_stmt")
  {
    if (root->children.size() == 1)
    {
      root->val = root->children[0]->val;
    }
    else
    {
      string ops = root->children[1]->children[0]->name;
      if (ops == "+=")
      {
        root->val = root->children[0]->val;
        raoa *temp = new raoa();
        temp->result = root->children[0]->val;
        temp->arg1 = root->children[0]->val;
        temp->arg2 = root->children[2]->val;
        temp->op = "+";
        tac_ins.push_back(temp);
      }
      else if (ops == "-=")
      {
        root->val = root->children[0]->val;
        raoa *temp = new raoa();
        temp->result = root->children[0]->val;
        temp->arg1 = root->children[0]->val;
        temp->arg2 = root->children[2]->val;
        temp->op = "-";
        tac_ins.push_back(temp);
      }
      else if (ops == "*=")
      {
        root->val = root->children[0]->val;
        raoa *temp = new raoa();
        temp->result = root->children[0]->val;
        temp->arg1 = root->children[0]->val;
        temp->arg2 = root->children[2]->val;
        temp->op = "*";
        tac_ins.push_back(temp);
      }
      else if (ops == "/=")
      {
        root->val = root->children[0]->val;
        raoa *temp = new raoa();
        temp->result = root->children[0]->val;
        temp->arg1 = root->children[0]->val;
        temp->arg2 = root->children[2]->val;
        temp->op = "/";
        tac_ins.push_back(temp);
      }
      else if (ops == "%=")
      {
        root->val = root->children[0]->val;
        raoa *temp = new raoa();
        temp->result = root->children[0]->val;
        temp->arg1 = root->children[0]->val;
        temp->arg2 = root->children[2]->val;
        temp->op = "%";
        tac_ins.push_back(temp);
      }
      else if (ops == "&=")
      {
        root->val = root->children[0]->val;
        raoa *temp = new raoa();
        temp->result = root->children[0]->val;
        temp->arg1 = root->children[0]->val;
        temp->arg2 = root->children[2]->val;
        temp->op = "&";
        tac_ins.push_back(temp);
      }
      else if (ops == "|=")
      {
        root->val = root->children[0]->val;
        raoa *temp = new raoa();
        temp->result = root->children[0]->val;
        temp->arg1 = root->children[0]->val;
        temp->arg2 = root->children[2]->val;
        temp->op = "|";
        tac_ins.push_back(temp);
      }
      else if (ops == "^=")
      {
        root->val = root->children[0]->val;
        raoa *temp = new raoa();
        temp->result = root->children[0]->val;
        temp->arg1 = root->children[0]->val;
        temp->arg2 = root->children[2]->val;
        temp->op = "^";
        tac_ins.push_back(temp);
      }
      else if (ops == "<<=")
      {
        root->val = root->children[0]->val;
        raoa *temp = new raoa();
        temp->result = root->children[0]->val;
        temp->arg1 = root->children[0]->val;
        temp->arg2 = root->children[2]->val;
        temp->op = "<<";
        tac_ins.push_back(temp);
      }
      else if (ops == ">>=")
      {
        root->val = root->children[0]->val;
        raoa *temp = new raoa();
        temp->result = root->children[0]->val;
        temp->arg1 = root->children[0]->val;
        temp->arg2 = root->children[2]->val;
        temp->op = ">>";
        tac_ins.push_back(temp);
      }
      else if (ops == "**=")
      {
        root->val = root->children[0]->val;
        raoa *temp = new raoa();
        temp->result = root->children[0]->val;
        temp->arg1 = root->children[0]->val;
        temp->arg2 = root->children[2]->val;
        temp->op = "**";
        tac_ins.push_back(temp);
      }
      else if (ops == "//=")
      {
        root->val = root->children[0]->val;
        raoa *temp = new raoa();
        temp->result = root->children[0]->val;
        temp->arg1 = root->children[0]->val;
        temp->arg2 = root->children[2]->val;
        temp->op = "//";
        tac_ins.push_back(temp);
      }
      else
      {
        root->val = root->children[0]->val;
      }
    }
  }
  else if (root->name == "and_test")
  {
    if (root->children.size() == 1)
      root->val = root->children[0]->val;
    else
    {
      root->val = getReg();
      raoa *temp = new raoa();
      temp->result = root->val;
      temp->arg1 = root->children[0]->val;
      temp->arg2 = root->children[2]->val;
      temp->op = "and";
      tac_ins.push_back(temp);
    }
  }

  else if (root->name == "not_test")
  {
    if (root->children.size() == 1)
      root->val = root->children[0]->val;
    else
    {
      root->val = getReg();
      raoa *temp = new raoa();
      temp->result = root->val;
      temp->arg1 = "";
      temp->arg2 = root->children[1]->val;
      temp->op = "not";
      tac_ins.push_back(temp);
    }
  }
  else if (root->name == "strings")
  {
    if (root->children.size() == 1)
    {
      root->val = "s" + getReg();
      raoa *temp = new raoa();
      temp->result = root->val;
      temp->arg1 = "";
      temp->arg2 = root->children[0]->val;
      temp->op = "STRING";
      temp->sting = true;
      tac_ins.insert(tac_ins.begin(), temp);
      stirs.insert({root->val, root->val});

      root->val = getReg();
      raoa *temp1 = new raoa();
      temp1->result = root->val;
      temp1->arg1 = "";
      temp1->arg2 = temp->result;
      temp1->op = "ASSIGN_TEMP";
      temp1->sting = true;
      tac_ins.push_back(temp1);

      stirs.insert({root->val, temp->result});
    }
  }
  else if (root->name == "comparison")
  {
    if (root->children.size() == 1)
      root->val = root->children[0]->val;
    else
    {
      root->val = getReg();
      raoa *temp = new raoa();
      temp->result = root->val;
      temp->arg1 = root->children[0]->val;
      temp->arg2 = root->children[2]->val;
      temp->op = root->children[1]->val;
      tac_ins.push_back(temp);
    }
  }
  else if (root->name == "comp_op")
  {
    int comp_flag = 1;
    for (auto k : root->children)
    {
      if (comp_flag)
      {
        root->val = k->name;
        comp_flag = 0;
      }
      else
      {
        root->val += " ";
        root->val += k->name;
      }
    }
  }
  else if (root->name == "star_expr")
  {
    root->val = getReg();
    raoa *temp = new raoa();
    temp->result = root->val;
    temp->arg1 = "";
    temp->arg2 = root->children[1]->val;
    temp->op = "pointer to";
    tac_ins.push_back(temp);
  }

  else if (root->name == "expr")
  {
    if (root->children.size() == 1)
      root->val = root->children[0]->val;
    else
    {
      root->val = getReg();
      raoa *temp = new raoa();
      temp->result = root->val;
      temp->arg1 = root->children[0]->val;
      temp->arg2 = root->children[2]->val;
      temp->op = "|";
      tac_ins.push_back(temp);
    }
  }
  else if (root->name == "xor_expr")
  {
    if (root->children.size() == 1)
      root->val = root->children[0]->val;
    else
    {
      root->val = getReg();
      raoa *temp = new raoa();
      temp->result = root->val;
      temp->arg1 = root->children[0]->val;
      temp->arg2 = root->children[2]->val;
      temp->op = "xor";
      tac_ins.push_back(temp);
    }
  }
  else if (root->name == "and_expr")
  {
    if (root->children.size() == 1)
      root->val = root->children[0]->val;
    else
    {
      root->val = getReg();
      raoa *temp = new raoa();
      temp->result = root->val;
      temp->arg1 = root->children[0]->val;
      temp->arg2 = root->children[2]->val;
      temp->op = "&";
      tac_ins.push_back(temp);
    }
  }
  else if (root->name == "shift_expr")
  {
    if (root->children.size() == 1)
      root->val = root->children[0]->val;
    else
    {
      if (root->children[1]->name == "<<")
      {
        root->val = getReg();
        raoa *temp = new raoa();
        temp->result = root->val;
        temp->arg1 = root->children[0]->val;
        temp->arg2 = root->children[2]->val;
        temp->op = "<<";
        tac_ins.push_back(temp);
      }
      else if (root->children[1]->name == ">>")
      {
        root->val = getReg();
        raoa *temp = new raoa();
        temp->result = root->val;
        temp->arg1 = root->children[0]->val;
        temp->arg2 = root->children[2]->val;
        temp->op = ">>";
        tac_ins.push_back(temp);
      }
    }
  }
  else if (root->name == "arith_expr")
  {
    if (root->children.size() == 1)
      root->val = root->children[0]->val;
    else
    {
      root->val = getReg();
      raoa *temp = new raoa();
      temp->result = root->val;
      temp->arg1 = root->children[0]->val;
      temp->arg2 = root->children[2]->val;
      temp->op = "+";
      tac_ins.push_back(temp);
    }
  }
  else if (root->name == "sub_term")
  {
    if (root->children.size() == 1)
      root->val = root->children[0]->val;
    else
    {
      root->val = getReg();
      raoa *temp = new raoa();
      temp->result = root->val;
      temp->arg1 = root->children[0]->val;
      temp->arg2 = root->children[2]->val;
      temp->op = "-";
      tac_ins.push_back(temp);
    }
  }
  else if (root->name == "term")
  {
    if (root->children.size() == 1)
      root->val = root->children[0]->val;
    else
    {
      root->val = getReg();
      raoa *temp = new raoa();
      temp->result = root->val;
      temp->arg1 = root->children[0]->val;
      temp->arg2 = root->children[2]->val;
      temp->op = "*";
      tac_ins.push_back(temp);
    }
  }
  else if (root->name == "div_term")
  {
    if (root->children.size() == 1)
      root->val = root->children[0]->val;
    else
    {
      if (root->children[1]->name == "%%")
      {
        root->val = getReg();
        raoa *temp = new raoa();
        temp->result = root->val;
        temp->arg1 = root->children[0]->val;
        temp->arg2 = root->children[2]->val;
        temp->op = "%%";
        tac_ins.push_back(temp);
      }
      else if (root->children[1]->name == "//")
      {
        root->val = getReg();
        raoa *temp = new raoa();
        temp->result = root->val;
        temp->arg1 = root->children[0]->val;
        temp->arg2 = root->children[2]->val;
        temp->op = "//";
        tac_ins.push_back(temp);
      }
      else if (root->children[1]->name == "/")
      {
        root->val = getReg();
        raoa *temp = new raoa();
        temp->result = root->val;
        temp->arg1 = root->children[0]->val;
        temp->arg2 = root->children[2]->val;
        temp->op = "/";
        tac_ins.push_back(temp);
      }
    }
  }

  else if (root->name == "factor")
  {
    if (root->children.size() == 1)
      root->val = root->children[0]->val;
    else
    {
      if (root->children[0]->name == "+")
      {
        root->val = getReg();
        raoa *temp = new raoa();
        temp->result = root->val;
        temp->arg1 = "";
        temp->arg2 = root->children[1]->val;
        temp->op = "+";
        tac_ins.push_back(temp);
      }
      else if (root->children[0]->name == "-")
      {
        root->val = getReg();
        raoa *temp = new raoa();
        temp->result = root->val;
        temp->arg1 = "";
        temp->arg2 = root->children[1]->val;
        temp->op = "-";
        tac_ins.push_back(temp);
      }
      else if (root->children[0]->name == "~")
      {
        root->val = getReg();
        raoa *temp = new raoa();
        temp->result = root->val;
        temp->arg1 = "";
        temp->arg2 = root->children[1]->val;
        temp->op = "~";
        tac_ins.push_back(temp);
      }
    }
  }
  else if (root->name == "power")
  {
    if (root->children.size() == 1)
      root->val = root->children[0]->val;
    else
    {
      root->val = getReg();
      raoa *temp = new raoa();
      temp->result = root->val;
      temp->arg1 = root->children[0]->val;
      temp->arg2 = root->children[2]->val;
      temp->op = "**";
      tac_ins.push_back(temp);
    }
  }
  else if (root->name == "range_func")
  {
    // ftacout << " ROOT " << root->children.size() << endl;
    if (root->children.size() == 2)
    {
      raoa *temp = new raoa();
      temp->result = root->parent->children[1]->val;
      temp->arg1 = "";
      temp->arg2 = "0";
      temp->op = "ASSIGN_INT";
      tac_ins.push_back(temp);

      raoa *temp1 = new raoa();
      temp1->result = "";
      temp1->arg1 = "";
      temp1->arg2 = ".L" + to_string(root->blck);
      temp1->op = "LABEL";
      tac_ins.push_back(temp1);

      root->val = getReg();
      raoa *temp2 = new raoa();
      temp2->result = root->val;
      temp2->arg1 = root->parent->children[1]->val;
      temp2->arg2 = root->children[1]->val;
      temp2->op = "<";
      tac_ins.push_back(temp2);

      raoa *temp3 = new raoa();
      temp3->result = "";
      temp3->arg1 = root->val;
      temp3->arg2 = ".E" + to_string(root->blck);
      temp3->op = "JIF";
      tac_ins.push_back(temp3);
    }
    else if (root->children.size() == 3)
    {
      raoa *temp = new raoa();
      temp->result = root->parent->children[1]->val;
      temp->arg1 = "";
      temp->arg2 = root->children[1]->val;
      temp->op = "ASSIGN_TEMP";
      tac_ins.push_back(temp);

      raoa *temp1 = new raoa();
      temp1->result = "";
      temp1->arg1 = "";
      temp1->arg2 = ".L" + to_string(root->blck);
      temp1->op = "LABEL";
      tac_ins.push_back(temp1);

      root->val = getReg();
      raoa *temp2 = new raoa();
      temp2->result = root->val;
      temp2->arg1 = root->parent->children[1]->val;
      temp2->arg2 = root->children[2]->val;
      temp2->op = "<";
      tac_ins.push_back(temp2);

      raoa *temp3 = new raoa();
      temp3->result = "";
      temp3->arg1 = temp2->result;
      temp3->arg2 = ".E" + to_string(root->blck);
      temp3->op = "JIF";
      tac_ins.push_back(temp3);
    }
  }
  else if (root->name == "number")
  {
    root->val = getReg();
    raoa *temp = new raoa();
    temp->result = root->val;
    temp->arg1 = "";
    temp->arg2 = root->children[0]->val;
    temp->op = "ASSIGN_INT";
    tac_ins.push_back(temp);
  }
  else if (root->name == "return_stmt")
  {
    root->val = getReg();
    raoa *temp = new raoa();
    temp->result = root->val;
    temp->arg1 = "";
    temp->arg2 = root->children.size() == 1 ? "" : root->children[1]->val;
    temp->op = "RETURN";
    tac_ins.push_back(temp);
  }
  // else if (root->name == "test" && root->parent && (root->parent->name == "if_stmt" || root->parent->name == "elif_block"))
  // {
  //   raoa *temp = new raoa();
  //   temp->result = root->children[0]->val;
  //   temp->arg1 = "";
  //   temp->arg2 = "";
  //   temp->op = "";
  //   tac_ins.push_back(temp);
  // }
  else if (root->name == "op_testlist_star_expr")
  {
    root->val = getReg();
    raoa *temp2 = new raoa();
    temp2->result = root->val;
    temp2->arg1 = "";
    temp2->arg2 = root->children[1]->val;
    temp2->op = "ASSIGN_TEMP";
    tac_ins.push_back(temp2);
  }
  else if (root->name == "atom")
  {
    if (root->children.size() == 1)
      root->val = root->children[0]->val;
    else if (root->children.size() == 2)
      root->val = "";
    else
      root->val = root->children[1]->val;
  }
  else if (root->name == "atom_expr")
  {
    if (root->children[1]->children[0]->name == "func_call" || root->children[1]->children[0]->name == "method_call")
    {
      root->val = root->children[1]->children[0]->val;
      // ftacout<<root->val<<"ROOT";
    }
    else
      root->val = root->children[0]->val;
  }
  else if (root->name == "func_call")
  {
    // ftacout<<"ok"<<root->val<<"ok"<<endl;
  }
  else if (root->name == "trailer")
  {
    // ftacout<<"ok"<<root->val<<"ok"<<endl;
  }
  else if (root->name == "assign_stmt")
  {
    root->val = root->children[0]->val;
    raoa *temp = new raoa();
    temp->result = root->children[0]->val;
    temp->arg1 = "";
    temp->arg2 = root->children[1]->val;
    temp->op = "ASSIGN_TEMP";
    tac_ins.push_back(temp);
  }
  else if (root->name == "decl_stmt")
  {
    root->val = root->children[0]->val;
    raoa *temp2 = new raoa();
    temp2->result = "";
    temp2->arg1 = "";
    temp2->arg2 = root->children[0]->val;
    temp2->op = "DECLARE";
    tac_ins.push_back(temp2);
  }
  else if (root->name == "equal_test" && root->children.size() > 1)
  {
    root->val = root->children[0]->val;
    raoa *temp2 = new raoa();
    temp2->result = root->val;
    temp2->arg1 = "";
    temp2->arg2 = root->children[2]->val;
    temp2->op = "ASSIGN_TEMP";
    tac_ins.push_back(temp2);
  }
  else if (root->name == "next_arglist")
  {
    if (root->children.size() == 3)
    {
      root->val = getReg();
      raoa *temp2 = new raoa();
      temp2->result = "";
      temp2->arg1 = "";
      temp2->arg2 = root->children[2]->val;
      ;
      temp2->op = "GETPARAM";
      tac_ins.push_back(temp2);
    }
    else
    {
      root->val = getReg();
      raoa *temp2 = new raoa();
      temp2->result = "";
      temp2->arg1 = "";
      temp2->arg2 = root->children[0]->val;
      ;
      temp2->op = "GETPARAM";
      tac_ins.push_back(temp2);
    }
  }

  else
  {
    if (root->children.size() > 0)
      root->val = root->children[0]->val;
  }

  return;
}

void printTAC()
{
  // ftacout << tac_ins.size() << endl;
  for (auto i : tac_ins)
  {
    if (stirs.find(i->arg2) != stirs.end())
    {
      i->sting = true;
      if (i->result != "")
      {
        stirs.insert({i->result, stirs[i->arg2]});
      }
    }
    ftacout << i->result << " = " << i->arg1 << " " << i->op << " " << i->arg2 << endl;
  }
  return;
}

#endif
