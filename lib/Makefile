YACC = /usr/bin/bison -dv
#LEX = /usr/bin/flex -Pnamarty_tpl -olex.yy.c
LEX = /home/junya/j/bin/flex -olex.yy.c
PARSER = parser.y
LEXER = lex.l

SRC = parser.tab.c \
	lex.yy.c \

.y:
	$(YACC) $<

.l:
	$(LEX) -Cfa -8 $<

.cc.o:
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< $(OUTPUT_OPTION) && $$YFIXDEP $(notdir $*.d) $(DEPDIR)

.c.o:
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< $(OUTPUT_OPTION) && $$YFIXDEP $(notdir $*.d) $(DEPDIR)


all::
	$(YACC) $(PARSER) -o parser.tab.c
	$(LEX) $(LEXER)
	$(CXX) -c parser.tab.c
	$(CXX) -c lex.yy.c
	$(CXX) -c namarty_core.cc
	$(CXX) -shared -o libnam.so namarty_core.o parser.tab.o lex.yy.o -lfl -lstdc++ -ly
	#$(CXX) -o command parser.tab.c lex.yy.c -lfl -ly -lstdc++

clean:
	rm -f *.o *~ lex.yy.c parser.tab.c
