APP     = tradingApp
SRCEXT  = cpp
SRCDIR = trading/src
SRCS    := $(shell find $(SRCDIR) -name '*.$(SRCEXT)')
SRCDIRS := $(shell find $(SRCDIR) -name '*.$(SRCEXT)' -exec dirname {} \; | uniq)
OBJDIR  = obj
OBJS    := $(patsubst %.$(SRCEXT),$(OBJDIR)/%.o,$(SRCS))
OFLAGS  = -lPocoNet -lPocoUtil -lPocoFoundation
INCLUDE = -I./trading/src
CFLAGS  = -Wall -std=c++14 -DEBUG -c $(DEBUG) $(INCLUDE)
CPP     = g++


$(APP): buildrepo $(OBJS)
	@echo "$(CPP) $(OFLAGS) $(OBJS) -o $@"
	@$(CPP) $(OBJS) $(OFLAGS) -o $@
 
$(OBJDIR)/%.o: %.$(SRCEXT)
	@echo "$(CPP) $(CFLAGS) $< -o $@"
	@$(CPP) $(CFLAGS) $< -o $@
 
clean:
	$(RM) -r $(OBJDIR) $(APP)
 
buildrepo:
	$(call make-repo)
 
define make-repo
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR)/$$dir; \
	done
endef
