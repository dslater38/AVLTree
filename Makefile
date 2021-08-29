
$(info windir $(WINDIR))

ifneq ($(WINDIR),)

CC?=cl64.cmd
CXX?=$(CC)
LD?=link64.cmd
OEXT?=obj
EXT?=.exe

CDEBUG?=-MDd -Od -Zi -DDEBUG
CRELEASE?=-MD -O2 -Ob3 -DNDEBUG
CXXSTD?=-std:c++17
OUTOPT?=-Fo:
EXEOPT?=-out:
CXXFLAGS+=-EHsc

else

CC?=clang
CXX?=clang++
OEXT?=o

CDEBUG?=-O0 -g -DDEBUG
CRELEASE?=-O3 -DNDEBUG
CXXSTD?=-std=c++17
OUTOPT?=-o
EXEOPT?="-o "

endif

TARGET=AVLTree$(EXT)

CXXFLAGS:=$(CXXFLAGS) $(COPTS) $(CXXSTD)

ifdef DEBUG
CXXFLAGS+=$(CDEBUG)
else
CXXFLAGS+=$(CRELEASE)
endif

OBJECTS=AVLTree.$(OEXT) main.$(OEXT)


$(TARGET) : $(OBJECTS)
	$(LD) $(EXEOPT)$(TARGET) $(LFLAGS) $(LIBS) $(OBJECTS)
	

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGET)

AVLTree.$(OEXT) : AVLTree.cpp AVLTree.h
main.$(OEXT) : main.cpp AVLTreeT.h


%.$(OEXT) : %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< $(OUTOPT) $@
