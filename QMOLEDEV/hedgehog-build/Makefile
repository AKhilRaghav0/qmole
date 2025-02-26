# This file is part of Hedgehog LISP.
# Copyright (C) 2003, 2004, 2005 Oliotalo Ltd.
# See file LICENSE.LGPL for pertinent licensing conditions.
#
# Author: Kenneth Oksanen <cessu@iki.fi>
#   	  Lars Wirzenius <liw@iki.fi>
#

srcdir = /home/chris/hedge_build/../hedgehog-2.1
VPATH = ../hedgehog-2.1

HHVER = $(shell awk -f $(srcdir)/hh_version.awk $(srcdir)/hh_common.h)

CC = gcc
CFLAGS = -O -g -I. -I$(srcdir) -DHH_BSD -DHH_SHARED_SECRET=\"\" -DHEDGEHOG_VERSION_STRING='"$(HHVER)"' \
    -DDEFAULT_PRELUDE='"$(prefix)/lib/hh/prelude.d$(suffix)"'
LIBS = 
LD = $(CC)
LFLAGS = 
LD2 = :
AWK = gawk

XCC = $(CC)
XCFLAGS = -O -g -I. -I$(srcdir) -DHH_BSD -DHH_TESTING -DHEDGEHOG_VERSION_STRING='"$(HHVER)"'
XLIBS = 
XLD = $(CC)
XLFLAGS = 
XLD2 = :
XEXEC = ./

platform = hh_interp_unix.o
boot = 
targets = hh_seed.h hhc hhi 300-unix.hl
prefix = /usr/local
suffix = 


all: $(targets) prelude.d

ver:
	echo $(HHVER)

hhc$(suffix): hh_lex.o hh_ast.o hh_compiler.o hh_lambda.o hh_macroexpand.o \
    hh_opt.o hh_uses.o hh_codegen.o hh_data_compiler.o hh_peephole.o \
    hh_output.o
	$(LD) $(LFLAGS) -o $@ $^ $(LIBS)
	$(LD2) 

hhi$(suffix): hh_data.o hh_interp.o hh_error.o hh_avl.o hh_printf.o \
    hh_crypto.o $(platform) $(boot)
	$(XLD) $(XLFLAGS) -o $@ $^ $(XLIBS)
	$(XLD2) 

hh_seed: hh_seed.o hh_crypto_host.o
	$(LD) $(LFLAGS) -o $@ $^ $(LIBS)
	$(LD2) 

hh_seed.h: hh_insn.def hh_seed
	rm -f hh_seed.h 
	./hh_seed < $(srcdir)/hh_insn.def > hh_seed.h

hh_avl.o hh_interp.o hh_data.o hh_error.o hh_printf.o hh_crypto.o $(platform):
	$(XCC) $(XCFLAGS) -c $<

hh_lex.c: hh_lex.l
	flex -t -Phh_ $(srcdir)/hh_lex.l > hh_lex.c

hh_data_compiler.c: hh_data.c
	echo '#define HH_COMPILER  1' > hh_data_compiler.c
	cat $(srcdir)/hh_data.c >> hh_data_compiler.c

hh_crypto_host.c: hh_crypto.c
	cp $(srcdir)/hh_crypto.c hh_crypto_host.c

hh_boot.h: hh_boot.hl hhc hh_seed.h
	./hhc -X -o hh_boot.h -p $(srcdir)/prelude.d $(srcdir)/hh_boot.hl

prelude.d: 300-unix.hl dummy
	if [ ! -d prelude.d ]; \
	then \
	  mkdir prelude.d; \
	fi; \
	cp -p $(srcdir)/prelude.d/* 300-unix.hl prelude.d
dummy:

300-unix: 300-unix.o
	$(XLD) $(XLFLAGS) -o $@ $^ $(XLIBS)
	$(XLD2) 

300-unix.o: 300-unix.c
	$(XCC) $(XCFLAGS) -I$(srcdir)/prelude.d -c $<

300-unix.c: prelude.d/interface-gen.pl prelude.d/interface-gen.c \
            prelude.d/interface-unix
	rm -f 300-unix.c
	perl $(srcdir)/prelude.d/interface-gen.pl unix- \
	    < $(srcdir)/prelude.d/interface-unix > 300-unix.c

300-unix.hl: 300-unix
	rm -f 300-unix.hl
	$(XEXEC)300-unix > 300-unix.hl

def2doc$(suffix): def2doc.o
	$(LD) $(LFLAGS) -o $@ $^ $(LIBS)
	$(LD2) 

hh_builtins.incl: def2doc hh_builtins.def
	./def2doc > hh_builtins.incl

prelude.incl: prelude.d dummy
	env LC_COLLATE=C sh -c 'cat prelude.d/*.hl' > prelude.hl
	python $(srcdir)/hhdoc.py prelude.hl > prelude.incl

doc: linked-list.png hh.html hh.ps hh.pdf

linked-list.png: linked-list.fig
	fig2dev -L png $(srcdir)/linked-list.fig linked-list.png

hh.html: hh.docbook hh_builtins.incl prelude.incl docbook2xhtml.xsl hh_common.h
	cp $(srcdir)/hh.docbook $(srcdir)/oliodoc.css .
	xsltproc --stringparam version \
	    `awk -f $(srcdir)/hh_version.awk $(srcdir)/hh_common.h` \
	    --novalid -o hh.html $(srcdir)/docbook2xhtml.xsl \
	    hh.docbook
	rm -f hh.docbook

hh.ps: $(srcdir)/hh.html2psrc hh.docbook hh_builtins.incl prelude.incl \
       docbook2xhtml.xsl hh_common.h
	cp $(srcdir)/hh.docbook .
	xsltproc --stringparam version \
	    `awk -f $(srcdir)/hh_version.awk $(srcdir)/hh_common.h` \
	    --novalid -o hh.temp $(srcdir)/docbook2xhtml2ps.xsl \
	    hh.docbook
	rm -f hh.docbook
	html2ps -o hh.ps -D -C bh -n -t -x 1 -f $(srcdir)/hh.html2psrc hh.temp
	rm -f hh.temp

hh.pdf: hh.ps
	ps2pdf hh.ps

TAGS: $(OBJS)
	etags *.h *.c

install:
	install -d $(prefix)/bin $(prefix)/lib/hh/prelude.d$(suffix)
	install hhc$(suffix) hhi$(suffix) $(prefix)/bin
	install prelude.d/*.hl $(prefix)/lib/hh/prelude.d$(suffix)

clean:
	rm -f core *~ *.o *.a \#* hh_lex.c *.axf *.incl def2doc
	rm -f def2doc hhc hhi hh_seed builtins.incl linked-list.png hh_seed.h
	rm -f hhi.bin *.exe hh_boot.cod hh_boot.asm hh_boot.h
	rm -f hh_data_compiler.c hh_crypto_host.c 
	rm -f hh.html hh.ps hh.pdf oliodoc.css prelude.hl
	rm -f 300-*
	rm -rf prelude.d


# The dependencies below this line have been generated with `gcc *.c -MM'.
# And then duplicate the dependencies of `hh_data.o' for `hh_data_compiler.o'.
def2doc.o: def2doc.c hh_common.h hh_builtins.def
hh_ast.o: hh_ast.c hh_common.h hh_ast.h hh_builtins.def hh_data.h \
  hh_interp.h hh_error.h hh_error.def hh_insn.def
hh_avl.o: hh_avl.c hh_common.h hh_error.h hh_error.def hh_avl.h hh_data.h \
  hh_interp.h hh_insn.def hh_printf.h
hh_codegen.o: hh_codegen.c hh_common.h hh_ast.h hh_builtins.def hh_data.h \
  hh_interp.h hh_error.h hh_error.def hh_insn.def hh_codegen.h \
  hh_lambda.h
hh_compiler.o: hh_compiler.c hh_common.h hh_ast.h hh_builtins.def \
  hh_macroexpand.h hh_opt.h hh_lambda.h hh_uses.h hh_codegen.h \
  hh_interp.h hh_error.h hh_error.def hh_insn.def hh_peephole.h \
  hh_output.h
hh_crypto.o: hh_crypto.c hh_common.h hh_interp.h hh_error.h hh_error.def \
  hh_insn.def
hh_data.o: hh_data.c hh_common.h hh_error.h hh_error.def hh_interp.h \
  hh_insn.def hh_data.h hh_printf.h hh_avl.h
hh_data_compiler.o: hh_data.c hh_common.h hh_error.h hh_error.def hh_interp.h \
  hh_insn.def hh_data.h hh_printf.h hh_avl.h
hh_error.o: hh_error.c hh_common.h hh_error.h hh_error.def hh_interp.h \
  hh_insn.def hh_data.h hh_printf.h
hh_interp.o: hh_interp.c hh_common.h hh_interp.h hh_error.h hh_error.def \
  hh_insn.def hh_data.h hh_printf.h hh_avl.h hh_crypto.h hh_seed.h
hh_interp_unix.o: hh_interp_unix.c hh_common.h hh_interp.h hh_error.h \
  hh_error.def hh_insn.def hh_data.h hh_printf.h
hh_lambda.o: hh_lambda.c hh_common.h hh_ast.h hh_builtins.def
hh_lex.o: hh_lex.c hh_common.h hh_ast.h hh_builtins.def
hh_macroexpand.o: hh_macroexpand.c hh_common.h hh_ast.h hh_builtins.def \
  hh_macroexpand.h
hh_opt.o: hh_opt.c hh_common.h hh_ast.h hh_builtins.def
hh_output.o: hh_output.c hh_common.h hh_codegen.h hh_ast.h \
  hh_builtins.def hh_interp.h hh_error.h hh_error.def hh_insn.def \
  hh_output.h hh_data.h hh_seed.h
hh_peephole.o: hh_peephole.c hh_common.h hh_data.h hh_interp.h hh_error.h \
  hh_error.def hh_insn.def hh_codegen.h hh_ast.h hh_builtins.def
hh_printf.o: hh_printf.c hh_printf.h
hh_seed.o: hh_seed.c hh_crypto.h hh_common.h
hh_uses.o: hh_uses.c hh_common.h hh_ast.h hh_builtins.def
