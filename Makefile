OBJDIR := obj
DEPDIR := deps
SRCDIR := src

BIN  := cppbc.a
SRCS := $(SRCDIR)/cppbc.cpp

DISTFILES  := $(BIN)
DISTOUTPUT := $(BIN).tar.gz

OBJS := $(patsubst %,$(OBJDIR)/%.o,$(basename $(SRCS)))
DEPS := $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))

$(shell mkdir -p $(dir $(OBJS)) >/dev/null)
$(shell mkdir -p $(dir $(DEPS)) >/dev/null)

CXX := g++
LD  := ar
TAR := tar

CXXFLAGS := -std=c++23
CPPFLAGS := -g -Wall -Wextra -pedantic
LDFLAGS  := rvs
DEPFLAGS = -MT $@ -MD -MP -MF $(DEPDIR)/$*.Td

COMPILE.cc  = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c -o $@
LINK.o      = $(LD) $(LDFLAGS) $(LDLIBS) $@
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

all: $(BIN)

dist: $(DISTFILES)
	$(TAR) -cvzf $(DISTOUTPUT) $^

example: $(BIN)
	cd ./example; \
		make -B && ./main.out

.PHONY: clean
clean:
	$(RM) -r $(OBJDIR) $(DEPDIR)

.PHONY: distclean
distclean: clean
	$(RM) $(BIN) $(DISTOUTPUT)

.PHONY: help
help:
	@echo "Available targets: help all dist example clean distclean"

$(BIN): $(OBJS)
	$(LINK.o) $^

$(OBJDIR)/%.o: %.cpp
$(OBJDIR)/%.o: %.cpp $(DEPDIR)/%.d
	$(PRECOMPILE)
	$(COMPILE.cc) $<
	$(POSTCOMPILE)

.PRECIOUS: $(DEPDIR)/%.d
$(DEPDIR)/%.d: ;

-include $(DEPS)
