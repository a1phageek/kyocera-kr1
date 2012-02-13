EXEC1 = noip2
OBJS1 = noip2.o

FLTFLAGS += -s 16384
all: $(EXEC1)

$(EXEC1): $(OBJS1)
	$(CC) $(LDFLAGS) -o $@ $(OBJS1) $(LIBCRYPT) $(LDLIBS$(LDLIBS-$(@)))


romfs:
	$(ROMFSINST)  $(EXEC1) /bin/$(EXEC1)

clean:
	rm -f $(EXEC1) *.elf *.gdb *.o

