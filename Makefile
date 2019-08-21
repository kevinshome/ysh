#
# the yikes shell (ysh)
#
# Copyright 2019 kevinshome
#
# the yikes shell (ysh) is released under the Massachusettes Institute of Technology (MIT) License. terms
# of the license can be found in the "LICENSE" file. if this file is not available,
# a copy of the MIT License can also be found at https://opensource.org/licenses/MIT
#

# Makefile for the yikes shell (ysh)

CC = clang -I include -c
LD = clang
CCDEBUG = clang -g -Wall -I include -c
INCLUDES = -lreadline

CL := \033[1;34m
NC := \033[0m

folder := src/
files := ysh.c builtins.c remchar.c

.SILENT build:
	for file in $(files); do \
		echo " 	$(CL)CC $$file$(NC)" ; \
		$(CC) $(folder)$$file ; \
	done
	echo  " 	$(CL)LD *.o$(NC)"
	$(LD) $(INCLUDES) *.o -o ysh
	echo "Cleaning up..."
	rm *.o
	echo "Done!"

debug:
	for file in $(files); do \
		echo  " 	$(CL)CCDEBUG $$file$(NC)" ; \
		$(CCDEBUG) $(folder)$$file ; \
	done
	echo  " 	$(CL)LD *.o$(NC)"
	$(LD) $(INCLUDES) *.o -o ysh
	echo "Cleaning up..."
	rm *.o
	echo "Done!"

install:
	echo "Installing ysh to /usr/bin"
	sudo mv ysh /usr/bin/ysh
