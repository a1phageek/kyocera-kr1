
# Establish working directory and pathnames.
TOP:=$(shell pwd)

# The Madwifi Makefiles must have environment variables
# HAL and ATH_RATE defined for properly compiling
# with HAL source and RATE source.
export HAL=$(TOP)/hal

all: driver 

clean:
	( make -C $(HAL)/linux clean )
	( make -C $(TOP)/madwifi clean)
	( make -C $(TOP)/hostapd-0.3.7 clean)
	( make -C $(TOP)/ratectrl clean )
	( rm -f module/* 1.log )
	( rm -f $(TOP)/ratectrl/*.o )

driver:
# Build HAL
	( make -C $(HAL)/linux release TARGET=mips1-be-elf  )
# Build Madwifi using HAL
	 if  ( test -e  $(TOP)/ratectrl )  then export ATH_RATE=$(TOP)/ratectrl;  fi; \
	 make -C $(TOP)/madwifi

install:
	 if  ( test -e  $(TOP)/ratectrl )  then export ATH_RATE=$(TOP)/ratectrl;  fi; \
	 make -C $(TOP)/madwifi install 

