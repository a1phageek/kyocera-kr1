
NMALLOC = 
INCLUDES = -I.. -I../lib

CFLAGS += -DHAVE_CONFIG_H -DSYSCONFDIR=\"/etc/config/\"

OBJS    = vtysh.o vtysh_cmd.o vtysh_config.o vtysh_main.o \
			vtysh_user.o 

all: vtysh 

ripd: $(OBJS) $(NMALLOC) ../lib/libzebra.a
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(NMALLOC) ../lib/libzebra.a $(LIBCRYPT) $(LDLIBS)

romfs:
	$(ROMFSINST) /bin/vtysh

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) -c $<

clean:
	rm -f *.gdb *.elf *.o vtysh 

dummy_target:

nfs: dummy_target
	@$(MAKE) MAKE_NFS=1

