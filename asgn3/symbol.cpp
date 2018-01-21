#include "symbol.h"
#include "lyutils.h"
#include <iostream>

using namespace std;

int master_block_nr = 1;
vector<sym_table*> sym_stack;
sym_table struct_table;

symbol::symbol(astree* in_ast){
    filenr = in_ast->lloc.filenr;
    linenr = in_ast->lloc.linenr;
    offset = in_ast->lloc.offset;
    fields = NULL;
    paramaters = NULL;
    block_nr = master_block_nr;
}

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
    sym_stack.push_back(new sym_table());
    construct_sym(in_ast);
}

void construct_sym(astree* in_ast){
    enter_block(in_ast);
    for(auto node : in_ast->children)
            construct_sym(node);
    typecheck(in_ast);
    leave_block(in_ast);
}

void typecheck(astree* in_ast){
    astree* ident = nullptr;
    if(in_ast->children.size() >= 1)
        ident = in_ast->children[0];

    switch(in_ast->symbol)
    {
        case TOK_DECLID:
        {
            break;
        }
        case TOK_IDENT:
        {
            break;
        }
        case TOK_BLOCK:
        {
            break;
        }
        case TOK_PROTOTYPE:
        case TOK_FIELD:
        {
            in_ast->attr[ATTR_field] = 1;
            break;
        }
        case TOK_INT:
        {
            if (!ident) break;
            in_ast->attr[ATTR_int] = 1;
            ident->attr[ATTR_int] = 1;
            break;
        }
        case TOK_INTCON:
        {
            in_ast->attr[ATTR_int] = 1;
            in_ast->attr[ATTR_const] = 1;
            break;
        }
        case TOK_TYPEID:
        {
            in_ast->attr[ATTR_typeid] = 1;
            break;
        }
        case TOK_VARDECL:
        {
            ident->children[0]->attr[ATTR_lval] = 1;
            ident->children[0]->attr[ATTR_variable] = 1;
            symbol *sym = new symbol(ident->children[0]);
            sym_stack.back()->insert(sym_entry(const_cast<string*>(ident->children[0]->lexinfo), sym));
            break;
        } 
        
    }
}
