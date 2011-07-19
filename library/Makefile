#----------------------------------------------------------------------------
# Makefile for marrinator and projects
#----------------------------------------------------------------------------

M8R_SRC_DIR = .
INSTALL_DIR = ~/Library/m8r


include shared.mk

all: ar
	
install: ar
	@echo
	@echo $(MSG_INSTALLING) $@
	$(MKDIR) -p $(INSTALL_DIR)/m8r
	$(COPY) $(LIB_MAIN_HEADER) $(INSTALL_DIR)/
	$(COPY) ${LIB_HEADERS:%.h=${LIB_HEADER_DIR}/%.h} $(INSTALL_DIR)/m8r
	$(COPY) $(LIB_SRC) $(INSTALL_DIR)/
	$(COPY) shared.mk $(INSTALL_DIR)/
