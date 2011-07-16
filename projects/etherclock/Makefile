#----------------------------------------------------------------------------
# Makefile for marrinator and projects
#----------------------------------------------------------------------------

include config.mk

debug release:
        make $@ -C $(M8R_SRC_DIR); exit_status=$$?; \
        if [ $$exit_status -ne 0 ]; then exit $$exit_status;

