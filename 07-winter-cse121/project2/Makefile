SUBDIRS = cached_my_fs my_fs fat_fs inode_fs benchmark
COMMANDS = all clean

.PHONY: $(COMMANDS) new
$(COMMANDS): $(SUBDIRS)
	@for i in $^; do \
		echo; \
		cd $$i; \
		make $@; \
		cd ..; \
	done

new: clean all
