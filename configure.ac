# Process this file with autoconf to produce a configure script.
AC_INIT(ptgen, 1.0, bonzini@gnu.org)
AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([images.cc])
AC_CONFIG_HEADER([config.h])

AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_CXX

#######################
## Host information. ##
#######################

AC_ARG_ENABLE([fast-math],
	      [AS_HELP_STRING([--disable-fast-math], [use accurate math])],,
	      [enable_fast_math=yes])
AC_ARG_ENABLE([float],
	      [AS_HELP_STRING([--enable-float], [use floats for math])],,
	      [enable_float=no])
AC_ARG_ENABLE([sse],
	      [AS_HELP_STRING([--enable-sse=ARCH], [use SSE for math])],
	      [enable_float="$enable_sse"],
	      [enable_sse=no])
AC_ARG_ENABLE([sse2],
	      [AS_HELP_STRING([--enable-sse2=ARCH], [use SSE2 for math])],,
	      [enable_sse2=no])

AC_MSG_CHECKING([for requested C compiler flags])
ARCH_CFLAGS=
if test "$GCC" = yes && test "$GXX" = yes; then
  if test $enable_fast_math != no; then
    ARCH_CFLAGS="$ARCH_CFLAGS -ffast-math"
  fi
  if test $enable_sse != no; then
    ARCH_CFLAGS="$ARCH_CFLAGS -mfpmath=sse,387 -msse"
    if test $enable_sse != yes; then
      ARCH_CFLAGS="$ARCH_CFLAGS -march=$enable_sse -mtune=$enable_sse"
    fi
  fi
  if test $enable_sse2 != no; then
    ARCH_CFLAGS="$ARCH_CFLAGS -mfpmath=sse,387 -msse2"
    if test $enable_sse2 != yes; then
      ARCH_CFLAGS="$ARCH_CFLAGS -march=$enable_sse2 -mtune=$enable_sse2"
    fi
  fi
fi
AC_SUBST(ARCH_CFLAGS)
AC_MSG_RESULT([${ARCH_CFLAGS:-none}])

AC_MSG_CHECKING([for requested math precision])
if test $enable_float = no; then
  AC_DEFINE(USE_DOUBLE, 1, [Define to 1 to use doubles for math])
  AC_MSG_RESULT([double])
else
  AC_MSG_RESULT([float])
fi

#####################
## Host libraries. ##
#####################

# Look for libpng-config.  Get the flags from it.
AC_CHECK_PROGS(LIBPNG_CONFIG, libpng-config libpng12-config, no)
if test "$LIBPNG_CONFIG" != no; then
  save_LIBS="$LIBS"
  save_CFLAGS="$CFLAGS"
  save_CPPFLAGS="$CPPFLAGS"
  LIBS="$LIBS `$LIBPNG_CONFIG --ldflags | tr '\n' ' '`"
  CFLAGS="$CFLAGS `$LIBPNG_CONFIG --ccopts | tr '\n' ' '`"
  CPPFLAGS="$CPPFLAGS `$LIBPNG_CONFIG --cppflags --I_opts | tr '\n' ' '`"
fi

# Now look for the library archive and includes.  Also do so
# if libpng-config is not there, hoping that no special flags
# are necessary.
HAVE_LIBPNG=false
AC_CHECK_LIB(png, png_write_image,
  [AC_CHECK_HEADER(png.h,
    [AC_DEFINE(HAVE_LIBPNG, 1, [Define if libpng is available])
     HAVE_LIBPNG=:
     LIBS="$LIBS -lpng"])
])

# If the feature tests failed, reset the flags. :-(
if test $HAVE_LIBPNG != :; then
  LIBS="$save_LIBS"
  CFLAGS="$save_CFLAGS"
  CPPFLAGS="$save_CPPFLAGS"
fi

AM_CONDITIONAL(HAVE_LIBPNG, $HAVE_LIBPNG)

####################
## Outpput files. ##
####################

AC_CONFIG_FILES(Makefile)
AC_OUTPUT
