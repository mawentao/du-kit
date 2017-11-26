###########################################################

AR   = ar
CC   = gcc
CPP  = g++
PERL = perl
PY   = python

###########################################################

CPPFLAGS = -I ./ -I ../ 

LDLIBS   = -L. -lpthread  -lz

CXXFLAGS = -g -W -Wall -Wno-unused-parameter -pipe

###########################################################

TARGETS = du-mkp du-mkbdp du-mktoolp du-mktoolexp du-mkubp du-mkubsp \
          du-mkphpscriptp du-mkpyscriptp du-mkdzpluginp du-mkphpetlp \
          du-mkjavamavenp du-mkjavawebp
OBJECTS = du.o Configure.o

.PHONY: all clean

all : $(TARGETS)
	rm -rf output
	mkdir -p output
	cp $^ output/
	rm -f *.o

du-mkp : mkp.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS) 
du-mkbdp : mkbdp.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS) 
du-mktoolp : mktoolp.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS) 
du-mktoolexp : mktoolexp.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS) 
du-mkubp : mkubp.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS) 
du-mkubsp : mkubsp.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS) 
du-mkphpscriptp : mkphpscriptp.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS) 
du-mkphpetlp : mkphpetlp.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS) 
du-mkpyscriptp : mkpyscriptp.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS)
du-mkdzpluginp : mkdzpluginp.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS)
du-mkjavamavenp : mk_java_maven_p.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS)
du-mkjavawebp : mk_java_web_p.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS)


##########################################################

Configure.o: ../Configure.cpp
	$(CPP) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(INCLUDES)
du.o: ../du.cpp
	$(CPP) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(INCLUDES)
%.o : %.cpp
	$(CPP) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ $(INCLUDES) 

clean : 
	rm -f $(TARGETS) *.o *.pyc
	rm -rf output
