dnl **************************************************************
dnl Process this file with autoconf to produce a configure script.
dnl **************************************************************

MAJOR_VERSION=0
MINOR_VERSION=9
MICRO_VERSION=2

AC_INIT([nemiver],
        [0.9.2],
        [http://bugzilla.gnome.org/enter_bug.cgi?product=nemiver],
        [nemiver],
        [http://projects.gnome.org/nemiver])

AC_PREREQ([2.63])

AC_CONFIG_HEADER([config.h])
AC_CONFIG_SRCDIR([README])
AC_CONFIG_MACRO_DIR([m4])

AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([1.11.1 no-dist-gzip dist-xz])
AM_MAINTAINER_MODE([enable])

dnl If we are running automake >= 1.11, we can have nicer
dnl compilation output.
AM_SILENT_RULES([yes])


NEMIVER_VERSION="$MAJOR_VERSION.$MINOR_VERSION.$MICRO_VERSION"
AC_SUBST(NEMIVER_VERSION)

dnl ************************************************************
dnl First, Here goes the list of the version of the librairies
dnl we depend on. Some variables are AC_SUBSTed so that they can be used in the
dnl .pc files as well
dnl ************************************************************
LIBGMODULE_VERSION=2.14
AC_SUBST([LIBGMODULE_VERSION])
LIBGTHREAD_VERSION=2.14
AC_SUBST([LIBGTHREAD_VERSION])
LIBGLIBMM_VERSION=2.14
AC_SUBST([LIBGLIBMM_VERSION])
LIBXML2_VERSION=2.6.22
AC_SUBST([LIBXML2_VERSION])
LIBGTK_VERSION=2.22
AC_SUBST([LIBGTK_VERSION])
LIBGTKMM_VERSION=3.0
AC_SUBST([LIBGTKMM_VERSION])
LIBGTKSOURCEVIEWMM_VERSION=3.0
AC_SUBST([LIBGTKSOURCEVIEWMM_VERSION])
LIBGIOMM_VERSION=2.15.2
AC_SUBST([LIBGIOMM_VERSION])
SQLITE3_VERSION=3.0
AC_SUBST([SQLITE3_VERSION])
LIBGTOP_VERSION=2.14
AC_SUBST([LIBGTOP_VERSION])
LIBVTE_VERSION=0.28
AC_SUBST([LIBVTE_VERSION])
GCONF_VERSION=2.14.0
AC_SUBST([GCONF_VERSION])
CPPUNIT_VERSION=1.10.0
AC_SUBST([CPPUNIT_VERSION])
GTKHEX_VERSION=2.90
AC_SUBST([GTKHEX_VERSION])
LIBGIOMM_WITH_GSETTINGS_VERSION=2.25.1
AC_SUBST([GIOMM_WITH_GSETTINGS_VERSION])
GSETTINGS_DESKTOP_SCHEMAS=0.0.1
AC_SUBST([GSETTINGS_DESKTOP_SCHEMAS])
LIBGDLMM_VERSION=3.0
AC_SUBST([LIBGDLMM_VERSION])

dnl *********************
dnl Checks for programs.
dnl *********************
AC_PROG_CXX
AC_USE_SYSTEM_EXTENSIONS
#AC_HEADER_STDC

AC_PROG_INSTALL

dnl Initialize libtool
LT_PREREQ([2.2])
LT_INIT

AC_LANG([C++])
AC_LANG_COMPILER_REQUIRE

AC_ARG_WITH(default-gdb, AC_HELP_STRING([--with-default-gdb=filename], [Path to the default GDB binary]), GDB_PROG=${withval})

if test x$GDB_PROG = x ; then
    AC_PATH_PROG(GDB_PROG,gdb)
fi

if test x$GDB_PROG = x ; then
    AC_MSG_ERROR([didn't find the gdb debugger. please, install it.])
fi
AC_SUBST(GDB_PROG)

ENABLE_IDEBUGGER_ONLY=no
ENABLE_DBGENGINE=yes
ENABLE_WORKBENCH=yes

dnl *************************************
dnl Internationalization support
dnl *************************************
dnl To add a new language, add language code xx to po/LINGUAS
IT_PROG_INTLTOOL([0.40.0])

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.17])

GETTEXT_PACKAGE=nemiver
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"],
                   [The domain to use with gettext])

nemiverlocaledir=[${datadir}/locale]
AC_SUBST(nemiverlocaledir)

dnl ***************************
dnl Initialize GNOME doc utils
dnl ***************************
GNOME_DOC_INIT

dnl ***************************
dnl a list of enablexxx options
dnl ***************************

AC_ARG_ENABLE(idebuggeronly,
              AS_HELP_STRING([--enable-idebugger-only=yes|no],
                             [compile the idebuger iface only (default is no)]),
              ENABLE_IDEBUGGER_ONLY=$enableval,
              ENABLE_IDEBUGGER_ONLY=no)
if test x$ENABLE_IDEBUGGER_ONLY = xyes ; then
    ENABLE_DBGENGINE=yes
    ENABLE_WORKBENCH=no
fi

AC_ARG_ENABLE(autotests,
              AS_HELP_STRING([--enable-autotests=yes|no],
                             [enable automated tests (default is no)]),
              ENABLE_AUTOTESTS=$enableval,
              ENABLE_AUTOTESTS=auto)
if test x$ENABLE_AUTOTESTS = xauto ; then
    if test x$NEMIVER_DEVEL = xon ; then
        ENABLE_AUTOTESTS=yes
    else
        ENABLE_AUTOTESTS=no
    fi
fi

if test x$ENABLE_AUTOTESTS = xyes ; then
#AM_PATH_CPPUNIT($CPP_UNIT_VERSION)
    AC_MSG_NOTICE(support of automated tests enabled)
else
    AC_MSG_NOTICE(disabled support of automated tests)
fi
AM_CONDITIONAL(AUTOTESTS, test x$ENABLE_AUTOTESTS = xyes)

AC_ARG_ENABLE(workbench,
              AS_HELP_STRING([--enable-workbench=yes|no],
                             [enable the graphical dbg workbench (default is yes)]),
              ENABLE_WORKBENCH=$enableval,
              ENABLE_WORKBENCH=yes)
if test x$ENABLE_WORKBENCH = xyes ; then
    AC_DEFINE([WITH_WORKBENCH], 1, [compile the workbench])
else
    ENABLE_WORKBENCH=no
fi


AC_ARG_ENABLE(memoryview,
              AS_HELP_STRING([--enable-memoryview=yes|no],
                             [enable the memory view hex editor (default is yes)]),
              ENABLE_MEMORYVIEW=$enableval,
              [PKG_CHECK_EXISTS([gtkhex-3 >= $GTKHEX_VERSION], [ENABLE_MEMORYVIEW=yes], [ENABLE_MEMORYVIEW=no])])
if test x$ENABLE_MEMORYVIEW = xyes ; then
    AC_DEFINE([WITH_MEMORYVIEW], 1, [compile the the memory view widget])
else
    ENABLE_MEMORYVIEW=no
fi
AM_CONDITIONAL(BUILD_MEMORYVIEW, test x$ENABLE_MEMORYVIEW = xyes)


AC_ARG_ENABLE(dynamiclayout,
              AS_HELP_STRING([--enable-dynamiclayout=yes|no],
                             [enable the dynamic layout (default is yes)]),
              ENABLE_DYNAMICLAYOUT=$enableval,
              [PKG_CHECK_EXISTS([gdlmm-3.0 >= $LIBGDLMM_VERSION], [ENABLE_DYNAMICLAYOUT=yes], [ENABLE_DYNAMICLAYOUT=no])])
if test x$ENABLE_DYNAMICLAYOUT = xyes ; then
    AC_DEFINE([WITH_DYNAMICLAYOUT], 1, [compile the dynamic layout])
else
    ENABLE_DYNAMICLAYOUT=no
fi
AM_CONDITIONAL(BUILD_DYNAMICLAYOUT, test x$ENABLE_DYNAMICLAYOUT = xyes)


ENABLE_DEBUG=yes
AC_ARG_ENABLE(debug,
              AS_HELP_STRING([--enable-debug=yes|no],
                             [enable a lot of debug message dumps(default is yes)]),
              ENABLE_DEBUG=$enableval,
              ENABLE_DEBUG=yes)
if test x$ENABLE_DEBUG = xyes ; then
        AC_DEFINE([NEMIVER_DEBUG], 1,
        [enable a lot of messages dumps in the terminal])
else
        ENABLE_DEBUG=no
fi


ENABLE_GCC_SYMBOLS_VISIBILITY=no
AC_ARG_ENABLE(symsvis,
              AS_HELP_STRING([--enable-symsvis=yes|no],
                             [use the gcc symbols visibility feature. >=n gcc4.  default is no)]),
              ENABLE_GCC_SYMBOLS_VISIBILITY=$enableval,
              ENABLE_GCC_SYMBOLS_VISIBILITY=no)


ENABLE_GSETTINGS=auto
AC_ARG_ENABLE(gsettings,
              AS_HELP_STRING([--enable-gsettings=yes|no],
                             [use gsettings instead of gconf (default is auto)]),
              ENABLE_GSETTINGS=$enableval,
              AC_CHECK_PROG([HAS_DCONF], [dconf], [yes], [no])
              [PKG_CHECK_EXISTS([giomm-2.4 >= $LIBGIOMM_WITH_GSETTINGS_VERSION gsettings-desktop-schemas >= $GSETTINGS_DESKTOP_SCHEMAS], [HAS_GSETTINGS=yes], [HAS_GSETTINGS=no])])
if test x$ENABLE_GSETTINGS = xyes || (test x$ENABLE_GSETTINGS = xauto \
      && test x$HAS_GSETTINGS = xyes \
      && test x$HAS_DCONF = xyes); then
    AC_DEFINE([WITH_GSETTINGS], 1, [build with gsettings instead of gconf])
    AC_DEFINE([CONFIG_MGR_MODULE_NAME], ["gsettingsmgr"], [configuration manager to use])
    CONF_MGR=gsettings
    ENABLE_GSETTINGS=yes
else
    AC_DEFINE([CONFIG_MGR_MODULE_NAME], ["gconfmgr"], [configuration manager to use])
    CONF_MGR=gconf
    ENABLE_GSETTINGS=no
fi
AM_CONDITIONAL(BUILD_GSETTINGS, test x$ENABLE_GSETTINGS = xyes)

dnl *******************************
dnl checking for libraries we use
dnl *******************************

#try to detect boost-dev include-only libs
AC_CHECK_HEADERS([boost/variant.hpp],
                 [],
                 [AC_MSG_ERROR([Please, install libboost development headers])])

#dnl checking for non-aggregated lib
PKG_CHECK_MODULES(SQLITE3, [sqlite3 >= $SQLITE3_VERSION])

dnl glibmm-2.4 version 2.30.0 uses many deprecated functions from
dnl glib-2.0 version 2.31.0 if we are building against such a
dnl combination, we need to define GLIB_DISABLE_DEPRECATION_WARNINGS
dnl to shut the compilation warnigs down.
PKG_CHECK_EXISTS([glib-2.0 >= 2.31.0 glibmm-2.4 <= 2.30.0],
                 [GLIBMM_USES_DEPRECATED_GLIB_FUNCS=yes],
		 [GLIBMM_USES_DEPRECATED_GLIB_FUNCS=no])
if test x$GLIBMM_USES_DEPRECATED_GLIB_FUNCS = xyes; then
   AC_DEFINE(GLIB_DISABLE_DEPRECATION_WARNINGS,1,[disables glib deprecation warnings])
   AC_MSG_NOTICE(glibmm uses deprecated glib functions!)
fi

#######################################################
#boost build system sucks no end.
#it is damn hard to detect the version of boost
#that is installed. All that because our friends of
#the boost project don't want to integrate to autofoo.
#So we resort to hugly hacks to detect the version of
#boost that is installed.
#######################################################
if test x$ENABLE_AUTOTESTS = xyes ; then
    AX_BOOST_BASE([1.33.1])
    AX_BOOST_UNIT_TEST_FRAMEWORK
    AX_BOOST_TEST_EXEC_MONITOR

    unit_framework_lib_candidates="/usr/lib/libboost_unit_test_framework-st.a /usr/lib/libboost_unit_test_framework.a"
    unit_framework_lib_candidates=$unit_framework_lib_candidates" /usr/lib64/libboost_unit_test_framework-st.a /usr/lib64/libboost_unit_test_framework.a"
    for i in $unit_framework_lib_candidates ; do
        if test -f $i ; then
            BOOST_UNIT_TEST_FRAMEWORK_STATIC_LIB=$i
            break
        fi
    done
    AC_SUBST(BOOST_UNIT_TEST_FRAMEWORK_STATIC_LIB)
fi

if test x$ENABLE_WORKBENCH = xyes ; then
    DEP_WORKBENCH=" gtkmm-3.0 >= $LIBGTKMM_VERSION \
                   gtksourceviewmm-3.0 >= $LIBGTKSOURCEVIEWMM_VERSION"
fi

if test x$ENABLE_MEMORYVIEW = xyes ; then
    DEP_MEMORYVIEW="gtkhex-3 >= $GTKHEX_VERSION"
fi

if test x$ENABLE_DYNAMICLAYOUT = xyes ; then
    DEP_DYNAMICLAYOUT="gdlmm-3.0 >= $LIBGDLMM_VERSION"
fi

dnl *******************
dnl Configuration Manager
dnl *******************
if test x$CONF_MGR = xgsettings ; then
    AM_CONDITIONAL([GCONF_SCHEMAS_INSTALL], /bin/false)

    dnl library dependencies for the nemiver gsettings plugin
    DEP_GSETTINGS="giomm-2.4 >= $LIBGIOMM_WITH_GSETTINGS_VERSION \
                   gsettings-desktop-schemas >= $GSETTINGS_DESKTOP_SCHEMAS"

    PKG_CHECK_MODULES(NEMIVERGSETTINGS, $DEP_GSETTINGS)

    AC_SUBST(NEMIVERGSETTINGS_LIBS)
    AC_SUBST(NEMIVERGSETTINGS_CFLAGS)

    GLIB_GSETTINGS
elif test x$CONF_MGR = xgconf ; then
    PKG_CHECK_MODULES(GCONF,[gconf-2.0 >= $GCONF_VERSION])

    AC_PATH_PROG(GCONFTOOL, gconftool-2)
    AC_SUBST(GCONFTOOL)

    AM_GCONF_SOURCE_2
    AC_SUBST(gsettingsschemadir, [dummy])
else
    AC_MSG_ERROR([Configuration Manager undetermined. Abort...])
fi

dnl library dependencies for libnemivercommon and the IDebugger libraries
DEP_COMMON="gmodule-2.0 >= $LIBGMODULE_VERSION \
           gthread-2.0 >= $LIBGTHREAD_VERSION \
           libxml-2.0 >= $LIBXML2_VERSION \
           libgtop-2.0 >= $LIBGTOP_VERSION \
           glibmm-2.4 >= $LIBGLIBMM_VERSION"

PKG_CHECK_MODULES(NEMIVERCOMMON, $DEP_COMMON)
AC_CHECK_FUNCS(readline)
NEMIVERCOMMON_CFLAGS="$NEMIVERCOMMON_CFLAGS $CPPUNIT_CFLAGS"
NEMIVERCOMMON_LIBS="$NEMIVERCOMMON_LIBS $LIBGTOP_LIBS $CPPUNIT_LIBS -lreadline"

AC_SUBST(NEMIVERCOMMON_LIBS)
AC_SUBST(NEMIVERCOMMON_CFLAGS)

dnl library dependencies for the nemiver common UI module
DEP_UICOMMON=" gtkmm-3.0 >= $LIBGTKMM_VERSION \
               gtk+-3.0 >= $LIBGTK_VERSION \
               gtksourceviewmm-3.0 >= $LIBGTKSOURCEVIEWMM_VERSION \
               vte-2.90 >= $LIBVTE_VERSION"

PKG_CHECK_MODULES(NEMIVERUICOMMON, $DEP_UICOMMON $DEP_COMMON $DEP_MEMORYVIEW)
NEMIVERUICOMMON_CFLAGS="$NEMIVERUICOMMON_CFLAGS $CPPUNIT_CFLAGS"
NEMIVERUICOMMON_LIBS="$NEMIVERUICOMMON_LIBS $CPPUNIT_LIBS"

AC_SUBST(NEMIVERUICOMMON_LIBS)
AC_SUBST(NEMIVERUICOMMON_CFLAGS)

DEP_VFS="giomm-2.4 >= $LIBGIOMM_VERSION"

dnl library dependencies for the nemiver workbench module
dnl dependencies list already declared above

PKG_CHECK_MODULES(NEMIVERWORKBENCH, $DEP_COMMON $DEP_VFS $DEP_WORKBENCH)
NEMIVERWORKBENCH_LIBS="$NEMIVERWORKBENCH_LIBS $CPPUNIT_LIBS"
NEMIVERWORKBENCH_CFLAGS="$NEMIVERWORKBENCH_CFLAGS $CPPUNIT_CFLAGS"

AC_SUBST(NEMIVERWORKBENCH_LIBS)
AC_SUBST(NEMIVERWORKBENCH_CFLAGS)

dnl library dependencies for the nemiver debug perspective plugin
DEP_PERSP="gtksourceviewmm-3.0 >= $LIBGTKSOURCEVIEWMM_VERSION \
           vte-2.90 >= $LIBVTE_VERSION"

PKG_CHECK_MODULES(NEMIVERDBGPERSP, $DEP_UICOMMON $DEP_VFS $DEP_PERSP $DEP_MEMORYVIEW $DEP_DYNAMICLAYOUT)
NEMIVERDBGPERSP_LIBS="$NEMIVERDBGPERSP_LIBS $CPPUNIT_LIBS"
NEMIVERDBGPERSP_CFLAGS="$NEMIVERDBGPERSP_CFLAGS $CPPUNIT_CFLAGS"

AC_SUBST(NEMIVERDBGPERSP_LIBS)
AC_SUBST(NEMIVERDBGPERSP_CFLAGS)

NEMIVER_LIBS="$NEMIVERUICOMMON_LIBS"
NEMIVER_CFLAGS="$NEMIVERUICOMMON_CFLAGS"

AC_SUBST(NEMIVER_LIBS)
AC_SUBST(NEMIVER_CFLAGS)

NEMIVER_INCLUDE_DIR=$includedir/nemiver

#has_lib_sixty_four=`echo $libdir | grep lib64 | wc -l`
#if test x$has_lib_sixty_four = x0 ; then
#NEMIVER_LIB_DIR_NAME=lib
#else
#NEMIVER_LIB_DIR_NAME=lib64
#fi

#expand libdir
if test x$prefix = xNONE ; then
        prefix=$ac_default_prefix
fi
if test x$exec_prefix = xNONE ; then
        exec_prefix=$prefix ;
fi
NEMIVER_LIBDIR=`eval echo $libdir`

AC_SUBST(NEMIVER_INCLUDE_DIR)
AC_SUBST(NEMIVER_LIB_DIR_NAME)
AC_SUBST(NEMIVER_LIBDIR)


if test x$ENABLE_GCC_SYMBOLS_VISIBILITY = xyes && test x$host_cpu != xx86_64 ; then
    AC_DEFINE(HAS_GCC_VISIBILITY_SUPPORT,1,[gcc visibility support])
    REQUIRED_FLAGS="-fvisibility=hidden -fvisibility-inlines-hidden"
else
    REQUIRED_FLAGS=""
fi

dnl ***************************
dnl Set Dodji devel environment.
dnl ***************************
if test  "x$NEMIVER_DEVEL" = "xon" ; then
    DODJI_CFLAGS="-Wall -g -Wextra -Wno-deprecated $REQUIRED_FLAGS"
    if test x$NEMIVER_ALLOW_WARNINGS != xon ; then
        DODJI_CFLAGS="$DODJI_CFLAGS -Werror"
    fi
    DODJI_CXXFLAGS="$DODJI_CFLAGS -fuse-cxa-atexit"
    ENABLE_DEBUG=yes
    CXXFLAGS="$DODJI_CXXFLAGS"
    CFLAGS="$DODJI_CFLAGS"
else
    DODJI_CFLAGS="$REQUIRED_FLAGS"
    DODJI_CXXFLAGS="$DODJI_CFAGS -fuse-cxa-atexit"
    CFLAGS="$CFLAGS $DODJI_CFLAGS"
    CXXFLAGS="$CXXFLAGS $DODJI_CXXFLAGS"
fi

if test x$ENABLE_DEBUG != x ; then
    AC_DEFINE([NEMIVER_DEBUG],1,[enable debug messages])
fi

NEMIVER_INSTALL_PREFIX=$prefix
NEMIVER_SYSTEM_CONFIG_DIR=$libdir/nemiver/config
NEMIVER_MODULES_DIR=$libdir/nemiver/modules
NEMIVER_PLUGINS_DIR=$libdir/nemiver/plugins
NEMIVER_TEST_MODULES_DIR=$NEMIVER_MODULES_DIR/test-modules


AC_SUBST(NEMIVER_INSTALL_PREFIX)
AC_SUBST(NEMIVER_SYSTEM_CONFIG_DIR)
AC_SUBST(NEMIVER_MODULES_DIR)
AC_SUBST(NEMIVER_PLUGINS_DIR)
AC_SUBST(NEMIVER_TEST_MODULES_DIR)

dnl don't try to install gconf schemas during distcheck
DISTCHECK_CONFIGURE_FLAGS="--disable-schemas-install"
AC_SUBST([DISTCHECK_CONFIGURE_FLAGS])

AC_PROG_MAKE_SET

AC_CONFIG_FILES([Makefile
docs/Makefile
docs/Doxyfile
src/Makefile
  src/common/Makefile
  src/common/libnemivercommon.pc
  src/dbdimpl/Makefile
  src/dbdimpl/sqlite/Makefile
  src/dbdimpl/sqlite/org.nemiver.db.sqlitedriver.conf
  src/langs/Makefile
  src/dbgengine/Makefile
  src/dbgengine/gdbengine.conf
  src/dbgengine/cpptrait.conf
  src/dbgengine/varlist.conf
  src/dbgengine/varwalker.conf
  src/dbgengine/varobjwalker.conf
  src/dbgengine/varlistwalker.conf
  src/confmgr/Makefile
  src/confmgr/gconfmgr.conf
  src/confmgr/gsettingsmgr.conf
  src/uicommon/Makefile
  src/workbench/Makefile
  src/workbench/workbench.conf
  src/persp/Makefile
  src/persp/dbgperspective/Makefile
  src/persp/dbgperspective/dbgperspective.conf
    src/persp/dbgperspective/ui/Makefile
    src/persp/dbgperspective/menus/Makefile
    src/persp/dbgperspective/icons/Makefile
    src/persp/dbgperspective/sqlscripts/Makefile
    src/persp/dbgperspective/sqlscripts/sqlite/Makefile
data/Makefile
    data/icons/Makefile
        data/icons/16x16/Makefile
        data/icons/22x22/Makefile
        data/icons/24x24/Makefile
        data/icons/32x32/Makefile
        data/icons/48x48/Makefile
        data/icons/scalable/Makefile
    data/schemas/Makefile
        data/schemas/gconf/Makefile
        data/schemas/gsettings/Makefile
ui/Makefile
menus/Makefile
tests/Makefile
po/Makefile.in
help/Makefile])

AC_OUTPUT

AC_MSG_NOTICE([
=================================================================
         Nemiver: $MAJOR_VERSION.$MINOR_VERSION.$MICRO_VERSION
=================================================================
        Here is the configuration of the package:

    Prefix                             : ${prefix}
    Source code location               : ${srcdir}
    C Compiler                         : ${CC}
    C++ Compiler		       : ${CXX}

    NEMIVER_DEVEL env var              : ${NEMIVER_DEVEL}
    NEMIVER_ALLOW_WARNINGS env var     : ${NEMIVER_ALLOW_WARNINGS}
    CFLAGS                             : ${CFLAGS}
    CXXFLAGS                           : ${CXXFLAGS}
    NEMIVER_LIBS                       : ${NEMIVER_LIBS}
    NEMIVER_CFLAGS                     : ${NEMIVER_CFLAGS}
    GLIBMM_USES_DEPRECATED_GLIB_FUNCS  : ${GLIBMM_USES_DEPRECATED_GLIB_FUNCS}

 OPTIONAL FEATURES:
    Enable autotests                : ${ENABLE_AUTOTESTS}
    Enable verbose debug messages   : ${ENABLE_DEBUG}
    Enable workbench                : ${ENABLE_WORKBENCH}
    Enable memory view              : ${ENABLE_MEMORYVIEW} (requires gtkhex > $GTKHEX_VERSION)
    Enable dynamic layout           : ${ENABLE_DYNAMICLAYOUT} (requires gdlmm > $LIBGDLMM_VERSION)
    Configuration Manager           : ${CONF_MGR}
    Enable symbols visibility ctrl  : ${ENABLE_GCC_SYMBOLS_VISIBILITY}
    Maintainer mode                 : ${USER_MAINTAINER_MODE}

Now, Type 'make' to compile Nemiver
Once done, type 'make install' with sufficient privileges to install it to $prefix

I hope you will enjoy it.

        --Dodji
])

