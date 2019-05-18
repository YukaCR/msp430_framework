.PHONY: all

SOURCES = main.cpp\
TIDriver/dma.cpp\
TIDriver/usci_b_i2c.cpp\
TIDriver/ucs.cpp


GFX = $(wildcard /home/yukacr/Workspace/msp430_framework/MSP_GFX/grlib/*.cpp) 
TIDriver = $(wildcard /home/yukacr/Workspace/msp430_framework/TIDriver/*.cpp)
LIBRARIES = $(wildcard Debug/obj/**.obj)
LIBRARIES_SOURCE = $(wildcard F5529PocketBoard/**/**.cpp)\
$(wildcard F5529PocketBoard/**.cpp)
FLAGS = -fb=/home/yukacr/Workspace/msp430_framework/Debug/temp\
-fs=/home/yukacr/Workspace/msp430_framework/Debug/temp\
-fr=/home/yukacr/Workspace/msp430_framework/Debug/temp\
-ft=/home/yukacr/Workspace/msp430_framework/Debug/temp\
-DARDUINO=101\
-DENERGIA=12\
-DF_CPU=25000000\
--c++14\
--opt_level=0\
--copy_compression=none\
--cinit_compression=none
INCLUDE = -I/home/yukacr/ti-cgt-msp430_18.12.1.LTS/msp430_lib/include/\
-I/home/yukacr/Workspace/ti-cgt-msp430_18.12.1.LTS/include/libcxx\
-I/home/yukacr/Workspace/ti-cgt-msp430_18.12.1.LTS/include\
-I/home/yukacr/ti-cgt-msp430_18.12.1.LTS/include/\
-I/home/yukacr/Workspace/msp430_framework/\
-I/home/yukacr/ti-cgt-msp430_18.12.1.LTS/lib/\
-I/home/yukacr/Workspace/msp430_framework/MSP_GFX/fonts\
-I/home/yukacr/Workspace/msp430_framework/MSP_GFX/grlib\
-I/home/yukacr/Workspace/msp430_framework/TIDriver\

GCC = /home/yukacr/Workspace/ti-cgt-msp430_18.12.1.LTS/bin/cl430

all: default
default:
	$(GCC) --define=__MSP430F5529__ $(FLAGS) $(INCLUDE) -ggdb --silicon_version=mspx $(SOURCES) -z -m Link_result.txt lnk_msp430f5529.cmd -o main.elf;
lib:
	rm *.obj
	rm -rf Debug/obj/
	mkdir Debug/obj
	$(GCC) --define=__MSP430F5529__  $(INCLUDE) -ggdb --silicon_version=mspx $(LIBRARIES_SOURCE) -c
	mv *.obj Debug/obj
	rm Debug/obj/ucs.obj
upload:
	$(GCC) --define=__MSP430F5529__  $(INCLUDE) -ggdb --silicon_version=mspx $(LIBRARIES) $(SOURCES) -z -m Link_result.txt lnk_msp430f5529.cmd -o main.elf;
	mspdebug --allow-fw-update tilib "prog main.elf" gdb
gfx:
	$(GCC) --define=__MSP430F5529__ $(FLAGS) $(INCLUDE) -ggdb --silicon_version=mspx $(SOURCES) $(GFX) $(TIDriver) -z -m Link_result.txt lnk_msp430f5529.cmd -o main.elf;
