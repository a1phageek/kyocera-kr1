#
# defaults
#
ifndef ATH_CAP_SUPERG_FF
export ATH_CAP_SUPERG_FF=1
endif

ifndef ATH_CAP_PHYERR_DIAG
export ATH_CAP_PHYERR_DIAG=1
endif

ifndef ATH_CAP_DYNTURBO
export ATH_CAP_DYNTURBO=1
endif

#
# directives
#
ifneq ($(strip ${ATH_CAP_SUPERG_FF}),0)
COPTS+= -DATH_SUPERG_FF=1
endif

ifneq ($(strip ${ATH_CAP_DYNTURBO}),0)
COPTS+= -DATH_SUPERG_DYNTURBO=1
#COPTS+= -DATH_SUPERG_DYNTURBO=1 -DATH_SUPERG_SCAN=1
endif

ifneq ($(strip ${ATH_CAP_PHYERR_DIAG}),0)
COPTS+= -DATH_PHYERR_DIAG=1
endif

