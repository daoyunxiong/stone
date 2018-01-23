###########################################################################
## Makefile generated for MATLAB file/project 'gnss_sins_ekf_loose_integration'. 
## 
## Makefile     : gnss_sins_ekf_loose_integration_rtw.mk
## Generated on : Mon Jun 12 06:52:35 2017
## MATLAB Coder version: 3.1 (R2016a)
## 
## Build Info:
## 
## Final product: $(RELATIVE_PATH_TO_ANCHOR)/gnss_sins_ekf_loose_integration.lib
## Product type : static-library
## 
###########################################################################

###########################################################################
## MACROS
###########################################################################

# Macro Descriptions:
# PRODUCT_NAME            Name of the system to build
# MAKEFILE                Name of this makefile
# COMPUTER                Computer type. See the MATLAB "computer" command.
# PERL                    PERL Tool
# GEN_LNK_SCRIPT          Perl script to generate the command file
# CMD_FILE                Command file

PRODUCT_NAME              = gnss_sins_ekf_loose_integration
MAKEFILE                  = gnss_sins_ekf_loose_integration_rtw.mk
COMPUTER                  = PCWIN64
MATLAB_ROOT               = E:/PROGRA~1/MATLAB/R2016a
MATLAB_BIN                = E:/PROGRA~1/MATLAB/R2016a/bin
MATLAB_ARCH_BIN           = E:/PROGRA~1/MATLAB/R2016a/bin/win64
MASTER_ANCHOR_DIR         = 
START_DIR                 = E:/workspace/matlab-20170430/matlab/sins
ARCH                      = win64
RELATIVE_PATH_TO_ANCHOR   = .
PERL                      = $(MATLAB_ROOT)/sys/perl/win32/bin/perl.exe
GEN_LNK_SCRIPT            = $(MATLAB_ROOT)/rtw/c/tools/mkvc_lnk.pl
CMD_FILE                  = $(PRODUCT_NAME).lnk

###########################################################################
## TOOLCHAIN SPECIFICATIONS
###########################################################################

# Toolchain Name:          LCC-win64 v2.4.1 | gmake (64-bit Windows)
# Supported Version(s):    2.4.1
# ToolchainInfo Version:   R2016a
# Specification Revision:  1.0
# 

#-----------
# MACROS
#-----------

SHELL              = cmd
LCC_ROOT           = $(MATLAB_ROOT)/sys/lcc64/lcc64
LCC_BUILDLIB       = $(LCC_ROOT)/bin/buildlib
LCC_LIB            = $(LCC_ROOT)/lib64
MW_EXTERNLIB_DIR   = $(MATLAB_ROOT)/extern/lib/win64/microsoft
MW_LIB_DIR         = $(MATLAB_ROOT)/lib/win64
TOOLCHAIN_INCLUDES = -I$(LCC_ROOT)/include64
MEX_OPTS_FILE      = $(MATLAB_ROOT/rtw/c/tools/lcc-win64.xml

TOOLCHAIN_SRCS = 
TOOLCHAIN_INCS = 
TOOLCHAIN_LIBS = 

#------------------------
# BUILD TOOL COMMANDS
#------------------------

# C Compiler: Lcc-win64 C Compiler
CC_PATH = $(LCC_ROOT)/bin
CC = $(CC_PATH)/lcc64

# Linker: Lcc-win64 Linker
LD_PATH = $(LCC_ROOT)/bin
LD = $(LD_PATH)/lcclnk64

# Archiver: Lcc-win64 Archiver
AR_PATH = $(LCC_ROOT)/bin
AR = $(AR_PATH)/lcclib64

# MEX Tool: MEX Tool
MEX_PATH = $(MATLAB_BIN)
MEX = $(MEX_PATH)/mex

# Download: Download
DOWNLOAD =

# Execute: Execute
EXECUTE = $(PRODUCT)

# Builder: GMAKE Utility
MAKE_PATH = %MATLAB%\bin\win64
MAKE = $(MAKE_PATH)/gmake


#-------------------------
# Directives/Utilities
#-------------------------

CDEBUG              = -g
C_OUTPUT_FLAG       = -Fo
LDDEBUG             =
OUTPUT_FLAG         = -o
ARDEBUG             =
STATICLIB_OUTPUT_FLAG = /out:
MEX_DEBUG           = -g
RM                  = @del /F
ECHO                = @echo
MV                  = @move
RUN                 =

#----------------------------------------
# "Faster Builds" Build Configuration
#----------------------------------------

ARFLAGS              =
CFLAGS               = -c -w -noregistrylookup -nodeclspec -I$(LCC_ROOT)/include64
DOWNLOAD_FLAGS       =
EXECUTE_FLAGS        =
LDFLAGS              = -s -L$(LCC_LIB) $(LDFLAGS_ADDITIONAL)
MEX_CFLAGS           = -win64 $(MEX_SRC) $(MEX_OPT_FILE)$(INCLUDES) -outdir $(RELATIVE_PATH_TO_ANCHOR)
MEX_LDFLAGS          = LINKFLAGS="$$LINKFLAGS $(LDFLAGS_ADDITIONAL)"
MAKE_FLAGS           = -f $(MAKEFILE)
SHAREDLIB_LDFLAGS    = -dll -entry LibMain -s -L$(LCC_LIB) $(LDFLAGS_ADDITIONAL) $(DEF_FILE)

#--------------------
# File extensions
#--------------------

H_EXT               = .h
OBJ_EXT             = .obj
C_EXT               = .c
EXE_EXT             = .exe
SHAREDLIB_EXT       = .dll
STATICLIB_EXT       = .lib
MEX_EXT             = .mexw64
MAKE_EXT            = .mk


###########################################################################
## OUTPUT INFO
###########################################################################

PRODUCT = $(RELATIVE_PATH_TO_ANCHOR)/gnss_sins_ekf_loose_integration.lib
PRODUCT_TYPE = "static-library"
BUILD_TYPE = "Static Library"

###########################################################################
## INCLUDE PATHS
###########################################################################

INCLUDES_BUILDINFO = -I$(START_DIR) -I$(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration -I$(MATLAB_ROOT)/extern/include -I$(MATLAB_ROOT)/simulink/include -I$(MATLAB_ROOT)/rtw/c/src -I$(MATLAB_ROOT)/rtw/c/src/ext_mode/common -I$(MATLAB_ROOT)/rtw/c/ert

INCLUDES = $(INCLUDES_BUILDINFO)

###########################################################################
## DEFINES
###########################################################################

DEFINES_STANDARD = -DMODEL=gnss_sins_ekf_loose_integration -DHAVESTDIO -DUSE_RTMODEL

DEFINES = $(DEFINES_STANDARD)

###########################################################################
## SOURCE FILES
###########################################################################

SRCS = $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/gnss_sins_ekf_loose_integration_rtwutil.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/gnss_sins_ekf_loose_integration_initialize.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/gnss_sins_ekf_loose_integration_terminate.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/gnss_sins_ekf_loose_integration.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/mrdivide.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/power.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/diag.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/norm.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/mod.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/var.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/mean.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/imu_update.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/eye.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/eul2Cbn.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/navrate.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/earthrad.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/glocal.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/cross.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/eul2q.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/qupdt.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/q2eul.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/expm.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/mldivide.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/xgetrf.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/discrete_process_noise.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/gnss_update.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/sind.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/cosd.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/wgsxyz2ned.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/rt_nonfinite.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/rtGetNaN.c $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/rtGetInf.c

ALL_SRCS = $(SRCS)

###########################################################################
## OBJECTS
###########################################################################

OBJS = gnss_sins_ekf_loose_integration_rtwutil.obj gnss_sins_ekf_loose_integration_initialize.obj gnss_sins_ekf_loose_integration_terminate.obj gnss_sins_ekf_loose_integration.obj mrdivide.obj power.obj diag.obj norm.obj mod.obj var.obj mean.obj imu_update.obj eye.obj eul2Cbn.obj navrate.obj earthrad.obj glocal.obj cross.obj eul2q.obj qupdt.obj q2eul.obj expm.obj mldivide.obj xgetrf.obj discrete_process_noise.obj gnss_update.obj sind.obj cosd.obj wgsxyz2ned.obj rt_nonfinite.obj rtGetNaN.obj rtGetInf.obj

ALL_OBJS = $(OBJS)

###########################################################################
## PREBUILT OBJECT FILES
###########################################################################

PREBUILT_OBJS = 

###########################################################################
## LIBRARIES
###########################################################################

LIBS = 

###########################################################################
## SYSTEM LIBRARIES
###########################################################################

SYSTEM_LIBS = 

###########################################################################
## ADDITIONAL TOOLCHAIN FLAGS
###########################################################################

#---------------
# C Compiler
#---------------

CFLAGS_BASIC = $(DEFINES) $(INCLUDES)

CFLAGS += $(CFLAGS_BASIC)

###########################################################################
## PHONY TARGETS
###########################################################################

.PHONY : all build clean info prebuild download execute


all : build
	@echo "### Successfully generated all binary outputs."


build : prebuild $(PRODUCT)


prebuild : 


download : build


execute : download


###########################################################################
## FINAL TARGET
###########################################################################

#---------------------------------
# Create a static library         
#---------------------------------

$(PRODUCT) : $(OBJS) $(PREBUILT_OBJS)
	$(PERL) $(GEN_LNK_SCRIPT) $(CMD_FILE) $(subst /,\,$(OBJS))
	@echo "### Creating static library "$(PRODUCT)" ..."
	$(AR) $(ARFLAGS) /out:$(PRODUCT) @$(CMD_FILE)
	@echo "### Created: $(PRODUCT)"
	$(RM) $(CMD_FILE)


###########################################################################
## INTERMEDIATE TARGETS
###########################################################################

#---------------------
# SOURCE-TO-OBJECT
#---------------------

%.obj : %.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


%.obj : $(RELATIVE_PATH_TO_ANCHOR)/%.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


%.obj : $(START_DIR)/%.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


%.obj : $(START_DIR)/codegen/lib/gnss_sins_ekf_loose_integration/%.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


%.obj : $(MATLAB_ROOT)/rtw/c/src/%.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


###########################################################################
## DEPENDENCIES
###########################################################################

$(ALL_OBJS) : $(MAKEFILE) rtw_proj.tmw


###########################################################################
## MISCELLANEOUS TARGETS
###########################################################################

info : 
	@echo "### PRODUCT = $(PRODUCT)"
	@echo "### PRODUCT_TYPE = $(PRODUCT_TYPE)"
	@echo "### BUILD_TYPE = $(BUILD_TYPE)"
	@echo "### INCLUDES = $(INCLUDES)"
	@echo "### DEFINES = $(DEFINES)"
	@echo "### ALL_SRCS = $(ALL_SRCS)"
	@echo "### ALL_OBJS = $(ALL_OBJS)"
	@echo "### LIBS = $(LIBS)"
	@echo "### MODELREF_LIBS = $(MODELREF_LIBS)"
	@echo "### SYSTEM_LIBS = $(SYSTEM_LIBS)"
	@echo "### TOOLCHAIN_LIBS = $(TOOLCHAIN_LIBS)"
	@echo "### CFLAGS = $(CFLAGS)"
	@echo "### LDFLAGS = $(LDFLAGS)"
	@echo "### SHAREDLIB_LDFLAGS = $(SHAREDLIB_LDFLAGS)"
	@echo "### ARFLAGS = $(ARFLAGS)"
	@echo "### MEX_CFLAGS = $(MEX_CFLAGS)"
	@echo "### MEX_LDFLAGS = $(MEX_LDFLAGS)"
	@echo "### DOWNLOAD_FLAGS = $(DOWNLOAD_FLAGS)"
	@echo "### EXECUTE_FLAGS = $(EXECUTE_FLAGS)"
	@echo "### MAKE_FLAGS = $(MAKE_FLAGS)"


clean : 
	$(ECHO) "### Deleting all derived files..."
	$(RM) $(subst /,\,$(PRODUCT))
	$(RM) $(subst /,\,$(ALL_OBJS))
	$(ECHO) "### Deleted all derived files."


