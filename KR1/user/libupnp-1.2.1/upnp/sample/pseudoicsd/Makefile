#CC=mips-uclibc-gcc
INCLUDES= -I../../inc  -I ../../inc/tools
LIBS= -lpthread \
../../bin/$(TARGET)/libupnp.a \
../../bin/$(TARGET)/libixml.a \
../../bin/$(TARGET)/libthreadutil.a

ifeq ($(DEBUG),1)
OPT = -g -O1
else
OPT = -O1
endif

CFLAGS += -Wall $(OPT)   $(LDFLAGS) 

APPS = pseudoicsd

all: $(APPS)

pseudoicsd: PseudoICSd.o sample_util.o IPCon.o Obj.o PortMap.o
	$(STRIP) --strip-unneeded PseudoICSd.o  sample_util.o IPCon.o Obj.o PortMap.o
	$(CC)  $(CFLAGS) PseudoICSd.o  sample_util.o IPCon.o Obj.o PortMap.o $(LIBS) -o  $@ 
	@echo "make $@ finished on `date`"

%.o:	%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

clean:
	rm -f *.o $(APPS)
	rm -f *.gdb $(APPS)


