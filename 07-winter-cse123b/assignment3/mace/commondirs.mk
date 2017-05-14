.PHONY: $(DIRS) cvsignore svnignore

CLEANDIRS=$(addprefix clean_,$(DIRS))
CLEANERDIRS=$(addprefix cleaner_,$(DIRS))
SLASHDIRS=$(addsuffix /,$(DIRS))

TODO_FILTER=
ifdef TODO_IGNORES
TODO_FILTER+=| egrep -v -e "$(TODO_IGNORES)"
endif
ifdef TODO_INCLUDES
TODO_FILTER+=| egrep -e "$(TODO_INCLUDES)"
endif

dirs : $(DIRS)
cleandirs : $(CLEANDIRS)
cleanerdirs : $(CLEANERDIRS)

ifeq (,$(findstring wipe,$(MAKECMDGOALS)))
$(DIRS) : 
	@$(MAKE) DIR=$@ -C $@ $(DIRRULE)

$(CLEANDIRS) : clean_% :
	@$(MAKE) DIR=$* -C $* clean

$(CLEANERDIRS) : cleaner_% :
	@$(MAKE) DIR=$* -C $* cleaner

else
wipe : 
$(DIRS) : 
	@$(MAKE) DIR=$@ -C $@ clean
endif

svnignore : .cvsignore
	for i in $(DIRS); do $(MAKE) DIR=$$i -C $$i svnignore; done
	if [ -f .cvsignore ]; then svn propset svn:ignore -F .cvsignore .; fi

todos: 
	@echo "Making TODO list based on TODO, FIXME, and XXX in files"
	@svn -R list | egrep -v "commondirs.mk" $(TODO_FILTER) | xargs egrep -n "TODO|FIXME|XXX" | grep -v "XXXX"

$(SLASHDIRS) : %/ : %
