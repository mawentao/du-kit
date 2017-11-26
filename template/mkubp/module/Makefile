WORK_ROOT=_WORKROOT_

###########################################################

AR   = ar
CC   = gcc
CPP  = g++
PERL = perl
PY   = python

###########################################################

CPPFLAGS = -I ./ -I ./include -I../common \
	-I$(WORK_ROOT)/public/ub/output/include \
	-I$(WORK_ROOT)/public/gm/galileo/output/include \
	-I$(WORK_ROOT)/public/bslext/output/include \
	-I$(WORK_ROOT)/public/comlog-plugin/output/include \
	-I$(WORK_ROOT)/public/connectpool/output \
	-I$(WORK_ROOT)/public/configure/output/include \
	-I$(WORK_ROOT)/public/idlcompiler/output/include \
	-I$(WORK_ROOT)/public/odict/output/include \
	-I$(WORK_ROOT)/lib2-64/dict/include \
	-I$(WORK_ROOT)/public/uconv/output/include \
	-I$(WORK_ROOT)/public/nshead/output/include \
	-I$(WORK_ROOT)/public/mcpack/output/include \
	-I$(WORK_ROOT)/public/spreg/output/include \
	-I$(WORK_ROOT)/third-64/pcre/include \
	-I$(WORK_ROOT)/lib2-64/bsl/include \
	-I$(WORK_ROOT)/lib2-64/ullib/include
	
LDLIBS = -L. -L../common -l_NAMESPACE_common \
	-L$(WORK_ROOT)/public/ub/output/lib -lub \
	-L$(WORK_ROOT)/public/gm/galileo/output/lib -lgalileo -lzookeeper_mt \
	-L$(WORK_ROOT)/public/bslext/output/lib -lbslext \
	-L$(WORK_ROOT)/public/comlog-plugin/output/lib -lcomlog -ldfsappender \
	-L$(WORK_ROOT)/public/connectpool/output -lconnectpool \
	-L$(WORK_ROOT)/public/configure/output/lib -lconfig \
	-L$(WORK_ROOT)/public/idlcompiler/output/lib -lmcpack_idl \
	-L$(WORK_ROOT)/public/odict/output/ -lodict \
	-L$(WORK_ROOT)/lib2-64/dict/lib -luldict \
	-L$(WORK_ROOT)/public/uconv/output -luconv \
	-L$(WORK_ROOT)/public/nshead/output/ -lnshead \
	-L$(WORK_ROOT)/public/mcpack/output/lib -lmcpack \
	-L$(WORK_ROOT)/public/spreg/output/lib -lspreg \
	-L$(WORK_ROOT)/third-64/pcre/lib -lpcre \
	-L$(WORK_ROOT)/lib2-64/bsl/lib -lbsl \
	-L$(WORK_ROOT)/lib2-64/ullib/lib -lullib \
	-lpthread -lcrypto -lstdc++ -lz

CXXFLAGS = -g -W -Wall -Werror -Wno-unused-parameter -pipe 

###########################################################

SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp,%.o,$(SRC)) main_proc.o 
BIN = _MODULE_NAME_

.PHONY: all clean

all : $(BIN)
	rm -rf output
	mkdir -p output
	cp $^ output/
	cp -r conf output/conf
	rm -f *.o

_MODULE_NAME_ : $(OBJ)
	$(CPP) -o $@ $^ $(LDLIBS) 

main_proc.o : procs/main_proc.cpp
	$(CPP) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ $(INCLUDES) 

%.o : %.cpp
	$(CPP) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ $(INCLUDES) 

clean : 
	rm -f $(BIN) *.o *.pyc
	rm -rf output
