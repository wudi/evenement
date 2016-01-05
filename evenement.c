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
  | Author: Eagle.Wu <0x05d@gmail.com>                                   |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_evenement.h"

const zend_function_entry evenement_functions[] = {
	PHP_FE_END	/* Must be the last line in evenement_functions[] */
};

zend_class_entry *event_emitter_interface_ce;
zend_class_entry *event_emitter_ce;
zend_class_entry *event_emitter_trait_ce;

zend_module_entry evenement_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"evenement",
	evenement_functions,
	PHP_MINIT(evenement),
	PHP_MSHUTDOWN(evenement),
	PHP_RINIT(evenement),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(evenement),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(evenement),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_EVENEMENT_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};


#ifdef COMPILE_DL_EVENEMENT
ZEND_GET_MODULE(evenement)
#endif

ZEND_BEGIN_ARG_INFO_EX(interface_event_listener_args, 0, 0, 2)
	ZEND_ARG_INFO(0, event)
	ZEND_ARG_TYPE_INFO(0, listener, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(interface_event_args, 0, 0, 1)
	ZEND_ARG_INFO(0, event)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(void_args, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(interface_event_arguments_args, 0, 0, 2)
	ZEND_ARG_INFO(0, event)
	ZEND_ARG_ARRAY_INFO(0, arguments, 0)
ZEND_END_ARG_INFO()

zend_function_entry event_emitter_interface_methods[] = {
	PHP_ABSTRACT_ME(event_emitter_interface, on, 				 interface_event_listener_args)
	PHP_ABSTRACT_ME(event_emitter_interface, once, 				 interface_event_listener_args)
	PHP_ABSTRACT_ME(event_emitter_interface, removeListener, 	 interface_event_listener_args)
	PHP_ABSTRACT_ME(event_emitter_interface, removeAllListeners, interface_event_args)
	PHP_ABSTRACT_ME(event_emitter_interface, listeners, 		 interface_event_args)
	PHP_ABSTRACT_ME(event_emitter_interface, emit, 				 interface_event_arguments_args)
	{NULL, NULL, NULL}
};

ZEND_MINIT_FUNCTION(event_emitter_interface){
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "Evenement\\EventEmitterInterface", event_emitter_interface_methods);
	event_emitter_interface_ce = zend_register_internal_class(&ce TSRMLS_CC);
	event_emitter_interface_ce->ce_flags |= ZEND_ACC_INTERFACE;

	return SUCCESS;
}

zend_function_entry event_emitter_methods[] = {
	{NULL, NULL, NULL}
};

ZEND_MINIT_FUNCTION(event_emitter){
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "Evenement\\EventEmitter", event_emitter_methods);
	event_emitter_ce = zend_register_internal_class(&ce TSRMLS_CC);
	//event_emitter_ce->ce_flags |= ZEND_ACC_INTERFACE;
	//zend_class_implements(event_emitter_ce, 1, event_emitter_interface_ce);

	return SUCCESS;
}


/* {{{ EventEmitterTrait }}}
 */

zval *get_listeners(zval *this_ptr TSRMLS_DC){

  zval *listeners = zend_read_property(event_emitter_trait_ce, this_ptr, ZEND_STRL("listeners"), 1 TSRMLS_CC);

  if(Z_TYPE_P (listeners) != IS_ARRAY){

    MAKE_STD_ZVAL(listeners);
    array_init(listeners);

    zend_update_property(event_emitter_trait_ce, this_ptr, ZEND_STRL("listeners"), listeners TSRMLS_CC);
    //zval_ptr_dtor (&listeners);
  }

  return listeners;
}


/* {{{ on
 * proto: on($event, callable $listener)
 */
PHP_METHOD(event_emitter_trait, on) {
	char *event;
    uint event_len;

    zval
      *listener,
      **list,
      *tmp;

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &event, &event_len, &listener) != SUCCESS){
       return;
    }

    if(!zend_is_callable(listener, IS_CALLABLE_CHECK_NO_ACCESS, NULL TSRMLS_CC)){
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Argument 2 is expected to be a valid callback");
        return;
    }

    zval *listeners = get_listeners(getThis () TSRMLS_CC);

    Z_ADDREF_P (listener);
    if(zend_hash_find (Z_ARRVAL_P (listeners), event, event_len + 1, (void **)&list) == SUCCESS){
      zend_hash_next_index_insert(Z_ARRVAL_PP (list), (void **)&listener, sizeof(zval *), NULL);
    }else{
      MAKE_STD_ZVAL (tmp);
      array_init (tmp);
      zend_hash_next_index_insert (Z_ARRVAL_P (tmp), (void **)&listener, sizeof(zval *), NULL);

      zend_hash_update (Z_ARRVAL_P (listeners), event, event_len + 1, (void **)&tmp, sizeof(zval *), NULL);
    }
}
/* }}} */

PHP_METHOD(event_emitter_trait, once) {
	char *event;
	uint event_len = 0;
    zval *listener;
    zval *onceListener = NULL;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &event, &event_len, &listener) != SUCCESS){
		return;
	}

    if(!zend_is_callable(listener, IS_CALLABLE_CHECK_NO_ACCESS, NULL TSRMLS_CC)){
      php_error_docref(NULL TSRMLS_CC, E_WARNING, "Argument 2 is expected to be a valid callback");
      return;
    }

    //TODO: Achieve once method
}

PHP_METHOD(event_emitter_trait, removeListener) {
	char *event;
	uint event_len = 0;

    zval
      *listener,
      *listeners,
      **list,
      **entry,
      res,
      *index;

    HashPosition pos;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &event, &event_len, &listener) != SUCCESS){
		return;
	}

    /*php_printf ("value: %s\n", event);*/

    if(!zend_is_callable (listener,IS_CALLABLE_CHECK_NO_ACCESS, NULL TSRMLS_CC)){
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Argument 2 is expected to be a valid callback");
        return;
    }

    listeners = get_listeners (getThis () TSRMLS_CC);
    if(zend_hash_find (Z_ARRVAL_P (listeners), event, event_len + 1, (void **)&list) != SUCCESS) {
      return;
    }

    zend_hash_internal_pointer_reset(Z_ARRVAL_PP(list)); //Reset
    while (zend_hash_get_current_data(Z_ARRVAL_PP(list), (void **)&entry) == SUCCESS) {

      is_identical_function(&res, *entry, listener TSRMLS_CC); //比对是否 [完全相等]

      if (Z_LVAL(res)) {
          MAKE_STD_ZVAL (index);
          zend_hash_get_current_key_zval(Z_ARRVAL_PP(list), index);
          /*php_printf ("[Index]: %ld\n", Z_LVAL_P (index));*/
          zend_hash_index_del (Z_ARRVAL_PP (list), Z_LVAL_P(index));
          zval_ptr_dtor (&index);
          return;
      }
      /*php_printf ("[move_forward]\n");*/
      zend_hash_move_forward(Z_ARRVAL_PP(list)); //Next
    }
    zend_hash_internal_pointer_reset(Z_ARRVAL_PP(list)); //Reset
}

PHP_METHOD(event_emitter_trait, removeAllListeners) {
	char *event = NULL;
	uint event_len = 0;
    zval *listeners;
    zval *value;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &event, &event_len) != SUCCESS){
		return;
	}

    listeners = get_listeners (getThis () TSRMLS_CC);

    if(ZEND_NUM_ARGS() > 0){
        zend_hash_del (Z_ARRVAL_P (listeners), event, event_len + 1);
        return;
    }

    zval_ptr_dtor (&listeners);

    MAKE_STD_ZVAL (value);
    array_init (value);
    zend_update_property(event_emitter_trait_ce, getThis (), ZEND_STRL("listeners"), value TSRMLS_CC);
}

PHP_METHOD(event_emitter_trait, listeners) {
	char *event;
	uint event_len = 0;
    zval *listeners;
    zval **value;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &event, &event_len) != SUCCESS){
		return;
	}

    listeners = get_listeners (getThis () TSRMLS_CC);

    if(zend_hash_find (Z_ARRVAL_P (listeners), event, event_len + 1, (void **)&value) == SUCCESS){
        RETURN_ZVAL (*value, 1, 0);
    }

    array_init (return_value);
}

PHP_METHOD(event_emitter_trait, emit) {
	char *event = NULL;
	uint event_len = 0;
    uint argument_len = 0;

	zval
        *arguments,
        *listeners,
        **array,
        **value_pp,
        *retval;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|a", &event, &event_len, &arguments) != SUCCESS){
		return;
	}

    if(ZEND_NUM_ARGS () < 2){
        MAKE_STD_ZVAL (arguments);
        array_init (arguments);
    }else{
        argument_len = Z_ARRVAL_P (arguments)->nNumOfElements;
    }

    /*php_printf ("argument_len : %d\n", argument_len);*/

    listeners = get_listeners (getThis () TSRMLS_CC);
    if(zend_hash_find (Z_ARRVAL_P (listeners), event, event_len + 1, (void **)&array) != SUCCESS){
        return;
    }

    MAKE_STD_ZVAL (retval);
    zend_hash_internal_pointer_reset(Z_ARRVAL_PP(array)); //Reset
    while (zend_hash_get_current_data(Z_ARRVAL_PP(array), (void **)&value_pp) == SUCCESS) {
        call_user_function (NULL, NULL, *value_pp, retval, argument_len, &arguments TSRMLS_CC);
        zend_hash_move_forward(Z_ARRVAL_PP(array)); //Next
    }
    zend_hash_internal_pointer_reset(Z_ARRVAL_PP(array)); //Reset

    zval_ptr_dtor (&retval);
}


zend_function_entry event_emitter_trait_methods[] = {
	PHP_ME(event_emitter_trait, on, interface_event_listener_args, ZEND_ACC_PUBLIC)
	PHP_ME(event_emitter_trait, once, interface_event_listener_args, ZEND_ACC_PUBLIC)
	PHP_ME(event_emitter_trait, removeListener, interface_event_listener_args, ZEND_ACC_PUBLIC)
	PHP_ME(event_emitter_trait, removeAllListeners, interface_event_args, ZEND_ACC_PUBLIC)
	PHP_ME(event_emitter_trait, listeners, interface_event_args, ZEND_ACC_PUBLIC)
	PHP_ME(event_emitter_trait, emit, interface_event_arguments_args, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

ZEND_MINIT_FUNCTION(event_emitter_trait){
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "Evenement\\EventEmitterTrait", event_emitter_trait_methods);

	event_emitter_trait_ce = zend_register_internal_class(&ce TSRMLS_CC);
	event_emitter_trait_ce->ce_flags |= ZEND_ACC_TRAIT;

    zend_declare_property_null(event_emitter_trait_ce, ZEND_STRL("listeners"), ZEND_ACC_PUBLIC TSRMLS_CC);

	return SUCCESS;
}

PHP_MINIT_FUNCTION(evenement)
{
	ZEND_MODULE_STARTUP_N(event_emitter_interface)(INIT_FUNC_ARGS_PASSTHRU);
	ZEND_MODULE_STARTUP_N(event_emitter)(INIT_FUNC_ARGS_PASSTHRU);
	ZEND_MODULE_STARTUP_N(event_emitter_trait)(INIT_FUNC_ARGS_PASSTHRU);

	return SUCCESS;
}


PHP_MSHUTDOWN_FUNCTION(evenement)
{
	return SUCCESS;
}



PHP_RINIT_FUNCTION(evenement)
{
	return SUCCESS;
}



PHP_RSHUTDOWN_FUNCTION(evenement)
{
	return SUCCESS;
}


PHP_MINFO_FUNCTION(evenement)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "evenement support", "enabled");
	php_info_print_table_end();

}





