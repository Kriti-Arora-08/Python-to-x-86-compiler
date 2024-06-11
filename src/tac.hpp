#include <bits/stdc++.h>

#ifndef TAC_HPP
#define TAC_HPP

using namespace std;

struct raoa
{
  string result;
  string arg1;
  string op;
  string arg2;
  int lineno;
  int tacno; //??
  unsigned long long size;
  bool go2;
  bool jump;
  unsigned long long int block;
  bool sting;

  raoa()
  {
    this->go2 = false;
    this->jump = false;
    this->block =0;
  }
};

void gentac(node* root);

#endif