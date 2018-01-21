# 
# Stephen Thomas, sgthomas@ucsc.edu
# Alex Hancock, abhancoc@ucsc.edu
# 

CPP  	 = g++ -g -O0 -Wall -Wextra -std=gnu++14
VAL  	 = valgrind --leak-check=full --show-reachable=yes
MAIN	 = main.cpp
O1SOURCE = main.o string_set.o auxlib.o lyutils.o symbol.o
O2SOURCE = astree.o yylex.o 
O3SOURCE = yyparse.o
OSOURCES = ${O1SOURCE} ${O2SOURCE} ${O3SOURCE}
LSOURCES = scanner.l
YSOURCES = parser.y
CLGEN 	 = yylex.cpp
HYGEN 	 = yyparse.h
CYGEN 	 = yyparse.cpp
LREPORT  = yylex.output
YREPORT  = yyparse.output
REPORTS  = ${LREPORT} ${YREPORT}
AUXSRC 	 = auxlib.cpp auxlib.h 
LYUSRC 	 = lyutils.cpp lyutils.h
ASTSRC 	 = astree.cpp astree.h
STRSRC 	 = string_set.cpp string_set.h
SYMSRC	 = symbol.cpp symbol.h

all : oc

oc : ${OSOURCES} 
	${CPP} ${OSOURCES} -o oc

%.o : %.cpp
	${CPP} -c $<

ci:
	git add ${LSOURCES} ${MAIN}
	git commit -m "Automated commit"
	git push

spotless : clean
	- rm oc test?.out test?.err ${REPORTS}

clean :
	- rm ${OSOURCES} ${CLGEN} ${CYGEN}

${CLGEN} : ${LSOURCES}
	flex --outfile=${CLGEN} ${LSOURCES}

${CYGEN} ${HYGEN} : ${YSOURCES}
	bison --defines=${HYGEN} --output=${CYGEN} ${YSOURCES}

submit :
	submit cmps104a-wm.f17 asg3 ${MAIN} ${STRSRC} ${AUXSRC}
	submit cmps104a-wm.f17 asg3 ${LSOURCES} ${YSOURCES} ${HYGEN}
	submit cmps104a-wm.f17 asg3 ${LYUSRC} $(ASTSRC)
	submit cmps104a-wm.f17 asg3 README Makefile

# Deps
main.o: main.cpp string_set.h auxlib.h astree.h lyutils.h symbol.h
string_set.o: ${STRSRC}
auxlib.o: ${AUXSRC}
lyutils.o: ${LYUSRC} 
astree.o: $(ASTSRC)
symbol.o: $(SYMSRC)
yylex.o: yylex.cpp lyutils.h yyparse.h
yyparse.o: yyparse.cpp yyparse.h
