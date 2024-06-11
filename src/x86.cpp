#ifndef ASM
#define ASM

#include <bits/stdc++.h>
#include "tac.cpp"

// converting tac to x86

// stack<map<string,int>> st;
// struct raoa {
//   string result;
//   string op;
//   string arg1;
//   string arg2;
// };

map<string, int> offset;

int off;
int label_id = 1;

int get_label_id()
{
  label_id++;
  return label_id;
}

void convert(vector<struct raoa *> tac)
{
  map<string, string> operators;

  operators["=="] = "je";
  operators[">"] = "jl";
  operators[">="] = "jle";
  operators["<="] = "jge";
  operators["<"] = "jg";
  operators["!="] = "jne";
  ofstream out;
  out.open("./out/asm.s", ios::out);

  out << ".section .data\n";
  out << "str_int : .string \"%d\"\n ";
  out << "str_s : .string \"%s\" ";

  out << "\n.text\n";

  ////////////////////making print
  out << ".globl print\n.type print,@function\nprint:\n";
  out << "pushq %rbp\n";
  out << "movq %rsp, %rbp\n";
  out << "movq $str_int, %rdi\n";
  out << "movq 16(%rbp),%rsi\n";
  out << "call printf\n";
  out << "movq %rbp, %rsp\n";
  out << "popq %rbp\n";
  out << "ret\n";
  ////////////////////ending print
  ////////////////////making print
  out << ".globl printstring\n.type printstring,@function\nprintstring:\n";
  out << "pushq %rbp\n";
  out << "movq %rsp, %rbp\n";
  out << "movq $str_s, %rdi\n";
  out << "movq 16(%rbp),%rsi\n";
  out << "call printf\n";
  out << "movq %rbp, %rsp\n";
  out << "popq %rbp\n";
  out << "ret\n";
  ////////////////////ending print

  int n = tac.size();
  off = 0;

  // print the header

  // now the programmable part starts

  for (int i = 0; i < n; i++)
  {
    if(tac[i]->op == "STRING") {
  //     .LC0:
	// .ascii "Hello World\0"
  int n = tac[i]->arg2.size(); 
    out<<"."<<tac[i]->result<<":\n"<<".ascii "<<tac[i]->arg2.substr(0, n-1)<<"\\0"<<tac[i]->arg2[0]<<endl;
    }

    if (tac[i]->op == "JIF")
    {
      // 193 # jif t1 , .L#
      // 194#  op,arg1,arg2
      // 195 movq $1,%r8
      // 196 movq -24(%rbp),%r9 #movq off1(%rbp),%r9
      // 197 #                          ^ offset of t1
      // 198 cmp %r8,%r9
      // 199 jne .L3  #jne .L#
      // 200
      // 201 #end jif
      string var = tac[i]->arg1;
      out << "#new line : " << var << " JIF " << tac[i]->arg2 << endl;
      out << "movq $1,%r8" << endl;
      out << "movq " << offset[var] << "(%rbp),%r9" << endl;
      out << "cmp %r8,%r9" << endl;
      out << "jne " << tac[i]->arg2 << endl;
      out << "#end cmd\n";
    }
    else if (tac[i]->op == "DECLARE")
    {

      // string res = tac[i]->result;

      string var = tac[i]->arg2;
      if (offset[var] == 0)
      {
        out << "pushq $0" << endl;
        off = off - 8;
        offset[var] = off;
        out << "#" << var << "--offset = " << off << endl;
      }
    }
    else if (tac[i]->op == "ASSIGN_TEMP")
    {

      string res = tac[i]->result;
      string var = tac[i]->arg2;
      if (tac[i]->sting && tac[i]->arg2[0] == 's')
      {
        //       leaq	.LC0(%rip), %rax
        // movq	%rax, -40(%rbp)

        if (offset[res] == 0)
        {
          // not a defined var
          out << "#" << res << "=" << var << "#" << res << " not declared" << endl;
          out << "leaq ." << var << "(%rip), %rax" << endl;
          out << "movq " << "%rax, %r8\n";
          out << "pushq %r8" << endl;
          off = off - 8;
          offset[res] = off;
          out << "#" << res << "---offset = " << off << endl;
        }
        else
        {
          out << "leaq ." << var << "(%rip), %rax" << endl;
          out << "movq " << "%rax, " << offset[res] << "(%rbp)\n";
        }
      }
      else
      {
        if (offset[res] == 0)
        {
          // not a defined var
          out << "#" << res << "=" << var << "#" << res << " not declared" << endl;
          out << "movq " << offset[var] << "(%rbp),%r8\n";
          out << "pushq %r8" << endl;
          off = off - 8;
          offset[res] = off;
          out << "#" << res << "---offset = " << off << endl;
        }
        else
        {
          out << "movq " << offset[var] << "(%rbp),%r8\n";
          out << "movq %r8, " << offset[res] << "(%rbp)\n";
        }
      }
    }
    else if (tac[i]->op == "ASSIGN_INT")
    {

      string res = tac[i]->result;
      string var = tac[i]->arg2;

      if (offset[res] == 0)
      {
        // not a defined var
        out << "#Assign_int #" << res << "not declared\n";
        out << "pushq $" << var << endl;
        off = off - 8;
        offset[res] = off;
        out << "#" << res << "--offset = " << off << endl;
      }
      else
      {
        out << "movq $" << var << ", " << offset[res] << "(%rbp)\n";
      }
    }
    else if (tac[i]->op == "BRANCH")
    {
      out << "jmp " << tac[i]->arg2 << endl;
    }
    else if (tac[i]->op == "+" || tac[i]->op == "-")
    {
      //       #performing addition : t3 = t1 + t2
      // #to perform sub change addq to subq
      // movq -8(%rbp) , %r8
      // movq -16(%rbp), %r9
      // subq %r9,%r8
      // #case 1
      // #---- if t3 is new var
      // #pushq %r8
      // #---- store the new offset for t3
      // #case 2
      // #---- if t3 already exists
      // movq %r8 , -24(%rbp)
      // #addition completed
      out << "movq " << offset[tac[i]->arg1] << "(%rbp) , %r8" << endl;
      out << "movq " << offset[tac[i]->arg2] << "(%rbp), %r9" << endl;

      if (tac[i]->op == "+")
      {
        out << "addq %r9,%r8" << endl;
      }
      else
      {
        out << "subq %r9,%r8" << endl;
      }

      if (offset[tac[i]->result] == 0 /*variable does not exists*/)
      {
        out << "#" << tac[i]->result << "not declared\n";
        out << "pushq %r8" << endl;
        off = off - 8;
        offset[tac[i]->result] = off;
        out << "#" << tac[i]->result << "---offset = " << off << endl;
      }
      else
      {
        out << "movq %r8 ," << offset[tac[i]->result] << "(%rbp)" << endl;
      }
    }
    else if (tac[i]->op == "and" || tac[i]->op == "or")
    {
      out << "movq " << offset[tac[i]->arg1] << "(%rbp) , %r8" << endl;
      out << "movq " << offset[tac[i]->arg2] << "(%rbp), %r9" << endl;

      if (tac[i]->op == "and")
      {
        out << "andq %r9,%r8" << endl;
      }
      else
      {
        out << "orq %r9,%r8" << endl;
      }

      if (offset[tac[i]->result] == 0 /*variable does not exists*/)
      {
        out << "#" << tac[i]->result << "not declared\n";
        out << "pushq %r8" << endl;
        off = off - 8;
        offset[tac[i]->result] = off;
        out << "#" << tac[i]->result << "---offset = " << off << endl;
      }
      else
      {
        out << "movq %r8 ," << offset[tac[i]->result] << "(%rbp)" << endl;
      }
    }
    else if (tac[i]->op == "*")
    {
      // #performing multipplication
      // # t3 = t1 * t2
      // movq -8(%rbp), %rax
      // movq -16(%rbp), %r8
      // imulq %r8
      // #case 1
      // #---- if t3 is new var
      // #pushq %rax    #---- store the new offset for t3
      // #case 2
      // #---- if t3 already exists
      // movq %rax , -24(%rbp)
      // #end mult

      int off1 = offset[tac[i]->arg1];
      int off2 = offset[tac[i]->arg2];

      out << "movq " << off1 << "(%rbp), %rax" << endl;
      out << "movq " << off2 << "(%rbp), %r8" << endl;
      out << "imulq %r8" << endl;

      if (offset[tac[i]->result] == 0)
      {
        out << "#" << tac[i]->result << "not declared\n";
        out << "pushq %rax" << endl;
        off = off - 8;
        offset[tac[i]->result] = off;
        out << "#" << tac[i]->result << "---offset = " << off << endl;
      }
      else
      {
        out << "movq %rax , " << offset[tac[i]->result] << "(%rbp)" << endl;
      }
    }
    // else if (tac[i]->op == "==" || tac[i]->op == "<=" || tac[i]->op == ">=" || tac[i]->op == ">" || tac[i]->op == "<" || tac[i]->op == "!=")
    // {
    //   string label = ".C" + to_string(get_label_id());

    //   out << "movq " << offset[tac[i]->arg1] << "(%rbp),%r8\n";
    //   out << "movq " << offset[tac[i]->arg2] << "(%rbp),%r9\n";
    //   out << "movq $1,%r10\n";
    //   if (offset[tac[i]->result] == 0)
    //   {
    //     // no offset defined
    //     out << "pushq $0\n";
    //     off = off - 8;
    //     offset[tac[i]->result] = off;
    //   }
    //   out << "cmp %r8,%r9\n";
    //   out << operators[tac[i]->op] << " " << label << endl;
    //   out << "movq $0,%r10\n";
    //   out << label << ":\n";
    //   out<<"movq %r10,"<<offset[tac[i]->result]<<"(%rbp)\n";
    // }
    else if (tac[i]->op == "==" || tac[i]->op == "<=" || tac[i]->op == ">=" || tac[i]->op == ">" || tac[i]->op == "<" || tac[i]->op == "!=")
    {
      string label = ".C" + to_string(get_label_id());
      out << "movq " << offset[tac[i]->arg1] << "(%rbp),%r8\n";
      out << "movq " << offset[tac[i]->arg2] << "(%rbp),%r9\n";
      out << "movq $1,%r10\n";
      if (offset[tac[i]->result] == 0)
      {
        // no offset defined
        out << "#" << tac[i]->result << "not declared\n";
        out << "pushq $0\n";
        off = off - 8;
        offset[tac[i]->result] = off;
        out << "#" << tac[i]->result << "---offset = " << off << endl;
      }

      out << "cmp %r8,%r9\n";
      out << operators[tac[i]->op] << " " << label << endl;
      out << "movq $0,%r10\n";
      out << label << ":\n";
      out << "movq %r10," << offset[tac[i]->result] << "(%rbp)\n";
    }
    else if (tac[i]->op == "//" || tac[i]->op == "%%")
    {

      int off1 = offset[tac[i]->arg1];
      int off2 = offset[tac[i]->arg2];

      out << "movq " << off1 << "(%rbp), %rax" << endl;
      out << "movq " << off2 << "(%rbp), %r8" << endl;
      out << "cqo" << endl;
      out << "idivq %r8" << endl;

      if (tac[i]->op == "%%")
      {
        out << "movq %rdx,%rax" << endl;
      }

      if (offset[tac[i]->result] == 0)
      {
        out << "#" << tac[i]->result << "not declared\n";
        out << "pushq %rax" << endl;
        off = off - 8;
        offset[tac[i]->result] = off;

        out << "#" << tac[i]->result << "---offset = " << off << endl;
      }
      else
      {
        out << "movq %rax , " << offset[tac[i]->result] << "(%rbp)" << endl;
      }
    }
    else if (tac[i]->op == "begin_func")
    {
      string func_name = tac[i]->arg2;
      out << "\n\n\n\n";
      out << ".globl " << func_name << endl;
      out << ".type " << func_name << ",@function" << endl;
      out << func_name << ":" << endl;
      out << "pushq %rbp\n";
      out << "movq %rsp, %rbp\n";
      off = 0;
      offset.clear();

      int done = 0;
      int j = i;
      ////define offset for each that appears in the function//////
      while (!done)
      {
        if (tac[j]->op == "end_func")
        {
          done = 1;
          break;
        }
        else
        {
          if (tac[j]->op == "+" || tac[j]->op == "-" || tac[j]->op == "*" || tac[j]->op == "//" || tac[j]->op == "%%" || tac[j]->op == ">" || tac[j]->op == ">=" || tac[j]->op == "<=" || tac[j]->op == "<" || tac[j]->op == "==" || tac[j]->op == "!=" || tac[j]->op == "ASSIGN_TEMP" || tac[j]->op == "ASSIGN_INT" || tac[j]->op == "call_func" || tac[j]->op == "or" || tac[j]->op == "and")
          {
            if (offset[tac[j]->result] == 0)
            {
              out << "pushq $0\n";
              off = off - 8;
              offset[tac[j]->result] = off;
              out << "#" << tac[j]->result << " ---offset =" << off << endl;
            }
          }
          else if (tac[j]->op == "DECLARE")
          {
            if (offset[tac[j]->arg2] == 0)
            {
              out << "pushq $0\n";
              off = off - 8;
              offset[tac[j]->arg2] = off;
              out << "#" << tac[j]->arg2 << " ---offset =" << off << endl;
            }
          }
        }

        j++;
      }

      /////end definition//////////
      done = 0;
      int par_off = 16;
      vector<string> para;
      i++;
      out << tac[i]->arg2 << ":\n";
      i++;
      while (!done)
      {
        if (tac[i]->op == "GETPARAM")
        {
          // offset[tac[i]->arg2] = par_off;
          para.push_back(tac[i]->arg2);
          i++;
        }
        else
        {
          done = 1;
          break;
        }
      }

      i--;
      int k = para.size() + 1;

      for (int l = 0; l < para.size(); l++)
      {
        offset[para[l]] = 8 * k;
        out << "#param " << para[l] << "---offset = " << 8 * k << endl;
        k--;
      }
    }
    else if (tac[i]->op == "LABEL")
    {
      out << tac[i]->arg2 << " :" << endl;
    }
    else if (tac[i]->op == "RETURN_FUNC")
    {
      string var = tac[i]->arg2;
      offset[var] = off;
      off -= 8;
    }
    else if (tac[i]->op == "RETURN")
    {

      if (tac[i]->arg2.size() == 0)
      {
        // nothing to return
        // cout << "#here\n";
        out << "movq %rbp,%rsp\n";
        out << "popq %rbp\n";
        out << "ret\n";
      }
      else
      {
        out << "movq " << offset[tac[i]->arg2] << "(%rbp),%rcx\n";
        out << "movq %rbp,%rsp\n";
        out << "popq %rbp\n";
        out << "ret\n";
      }
    }
    else if (tac[i]->op == "end_func")
    {
      out << "movq %rbp,%rsp\n";
      out << "popq %rbp\n";
      out << "ret\n";
    }
    else if (tac[i]->op == "PUSHPARAM")
    {
      // we have parameters push till we get func call

      int done = 0;
      vector<string> para;
      out << "#pusparams starting---- curr offset =" << off << endl;
      while (!done)
      {
        if (tac[i]->op == "PUSHPARAM")
        {
          para.push_back(tac[i]->arg2);
          off -= 8;
          i++;
        }
        else
        {
          done = 1;
          break;
        }
      }
      // i points to call_func line
      int extra = 0;
      out << "#offset after pushing params=" << off << endl;
      if (off % 16 != 0)
      {
        extra = 1;
        out << "#it is not mul of 16, therefore one more push" << endl;
        out << "pushq $0\n";
        off = off - 8;
      }
      out << "#FINAL offset after pushing params=" << off << endl;

      for (int l = 0; l < para.size(); l++)
      {
        out << "pushq " << offset[para[l]] << "(%rbp)\n";
      }
      if (tac[i]->arg2 == "print" && tac[i - 1]->sting)
      {
        out << "call printstring" << endl;
      }
      else
      {

        out << "call " << tac[i]->arg2 << endl;
      }
      int j = i; // stores instr of call_func line

      i++;
      done = 0;
      while (!done)
      {
        if (tac[i]->op == "POPPARAM")
        {
          out << "popq %r13\n";
          off = off + 8;
          i++;
        }
        else
        {
          done = 1;
          break;
        }
      }

      if (extra)
      {
        out << "popq %r13\n";
        off = off + 8;
      }

      i--;
      // i points to last pop_param line
      // in the end of this loop ->i++ -> i points to next line in next iter
      string var = tac[j]->result;
      if (offset[var] == 0)
      {
        out << "#" << var << "not declared\n";

        out << "pushq %rcx\n";
        off = off - 8;
        offset[var] = off;
        out << "#" << var << "---- offset =" << off << endl;
      }
      else
      {
        out << "movq %rcx, " << offset[var] << "(%rbp)\n";
      }
    }
    else if (tac[i]->op == "POPPARAM")
    {
      out << "#POPARAM ENTERING----------ERROR\n";

      int done = 0;
      while (!done)
      {
        if (tac[i]->op == "POPPARAM")
        {
          out << "popq %r13\n";
          off = off + 8;
          i++;
        }
        else
        {
          done = 1;
          break;
        }
      }
      i--;
    }
    else
    {
      out << "#SKIPPING " << tac[i]->op << endl;
    }
  }
}

#endif