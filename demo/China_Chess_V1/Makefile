CC = gcc
LD = ld
TOPDIR = $(shell pwd)

MAKE = make

export CC LD TOPDIR

TARGET = chess

SUBDIRS = fyu/
SUBDIRS += kyo/

SUBLIBS := $(addsuffix built-in.o,$(SUBDIRS))

.PHONY: $(TARGET) $(SUBDIRS) $(SUBLIBS)

all:$(TARGET)

$(TARGET):$(SUBDIRS)
	@$(CC) $(SUBLIBS) -o $@ -lSDL -lSDL_image

$(SUBDIRS):
	$(MAKE) -C $@ all

clean:
	@for i in $(SUBDIRS);do make -C $$i clean;done
	@rm -f $(TARGET) 
