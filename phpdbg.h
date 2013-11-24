/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2013 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Felipe Pena <felipe@php.net>                                |
   | Authors: Joe Watkins <joe.watkins@live.co.uk>                        |
   +----------------------------------------------------------------------+
*/

#ifndef PHPDBG_H
#define PHPDBG_H

#ifdef PHP_WIN32
# define PHPDBG_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
# define PHPDBG_API __attribute__ ((visibility("default")))
#else
# define PHPDBG_API
#endif

#include "php.h"
#include "php_globals.h"
#include "php_variables.h"
#include "php_getopt.h"
#include "zend_builtin_functions.h"
#include "zend_extensions.h"
#include "zend_modules.h"
#include "zend_globals.h"
#include "zend_ini_scanner.h"
#include "zend_stream.h"
#include "SAPI.h"
#include <fcntl.h>
#include <sys/types.h>
#if defined(_WIN32) && !defined(__MINGW32__)
# include <windows.h>
# include "config.w32.h"
#else
# include "php_config.h"
#endif
#ifndef O_BINARY
#	define O_BINARY 0
#endif
#include "php_main.h"

#ifdef ZTS
# include "TSRM.h"
#endif

#ifdef HAVE_LIBREADLINE
#   include <readline/readline.h>
#   include <readline/history.h>
#endif

#include "phpdbg_cmd.h"
#include "phpdbg_utils.h"

#ifdef ZTS
# define PHPDBG_G(v) TSRMG(phpdbg_globals_id, zend_phpdbg_globals *, v)
#else
# define PHPDBG_G(v) (phpdbg_globals.v)
#endif

#define PHPDBG_NEXT   2
#define PHPDBG_UNTIL  3
#define PHPDBG_FINISH 4
#define PHPDBG_LEAVE  5

/* {{{ tables */
#define PHPDBG_BREAK_FILE       0
#define PHPDBG_BREAK_SYM        1
#define PHPDBG_BREAK_OPLINE     2
#define PHPDBG_BREAK_METHOD     3
#define PHPDBG_BREAK_COND       4
#define PHPDBG_BREAK_OPCODE     5
#define PHPDBG_BREAK_TABLES     6 /* }}} */

/* {{{ flags */
#define PHPDBG_HAS_FILE_BP      (1<<1)
#define PHPDBG_HAS_SYM_BP       (1<<2)
#define PHPDBG_HAS_OPLINE_BP    (1<<3)
#define PHPDBG_HAS_METHOD_BP    (1<<4)
#define PHPDBG_HAS_COND_BP      (1<<5)
#define PHPDBG_HAS_OPCODE_BP    (1<<6)
#define PHPDBG_BP_MASK          (PHPDBG_HAS_FILE_BP|PHPDBG_HAS_SYM_BP|PHPDBG_HAS_METHOD_BP|PHPDBG_HAS_OPLINE_BP|PHPDBG_HAS_COND_BP|PHPDBG_HAS_OPCODE_BP)

#define PHPDBG_IN_COND_BP       (1<<7)
#define PHPDBG_IN_EVAL          (1<<8)

#define PHPDBG_IS_STEPPING      (1<<9)
#define PHPDBG_IS_QUIET         (1<<10)
#define PHPDBG_IS_QUITTING      (1<<11)
#define PHPDBG_IS_COLOURED      (1<<12)
#define PHPDBG_IS_CLEANING      (1<<13)

#define PHPDBG_IN_UNTIL			(1<<14)
#define PHPDBG_IN_FINISH		(1<<15)
#define PHPDBG_IN_LEAVE			(1<<16)
#define PHPDBG_SEEK_MASK		(PHPDBG_IN_UNTIL|PHPDBG_IN_FINISH|PHPDBG_IN_LEAVE)

#define PHPDBG_IS_REGISTERED	(1<<17)
#define PHPDBG_IS_STEPONEVAL	(1<<18)
#define PHPDBG_IS_INITIALIZING	(1<<19)
#define PHPDBG_IS_SIGNALED      (1<<20)
#define PHPDBG_IS_INTERACTIVE	(1<<21)

#ifndef _WIN32
#   define PHPDBG_DEFAULT_FLAGS    (PHPDBG_IS_QUIET|PHPDBG_IS_COLOURED)
#else
#   define PHPDBG_DEFAULT_FLAGS    (PHPDBG_IS_QUIET)
#endif /* }}} */

/* {{{ strings */
#define PHPDBG_ISSUES "http://github.com/krakjoe/phpdbg/issues"
#define PHPDBG_VERSION "0.2.0-dev" /* }}} */

/* {{{ output descriptors */
#define PHPDBG_STDIN 			0
#define PHPDBG_STDOUT			1
#define PHPDBG_STDERR			2
#define PHPDBG_IO_FDS 			3 /* }}} */

/* {{{ structs */
ZEND_BEGIN_MODULE_GLOBALS(phpdbg)
    HashTable bp[PHPDBG_BREAK_TABLES];  			/* break points */
	HashTable registered;							/* registered */
	HashTable seek;									/* seek oplines */
	phpdbg_frame_t frame;							/* frame */
	
	char *exec;                         			/* file to execute */
	size_t exec_len;                    			/* size of exec */
	zend_op_array *ops;                 			/* op_array */
	zval *retval;                       			/* return value */
	int bp_count;                       			/* breakpoint count */
	int del_bp_num;                     			/* breakpoint to delete */
	int vmret;                          			/* return from last opcode handler execution */

	FILE *oplog;                        			/* opline log */
	FILE *io[PHPDBG_IO_FDS];						/* io */

    char *prompt[2];                       			/* prompt */
    const phpdbg_color_t *colors[PHPDBG_COLORS];	/* colors */
    
	phpdbg_command_t *lcmd;							/* last command */
	phpdbg_param_t lparam;              			/* last param */
	
	zend_ulong flags;                   			/* phpdbg flags */
ZEND_END_MODULE_GLOBALS(phpdbg) /* }}} */

#endif /* PHPDBG_H */
