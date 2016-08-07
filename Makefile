APP     = tradingApp
SRCEXT  = cpp
SRCDIR = trading/src
MAIN = app.cpp
SRCS := $(shell find $(SRCDIR) -name '*.$(SRCEXT)')
SRCDIRS := $(shell find $(SRCDIR) -name '*.$(SRCEXT)' -exec dirname {} \; | uniq)
OBJDIR  = obj
OBJS    := $(patsubst %.$(SRCEXT),$(OBJDIR)/%.o,$(SRCS))
OFLAGS  = -lPocoNet -lPocoUtil -lPocoFoundation
INCLUDE = -I./$(SRCDIR)
GCFLAGS = -Wall -std=c++14 -DEBUG
CFLAGS  = $(GCFLAGS) -c $(DEBUG) $(INCLUDE)
CPP     = g++
TESTDIR = trading/testsuite/src/


$(APP): buildrepo $(OBJS)
	@echo "$(CPP) $(OFLAGS) $(OBJS) -o $@"
	@$(CPP) $(OBJS) $(OFLAGS) -o $@
 
test: $(APP)
	$(CPP) $(GCFLAGS) $(INCLUDE) -I./$(TESTDIR) $(TESTDIR)Main.cpp $(TESTDIR)HTTPServerTest.cpp  $(OFLAGS) -lcppunit -o testsuite 

$(OBJDIR)/%.o: %.$(SRCEXT)
	@echo "$(CPP) $(CFLAGS) $< -o $@"
	@$(CPP) $(CFLAGS) $< -o $@
 
clean:
	$(RM) -r $(OBJDIR) $(APP) testsuite
 
buildrepo:
	$(call make-repo)
 
define make-repo
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR)/$$dir; \
	done
endef
