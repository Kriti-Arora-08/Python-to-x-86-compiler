#include <bits/stdc++.h>
#define RESET "\033[0m"
#define RED "\033[31m"
#include "parser.tab.h"
#include "node.hpp"

#include "symtab.cpp"
#include "tac.cpp"
#include "x86.cpp"

extern FILE *yyin;
extern int yylex();
extern int yylineno;
extern int yyparse();
extern int yydebug;
extern bool flag;
extern vector<raoa*> tac_ins;

using namespace std;

void print_help_page();
ofstream fout("./out/in.dot", std::ios::out | std::ios::trunc);

#define SHELLSCRIPT "\
#/bin/bash \n\
dot -Tpdf ./out/in.dot -o ./out/out.pdf\
"

// void print_tree (struct node* r) {
//     int n = r->children.size();
//     // int y = k+1;
//     for(int i=0; i<n; i++) {
//         fout<<to_string(r->children[i]->node_number)<<"[label=\""<<r->children[i]->name<<"\"]"<<endl;
//     }
//     for(int i=0; i<n; i++) {
//         fout<<to_string(r->node_number)<<"->"<<to_string(r->children[i]->node_number)<<endl;
//         print_tree(r->children[i]);
//     }
//     return;
// }

void print_tree(struct node *r)
{
    int n = r->children.size();
    // int y = k+1;
    for (int i = 0; i < n; i++)
    {
        if (r->children[i]->children.size() == 0)
        {
            if (r->children[i]->name[0] == '\"')
            {
                fout << to_string(r->children[i]->node_number) << "[label=\"" << r->children[i]->name.substr(1, r->children[i]->name.size() - 2) << "_" << r->children[i]->blck << "\",color = \"red\"]" << endl;
            }
            else
            {
                fout << to_string(r->children[i]->node_number) << "[label=\"" << r->children[i]->name << "_" << r->children[i]->blck << "\",color = \"red\"]" << endl;
            }
        }
        else
        {
            fout << to_string(r->children[i]->node_number) << "[label=\"" << r->children[i]->name << "_" << r->children[i]->blck << "\"]" << endl;
        }
    }
    for (int i = 0; i < n; i++)
    {
        fout << to_string(r->node_number) << "->" << to_string(r->children[i]->node_number) << endl;
        print_tree(r->children[i]);
    }
    return;
}

struct node *reduce_7(struct node *r)
{
    int n = r->children.size();
    if (n == 0)
        return r;

    for (int i = 0; i < r->children.size(); i++)
    {
        r->children[i] = reduce_7(r->children[i]);
    }

    vector<struct node *> temp;

    bool found = false;

    for (int i = 0; i < r->children.size(); i++)
    {
        if (!found)
        {
            if ((r->children[i]->name == "==" || r->children[i]->name == "=" || r->children[i]->name == ">=" || r->children[i]->name == "<=" || r->children[i]->name == ">" || r->children[i]->name == "<" || r->children[i]->name == "!=" || r->children[i]->name == "+" || r->children[i]->name == "-" || r->children[i]->name == "+=" || r->children[i]->name == "-=" || r->children[i]->name == "*") && r->children[i]->children.size() == 0)
            {
                found = true;
                r->name = r->children[i]->name;
            }
            else
            {
                temp.push_back(r->children[i]);
            }
        }
        else
        {
            temp.push_back(r->children[i]);
        }
    }

    r->children = temp;

    return r;
}

struct node *reduce_9(struct node *r)
{
    int n = r->children.size();

    if (n == 0)
        return r;

    for (int i = 0; i < r->children.size(); i++)
    {
        r->children[i] = reduce_9(r->children[i]);
    }

    vector<struct node *> temp;

    for (int i = 0; i < r->children.size(); i++)
    {
        if ((r->name == r->children[i]->name && r->name != "comp_op") || (r->name == "trailers" && r->children[i]->name == "trailer"))
        {
            for (int j = 0; j < r->children[i]->children.size(); j++)
            {
                temp.push_back(r->children[i]->children[j]);
            }
        }
        else
        {
            temp.push_back(r->children[i]);
        }
    }

    r->children = temp;

    return r;
}

struct node *reduce_1(struct node *r /*, struct node* par*/)
{
    // int n = r->children.size();
    // if(n==0) return;
    // if(n==1) {
    //     if(par) {
    //         for(int i=0; i<par->children.size(); i++) {
    //             if(par->children[i]==r) {
    //                 par->children[i] = r->children[0];
    //                 break;
    //             }
    //         }
    //         reduce_1(r->children[0], par);
    //     }
    //     else {
    //         r = r->children[0];
    //         reduce_1(r->children[0], par);
    //     }
    //     return;
    // }
    // for(int i=0; i<n; i++) {
    //     reduce_1(r->children[i], r);
    // }
    //////////////////////////////////////////////////////////////

    int n = r->children.size();
    if (n == 0)
        return r;

    for (int i = 0; i < n; i++)
    {
        r->children[i] = reduce_1(r->children[i]);
    }

    vector<struct node *> temp;

    for (int i = 0; i < r->children.size(); i++)
    {
        if (r->children[i]->children.size() == 1)
        {
            temp.push_back(r->children[i]->children[0]);
        }
        else
        {
            temp.push_back(r->children[i]);
        }
    }

    r->children = temp;

    return r;
}

struct node *reduce_2(struct node *r)
{
    int n = r->children.size();
    if (n == 0)
        return r;
    return NULL;
}

void reduce_0(struct node *r)
{

    vector<struct node *> temp = {};
    for (int i = 0; i < r->children.size(); i++)
    {
        if (r->children[i]->name == "e" || r->children[i]->name == "empty" || r->children[i]->name == "newlines" || r->children[i]->name == "newline" || r->children[i]->name == "NEWLINE" || r->children[i]->name == ":" || r->children[i]->name == "," || r->children[i]->name == "COMMA" || r->children[i]->name == ".")
        {
            continue;
        }
        else
        {
            temp.push_back(r->children[i]);
        }
    }

    r->children = temp;

    for (auto e : r->children)
    {
        reduce_0(e);
    }

    return;
}

void create_csv_func(struct symtab *cur, int type /*0 means normal func and 1 means class method*/)
{
    ofstream out;
    // cout<<"in func csv\n";
    if (type == 0)
    {
        string name = "function_" + cur->name + ".csv";
        out.open(name, ios::out);
    }
    else
    {
        string name = "Class_" + cur->name + ".csv";
        out.open(name, ios::out);
    }
    out << "Lexeme,Token,Type,Line_num\n";

    stack<pair<int, struct symtab *>> st;

    st.push(make_pair(-1, cur));

    while (!st.empty())
    {
        auto i = st.top();
        int j = i.first;
        struct symtab *temp = i.second;
        if (j == -1)
        {
            auto it2 = temp->entries.begin();

            for (; it2 != temp->entries.end(); it2++)
            {
                auto ent = it2->second;
                out << ent->name << "," << ent->token << "," << ent->type << "," << ent->lineno << endl;
            }

            st.pop();
            st.push(make_pair(1, temp));
        }
        else if (j < temp->children.size())
        {
            // st.pop();
            st.top().first++;
            st.push(make_pair(-1, temp->children[j - 1]));
        }
        else
        {

            st.pop();
        }
    }
    out.close();
}

void create_csv_class(struct symtab *cur)
{
    ofstream out;
    out.open(cur->name+".csv", ios::out);
    out << "Lexeme,Token\n";

    auto it = cur->attributes.begin();
    for (; it != cur->attributes.end(); it++)
    {
        out << it->first << "," << "attribute\n";
    }

    for (int i = 0; i < cur->children.size(); i++)
    {
        if (cur->children[i]->scope == 3)
        {
            // method
            out << cur->children[i]->name << ",Method\n";
            create_csv_func(cur->children[i], 1);
        }
    }

    out.close();
}

void create_csv(struct symtab *cur)
{
    // cout<<"hello"<<endl;
    for (int i = 0; i < cur->children.size(); i++)
    {
        if (cur->children[i]->scope == 1)
        {
            // it is a function name
            create_csv_func(cur->children[i], 0);
        }
        else if (cur->children[i]->scope == 2)
        {
            // it is a class
            create_csv_class(cur->children[i]);
        }
    }
}

int main(int argc, char *argv[])
{
    FILE *pt;
    yydebug = 0;
    bool verbose = false;
    bool input = false;
    bool output = false;
    for (int i = 1; i < argc; i++)
    {
        if (std::string(argv[i]) == "--help" || std::string(argv[i]) == "-h")
        {
            print_help_page();
            return -1;
        }
        else if (std::string(argv[i]) == "--input" || std::string(argv[i]) == "-i")
        {

            if ((i + 1) < argc)
            {
                pt = fopen(argv[i + 1], "r");
                input = true;
            }
            else
                cout << "Error: No input filename given";
            i++;
        }
        else if (std::string(argv[i]) == "--verbose" || std::string(argv[i]) == "-v")
        {
            verbose = true;
        }
        else if (std::string(argv[i]) == "--output" || std::string(argv[i]) == "-o")
        {
            output = true;
        }
        else
        {
            cout << "Error: Invalid parameter\n";
            print_help_page();
            return -1;
        }
    }
    if (!input && verbose)
        cout << "No input file for running\nUse the -h or --help tag for man page" << endl;
    if (!input)
        exit(0);
    yyin = pt;
    if (verbose)
        yydebug = 1;
    do
    {
        yyparse();
    } while (!feof(yyin));
    if (flag)
    {
        cout << "Error Ocurred" << endl;
    }
    else
    {
        // cout << "Parse Successful" << endl;
        if (output)
        {
            cout << "out.pdf is ready with the AST." << endl;
            fout << "digraph {" << endl;
            fout << to_string(root->node_number) << "[label=\"" << root->name << "\"]" << endl;

            root_tab = new symtab();
            root_tab->name = "global";
            root_tab->scope = 0;
            struct syment *temp = new syment();
            temp->name = "__name__";
            temp->lineno = 0;
            temp->type = "str";
            temp->token = "NAME";
            fill_type_comp();
            root_tab->entries[temp->name] = temp;
            assgnB(root);
            print_tree(root);
            trav(root, root_tab);
            // check_type(root);
            print_symtab(root_tab);
            // create_csv(root_tab);
            makeTAC(root);
            printTAC();
            // root=reduce_1(root);
            // reduce_0(root);
            // root = reduce_1(root);
            // root= reduce_7(root);
            convert(tac_ins);
            fout << "}" << endl;
            system(SHELLSCRIPT);
        }
        else
        {
            fout << "Put -o tag to generate the DOT file correctly." << endl;
            cout << "Put -o or --output tag to generate out.pdf (AST)" << endl;
        }
    }
    fout.close();
    fclose(yyin);

    return 0;
}

void print_help_page()
{
    cout << "Usage: ./parser [options]     \n\n";
    cout << "Commands:\n-h, --help \t\t\t\t\t Show help page\n";
    cout << "-i, --input <input_file_name> \t\t\t Give input file\n";
    cout << "-v, --verbose \t\t\t\t\t View stack via YYDEBUG\n";
    cout << "-o, --output \t\t\t\t\t Generates the AST in out.pdf\n";
    return;
}
