#### Makefile for documentation other than the Emacs manual.

# Copyright (C) 1994, 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003,
#   2004, 2005, 2006, 2007, 2008, 2009, 2010 Free Software Foundation, Inc.

# This file is part of GNU Emacs.

# GNU Emacs is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# GNU Emacs is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.


# Where to find the source code.  $(srcdir) will be the man-aux
# subdirectory of the source tree.  This is
# set by the configure script's `--srcdir' option.
srcdir=/home/chris/emacs-23.2.x11/doc/misc
top_srcdir=/home/chris/emacs-23.2.x11

# Tell make where to find source files; this is needed for the makefiles.
VPATH=/home/chris/emacs-23.2.x11/doc/misc

## Where the output files go.
## Note that the setfilename command in the .texi files assumes this.
infodir=../../info

# The makeinfo program is part of the Texinfo distribution.
# Use --force so that it generates output even if there are errors.
MAKEINFO = makeinfo --force

# Also add new entries to INFO_FILES in the top-level Makefile.in.
INFO_TARGETS = \
	$(infodir)/ada-mode \
	$(infodir)/auth \
	$(infodir)/autotype \
	$(infodir)/calc \
	$(infodir)/ccmode \
	$(infodir)/cl \
	$(infodir)/dbus \
	$(infodir)/dired-x \
	$(infodir)/ebrowse \
	$(infodir)/ede \
	$(infodir)/ediff \
	$(infodir)/edt \
	$(infodir)/eieio \
	$(infodir)/emacs-mime \
	$(infodir)/epa \
	$(infodir)/erc \
	$(infodir)/eshell \
	$(infodir)/eudc \
	$(infodir)/efaq \
	$(infodir)/flymake \
	$(infodir)/forms \
	$(infodir)/gnus \
	$(infodir)/idlwave \
	$(infodir)/info \
	$(infodir)/mairix-el \
	$(infodir)/message \
	$(infodir)/mh-e \
	$(infodir)/newsticker \
	$(infodir)/nxml-mode \
	$(infodir)/org \
	$(infodir)/pcl-cvs \
	$(infodir)/pgg \
	$(infodir)/rcirc \
	$(infodir)/remember \
	$(infodir)/reftex \
	$(infodir)/sasl \
	$(infodir)/sc \
	$(infodir)/semantic \
	$(infodir)/ses \
	$(infodir)/sieve \
	$(infodir)/smtpmail \
	$(infodir)/speedbar \
	$(infodir)/tramp \
	$(infodir)/url \
	$(infodir)/vip \
	$(infodir)/viper \
	$(infodir)/widget \
	$(infodir)/woman

DVI_TARGETS = \
	ada-mode.dvi \
	auth.dvi \
	autotype.dvi \
	calc.dvi \
	cc-mode.dvi \
	cl.dvi \
	dbus.dvi \
	dired-x.dvi \
	ebrowse.dvi \
	ede.dvi \
	ediff.dvi \
	edt.dvi \
	eieio.dvi \
	emacs-mime.dvi \
	epa.dvi \
	erc.dvi \
	eshell.dvi \
	eudc.dvi \
	faq.dvi \
	flymake.dvi \
	forms.dvi \
	gnus.dvi \
	idlwave.dvi \
	info.dvi \
	mairix-el.dvi \
	message.dvi \
	mh-e.dvi \
	newsticker.dvi \
	nxml-mode.dvi \
	org.dvi \
	pcl-cvs.dvi \
	pgg.dvi \
	rcirc.dvi \
	reftex.dvi \
	remember.dvi \
	sasl.dvi \
	sc.dvi \
	semantic.dvi \
	ses.dvi \
	sieve.dvi \
	smtpmail.dvi \
	speedbar.dvi \
	tramp.dvi \
	url.dvi \
	vip.dvi \
	viper.dvi \
	widget.dvi \
	woman.dvi

PDF_TARGETS = \
	ada-mode.pdf \
	auth.pdf \
	autotype.pdf \
	calc.pdf \
	cc-mode.pdf \
	cl.pdf \
	dbus.pdf \
	dired-x.pdf \
	ebrowse.pdf \
	ede.pdf \
	ediff.pdf \
	edt.pdf \
	eieio.pdf \
	emacs-mime.pdf \
	epa.pdf \
	erc.pdf \
	eshell.pdf \
	eudc.pdf \
	faq.pdf \
	flymake.pdf \
	forms.pdf \
	gnus.pdf \
	idlwave.pdf \
	info.pdf \
	mairix-el.pdf \
	message.pdf \
	mh-e.pdf \
	newsticker.pdf \
	nxml-mode.pdf \
	org.pdf \
	pcl-cvs.pdf \
	pgg.pdf \
	rcirc.pdf \
	reftex.pdf \
	remember.pdf \
	sasl.pdf \
	sc.pdf \
	semantic.pdf \
	ses.pdf \
	sieve.pdf \
	smtpmail.pdf \
	speedbar.pdf \
	tramp.pdf \
	url.pdf \
	vip.pdf \
	viper.pdf \
	widget.pdf \
	woman.pdf

TEXI2DVI = texi2dvi
TEXI2PDF = texi2pdf

# The following rule does not work with all versions of `make'.
.SUFFIXES: .texi .dvi
.texi.dvi:
	$(TEXI2DVI) $<

ENVADD = TEXINPUTS="$(srcdir):$(TEXINPUTS)" MAKEINFO="$(MAKEINFO) -I$(srcdir)"


info: $(infodir) $(INFO_TARGETS)

$(infodir):
	mkdir $@

dvi: $(DVI_TARGETS)

pdf: $(PDF_TARGETS)

# Note that all the Info targets build the Info files
# in srcdir.  There is no provision for Info files
# to exist in the build directory.
# In a distribution of Emacs, the Info files should be up to date.

## "short" target names for convenience, to just rebuild one manual.
ada-mode : $(infodir)/ada-mode
$(infodir)/ada-mode: ada-mode.texi
	cd $(srcdir); $(MAKEINFO) ada-mode.texi
ada-mode.dvi: ada-mode.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/ada-mode.texi
ada-mode.pdf: ada-mode.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/ada-mode.texi

auth : $(infodir)/auth
$(infodir)/auth: auth.texi
	cd $(srcdir); $(MAKEINFO) auth.texi
auth.dvi: auth.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/auth.texi
auth.pdf: auth.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/auth.texi

autotype : $(infodir)/autotype
$(infodir)/autotype: autotype.texi
	cd $(srcdir); $(MAKEINFO) autotype.texi
autotype.dvi: autotype.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/autotype.texi
autotype.pdf: autotype.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/autotype.texi

calc : $(infodir)/calc
$(infodir)/calc: calc.texi
	cd $(srcdir); $(MAKEINFO) calc.texi
calc.dvi: calc.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/calc.texi
calc.pdf: calc.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/calc.texi

ccmode : $(infodir)/ccmode
$(infodir)/ccmode: cc-mode.texi
	cd $(srcdir); $(MAKEINFO) cc-mode.texi
cc-mode.dvi: cc-mode.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/cc-mode.texi
cc-mode.pdf: cc-mode.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/cc-mode.texi

cl : $(infodir)/cl
$(infodir)/cl: cl.texi
	cd $(srcdir); $(MAKEINFO) cl.texi
cl.dvi: cl.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/cl.texi
cl.pdf: cl.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/cl.texi

dbus : $(infodir)/dbus
$(infodir)/dbus: dbus.texi
	cd $(srcdir); $(MAKEINFO) dbus.texi
dbus.dvi: dbus.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/dbus.texi
dbus.pdf: dbus.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/dbus.texi

dired-x : $(infodir)/dired-x
$(infodir)/dired-x: dired-x.texi
	cd $(srcdir); $(MAKEINFO) dired-x.texi
dired-x.dvi: dired-x.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/dired-x.texi
dired-x.pdf: dired-x.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/dired-x.texi

ebrowse : $(infodir)/ebrowse
$(infodir)/ebrowse: ebrowse.texi
	cd $(srcdir); $(MAKEINFO) ebrowse.texi
ebrowse.dvi: ebrowse.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/ebrowse.texi
ebrowse.pdf: ebrowse.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/ebrowse.texi

ede : $(infodir)/ede
$(infodir)/ede: ede.texi
	cd $(srcdir); $(MAKEINFO) ede.texi
ede.dvi: ede.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/ede.texi
ede.pdf: ede.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/ede.texi

ediff : $(infodir)/ediff
$(infodir)/ediff: ediff.texi
	cd $(srcdir); $(MAKEINFO) ediff.texi
ediff.dvi: ediff.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/ediff.texi
ediff.pdf: ediff.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/ediff.texi

edt : $(infodir)/edt
$(infodir)/edt: edt.texi
	cd $(srcdir); $(MAKEINFO) edt.texi
edt.dvi: edt.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/edt.texi
edt.pdf: edt.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/edt.texi

eieio : $(infodir)/eieio
$(infodir)/eieio: eieio.texi
	cd $(srcdir); $(MAKEINFO) eieio.texi
eieio.dvi: eieio.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/eieio.texi
eieio.pdf: eieio.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/eieio.texi

emacs-mime : $(infodir)/emacs-mime
$(infodir)/emacs-mime: emacs-mime.texi
	cd $(srcdir); $(MAKEINFO) --enable-encoding emacs-mime.texi
emacs-mime.dvi: emacs-mime.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/emacs-mime.texi
emacs-mime.pdf: emacs-mime.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/emacs-mime.texi

epa : $(infodir)/epa
$(infodir)/epa: epa.texi
	cd $(srcdir); $(MAKEINFO) epa.texi
epa.dvi: epa.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/epa.texi
epa.pdf: epa.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/epa.texi

erc : $(infodir)/erc
$(infodir)/erc: erc.texi
	cd $(srcdir); $(MAKEINFO) erc.texi
erc.dvi: erc.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/erc.texi
erc.pdf: erc.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/erc.texi

eshell : $(infodir)/eshell
$(infodir)/eshell: eshell.texi
	cd $(srcdir); $(MAKEINFO) eshell.texi
eshell.dvi: eshell.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/eshell.texi
eshell.pdf: eshell.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/eshell.texi

eudc : $(infodir)/eudc
$(infodir)/eudc: eudc.texi
	cd $(srcdir); $(MAKEINFO) eudc.texi
eudc.dvi: eudc.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/eudc.texi
eudc.pdf: eudc.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/eudc.texi

efaq : $(infodir)/efaq
$(infodir)/efaq: faq.texi
	cd $(srcdir); $(MAKEINFO) faq.texi
faq.dvi: faq.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/faq.texi
faq.pdf: faq.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/faq.texi

flymake : $(infodir)/flymake
$(infodir)/flymake: flymake.texi
	cd $(srcdir); $(MAKEINFO) flymake.texi
flymake.dvi: flymake.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/flymake.texi
flymake.pdf: flymake.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/flymake.texi

forms : $(infodir)/forms
$(infodir)/forms: forms.texi
	cd $(srcdir); $(MAKEINFO) forms.texi
forms.dvi: forms.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/forms.texi
forms.pdf: forms.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/forms.texi

# gnus/message/emacs-mime/sieve/pgg are part of Gnus:
gnus : $(infodir)/gnus
$(infodir)/gnus: gnus.texi gnus-faq.texi
	cd $(srcdir); $(MAKEINFO) gnus.texi
gnus.dvi: gnus.texi gnus-faq.texi
	sed -e '/@iflatex/,/@end iflatex/d' ${srcdir}/gnus.texi > gnustmp.texi
	$(ENVADD) $(TEXI2DVI) gnustmp.texi
	cp gnustmp.dvi $*.dvi
	rm gnustmp.*
gnus.pdf: gnus.texi gnus-faq.texi
	sed -e '/@iflatex/,/@end iflatex/d' ${srcdir}/gnus.texi > gnustmp.texi
	$(ENVADD) $(TEXI2PDF) gnustmp.texi
	cp gnustmp.pdf $@
	rm gnustmp.*

# This is produced with --no-split to avoid making files whose
# names clash on DOS 8+3 filesystems
idlwave : $(infodir)/idlwave
$(infodir)/idlwave: idlwave.texi
	cd $(srcdir); $(MAKEINFO) --no-split idlwave.texi
idlwave.dvi: idlwave.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/idlwave.texi
idlwave.pdf: idlwave.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/idlwave.texi

# The following target uses an explicit -o switch to work around
# the @setfilename directive in info.texi, which is required for
# the Texinfo distribution.
###info : $(infodir)/info   # circular!
$(infodir)/info: info.texi
	cd $(srcdir); $(MAKEINFO) --no-split info.texi -o $@
info.dvi: info.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/info.texi
info.pdf: info.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/info.texi

mairix-el : $(infodir)/mairix-el
$(infodir)/mairix-el: mairix-el.texi
	cd $(srcdir); $(MAKEINFO) mairix-el.texi
mairix-el.dvi: mairix-el.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/mairix-el.texi
mairix-el.pdf: mairix-el.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/mairix-el.texi

message : $(infodir)/message
$(infodir)/message: message.texi
	cd $(srcdir); $(MAKEINFO) message.texi
message.dvi: message.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/message.texi
message.pdf: message.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/message.texi

mh-e : $(infodir)/mh-e
$(infodir)/mh-e: mh-e.texi
	cd $(srcdir); $(MAKEINFO) mh-e.texi
mh-e.dvi: mh-e.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/mh-e.texi
mh-e.pdf: mh-e.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/mh-e.texi

newsticker : $(infodir)/newsticker
$(infodir)/newsticker: newsticker.texi
	cd $(srcdir); $(MAKEINFO) newsticker.texi
newsticker.dvi: newsticker.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/newsticker.texi
newsticker.pdf: newsticker.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/newsticker.texi

nxml-mode : $(infodir)/nxml-mode
$(infodir)/nxml-mode: nxml-mode.texi
	cd $(srcdir); $(MAKEINFO) nxml-mode.texi
nxml-mode.dvi: nxml-mode.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/nxml-mode.texi
nxml-mode.pdf: nxml-mode.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/nxml-mode.texi

org : $(infodir)/org
$(infodir)/org: org.texi
	cd $(srcdir); $(MAKEINFO) org.texi
org.dvi: org.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/org.texi
org.pdf: org.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/org.texi

pcl-cvs : $(infodir)/pcl-cvs
$(infodir)/pcl-cvs: pcl-cvs.texi
	cd $(srcdir); $(MAKEINFO) pcl-cvs.texi
pcl-cvs.dvi: pcl-cvs.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/pcl-cvs.texi
pcl-cvs.pdf: pcl-cvs.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/pcl-cvs.texi

pgg : $(infodir)/pgg
$(infodir)/pgg: pgg.texi
	cd $(srcdir); $(MAKEINFO) pgg.texi
pgg.dvi: pgg.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/pgg.texi
pgg.pdf: pgg.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/pgg.texi

rcirc : $(infodir)/rcirc
$(infodir)/rcirc: rcirc.texi
	cd $(srcdir); $(MAKEINFO) rcirc.texi
rcirc.dvi: rcirc.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/rcirc.texi
rcirc.pdf: rcirc.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/rcirc.texi

reftex : $(infodir)/reftex
$(infodir)/reftex: reftex.texi
	cd $(srcdir); $(MAKEINFO) reftex.texi
reftex.dvi: reftex.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/reftex.texi
reftex.pdf: reftex.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/reftex.texi

remember : $(infodir)/remember
$(infodir)/remember: remember.texi
	cd $(srcdir); $(MAKEINFO) remember.texi
remember.dvi: remember.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/remember.texi
remember.pdf: remember.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/remember.texi

sasl : $(infodir)/sasl
$(infodir)/sasl: sasl.texi
	cd $(srcdir); $(MAKEINFO) sasl.texi
sasl.dvi: sasl.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/sasl.texi
sasl.pdf: sasl.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/sasl.texi

sc : $(infodir)/sc
$(infodir)/sc: sc.texi
	cd $(srcdir); $(MAKEINFO) sc.texi
sc.dvi: sc.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/sc.texi
sc.pdf: sc.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/sc.texi

semantic : $(infodir)/semantic
$(infodir)/semantic: semantic.texi sem-user.texi
	cd $(srcdir); $(MAKEINFO) semantic.texi
semantic.dvi: semantic.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/semantic.texi
semantic.pdf: semantic.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/semantic.texi

ses : $(infodir)/ses
$(infodir)/ses: ses.texi
	cd $(srcdir); $(MAKEINFO) ses.texi
ses.dvi: ses.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/ses.texi
ses.pdf: ses.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/ses.texi

sieve : $(infodir)/sieve
$(infodir)/sieve: sieve.texi
	cd $(srcdir); $(MAKEINFO) sieve.texi
sieve.dvi: sieve.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/sieve.texi
sieve.pdf: sieve.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/sieve.texi

smtpmail : $(infodir)/smtpmail
$(infodir)/smtpmail: smtpmail.texi
	cd $(srcdir); $(MAKEINFO) smtpmail.texi
smtpmail.dvi: smtpmail.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/smtpmail.texi
smtpmail.pdf: smtpmail.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/smtpmail.texi

speedbar : $(infodir)/speedbar
$(infodir)/speedbar: speedbar.texi
	cd $(srcdir); $(MAKEINFO) speedbar.texi
speedbar.dvi: speedbar.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/speedbar.texi
speedbar.pdf: speedbar.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/speedbar.texi

tramp : $(infodir)/tramp
$(infodir)/tramp: tramp.texi trampver.texi
	cd $(srcdir); $(MAKEINFO) -D emacs tramp.texi
tramp.dvi: tramp.texi trampver.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/tramp.texi
tramp.pdf: tramp.texi trampver.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/tramp.texi

url : $(infodir)/url
$(infodir)/url: url.texi
	cd $(srcdir); $(MAKEINFO) url.texi
url.dvi: url.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/url.texi
url.pdf: url.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/url.texi

vip : $(infodir)/vip
$(infodir)/vip: vip.texi
	cd $(srcdir); $(MAKEINFO) vip.texi
vip.dvi: vip.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/vip.texi
vip.pdf: vip.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/vip.texi

viper : $(infodir)/viper
$(infodir)/viper: viper.texi
	cd $(srcdir); $(MAKEINFO) viper.texi
viper.dvi: viper.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/viper.texi
viper.pdf: viper.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/viper.texi

widget : $(infodir)/widget
$(infodir)/widget: widget.texi
	cd $(srcdir); $(MAKEINFO) widget.texi
widget.dvi: widget.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/widget.texi
widget.pdf: widget.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/widget.texi

woman : $(infodir)/woman
$(infodir)/woman: woman.texi
	cd $(srcdir); $(MAKEINFO) woman.texi
woman.dvi: woman.texi
	$(ENVADD) $(TEXI2DVI) ${srcdir}/woman.texi
woman.pdf: woman.texi
	$(ENVADD) $(TEXI2PDF) ${srcdir}/woman.texi


mostlyclean:
	rm -f *.log *.cp *.fn *.ky *.op *.ops *.pg *.vr core *.tp \
	*.tps *.core gnustmp.*
	rm -f *.aux *.cps *.fns *.kys *.pgs *.vrs *.toc

clean: mostlyclean
	rm -f *.dvi *.pdf

distclean: clean
#	rm -f Makefile

maintainer-clean: distclean
	for file in $(INFO_TARGETS); do rm -f $${file}*; done


### Makefile ends here
