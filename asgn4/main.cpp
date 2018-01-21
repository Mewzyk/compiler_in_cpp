/*
 * Stephen Thomas, sgthomas@ucsc.edu
 * Alex Hancock, abhancoc@ucsc.edu
 */

using namespace std;

#include <string>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <wait.h>
#include <stdlib.h>
#include <libgen.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>

#include "string_set.h"
#include "auxlib.h"
#include "astree.h"
#include "lyutils.h"
#include "symbol.h"

string filename;
const string CPP = "/usr/bin/cpp";
#define LINESIZE 1024
FILE *tok_file;

// Chomp the last character from a buffer if it is delim.
// Taken from cppstrok
void chomp (char* string, char delim) {
   size_t len = strlen (string);
   if (len == 0) return;
   char* nlpos = string + len - 1;
   if (*nlpos == delim) *nlpos = '\0';
}


/*
 * Method to intake and compose input options
 * @index: positon of the . in the program_name input
 * @opt: variable to be used while processing input options
 */
string set_options(int argc, char* argv[], string program_name){
   size_t index = program_name.rfind('.', program_name.length());
   int opt;
   string cpp_input = CPP + " " + program_name; 
   if (program_name.substr(index, program_name.length()-index) != ".oc"
       || index == string::npos) {
      cerr << "Input file must be of type .oc";
      exit(EXIT_FAILURE);
   } 
   else {
      while((opt = getopt (argc, argv, "@:D:ly")) != -1) {
         switch(opt){
            case 'l':
               yy_flex_debug = 1;
               break;
            case 'y':
               yydebug = 1;
               break;
            case '@':
               set_debugflags(optarg);
               break;
            case 'D':
               cpp_input = CPP + " -D" + optarg + " " + program_name;
               break;            
         }  
      }
   }
   return cpp_input;
}

/**
 * Scans tokens and writes them to the appropriate .tok file
 */
void scan_tokens(){
    string basefile(basename(const_cast<char*> (filename.c_str())));
    
    int token_rc;
    char* tokenfile = strdup((basefile.substr(0, 
                              basefile.length()-3)+".tok").c_str());

    tok_file = fopen (tokenfile, "w");
    if (tok_file == NULL) {
        cerr << "Failed to open token file " << tokenfile << endl;
    } else {
        for(;;) {
            token_rc = yylex();             
            if(token_rc == YYEOF) break;
            const string* inputToken = string_set::intern(yytext);
            (void) inputToken;
            
           //fprintf(tok_file, "%zd %zd.%zd %d %s %s \n", 
           //                  lexer::lloc.filenr, 
           //                  lexer::lloc.linenr, 
           //                  lexer::lloc.offset, 
           //                  token_rc, 
           //                  parser::get_tname(token_rc), 
           //                  yytext);
            
            //fprintf(tok_file, "%s \n", yytext);
            
        }
        fclose(tok_file);
    }
}

void tok_file_open(){ 
    string basefile(basename(const_cast<char*> (filename.c_str())));
    char* tokenfile = strdup((basefile.substr(0, 
                              basefile.length()-3)+".tok").c_str());

    tok_file = fopen (tokenfile, "w");
    if (tok_file == NULL) {
        cerr << "Failed to open token file " << tokenfile << endl;
    }
}

void tok_file_close(){
   fclose(tok_file);
}

/*
 * Helper function to pass arguments to the c pre processor
 * Handles file and error managment for the scanner
 */
void cpp_driver(string cpp_input, char* argv[]){
    exec::execname = basename (argv[0]);

    //Opens a pipe from the cpp to yyin
    yyin = popen (cpp_input.c_str(), "r");
    if (yyin == NULL) {
       cerr << "Invalid command: " << cpp_input;
       exit(EXIT_FAILURE);
    } else { 
       scan_tokens();       
       int pclose_rc = pclose (yyin);
       eprint_status (cpp_input.c_str(), pclose_rc);
       if (pclose_rc != 0) exit(EXIT_FAILURE);
   }
}

void cpp_pipe_open(string cpp_input){
    yyin = popen (cpp_input.c_str(), "r");
    if (yyin == NULL) {
       cerr << "Invalid command: " << cpp_input;
       exit(EXIT_FAILURE);
    }
}

void cpp_pipe_close(string cpp_input){
    int pclose_rc = pclose (yyin);
    eprint_status (cpp_input.c_str(), pclose_rc);
    if (pclose_rc != 0) exit(EXIT_FAILURE);
}

/*
 * Open filename.str, dump the stringset to the file,
 * and then close the file. 
 */
void print_stringset(int argc, char* argv[]){
   FILE *str_file;
   string basefile(basename(const_cast<char*> (filename.c_str())));

   char *stringfile = strdup((basefile.substr(0, 
                                basefile.length()-3)+".str").c_str());
   str_file = fopen(stringfile, "w");
   if (str_file == NULL) {
      cerr << "Failed to open stringset file " << stringfile << endl;
   } else {
       for (int i = 1; i < argc; ++i) {
          const string* str = string_set::intern (argv[i]);
          (void) str;
          //fprintf(str_file, "intern (\"%s\") returned %p->\"%s\"\n",
          //        argv[i], str, str->c_str());
       }
       string_set::dump(str_file);
       fclose(str_file);
   }
}

void print_astree (string filename) {
    string basefile(basename(const_cast<char*> (filename.c_str())));
    char* name = strdup((basefile.substr(0, basefile.length()-3) + 
                                                       ".ast").c_str());
    FILE* output = fopen(name, "w");
    if (output == NULL){
        exit (EXIT_FAILURE);
    }

    parser::root->dump_tree(output);

    fclose(output);
}

/*
 * Intitial Variables:
 * @program: name of the program being compiled
 * @c_program: name of the program being compiled represented 
 *             as a c style string
 * @cpp_input: string to be sent to the c pre processor 
 */
int main (int argc, char* argv[]) {
   yy_flex_debug = 0;
   string program = argv[argc-1];   
   filename = program; 
   exec::execname = basename (argv[0]); 
   string basefile(basename(const_cast<char*> (program.c_str())));

   cout << "HEY" << endl;
  
   string cpp_input = set_options(argc, argv, program);
   
   tok_file_open(); 
   cpp_pipe_open(cpp_input);
    
   cout << "HA" << endl;

   yyparse();       
   
   cout << "yyparse?" << endl;
 
   cpp_pipe_close(cpp_input);

   cout << "WOAH" << endl;

   tok_file_close();

   cout << "HA" << endl << endl;

//   init_table(parser::root);

   print_astree(filename);

   print_stringset(argc, argv);

   return EXIT_SUCCESS;
}
