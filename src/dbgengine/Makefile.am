gdbmod_LTLIBRARIES=libgdbmod.la
gdbmoddir=@NEMIVER_MODULES_DIR@

varlistmod_LTLIBRARIES=libvarlistmod.la
varlistmoddir=@NEMIVER_MODULES_DIR@

varwalkermod_LTLIBRARIES=libvarwalkermod.la
varwalkermoddir=@NEMIVER_MODULES_DIR@

varobjwalkermod_LTLIBRARIES=libvarobjwalkermod.la
varobjwalkermoddir=@NEMIVER_MODULES_DIR@

varlistwalkermod_LTLIBRARIES=libvarlistwalkermod.la
varlistwalkermoddir=@NEMIVER_MODULES_DIR@

cpptraitmod_LTLIBRARIES=libcpptraitmod.la
cpptraitmoddir=@NEMIVER_MODULES_DIR@

noinst_LTLIBRARIES=\
libgdbmiparser.la \
libdbgcommon.la \
libdebuggerutils.la \
libgdbengine.la \
libdbgconsole.la

h=$(abs_srcdir)

idebuggerheaders= \
$(h)/nmv-i-debugger.h

ivarlistheaders= \
$(h)/nmv-i-var-list.h

ilangtraitheaders= \
$(h)/nmv-i-lang-trait.h

ivarwalkerheaders= $(h)/nmv-i-var-walker.h $(h)/nmv-var.h

ivarlistwalkerheaders= $(h)/nmv-i-var-list-walker.h

dynmodheaders = \
$(idebuggerheaders) \
$(ivarlistheaders) \
$(ilangtraitheaders) \
$(ivarwalkerheaders) \
$(ivarlistwalkerheaders)

libgdbmod_la_SOURCES= \
$(idebuggerheaders)

libvarlistmod_la_SOURCES= \
$(ivarlistheaders) \
$(h)/nmv-var-list.cc

libvarwalkermod_la_SOURCES= \
$(ivarwalkerheaders) \
$(h)/nmv-var-walker.cc

libvarobjwalkermod_la_SOURCES= \
$(ivarwalkerheaders) \
$(h)/nmv-varobj-walker.cc

libvarlistwalkermod_la_SOURCES= \
$(ivarlistwalkerheaders) \
$(h)/nmv-var-list-walker.cc

libgdbmiparser_la_SOURCES= \
$(h)/nmv-gdbmi-parser.cc \
$(h)/nmv-gdbmi-parser.h

libgdbmiparser_la_CFLAGS=-fPIC -DPIC

libcpptraitmod_la_SOURCES= \
$(ilangtraitheaders) \
$(h)/nmv-cpp-trait.cc

libdbgcommon_la_SOURCES= \
$(h)/nmv-dbg-common.cc \
$(h)/nmv-dbg-common.h

libdbgcommon_la_CFLAGS=-fPIC -DPIC

libdebuggerutils_la_SOURCES= \
$(h)/nmv-debugger-utils.h \
$(h)/nmv-debugger-utils.cc

libdebuggerutils_la_CFLAGS=-fPIC -DPIC

libdbgconsole_la_SOURCES= \
$(h)/nmv-dbg-console.cc \
$(h)/nmv-dbg-console.h

libdbgconsole_la_CFLAGS=-fPIC -DPIC

libgdbengine_la_SOURCES= \
$(h)/nmv-gdb-engine.cc \
$(h)/nmv-gdb-engine.h

libgdbengine_la_CFLAGS=-fPIC -DPIC

publicheaders_DATA=$(dynmodheaders)
publicheadersdir=$(NEMIVER_INCLUDE_DIR)/dynmods

libgdbmod_la_LDFLAGS=-module -avoid-version -Wl,--as-needed
libgdbmod_la_LIBADD=libgdbmiparser.la \
libgdbengine.la libdebuggerutils.la libdbgconsole.la @NEMIVERCOMMON_LIBS@ \
$(abs_top_builddir)/src/langs/libnemivercparser.la \
$(abs_top_builddir)/src/common/libnemivercommon.la

libvarlistmod_la_LDFLAGS=-module -avoid-version
libvarlistmod_la_LIBADD=@NEMIVERCOMMON_LIBS@ \
	$(abs_top_builddir)/src/common/libnemivercommon.la

libcpptraitmod_la_LDFLAGS=-module -avoid-version
libcpptraitmod_la_LIBADD=@NEMIVERCOMMON_LIBS@ \
	$(abs_top_builddir)/src/common/libnemivercommon.la

libvarwalkermod_la_LDFLAGS=-module -avoid-version
libvarwalkermod_la_LIBADD=@NEMIVERCOMMON_LIBS@ \
	$(abs_top_builddir)/src/common/libnemivercommon.la

libvarobjwalkermod_la_LDFLAGS=-module -avoid-version
libvarobjwalkermod_la_LIBADD=@NEMIVERCOMMON_LIBS@ \
	$(abs_top_builddir)/src/common/libnemivercommon.la

libvarlistwalkermod_la_LDFLAGS=-module -avoid-version
libvarlistwalkermod_la_LIBADD=@NEMIVERCOMMON_LIBS@ \
$(abs_top_builddir)/src/common/libnemivercommon.la

libgdbmiparser_la_LIBADD= \
@NEMIVERCOMMON_LIBS@ \
$(abs_top_builddir)/src/common/libnemivercommon.la libdbgcommon.la

config_DATA=gdbengine.conf varlist.conf varwalker.conf varobjwalker.conf varlistwalker.conf cpptrait.conf
configdir=@NEMIVER_SYSTEM_CONFIG_DIR@

INCLUDES=@NEMIVERCOMMON_CFLAGS@ -DENABLE_NLS=1 -DDATADIR=\"${datadir}\" \
-I$(top_srcdir)/src -I$(top_srcdir)/src/confmgr

