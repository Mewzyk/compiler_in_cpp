#include "astree.h"
#include <stdlib.h>
#include <unordered_map>
using namespace std;

struct symbol;
using sym_table = unordered_map<string*,symbol*>;
using sym_entry = sym_table::value_type;

void init_table(astree* in_ast);
void construct_sym(astree* in_ast);
void typecheck(astree* in_ast);

struct symbol {
    attr_bitset attributes;
    sym_table* fields;
    size_t filenr, linenr, offset, block_nr;
    vector<symbol*>* paramaters;
    symbol(astree*);
};

