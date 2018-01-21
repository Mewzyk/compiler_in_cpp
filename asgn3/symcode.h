#define __SYMTABLE_H__

#include <string>
#include <vector>
#include <bitset>

#include "astree.h"

using symbol_table = unordered_map<string*, symbol*>
using symbol_entry = symbol_table::value_type;

extern size_t next_block;
