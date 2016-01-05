dnl $Id$
dnl config.m4 for extension evenement

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(evenement, for evenement support,
Make sure that the comment is aligned:
[  --with-evenement             Include evenement support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(evenement, whether to enable evenement support,
dnl Make sure that the comment is aligned:
dnl [  --enable-evenement           Enable evenement support])

if test "$PHP_EVENEMENT" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-evenement -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/evenement.h"  # you most likely want to change this
  dnl if test -r $PHP_EVENEMENT/$SEARCH_FOR; then # path given as parameter
  dnl   EVENEMENT_DIR=$PHP_EVENEMENT
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for evenement files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       EVENEMENT_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$EVENEMENT_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the evenement distribution])
  dnl fi

  dnl # --with-evenement -> add include path
  dnl PHP_ADD_INCLUDE($EVENEMENT_DIR/include)

  dnl # --with-evenement -> check for lib and symbol presence
  dnl LIBNAME=evenement # you may want to change this
  dnl LIBSYMBOL=evenement # you most likely want to change this

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $EVENEMENT_DIR/$PHP_LIBDIR, EVENEMENT_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_EVENEMENTLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong evenement lib version or lib not found])
  dnl ],[
  dnl   -L$EVENEMENT_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(EVENEMENT_SHARED_LIBADD)

  PHP_NEW_EXTENSION(evenement, evenement.c, $ext_shared)
fi
