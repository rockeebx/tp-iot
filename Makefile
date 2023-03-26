CONTIKI_PROJECT = experiment1
all: $(CONTIKI_PROJECT)

#UIP_CONF_IPV6=1

CONTIKI =  /home/user/contiki
include $(CONTIKI)/Makefile.include
