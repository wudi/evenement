/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_EVENEMENT_H
#define PHP_EVENEMENT_H

extern zend_module_entry evenement_module_entry;
#define phpext_evenement_ptr &evenement_module_entry

#define PHP_EVENEMENT_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_EVENEMENT_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_EVENEMENT_API __attribute__ ((visibility("default")))
#else
#	define PHP_EVENEMENT_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

extern zend_class_entry  *event_emitter_interface_ce;
extern zend_class_entry  *event_emitter_ce;
extern zend_class_entry  *event_emitter_trait_ce;

ZEND_MINIT_FUNCTION(event_emitter_interface);
ZEND_MINIT_FUNCTION(event_emitter);
ZEND_MINIT_FUNCTION(event_emitter_trait);

PHP_MINIT_FUNCTION(evenement);
PHP_MSHUTDOWN_FUNCTION(evenement);
PHP_RINIT_FUNCTION(evenement);
PHP_RSHUTDOWN_FUNCTION(evenement);
PHP_MINFO_FUNCTION(evenement);


#ifdef ZTS
#define EVENEMENT_G(v) TSRMG(evenement_globals_id, zend_evenement_globals *, v)
#else
#define EVENEMENT_G(v) (evenement_globals.v)
#endif

#endif	/* PHP_EVENEMENT_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
