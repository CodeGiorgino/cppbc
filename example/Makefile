OBJDIR := obj
DEPDIR := deps
SRCDIR := src

BIN  := main.out
SRCS := $(SRCDIR)/main.cpp

DISTFILES  := $(BIN)
DISTOUTPUT := $(BIN).tar.gz

OBJS := $(patsubst %,$(OBJDIR)/%.o,$(basename $(SRCS)))
DEPS := $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))

$(shell mkdir -p $(dir $(OBJS)) >/dev/null)
$(shell mkdir -p $(dir $(DEPS)) >/dev/null)

CXX := g++
LD  := g++
TAR := tar

CXXFLAGS := -std=c++23
CPPFLAGS := -g -Wall -Wextra -pedantic
DEPFLAGS = -MT $@ -MD -MP -MF $(DEPDIR)/$*.Td

COMPILE.cc  = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c -o $@
LINK.o      = $(LD) $(LDFLAGS) $(LDLIBS) -o $@
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

all: $(BIN)

dist: $(DISTFILES)
	$(TAR) -cvzf $(DISTOUTPUT) $^

.PHONY: clean
clean:
	$(RM) -r $(OBJDIR) $(DEPDIR)

.PHONY: distclean
distclean: clean
	$(RM) $(BIN) $(DISTOUTPUT)

.PHONY: help
help:
	@echo "Makefile available targets: all dist clean distclean"

$(BIN): $(OBJS)
	$(LINK.o) $^ ../cppbc.a

$(OBJDIR)/%.o: %.cpp
$(OBJDIR)/%.o: %.cpp $(DEPDIR)/%.d
	$(PRECOMPILE)
	$(COMPILE.cc) $<
	$(POSTCOMPILE)

.PRECIOUS: $(DEPDIR)/%.d
$(DEPDIR)/%.d: ;

-include $(DEPS)
