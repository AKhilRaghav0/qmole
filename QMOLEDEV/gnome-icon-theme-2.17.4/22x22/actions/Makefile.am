
size = 22x22
context = actions

iconsdir = $(themedir)/$(size)/$(context)

icons_DATA =				\
	address-book-new.png		\
	application-exit.png		\
	appointment-new.png		\
	bookmark-new.png		\
	contact-new.png	\
	document-new.png		\
	document-open.png		\
	document-print.png		\
	document-print-preview.png	\
	document-properties.png		\
	document-save.png		\
	document-save-as.png		\
	edit-copy.png			\
	edit-cut.png			\
	edit-delete.png			\
	edit-find.png			\
	edit-find-replace.png		\
	edit-paste.png			\
	edit-redo.png			\
	edit-select-all.png		\
	edit-undo.png			\
	folder-new.png			\
	format-indent-less.png		\
	format-indent-more.png		\
	format-justify-center.png	\
	format-justify-fill.png		\
	format-justify-left.png		\
	format-justify-right.png	\
	format-text-bold.png		\
	format-text-italic.png		\
	format-text-strikethrough.png	\
	format-text-underline.png	\
	go-bottom.png			\
	go-down.png			\
	go-first.png			\
	go-home.png			\
	go-last.png			\
	go-next.png			\
	go-previous.png			\
	go-top.png			\
	go-up.png			\
	gtk-edit.png			\
	help-faq.png			\
	list-add.png			\
	list-remove.png			\
	mail-forward.png		\
	mail-mark-junk.png		\
	mail-mark-not-junk.png		\
	mail-message-new.png		\
	mail-reply-all.png		\
	mail-reply-sender.png		\
	mail-send-receive.png		\
	media-playback-pause.png	\
	media-playback-start.png	\
	media-playback-stop.png		\
	media-record.png		\
	media-seek-backward.png		\
	media-seek-forward.png		\
	media-skip-backward.png		\
	media-skip-forward.png		\
	object-flip-horizontal.png	\
	object-flip-vertical.png	\
	object-rotate-left.png		\
	object-rotate-right.png		\
	process-stop.png		\
	system-lock-screen.png		\
	system-run.png			\
	tab-new.png			\
	view-fullscreen.png		\
	view-refresh.png 		\
	view-restore.png		\
	view-sort-ascending.png		\
	view-sort-descending.png	\
	window-close.png		\
	window-new.png			\
	zoom-best-fit.png		\
	zoom-in.png			\
	zoom-original.png		\
	zoom-out.png

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
