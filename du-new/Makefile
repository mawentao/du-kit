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

TARGETS = du-newsh du-newpy du-newh du-newcpp du-newclass \
          du-newhtml du-newcss \
          du-newjs du-newjsgrid du-newjsdialog du-newjscombox \
          du-newphp du-newphpct du-newphptool du-newdztable \
          du-newppt
OBJECTS = du.o Configure.o

.PHONY: all clean

all : $(TARGETS)
	rm -rf output
	mkdir -p output
	cp $^ output/
	rm -f *.o

du-newsh : newsh.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS) 
du-newpy : newpy.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS) 
du-newh : newh.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS) 
du-newcpp : newcpp.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS) 
du-newclass : newclass.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS) 
du-newhtml : newhtml.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS) 
du-newjs : newjs.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS) 
du-newjsgrid : newjsgrid.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS) 
du-newjsdialog : newjsdialog.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS) 
du-newjscombox : newjscombox.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS) 
du-newcss : newcss.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS) 
du-newphp : newphp.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS)
du-newphpct : newphpct.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS)
du-newphptool : newphptool.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS)
du-newdztable : newdztable.o $(OBJECTS)
	$(CPP) -o $@ $^ $(LDLIBS)
du-newppt : newppt.o $(OBJECTS)
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
