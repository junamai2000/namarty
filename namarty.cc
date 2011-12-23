/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group                                |
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

/* $Id: header,v 1.16.2.1.2.1 2007/01/01 19:32:09 iliaa Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
	#include "php.h"
	#include "php_ini.h"
	#include "ext/standard/php_var.h"
	#include "ext/standard/info.h"
	#include "php_namarty.h"
};

#include "lib/namarty_core.h"

/* If you declare any globals in php_namarty.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(namarty)
*/

/* True global resources - no need for thread safety here */
static zend_class_entry *php_namarty_ce;
static zend_object_handlers php_bar_object_handlers;
static int le_namarty;

class php_namarty : public namarty_core {
 private:
	zval *_tpl_vars;
 protected:
	virtual bool concreteForeach(const char* from, const char* item);

 public:
	php_namarty();
	~php_namarty();

	virtual const char* get_template_vars(const char* key);
	virtual const char* get(const char* key);
	virtual void assign(const char* key, void* val);
	virtual void register_outputfilter(void *funcs);
	virtual const char* through_outputfilter(const char * buffer);
};

php_namarty::php_namarty() {
	MAKE_STD_ZVAL(_tpl_vars);
	if(array_init(_tpl_vars) != SUCCESS)
	{
		// ここでエラーの取り扱いを行う
	}
    ZVAL_DELREF(_tpl_vars);
}

php_namarty::~php_namarty() {

}

bool php_namarty::concreteForeach(const char* from, const char* item)
{
	zval **zvalue;
	if (zend_hash_find(HASH_OF(_tpl_vars),  (char*)from, strlen(from)+1, (void**)&zvalue) == FAILURE) {
		printf("from: %s can't find\n", from);
		return false;
	}
	int for_count = zend_hash_num_elements(Z_ARRVAL_P(*zvalue));
	long count = this->getForCounter();

	if (count >= for_count) {
		return false;
	}

	zval *tmp = *zvalue;
	zval **tmp2 = NULL;

	switch (Z_TYPE_P(*zvalue)) {
		case IS_NULL:
		case IS_BOOL:
		case IS_DOUBLE:
		case IS_STRING:
		case IS_LONG:
		case IS_OBJECT:
			printf("not array\n");
			return false;
		case IS_ARRAY:
			if (zend_hash_index_find(HASH_OF(tmp), count, (void**)&tmp2) == FAILURE) {
				printf("can't find index %ld\n", count);
				//php_var_dump(zvalue, 1 TSRMLS_CC);
				return false;
			}
			zval_add_ref(&*zvalue);
			//printf("item: %s\n", item);
			add_assoc_zval(_tpl_vars, (char*)item, *tmp2);
			count++;
			this->setForCounter(count);
			break;
		default:
			printf("unknown value\n");
			return false;
			break;
	}
	return true;
}

const char* php_namarty::get(const char* key)
{
//	php_var_dump(&_tpl_vars, 1 TSRMLS_CC);
	zval **zvalue;
	char *save_point = NULL;
	char *hash_val = NULL;

	if (strstr(key, ".")) {
		zval *tmp_ptr = _tpl_vars;
		for (hash_val = strtok_r((char *)key, ".", &save_point);
		     hash_val;
			 hash_val = strtok_r(NULL, ".", &save_point))
		{
			if (zend_hash_find(HASH_OF(tmp_ptr),  (char*)hash_val, strlen(hash_val)+1, (void**)&zvalue) == FAILURE) {
				return "";
			}
			//php_var_dump(zvalue, 1 TSRMLS_CC);
			tmp_ptr = *zvalue;
		}
		convert_to_string_ex(zvalue);
		return Z_STRVAL_PP(zvalue);
	}
	
	if (zend_hash_find(HASH_OF(_tpl_vars),  (char*)key, strlen(key)+1, (void**)&zvalue) == FAILURE) {
		return "";
	} else if (zend_hash_index_find(HASH_OF(_tpl_vars), 0, (void**)&zvalue) == FAILURE) {

	} else if (zend_hash_find(&EG(symbol_table), (char*)key, strlen(key)+1, (void**)&zvalue) == FAILURE) {

	}
	
	switch (Z_TYPE_P(*zvalue)) {
		case IS_ARRAY:
			return "Array";
		decault:
			break;
	}

	convert_to_string_ex(zvalue);
	return Z_STRVAL_PP(zvalue);
}

const char* php_namarty::get_template_vars(const char* key)
{
	zval **zvalue;
	
	if (zend_hash_find(Z_ARRVAL_P(_tpl_vars),  (char*)key, strlen(key)+1, (void**)&zvalue) == FAILURE) {

	} else if (zend_hash_index_find(Z_ARRVAL_P(_tpl_vars), 0, (void**)&zvalue) == FAILURE) {

	} else if (zend_hash_find(&EG(symbol_table), (char*)key, strlen(key)+1, (void**)&zvalue) == FAILURE) {

	}

	convert_to_string_ex(zvalue);
	return Z_STRVAL_PP(zvalue);
}

void php_namarty::assign(const char* key, void* _val)
{
	zval *zvalue = (zval*)_val;

	switch (Z_TYPE_P(zvalue)) {
		case IS_NULL:
			add_assoc_unset(_tpl_vars, (char*)key);
			break;
		case IS_BOOL:
			add_assoc_bool(_tpl_vars, (char*)key, Z_BVAL_P(zvalue));
			break;
		case IS_DOUBLE:
			add_assoc_double(_tpl_vars, (char*)key, Z_DVAL_P(zvalue));
			break;
		case IS_STRING:
			add_assoc_stringl(_tpl_vars, (char*)key, Z_STRVAL_P(zvalue), Z_STRLEN_P(zvalue), 1);
			break;
		case IS_LONG:
			add_assoc_long(_tpl_vars, (char*)key, Z_LVAL_P(zvalue));
			break;
		case IS_ARRAY:
			zval_add_ref(&zvalue);
			add_assoc_zval(_tpl_vars, (char*)key, zvalue);
			break;
		case IS_OBJECT:
			zval_add_ref(&zvalue);
			add_assoc_zval(_tpl_vars, (char*)key, zvalue);
	}

	//php_var_dump(&_tpl_vars, 1 TSRMLS_CC);

	return;
}

void php_namarty::register_outputfilter(void *funcs)
{
	_has_outputfilter = true;

}

const char* php_namarty::through_outputfilter(const char *buffer)
{

}

// class property
#define CLASS_PROP_TEMPLATE_DIR "template_dir"


typedef struct _php_namarty_object {
	zend_object zo;
	namarty_core *_core;
} php_namarty_object;

static void php_namarty_object_free_storage(php_namarty_object *intern TSRMLS_DC) {
	zend_object_std_dtor(&intern->zo TSRMLS_CC);
	delete intern->_core;
	delete intern;
}

static zend_object_value php_namarty_object_handler_new(zend_class_entry *ce TSRMLS_DC) {
	zend_object_value retval;
	php_namarty_object *intern;
	zval *tmp;

	intern = new php_namarty_object();
	intern->_core = new php_namarty();

	zend_object_std_init(&intern->zo, ce TSRMLS_CC);
	zend_hash_copy(intern->zo.properties, &ce->default_properties, (copy_ctor_func_t)zval_add_ref, (void*)&tmp, sizeof(zval*));

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t)zend_objects_destroy_object, (zend_objects_free_object_storage_t)php_namarty_object_free_storage, NULL TSRMLS_CC);
	retval.handlers = &php_bar_object_handlers;



	return retval;
}


ZEND_BEGIN_ARG_INFO_EX(arginfo_namarty_assign, 0, 0, 2)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_namarty_display, 0, 0, 1)
	ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()


/* {{{ namarty_functions[]
 *
 * Every userr visible function must have an entry in namarty_functions[].
 */
zend_function_entry namarty_functions[] = {
	PHP_FE(confirm_namarty_compiled,	NULL)		/* For testing, remove later. */
	PHP_ME(namarty, __construct, NULL, 0)
	PHP_ME(namarty, assign, arginfo_namarty_assign, ZEND_ACC_PUBLIC)
	PHP_ME(namarty, display, arginfo_namarty_display, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}	/* Must be the last line in namarty_functions[] */
};
/* }}} */

/* {{{ namarty_module_entry
 */
zend_module_entry namarty_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"namarty",
	namarty_functions,
	PHP_MINIT(namarty),
	PHP_MSHUTDOWN(namarty),
	PHP_RINIT(namarty),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(namarty),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(namarty),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_NAMARTY
ZEND_GET_MODULE(namarty)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("namarty.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_namarty_globals, namarty_globals)
    STD_PHP_INI_ENTRY("namarty.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_namarty_globals, namarty_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_namarty_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_namarty_init_globals(zend_namarty_globals *namarty_globals)
{
	namarty_globals->global_value = 0;
	namarty_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(namarty)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "Namarty", namarty_functions);
	php_namarty_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_namarty_ce->create_object = php_namarty_object_handler_new;
	memcpy(&php_bar_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	// property
	zend_declare_property_string(php_namarty_ce, CLASS_PROP_TEMPLATE_DIR,
			strlen(CLASS_PROP_TEMPLATE_DIR), "", ZEND_ACC_PUBLIC TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(namarty)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(namarty)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(namarty)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(namarty)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "namarty support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_namarty_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_namarty_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "namarty", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

PHP_METHOD(namarty, __construct) {
}

PHP_METHOD(namarty, assign) {
	/*
	zval **arg_key, **arg_val;

	if (zend_get_parameters_ex(ZEND_NUM_ARGS(), &arg_key, &arg_val) == FAILURE) {
		return;
	}

	php_namarty_object *n = (php_namarty_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	convert_to_string_ex(arg_key);
	//convert_to_string_ex(arg_val);
	n->_core->assign(Z_STRVAL_PP(arg_key), arg_val);
  	*/
	
	char *key=NULL;
	zval *zvalue;
	int key_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &key, &key_len, &zvalue) == FAILURE) {
		return;
	}

	php_namarty_object *n = (php_namarty_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	n->_core->assign(key, zvalue);
	return;
}

PHP_METHOD(namarty, display) {
	const char *file = NULL;
	int file_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &file, &file_len) == FAILURE) {
		return;
	}
	php_namarty_object *n = (php_namarty_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	zval *temp_zval;
	temp_zval = zend_read_property(Z_OBJCE_P(getThis()), getThis(), CLASS_PROP_TEMPLATE_DIR, strlen(CLASS_PROP_TEMPLATE_DIR), 1 TSRMLS_CC);
	if( Z_TYPE_P(temp_zval) == IS_STRING ){
		n->_core->template_dir = Z_STRVAL_P(temp_zval);
	}
	n->_core->display(file);
}


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
