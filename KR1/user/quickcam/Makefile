
EXEC1 = quickcam
OBJS1 = quickcam.o
EXEC2 = qcset
OBJS2 = qcset.o

all: $(EXEC1) $(EXEC3)

$(EXEC1): $(OBJS1)
	$(CC)  $(LDFLAGS) -o $@ $(OBJS1) $(LDLIBS) 

$(EXEC2): $(OBJS2)
	$(CC)  $(LDFLAGS) -o $@ $(OBJS2) $(LDLIBS) 

clean:
	rm -rf *.o
	rm -rf quickcam qset

romfs:
	@if egrep "^#define CONFIG_USB_QC_MODULE 1" ../../linux-2.4.x/include/linux/autoconf.h > /dev/null; then \
	$(ROMFSINST) /bin/quickcam; \
	fi

#	$(ROMFSINST) /bin/qcset

