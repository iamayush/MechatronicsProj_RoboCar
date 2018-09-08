#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/CCStudio_v7/bios_6_51_00_15/packages;C:/CCStudio_v7/ccsv7/ccs_base;C:/dan/omapl138/GE423Sp17/mystuff/omapSYSBIOS/.config
override XDCROOT = C:/CCStudio_v7/xdctools_3_50_02_20_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/CCStudio_v7/bios_6_51_00_15/packages;C:/CCStudio_v7/ccsv7/ccs_base;C:/dan/omapl138/GE423Sp17/mystuff/omapSYSBIOS/.config;C:/CCStudio_v7/xdctools_3_50_02_20_core/packages;..
HOSTOS = Windows
endif
