#
# (C) Copyright 2004 Atheros Communications, Inc.
# All rights reserved.
#

# Set the HAL directory if you have the HAL sources
ifndef HAL 
export HAL=$(TOPDIR)/src/802_11/madwifi/hal/main
endif

# Set the Atheros Rate Control directory if you have the proprietary rate control
ifndef ATH_RATE 
export ATH_RATE=$(TOPDIR)/src/802_11/madwifi/ratectrl
endif
