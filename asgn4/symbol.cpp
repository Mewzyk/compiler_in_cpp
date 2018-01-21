#include "symbol.h"
#include "lyutils.h"
#include <iostream>

using namespace std;

int master_block_nr = 1;
vector<sym_table*> sym_stack;
sym_table struct_table;

void enter_block(astree* in_ast){
    if (in_ast->symbol == TOK_BLOCK){
        master_block_nr++;
        sym_stack.push_back(new sym_table());
        in_ast->block_nr = master_block_nr;
    }
    else if (in_ast->symbol == TOK_FUNCTION){
        master_block_nr++;
        sym_stack.push_back(new sym_table());

        in_ast->block_nr = master_block_nr;

        master_block_nr++;
        sym_stack.push_back(new sym_table());
    }
    else if (in_ast->symbol == TOK_PROTOTYPE){
        master_block_nr++;
        sym_stack.push_back(new sym_table());

        in_ast->block_nr = master_block_nr;

        master_block_nr++;
        sym_stack.push_back(new sym_table());
     }
     else{
        in_ast->block_nr = master_block_nr;
     }
    
}

void leave_block(astree* in_ast){
    if (in_ast->symbol == TOK_BLOCK){
        sym_stack.pop_back();
    }
    else if (in_ast->symbol == TOK_FUNCTION){
        sym_stack.pop_back();
    }
}

void init_table(astree* in_ast){
    //sym_stack.push_back(new sym_table());
    //construct_sym(in_ast);
    cout << 'Hello!' << endl;
}

void construct_sym(astree* in_ast){
    //enter_block(in_ast);
    for(auto node : in_ast->children)
            construct_sym(node);
    //typecheck(in_ast);
    //leave_block(in_ast);
}

void typecheck(astree* in_ast){
    switch(in_ast->symbol)
    {
        case TOK_DECLID:
        {
            break;
        }
        case TOK_INT:
        {
            astree *left = in_ast->children[0];
            if (!left) break;
            in_ast->attr[ATTR_int] = 1;
            left->attr[ATTR_int] = 1;
            break;
        }
    }
}
