ifndef CONTIKI
CONTIKI=$(HOME)/contiki
endif
APPS+=powertrace
CONTIKI_WITH_RIME = 1
include $(CONTIKI)/Makefile.include

.PHONY : all
all : sink nodo baliza
To achieve 2 all should either be the first target defined in the make file or be assigned as the default goal:

.DEFAULT_GOAL := all
