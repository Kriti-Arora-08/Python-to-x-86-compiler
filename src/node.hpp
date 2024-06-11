#ifndef NODE_HPP
#define NODE_HPP

#include <bits/stdc++.h>
#include "./symtab.hpp"
using namespace std;

struct node
{
    string name = "";
    string val = "";                    // stores the lexeme if terminal or the name of the non terminal otherwise
    bool terminal = false;              // To be used only if node is a terminal, empty otherwise. stores the token
    unsigned long long node_number = 0; // For disambiguity in AST code
    node *parent = NULL;
    int lineno;
    bool opet = false;
    struct symtab *tab;
    vector<node *> children;
    string type;
    unsigned long long blck;
    node(unsigned long long _c) : name(""), val(""), terminal(false), node_number(_c), parent(NULL), opet(false), children({}), tab(NULL), type(""), blck(0){};
};

extern node *root;

#endif