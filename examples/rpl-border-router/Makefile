CONTIKI_PROJECT = border-router
MAKE_ROUTING = MAKE_ROUTING_RPL_CLASSIC

all: $(CONTIKI_PROJECT)
CONTIKI = ../..

# The BR is either native or embedded, and in the latter case must support SLIP
PLATFORMS_EXCLUDE = z1

# Include RPL BR module
include $(CONTIKI)/Makefile.dir-variables
MODULES += $(CONTIKI_NG_SERVICES_DIR)/rpl-border-router
# Include webserver module
MODULES_REL += webserver

include $(CONTIKI)/Makefile.include
