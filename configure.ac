AC_INIT(sim, 0.1, juanrgar@gmail.com, sim-release)

AM_INIT_AUTOMAKE([subdir-objects tar-ustar dist-xz no-dist-gzip foreign])

AC_PROG_CC

LIBGSIM_LT_VERSION=0:0:0
:qa
AC_SUBST(LIBGSIM_LT_VERSION)

LT_INIT([disable-static win32-dll])

# For each cycle:
#  first release: increment major += 1, minor = micro = 0;
#  each release before API freeze: minor += 1;
#  each release after API freeze: micro += 1;
m4_define([libgsim_major_version], [0])
m4_define([libgsim_minor_version], [1])
m4_define([libgsim_micro_version], [0])
m4_define([libgsim_version],
          [libgsim_major_version.libgsim_minor_version.libgsim_micro_version])

LIBGSIM_MAJOR_VERSION=libgsim_major_version
LIBGSIM_MINOR_VERSION=libgsim_minor_version
LIBGSIM_MICRO_VERSION=libgsim_micro_version
LIBGSIM_VERSION=libgsim_version

AC_SUBST(LIBGSIM_MAJOR_VERSION)
AC_SUBST(LIBGSIM_MINOR_VERSION)
AC_SUBST(LIBGSIM_MICRO_VERSION)
AC_SUBST(LIBGSIM_VERSION)

GLIB_MIN_VERSION=2.26.1

PKG_CHECK_MODULES(LIBGSIM,
                  glib-2.0 >= $GLIB_MIN_VERSION
                  gobject-2.0 >= $GLIB_MIN_VERSION)

AC_CONFIG_FILES([
                 Makefile
                 libgsim/Makefile
                 tests/Makefile
                 ])
AC_OUTPUT()

dnl ==========================================================================
echo "
GSIM $VERSION
=============

prefix: ${prefix}
compiler: ${CC}
"
