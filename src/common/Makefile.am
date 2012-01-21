nemivercommon_LTLIBRARIES=libnemivercommon.la
nemivercommondir=$(libdir)/nemiver

h=$(abs_srcdir)

headers= \
$(h)/nmv-api-macros.h \
$(h)/nmv-namespace.h \
$(h)/nmv-ustring.h \
$(h)/nmv-address.h \
$(h)/nmv-asm-instr.h \
$(h)/nmv-asm-utils.h \
$(h)/nmv-range.h \
$(h)/nmv-str-utils.h \
$(h)/nmv-libxml-utils.h \
$(h)/nmv-safe-ptr-utils.h \
$(h)/nmv-object.h \
$(h)/nmv-safe-ptr.h \
$(h)/nmv-log-stream.h \
$(h)/nmv-scope-logger.h \
$(h)/nmv-exception.h \
$(h)/nmv-initializer.h \
$(h)/nmv-dynamic-module.h \
$(h)/nmv-log-stream-utils.h \
$(h)/nmv-date-utils.h \
$(h)/nmv-i-input-stream.h \
$(h)/nmv-env.h \
$(h)/nmv-plugin.h \
$(h)/nmv-sequence.h \
$(h)/nmv-option-utils.h \
$(h)/nmv-i-connection-driver.h \
$(h)/nmv-connection.h \
$(h)/nmv-i-connection-manager-driver.h \
$(h)/nmv-connection-manager.h \
$(h)/nmv-parsing-utils.h  \
$(h)/nmv-conf-manager.h \
$(h)/nmv-buffer.h \
$(h)/nmv-tools.h \
$(h)/nmv-transaction.h \
$(h)/nmv-sql-statement.h \
$(h)/nmv-insert-statement.h \
$(h)/nmv-delete-statement.h \
$(h)/nmv-proc-utils.h \
$(h)/nmv-proc-mgr.h \
$(h)/nmv-loc.h

libnemivercommon_la_SOURCES= $(headers) \
$(h)/nmv-ustring.cc \
$(h)/nmv-address.cc \
$(h)/nmv-asm-utils.cc \
$(h)/nmv-str-utils.cc \
$(h)/nmv-object.cc \
$(h)/nmv-libxml-utils.cc \
$(h)/nmv-log-stream.cc \
$(h)/nmv-scope-logger.cc \
$(h)/nmv-exception.cc \
$(h)/nmv-initializer.cc \
$(h)/nmv-dynamic-module.cc \
$(h)/nmv-date-utils.cc \
$(h)/nmv-env.cc \
$(h)/nmv-plugin.cc \
$(h)/nmv-sequence.cc \
$(h)/nmv-option-utils.cc \
$(h)/nmv-connection-manager.cc \
$(h)/nmv-connection.cc \
$(h)/nmv-parsing-utils.cc \
$(h)/nmv-conf-manager.cc \
$(h)/nmv-tools.cc \
$(h)/nmv-transaction.cc \
$(h)/nmv-sql-statement.cc \
$(h)/nmv-insert-statement.cc \
$(h)/nmv-delete-statement.cc \
$(h)/nmv-proc-utils.cc \
$(h)/nmv-proc-mgr.cc \
$(h)/nmv-console.cc

publicheaders_DATA=$(headers)
publicheadersdir=$(NEMIVER_INCLUDE_DIR)/common
libnemivercommon_la_LDFLAGS=@NEMIVERCOMMON_LIBS@ -lutil \
		-avoid-version -rpath $(libdir)/nemiver \
		 -Wl,--as-needed

#don't install libnemivercommon.pc yet, as no other program
#but nemiver uses the libnemivercommon library yet.
#pkgconfigdir = $(libdir)/pkgconfig
#pkgconfig_DATA = libnemivercommon.pc
#EXTRA_DIST = $(pkgconfig_DATA)

INCLUDES=@NEMIVERCOMMON_CFLAGS@ \
-DENABLE_NLS=1 -DDATADIR=\"${datadir}\" \
-DSYSTEM_LIBDIR=\"/usr/lib\" \
-DNEMIVER_INSTALL_PREFIX=\"@NEMIVER_INSTALL_PREFIX@\" \
-DNEMIVER_SYSTEM_CONFIG_DIR=\"@NEMIVER_SYSTEM_CONFIG_DIR@\" \
-DNEMIVER_MODULES_DIR=\"@NEMIVER_MODULES_DIR@\" \
-DNEMIVER_PLUGINS_DIR=\"@NEMIVER_PLUGINS_DIR@\" \
-DNEMIVER_VERSION=\"@NEMIVER_VERSION@\" \
-DGDB_PROG=\"@GDB_PROG@\"

