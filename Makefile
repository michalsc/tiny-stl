include version.mk

#CXX := /usr/bin/clang++
CXX ?= /usr/local/bin/g++-8
CXXFLAGS:= -std=c++11 -Os -Iinclude -pedantic -pedantic-errors -Wall -Wextra -Werror -c -fmessage-length=0 
LDFLAGS := Build/libtinystd.a

HOST_CXX := /usr/bin/clang++
HOST_CXXFLAGS := -Os -std=c++11 -Iinclude
HOST_LDFLAGS := BuildTest/libtinystd.a

VERSTRING := -DVERSION_STRING='$(VERSION_STRING_DATE)'

LIB_OBJS := tinystd/string.o

OBJS := main.o support.o

OBJDIR := Build

TESTOBJS := tests/string_test.o tests/list_test.o support.o tests/run_tests.o

TESTOBJDIR := BuildTest

all: pre-build main-build

pre-build:
	@mkdir -p $(OBJDIR) >/dev/null

main-build: pre-build
	@make --no-print-directory $(OBJDIR)/MiniStd

test:
	@make --no-print-directory $(TESTOBJDIR)/MiniStdTest
	@$(TESTOBJDIR)/MiniStdTest

$(TESTOBJDIR)/MiniStdTest: $(addprefix $(TESTOBJDIR)/, $(TESTOBJS)) $(TESTOBJDIR)/libtinystd.a
	@echo "Building test: $@"
	@$(HOST_CXX) $(foreach f,$(TESTOBJS),$(TESTOBJDIR)/$(f)) $(HOST_LDFLAGS) -o $@

$(OBJDIR)/MiniStd: $(addprefix $(OBJDIR)/, $(OBJS)) $(OBJDIR)/libtinystd.a
	@echo "Building target: $@"
	@$(CXX) $(foreach f,$(OBJS),$(OBJDIR)/$(f)) $(LDFLAGS) -o $@
	@echo "Build completed"

$(OBJDIR)/libtinystd.a: $(addprefix $(OBJDIR)/, $(LIB_OBJS))
	@echo "Building target: $@"
	@ar -r $@ $(foreach f,$(LIB_OBJS),$(OBJDIR)/$(f)) 2>/dev/null
	@echo "Build completed"

$(TESTOBJDIR)/libtinystd.a: $(addprefix $(TESTOBJDIR)/, $(LIB_OBJS))
	@echo "Building target: $@"
	@ar -r $@ $(foreach f,$(LIB_OBJS),$(TESTOBJDIR)/$(f)) 2>/dev/null
	@echo "Build completed"

.PHONY: all clean
.SECONDARY: main-build pre-build test

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@echo "Compiling: $*.cpp"
	@$(CXX) -c $(CXXFLAGS) $(VERSTRING) $< -o $@

$(OBJDIR)/%.d: %.cpp
	@mkdir -p $(@D)
	@set -e; rm -f $@; \
         $(CXX) -MM -MT $(basename $@).o $(CXXFLAGS) $(VERSTRING) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

$(TESTOBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@echo "Compiling: $*.cpp"
	@$(HOST_CXX) -c $(HOST_CXXFLAGS) $< -o $@

$(TESTOBJDIR)/%.d: %.cpp
	@mkdir -p $(@D)
	@set -e; rm -f $@; \
         $(HOST_CXX) -MM -MT $(basename $@).o $(HOST_CXXFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

clean:
	rm -rf *.o *.d $(OBJDIR) $(TESTOBJDIR)

-include $(foreach f,$(OBJS:.o=.d),$(OBJDIR)/$(f))
-include $(foreach f,$(TESTOBJS:.o=.d),$(TESTOBJDIR)/$(f))
