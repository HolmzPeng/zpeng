
OBJS := $(notdir $(obj-y))
SUBDIRS := $(subst ./,,$(dir $(obj-y)))
SUBLIBS := $(addsuffix built-in.o,$(SUBDIRS))

.PHONY:all

ifeq ($(obj-y),)

all:
	@rm -f built-in.o
	@$(AR) rcs built-in.o

else
	
all:$(SUBDIRS) $(OBJS)
	@$(LD) -r -o built-in.o $(OBJS) $(SUBLIBS)

endif

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ all

CFLAGS := -I$(TOPDIR)/include 

CURDIR := $(subst $(TOPDIR),,$(shell pwd))

%.o: %.c
	@$(CC) -c $(CFLAGS) -o $@ $<
	@echo "CC		.$(CURDIR)/$@"

.PHONY:clean
clean:
	@for i in $(SUBDIRS);do make -C $$i clean;done
	@rm -f $(OBJS) built-in.o
