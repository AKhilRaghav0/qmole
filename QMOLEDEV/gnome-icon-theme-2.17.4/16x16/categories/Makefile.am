
size = 16x16
context = categories

iconsdir = $(themedir)/$(size)/$(context)

icons_DATA =				\
	applications-accessories.png	\
	applications-development.png	\
	applications-games.png	\
	applications-graphics.png	\
	applications-internet.png	\
	applications-multimedia.png	\
	applications-office.png	\
	preferences-desktop.png	\
	preferences-system.png	

EXTRA_DIST =				\
	$(icons_DATA)

MAINTAINERCLEANFILES =			\
	Makefile.in

install-data-local: install-iconsDATA
	(cd $(DESTDIR)$(themedir)/$(size) && $(ICONMAP) -c $(context))

check:
	if test -r CVS/Entries; then \
		for icon in *.png; do \
			if grep $$icon CVS/Entries > /dev/null; then \
				if ! grep $$icon Makefile > /dev/null; then \
					echo "Missing $$icon in Makefile.am"; \
					exit 1; \
				fi; \
			fi; \
		done; \
	else \
		for icon in *.png; do \
			if ! grep $$icon Makefile > /dev/null; then \
				echo "Missing $$icon in Makefile.am"; \
				exit 1; \
			fi; \
		done; \
	fi
