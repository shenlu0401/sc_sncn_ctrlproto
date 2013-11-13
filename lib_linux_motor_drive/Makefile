UNAME := $(shell uname)
MASTER_INSTALLDIR := /opt/etherlab
INCLUDE_DIRS = -I include
INCLUDE_DIRS += -I ../app_linux_ctrlproto-master-example/config/motor
INCLUDE_DIRS += -I ../lib_linux_ctrlproto/include/
INCLUDE_DIRS += -I ../inc_ctrlproto-common/
INCLUDE_DIRS += -I ../../sc_sncn_ethercat/module_ethercat/src/canod/
INCLUDE_DIRS += -I $(MASTER_INSTALLDIR)/include
INCLUDE_DIRS += -I ../../sc_sncn_motorctrl_sin/module_profile/include
SOURCES = ../../sc_sncn_motorctrl_sin/module_profile/profile_position_mode/profile_position.c
LIB_DIRS += -L $(MASTER_INSTALLDIR)/lib

LIBS += -lethercat

all:

ifneq "$(UNAME)" "Linux"
		@echo
		@echo "----------------------------------------------------------------------------------------------------"
		@echo "||Your System isn't Linux, so the ctrlproto libary for the Linux Ethercat Master can not be built!||"
		@echo "----------------------------------------------------------------------------------------------------"
		@echo
else
		@if test -d $(MASTER_INSTALLDIR); \
		then \
		mkdir lib -p;\
		gcc ../../sc_sncn_motorctrl_sin/module_profile/profile_position_mode/profile_position.c $(INCLUDE_DIRS) -o lib/libsncn_profile.so -lstdc++ -shared -fPIC;\
		gcc -c ../../sc_sncn_motorctrl_sin/module_profile/profile_position_mode/profile_position.c $(INCLUDE_DIRS) -o lib/libsncn_profile.o;\
		gcc src/drive_function.c  $(INCLUDE_DIRS) $(LIBS) $(LIB_DIRS) -o lib/libsncn_drive_function.so -lstdc++ -shared -fPIC;\
		gcc -c src/drive_function.c  $(INCLUDE_DIRS) $(LIBS) $(LIB_DIRS) -o lib/libsncn_drive_function.o;\
		ar rcs lib/libsncn_motor_drive.a lib/*.o;\
		rm -f lib/*.o;\
		else echo \
		"\n\
		-----------------------------------------------------------------------------------------------------------------\n\
		||Ethercat Master is not installed at given directory if you installed it elsewhere change location in Makefile||\n\
		-----------------------------------------------------------------------------------------------------------------\n";\
		fi
endif

clean:
	rm -f lib/*.so
	rm -f lib/*.a