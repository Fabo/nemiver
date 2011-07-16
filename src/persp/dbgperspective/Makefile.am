SUBDIRS=ui menus icons sqlscripts language-specs

h=$(abs_srcdir)

plugin_config_files= \
plugin-descriptor.xml \
dbgperspective.conf

EXTRA_DIST=$(plugin_config_files)

pluginconfig_DATA=$(plugin_config_files)

PLUGIN_NAME=dbgperspective
pluginconfigdir=@NEMIVER_PLUGINS_DIR@/$(PLUGIN_NAME)

plugin_LTLIBRARIES=libdbgperspectiveplugin.la
plugindir=@NEMIVER_PLUGINS_DIR@/$(PLUGIN_NAME)
sources= \
$(h)/nmv-run-program-dialog.cc \
$(h)/nmv-run-program-dialog.h \
$(h)/nmv-load-core-dialog.cc \
$(h)/nmv-load-core-dialog.h \
$(h)/nmv-saved-sessions-dialog.h \
$(h)/nmv-saved-sessions-dialog.cc \
$(h)/nmv-preferences-dialog.h \
$(h)/nmv-preferences-dialog.cc \
$(h)/nmv-proc-list-dialog.cc \
$(h)/nmv-proc-list-dialog.h \
$(h)/nmv-var-inspector-dialog.cc \
$(h)/nmv-var-inspector-dialog.h \
$(h)/nmv-find-text-dialog.cc \
$(h)/nmv-find-text-dialog.h \
$(h)/nmv-sess-mgr.cc \
$(h)/nmv-sess-mgr.h \
$(h)/nmv-call-stack.cc \
$(h)/nmv-call-stack.h \
$(h)/nmv-variables-utils.cc \
$(h)/nmv-variables-utils.h \
$(h)/nmv-local-vars-inspector.cc \
$(h)/nmv-local-vars-inspector.h \
$(h)/nmv-global-vars-inspector-dialog.cc \
$(h)/nmv-global-vars-inspector-dialog.h \
$(h)/nmv-var-inspector.cc \
$(h)/nmv-var-inspector.h \
$(h)/nmv-breakpoints-view.cc \
$(h)/nmv-breakpoints-view.h \
$(h)/nmv-registers-view.cc \
$(h)/nmv-registers-view.h \
$(h)/nmv-thread-list.h \
$(h)/nmv-thread-list.cc \
$(h)/nmv-file-list.cc \
$(h)/nmv-file-list.h \
$(h)/nmv-open-file-dialog.cc \
$(h)/nmv-open-file-dialog.h \
$(h)/nmv-set-breakpoint-dialog.cc \
$(h)/nmv-set-breakpoint-dialog.h \
$(h)/nmv-watchpoint-dialog.h \
$(h)/nmv-watchpoint-dialog.cc \
$(h)/nmv-choose-overloads-dialog.cc \
$(h)/nmv-choose-overloads-dialog.h \
$(h)/nmv-remote-target-dialog.cc \
$(h)/nmv-remote-target-dialog.h \
$(h)/nmv-dbg-perspective.cc \
$(h)/nmv-dbg-perspective.h \
$(h)/nmv-vars-treeview.h \
$(h)/nmv-vars-treeview.cc \
$(h)/nmv-call-function-dialog.h \
$(h)/nmv-call-function-dialog.cc \
$(h)/nmv-set-jump-to-dialog.h \
$(h)/nmv-set-jump-to-dialog.cc

if BUILD_MEMORYVIEW
memoryview_sources = \
$(h)/nmv-memory-view.cc \
$(h)/nmv-memory-view.h
else
memoryview_sources =
endif

libdbgperspectiveplugin_la_SOURCES=$(sources) $(memoryview_sources)
libdbgperspectiveplugin_la_LDFLAGS= -module -avoid-version -Wl,--as-needed
libdbgperspectiveplugin_la_LIBADD= \
@NEMIVERDBGPERSP_LIBS@ \
$(abs_top_builddir)/src/common/libnemivercommon.la \
$(abs_top_builddir)/src/uicommon/libnemiveruicommon.la \
$(abs_top_builddir)/src/dbgengine/libdebuggerutils.la

INCLUDES=@NEMIVERDBGPERSP_CFLAGS@ -DENABLE_NLS=1 -DDATADIR=\"${datadir}\" \
-I$(abs_top_srcdir)/src \
-I$(abs_top_srcdir)/src/confmgr \
-I$(abs_top_srcdir)/src/dbgengine \
-I$(abs_top_srcdir)/src/uicommon \
-I$(abs_top_srcdir)/src/workbench \
-I$(abs_top_srcdir)/src/persp \
-I$(abs_top_srcdir)/src/dbgperspective

