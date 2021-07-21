PRODUCT := fps
CXX := g++
LINKER := g++
INCDIRS := -I. -I./include
CXXFLAGS := -std=c++11 -Wall -Wextra

BINDIR= ../bin

SRCFILES := $(wildcard *.cpp)
OBJFILES := $(patsubst %.cpp,%.o,$(SRCFILES))
OBJs := $(patsubst %.cpp,$(BINDIR)/%.o,$(SRCFILES)) 
DEPFILES := $(patsubst %.cpp,%.d,$(SRCFILES))

$(PRODUCT): $(OBJFILES)
	$(LINKER) $(OBJs) -o $(BINDIR)/$@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $(BINDIR)/$@

%.d: %.cpp
	$(CXX) $(INCDIRS) -MM $< > $(BINDIR)/$@

clean:
	rm -rf $(BINDIR)/*

-include $(DEPFILES)
