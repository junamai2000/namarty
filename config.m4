dnl $Id$
dnl config.m4 for extension namarty

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(namarty, for namarty support,
dnl Make sure that the comment is aligned:
dnl [  --with-namarty             Include namarty support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(namarty, whether to enable namarty support,
Make sure that the comment is aligned:
[  --enable-namarty           Enable namarty support])

if test "$PHP_NAMARTY" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-namarty -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/namarty.h"  # you most likely want to change this
  dnl if test -r $PHP_NAMARTY/$SEARCH_FOR; then # path given as parameter
  dnl   NAMARTY_DIR=$PHP_NAMARTY
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for namarty files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       NAMARTY_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$NAMARTY_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the namarty distribution])
  dnl fi

  dnl # --with-namarty -> add include path
  dnl PHP_ADD_INCLUDE($NAMARTY_DIR/include)

  dnl # --with-namarty -> check for lib and symbol presence
  dnl LIBNAME=namarty # you may want to change this
  dnl LIBSYMBOL=namarty # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $NAMARTY_DIR/lib, NAMARTY_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_NAMARTYLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong namarty lib version or lib not found])
  dnl ],[
  dnl   -L$NAMARTY_DIR/lib -lm -ldl
  dnl ])
  dnl
  dnl PHP_SUBST(NAMARTY_SHARED_LIBADD)

  NAMARTY_LIBNAME=nam
  PHP_ADD_LIBRARY_WITH_PATH($NAMARTY_LIBNAME, /home/junya/namarty/lib/, NAMARTY_SHARED_LIBADD)
  PHP_SUBST(NAMARTY_SHARED_LIBADD)
  
  PHP_NEW_EXTENSION(namarty, namarty.cc, $ext_shared)
fi
