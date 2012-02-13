EXEC1 = ez-ipupdate
OBJS1 = ez-ipupdate.o conf_file.o md5.o cache_file.o pid_file.o

CFLAGS += -DHAVE_CONFIG_H -I. 

FLTFLAGS += -s 16384
all: $(EXEC1)

$(EXEC1): $(OBJS1)
	$(CC) $(LDFLAGS) -o $@ $(OBJS1) $(LDLIBS$(LDLIBS-$(@)))


romfs:
	$(ROMFSINST)  $(EXEC1) /bin/$(EXEC1)
	$(ROMFSINST) /var/dyndns.conf

clean:
	rm -f $(EXEC1) *.elf *.gdb *.o *.d

