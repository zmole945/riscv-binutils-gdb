/* *INDENT-OFF* */ /* ATTR_FORMAT confuses indent, avoid running it for now */
/* Basic, host-specific, and target-specific definitions for GDB.
   Copyright (C) 1986, 1989, 1991-1996, 1998-2000
   Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifndef DEFS_H
#define DEFS_H

#include "config.h"		/* Generated by configure */
#include <stdio.h>
#include <errno.h>		/* System call error return status */
#include <limits.h>

#ifdef HAVE_STDDEF_H
#include <stddef.h>
#else
#include <sys/types.h>		/* for size_t */
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

/* Just in case they're not defined in stdio.h. */

#ifndef SEEK_SET
#define SEEK_SET 0
#endif
#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif

/* First include ansidecl.h so we can use the various macro definitions
   here and in all subsequent file inclusions.  */

#include "ansidecl.h"

#include <stdarg.h>		/* for va_list */

#include "libiberty.h"

#include "progress.h"

#ifdef USE_MMALLOC
#include "mmalloc.h"
#endif

/* For BFD64 and bfd_vma.  */
#include "bfd.h"

/* An address in the program being debugged.  Host byte order.  Rather
   than duplicate all the logic in BFD which figures out what type
   this is (long, long long, etc.) and whether it needs to be 64
   bits (the host/target interactions are subtle), we just use
   bfd_vma.  */

typedef bfd_vma CORE_ADDR;

/* This is to make sure that LONGEST is at least as big as CORE_ADDR.  */

#ifndef LONGEST

#ifdef BFD64

#define LONGEST BFD_HOST_64_BIT
#define ULONGEST BFD_HOST_U_64_BIT

#else /* No BFD64 */

#ifdef CC_HAS_LONG_LONG
#define LONGEST long long
#define ULONGEST unsigned long long
#else
#ifdef BFD_HOST_64_BIT
/* BFD_HOST_64_BIT is defined for some hosts that don't have long long
   (e.g. i386-windows) so try it.  */
#define LONGEST BFD_HOST_64_BIT
#define ULONGEST BFD_HOST_U_64_BIT
#else
#define LONGEST long
#define ULONGEST unsigned long
#endif
#endif

#endif /* No BFD64 */

#endif /* ! LONGEST */

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

/* Macros to do string compares.

   NOTE: cagney/2000-03-14:

   While old code can continue to refer to these macros, new code is
   probably better off using strcmp() directly vis: ``strcmp() == 0''
   and ``strcmp() != 0''.

   This is because modern compilers can directly inline strcmp()
   making the original justification for these macros - avoid function
   call overhead by pre-testing the first characters
   (``*X==*Y?...:0'') - redundant.

   ``Even if [...] testing the first character does have a modest
   performance improvement, I'd rather that whenever a performance
   issue is found that we spend the effort on algorithmic
   optimizations than micro-optimizing.'' J.T. */

#define STRCMP(a,b) (*(a) == *(b) ? strcmp ((a), (b)) : (int)*(a) - (int)*(b))
#define STREQ(a,b) (*(a) == *(b) ? !strcmp ((a), (b)) : 0)
#define STREQN(a,b,c) (*(a) == *(b) ? !strncmp ((a), (b), (c)) : 0)

/* The character GNU C++ uses to build identifiers that must be unique from
   the program's identifiers (such as $this and $$vptr).  */
#define CPLUS_MARKER '$'	/* May be overridden to '.' for SysV */

/* Check if a character is one of the commonly used C++ marker characters.  */
extern int is_cplus_marker (int);

/* use tui interface if non-zero */
extern int tui_version;

#if defined(TUI)
/* all invocations of TUIDO should have two sets of parens */
#define TUIDO(x)	tuiDo x
#else
#define TUIDO(x)
#endif

/* enable xdb commands if set */
extern int xdb_commands;

/* enable dbx commands if set */
extern int dbx_commands;

extern int quit_flag;
extern int immediate_quit;
extern int sevenbit_strings;

extern void quit (void);

/* FIXME: cagney/2000-03-13: It has been suggested that the peformance
   benefits of having a ``QUIT'' macro rather than a function are
   marginal.  If the overhead of a QUIT function call is proving
   significant then its calling frequency should probably be reduced
   [kingdon].  A profile analyzing the current situtation is
   needed. */

#ifdef QUIT
/* do twice to force compiler warning */
#define QUIT_FIXME "FIXME"
#define QUIT_FIXME "ignoring redefinition of QUIT"
#else
#define QUIT { \
  if (quit_flag) quit (); \
  if (interactive_hook) interactive_hook (); \
  PROGRESS (1); \
}
#endif

/* Languages represented in the symbol table and elsewhere.
   This should probably be in language.h, but since enum's can't
   be forward declared to satisfy opaque references before their
   actual definition, needs to be here. */

enum language
  {
    language_unknown,		/* Language not known */
    language_auto,		/* Placeholder for automatic setting */
    language_c,			/* C */
    language_cplus,		/* C++ */
    language_java,		/* Java */
    language_chill,		/* Chill */
    language_fortran,		/* Fortran */
    language_m2,		/* Modula-2 */
    language_asm,		/* Assembly language */
    language_scm,    		/* Scheme / Guile */
    language_pascal		/* Pascal */
  };

enum precision_type
  {
    single_precision,
    double_precision,
    unspecified_precision
  };

/* the cleanup list records things that have to be undone
   if an error happens (descriptors to be closed, memory to be freed, etc.)
   Each link in the chain records a function to call and an
   argument to give it.

   Use make_cleanup to add an element to the cleanup chain.
   Use do_cleanups to do all cleanup actions back to a given
   point in the chain.  Use discard_cleanups to remove cleanups
   from the chain back to a given point, not doing them.  */

struct cleanup
  {
    struct cleanup *next;
    void (*function) (PTR);
    PTR arg;
  };


/* The ability to declare that a function never returns is useful, but
   not really required to compile GDB successfully, so the NORETURN and
   ATTR_NORETURN macros normally expand into nothing.  */

/* If compiling with older versions of GCC, a function may be declared
   "volatile" to indicate that it does not return.  */

#ifndef NORETURN
#if defined(__GNUC__) \
     && (__GNUC__ == 1 || (__GNUC__ == 2 && __GNUC_MINOR__ < 7))
#define NORETURN volatile
#else
#define NORETURN		/* nothing */
#endif
#endif

/* GCC 2.5 and later versions define a function attribute "noreturn",
   which is the preferred way to declare that a function never returns.
   However GCC 2.7 appears to be the first version in which this fully
   works everywhere we use it. */

#ifndef ATTR_NORETURN
#if defined(__GNUC__) && (__GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7))
#define ATTR_NORETURN __attribute__ ((noreturn))
#else
#define ATTR_NORETURN		/* nothing */
#endif
#endif

#ifndef ATTR_FORMAT
#if defined(__GNUC__) && (__GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 4))
#define ATTR_FORMAT(type, x, y) __attribute__ ((format(type, x, y)))
#else
#define ATTR_FORMAT(type, x, y)	/* nothing */
#endif
#endif

/* Needed for various prototypes */

struct symtab;
struct breakpoint;

/* From blockframe.c */

extern int inside_entry_func (CORE_ADDR);

extern int inside_entry_file (CORE_ADDR addr);

extern int inside_main_func (CORE_ADDR pc);

/* From ch-lang.c, for the moment. (FIXME) */

extern char *chill_demangle (const char *);

/* From utils.c */

extern void initialize_utils (void);

extern void notice_quit (void);

extern int strcmp_iw (const char *, const char *);

extern int subset_compare (char *, char *);

extern char *safe_strerror (int);

extern void init_malloc (void *);

extern void request_quit (int);

extern void do_cleanups (struct cleanup *);
extern void do_final_cleanups (struct cleanup *);
extern void do_my_cleanups (struct cleanup **, struct cleanup *);
extern void do_run_cleanups (struct cleanup *);
extern void do_exec_cleanups (struct cleanup *);
extern void do_exec_error_cleanups (struct cleanup *);

extern void discard_cleanups (struct cleanup *);
extern void discard_final_cleanups (struct cleanup *);
extern void discard_exec_error_cleanups (struct cleanup *);
extern void discard_my_cleanups (struct cleanup **, struct cleanup *);

/* NOTE: cagney/2000-03-04: This typedef is strictly for the
   make_cleanup function declarations below. Do not use this typedef
   as a cast when passing functions into the make_cleanup() code.
   Instead either use a bounce function or add a wrapper function.
   Calling a f(char*) function with f(void*) is non-portable. */
typedef void (make_cleanup_ftype) (void *);

extern struct cleanup *make_cleanup (make_cleanup_ftype *, void *);

extern struct cleanup *make_cleanup_freeargv (char **);

struct ui_file;
extern struct cleanup *make_cleanup_ui_file_delete (struct ui_file *);

extern struct cleanup *make_cleanup_close (int fd);

extern struct cleanup *make_cleanup_bfd_close (bfd *abfd);

extern struct cleanup *make_final_cleanup (make_cleanup_ftype *, void *);

extern struct cleanup *make_my_cleanup (struct cleanup **,
					make_cleanup_ftype *, void *);

extern struct cleanup *make_run_cleanup (make_cleanup_ftype *, void *);

extern struct cleanup *make_exec_cleanup (make_cleanup_ftype *, void *);
extern struct cleanup *make_exec_error_cleanup (make_cleanup_ftype *, void *);

extern struct cleanup *save_cleanups (void);
extern struct cleanup *save_final_cleanups (void);
extern struct cleanup *save_my_cleanups (struct cleanup **);

extern void restore_cleanups (struct cleanup *);
extern void restore_final_cleanups (struct cleanup *);
extern void restore_my_cleanups (struct cleanup **, struct cleanup *);

extern void free_current_contents (void *);

extern void null_cleanup (void *);

extern int myread (int, char *, int);

extern int query (char *, ...) ATTR_FORMAT (printf, 1, 2);

#if !defined (USE_MMALLOC)
/* NOTE: cagney/2000-03-04: The mmalloc functions need to use PTR
   rather than void* so that they are consistent with
   ../mmalloc/mmalloc.h. */
extern PTR mcalloc (PTR, size_t, size_t);
extern PTR mmalloc (PTR, size_t);
extern PTR mrealloc (PTR, PTR, size_t);
extern void mfree (PTR, PTR);
#endif

extern void init_page_info (void);

extern CORE_ADDR host_pointer_to_address (void *ptr);
extern void *address_to_host_pointer (CORE_ADDR addr);

/* From demangle.c */

extern void set_demangling_style (char *);

/* From tm.h */

struct type;
typedef int (use_struct_convention_fn) (int gcc_p, struct type * value_type);
extern use_struct_convention_fn generic_use_struct_convention;

typedef unsigned char *(breakpoint_from_pc_fn) (CORE_ADDR * pcptr, int *lenptr);

/* Annotation stuff.  */

extern int annotation_level;	/* in stack.c */

extern void begin_line (void);

extern void wrap_here (char *);

extern void reinitialize_more_filter (void);

/* Normal results */
extern struct ui_file *gdb_stdout;
/* Serious error notifications */
extern struct ui_file *gdb_stderr;
/* Log/debug/trace messages that should bypass normal stdout/stderr
   filtering.  For momement, always call this stream using
   *_unfiltered. In the very near future that restriction shall be
   removed - either call shall be unfiltered. (cagney 1999-06-13). */
extern struct ui_file *gdb_stdlog;
/* Target output that should bypass normal stdout/stderr filtering.
   For momement, always call this stream using *_unfiltered. In the
   very near future that restriction shall be removed - either call
   shall be unfiltered. (cagney 1999-07-02). */
extern struct ui_file *gdb_stdtarg;

#if defined(TUI)
#include "tui.h"
#include "tuiCommand.h"
#include "tuiData.h"
#include "tuiIO.h"
#include "tuiLayout.h"
#include "tuiWin.h"
#endif

#include "ui-file.h"

/* More generic printf like operations */

extern void fputs_filtered (const char *, struct ui_file *);

extern void fputs_unfiltered (const char *, struct ui_file *);

extern int fputc_filtered (int c, struct ui_file *);

extern int fputc_unfiltered (int c, struct ui_file *);

extern int putchar_unfiltered (int c);

extern void puts_filtered (const char *);

extern void puts_unfiltered (const char *);

extern void puts_debug (char *prefix, char *string, char *suffix);

extern void vprintf_filtered (const char *, va_list) ATTR_FORMAT (printf, 1, 0);

extern void vfprintf_filtered (struct ui_file *, const char *, va_list) ATTR_FORMAT (printf, 2, 0);

extern void fprintf_filtered (struct ui_file *, const char *, ...) ATTR_FORMAT (printf, 2, 3);

extern void fprintfi_filtered (int, struct ui_file *, const char *, ...) ATTR_FORMAT (printf, 3, 4);

extern void printf_filtered (const char *, ...) ATTR_FORMAT (printf, 1, 2);

extern void printfi_filtered (int, const char *, ...) ATTR_FORMAT (printf, 2, 3);

extern void vprintf_unfiltered (const char *, va_list) ATTR_FORMAT (printf, 1, 0);

extern void vfprintf_unfiltered (struct ui_file *, const char *, va_list) ATTR_FORMAT (printf, 2, 0);

extern void fprintf_unfiltered (struct ui_file *, const char *, ...) ATTR_FORMAT (printf, 2, 3);

extern void printf_unfiltered (const char *, ...) ATTR_FORMAT (printf, 1, 2);

extern void print_spaces (int, struct ui_file *);

extern void print_spaces_filtered (int, struct ui_file *);

extern char *n_spaces (int);

extern void fputstr_filtered (const char *str, int quotr, struct ui_file * stream);

extern void fputstr_unfiltered (const char *str, int quotr, struct ui_file * stream);

extern void fputstrn_unfiltered (const char *str, int n, int quotr, struct ui_file * stream);

/* Display the host ADDR on STREAM formatted as ``0x%x''. */
extern void gdb_print_host_address (void *addr, struct ui_file *stream);

/* Convert a CORE_ADDR into a HEX string.  paddr() is like %08lx.
   paddr_nz() is like %lx.  paddr_u() is like %lu. paddr_width() is
   for ``%*''. */
extern int strlen_paddr (void);
extern char *paddr (CORE_ADDR addr);
extern char *paddr_nz (CORE_ADDR addr);
extern char *paddr_u (CORE_ADDR addr);
extern char *paddr_d (LONGEST addr);

extern char *phex (ULONGEST l, int sizeof_l);
extern char *phex_nz (ULONGEST l, int sizeof_l);

extern void fprintf_symbol_filtered (struct ui_file *, char *,
				     enum language, int);

extern NORETURN void perror_with_name (char *) ATTR_NORETURN;

extern void print_sys_errmsg (char *, int);

/* From regex.c or libc.  BSD 4.4 declares this with the argument type as
   "const char *" in unistd.h, so we can't declare the argument
   as "char *".  */

extern char *re_comp (const char *);

/* From symfile.c */

extern void symbol_file_command (char *, int);

/* Remote targets may wish to use this as their load function.  */
extern void generic_load (char *name, int from_tty);

/* Summarise a download */
extern void print_transfer_performance (struct ui_file *stream,
					unsigned long data_count,
					unsigned long write_count,
					unsigned long time_count);

/* From top.c */

typedef void initialize_file_ftype (void);

extern char *skip_quoted (char *);

extern char *gdb_readline (char *);

extern char *command_line_input (char *, int, char *);

extern void print_prompt (void);

extern int input_from_terminal_p (void);

extern int info_verbose;

/* From printcmd.c */

extern void set_next_address (CORE_ADDR);

extern void print_address_symbolic (CORE_ADDR, struct ui_file *, int,
				    char *);

extern int build_address_symbolic (CORE_ADDR addr,
				   int do_demangle, 
				   char **name, 
				   int *offset, 
				   char **filename, 
				   int *line, 	
				   int *unmapped);

extern void print_address_numeric (CORE_ADDR, int, struct ui_file *);

extern void print_address (CORE_ADDR, struct ui_file *);

/* From source.c */

extern int openp (char *, int, char *, int, int, char **);

extern int source_full_path_of (char *, char **);

extern void mod_path (char *, char **);

extern void directory_command (char *, int);

extern void init_source_path (void);

extern char *symtab_to_filename (struct symtab *);

/* From exec.c */

extern void exec_set_section_offsets (bfd_signed_vma text_off,
				      bfd_signed_vma data_off,
				      bfd_signed_vma bss_off);

/* From findvar.c */

extern int read_relative_register_raw_bytes (int, char *);

/* Possible lvalue types.  Like enum language, this should be in
   value.h, but needs to be here for the same reason. */

enum lval_type
  {
    /* Not an lval. */
    not_lval,
    /* In memory.  Could be a saved register.  */
    lval_memory,
    /* In a register.  */
    lval_register,
    /* In a gdb internal variable.  */
    lval_internalvar,
    /* Part of a gdb internal variable (structure field).  */
    lval_internalvar_component,
    /* In a register series in a frame not the current one, which may have been
       partially saved or saved in different places (otherwise would be
       lval_register or lval_memory).  */
    lval_reg_frame_relative
  };

struct frame_info;

void default_get_saved_register (char *raw_buffer, int *optimized,
				 CORE_ADDR * addrp,
				 struct frame_info *frame, int regnum,
				 enum lval_type *lval);

/* From readline (but not in any readline .h files).  */

extern char *tilde_expand (char *);

/* Control types for commands */

enum misc_command_type
  {
    ok_command,
    end_command,
    else_command,
    nop_command
  };

enum command_control_type
  {
    simple_control,
    break_control,
    continue_control,
    while_control,
    if_control,
    invalid_control
  };

/* Structure for saved commands lines
   (for breakpoints, defined commands, etc).  */

struct command_line
  {
    struct command_line *next;
    char *line;
    enum command_control_type control_type;
    int body_count;
    struct command_line **body_list;
  };

extern struct command_line *read_command_lines (char *, int);

extern void free_command_lines (struct command_line **);

/* To continue the execution commands when running gdb asynchronously. 
   A continuation structure contains a pointer to a function to be called 
   to finish the command, once the target has stopped. Such mechanism is
   used bt the finish and until commands, and in the remote protocol
   when opening an extended-remote connection. */

struct continuation_arg
  {
    struct continuation_arg *next;
    union continuation_data {
      void *pointer;
      int   integer;
      long  longint;
    } data;
  };

struct continuation
  {
    void (*continuation_hook) (struct continuation_arg *);
    struct continuation_arg *arg_list;
    struct continuation *next;
  };
struct continuation continuation;

/* In infrun.c. */
extern struct continuation *cmd_continuation;
/* Used only by the step_1 function. */
extern struct continuation *intermediate_continuation;

/* From utils.c */
extern void add_continuation (void (*)(struct continuation_arg *),
			      struct continuation_arg *);
extern void do_all_continuations (void);
extern void discard_all_continuations (void);

extern void add_intermediate_continuation (void (*)(struct continuation_arg *),
			      struct continuation_arg *);
extern void do_all_intermediate_continuations (void);
extern void discard_all_intermediate_continuations (void);

/* String containing the current directory (what getwd would return).  */

extern char *current_directory;

/* Default radixes for input and output.  Only some values supported.  */
extern unsigned input_radix;
extern unsigned output_radix;

/* Possibilities for prettyprint parameters to routines which print
   things.  Like enum language, this should be in value.h, but needs
   to be here for the same reason.  FIXME:  If we can eliminate this
   as an arg to LA_VAL_PRINT, then we can probably move it back to
   value.h. */

enum val_prettyprint
  {
    Val_no_prettyprint = 0,
    Val_prettyprint,
    /* Use the default setting which the user has specified.  */
    Val_pretty_default
  };


/* Host machine definition.  This will be a symlink to one of the
   xm-*.h files, built by the `configure' script.  */

#include "xm.h"

/* Native machine support.  This will be a symlink to one of the
   nm-*.h files, built by the `configure' script.  */

#include "nm.h"

/* Target machine definition.  This will be a symlink to one of the
   tm-*.h files, built by the `configure' script.  */

#include "tm.h"

/* If the xm.h file did not define the mode string used to open the
   files, assume that binary files are opened the same way as text
   files */
#ifndef FOPEN_RB
#include "fopen-same.h"
#endif

/* Microsoft C can't deal with const pointers */

#ifdef _MSC_VER
#define CONST_PTR
#else
#define CONST_PTR const
#endif

/*
 * Allow things in gdb to be declared "volatile".  If compiling ANSI, it
 * just works.  If compiling with gcc but non-ansi, redefine to __volatile__.
 * If non-ansi, non-gcc, then eliminate "volatile" entirely, making those
 * objects be read-write rather than read-only.
 */

#ifndef volatile
#ifndef __STDC__
#ifdef __GNUC__
#define volatile __volatile__
#else
#define volatile		/* nothing */
#endif /* GNUC */
#endif /* STDC */
#endif /* volatile */

/* Defaults for system-wide constants (if not defined by xm.h, we fake it).
   FIXME: Assumes 2's complement arithmetic */

#if !defined (UINT_MAX)
#define	UINT_MAX ((unsigned int)(~0))	/* 0xFFFFFFFF for 32-bits */
#endif

#if !defined (INT_MAX)
#define	INT_MAX ((int)(UINT_MAX >> 1))	/* 0x7FFFFFFF for 32-bits */
#endif

#if !defined (INT_MIN)
#define INT_MIN ((int)((int) ~0 ^ INT_MAX))	/* 0x80000000 for 32-bits */
#endif

#if !defined (ULONG_MAX)
#define	ULONG_MAX ((unsigned long)(~0L))	/* 0xFFFFFFFF for 32-bits */
#endif

#if !defined (LONG_MAX)
#define	LONG_MAX ((long)(ULONG_MAX >> 1))	/* 0x7FFFFFFF for 32-bits */
#endif

#if !defined (ULONGEST_MAX)
#define	ULONGEST_MAX (~(ULONGEST)0)        /* 0xFFFFFFFFFFFFFFFF for 32-bits */
#endif

#if !defined (LONGEST_MAX)                 /* 0x7FFFFFFFFFFFFFFF for 32-bits */
#define	LONGEST_MAX ((LONGEST)(ULONGEST_MAX >> 1))
#endif

/* Convert a LONGEST to an int.  This is used in contexts (e.g. number of
   arguments to a function, number in a value history, register number, etc.)
   where the value must not be larger than can fit in an int.  */

extern int longest_to_int (LONGEST);

/* Assorted functions we can declare, now that const and volatile are 
   defined.  */

extern char *savestring (const char *, int);

extern char *msavestring (void *, const char *, int);

extern char *strsave (const char *);

extern char *mstrsave (void *, const char *);

/* FIXME; was long, but this causes compile errors in msvc if already
   defined */
#ifdef _MSC_VER
extern PTR xmmalloc (PTR, size_t);
extern PTR xmrealloc (PTR, PTR, size_t);
#else
extern PTR xmmalloc (PTR, long);
extern PTR xmrealloc (PTR, PTR, long);
#endif

extern int parse_escape (char **);

/* Message to be printed before the error message, when an error occurs.  */

extern char *error_pre_print;

/* Message to be printed before the error message, when an error occurs.  */

extern char *quit_pre_print;

/* Message to be printed before the warning message, when a warning occurs.  */

extern char *warning_pre_print;

extern NORETURN void verror (const char *fmt, va_list ap) ATTR_NORETURN;

extern NORETURN void error (const char *fmt, ...) ATTR_NORETURN;

/* DEPRECATED: Use error(), verror() or error_stream(). */
extern NORETURN void error_begin (void);

extern NORETURN void error_stream (struct ui_file *) ATTR_NORETURN;

/* Returns a freshly allocate buffer containing the last error
   message.  */
extern char *error_last_message (void);

extern NORETURN void internal_verror (const char *, va_list ap) ATTR_NORETURN;

extern NORETURN void internal_error (char *, ...) ATTR_NORETURN;

extern NORETURN void nomem (long) ATTR_NORETURN;

/* Reasons for calling return_to_top_level.  Note: enum value 0 is
   reserved for internal use as the return value from an initial
   setjmp().  */

enum return_reason
  {
    /* User interrupt.  */
    RETURN_QUIT = 1,
    /* Any other error.  */
    RETURN_ERROR
  };

#define	ALL_CLEANUPS	((struct cleanup *)0)

#define RETURN_MASK(reason)	(1 << (int)(reason))
#define RETURN_MASK_QUIT	RETURN_MASK (RETURN_QUIT)
#define RETURN_MASK_ERROR	RETURN_MASK (RETURN_ERROR)
#define RETURN_MASK_ALL		(RETURN_MASK_QUIT | RETURN_MASK_ERROR)
typedef int return_mask;

extern NORETURN void return_to_top_level (enum return_reason) ATTR_NORETURN;

/* If CATCH_ERRORS_FTYPE throws an error, catch_errors() returns zero
   otherwize the result from CATCH_ERRORS_FTYPE is returned. It is
   probably useful for CATCH_ERRORS_FTYPE to always return a non-zero
   value. It's unfortunate that, catch_errors() does not return an
   indication of the exact exception that it caught - quit_flag might
   help. */

typedef int (catch_errors_ftype) (PTR);
extern int catch_errors (catch_errors_ftype *, PTR, char *, return_mask);

/* Template to catch_errors() that wraps calls to command
   functions. */

typedef void (catch_command_errors_ftype) (char *, int);
extern int catch_command_errors (catch_command_errors_ftype *func, char *command, int from_tty, return_mask);

extern void warning_begin (void);

extern void warning (const char *, ...) ATTR_FORMAT (printf, 1, 2);

/* Global functions from other, non-gdb GNU thingies.
   Libiberty thingies are no longer declared here.  We include libiberty.h
   above, instead.  */

#ifndef GETENV_PROVIDED
extern char *getenv (const char *);
#endif

/* From other system libraries */

#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif

#ifdef HAVE_STDLIB_H
#if defined(_MSC_VER) && !defined(__cplusplus)
/* msvc defines these in stdlib.h for c code */
#undef min
#undef max
#endif
#include <stdlib.h>
#endif
#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif


/* We take the address of fclose later, but some stdio's forget
   to declare this.  We can't always declare it since there's
   no way to declare the parameters without upsetting some compiler
   somewhere. */

#ifndef FCLOSE_PROVIDED
extern int fclose (FILE *);
#endif

#ifndef atof
extern double atof (const char *);	/* X3.159-1989  4.10.1.1 */
#endif

#ifndef MALLOC_INCOMPATIBLE

#ifdef NEED_DECLARATION_MALLOC
extern PTR malloc ();
#endif

#ifdef NEED_DECLARATION_REALLOC
extern PTR realloc ();
#endif

#ifdef NEED_DECLARATION_FREE
extern void free ();
#endif

#endif /* MALLOC_INCOMPATIBLE */

/* Various possibilities for alloca.  */
#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* Not GNU C */
#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#else
#ifdef _AIX
#pragma alloca
#else

/* We need to be careful not to declare this in a way which conflicts with
   bison.  Bison never declares it as char *, but under various circumstances
   (like __hpux) we need to use void *.  */
#if defined (__STDC__) || defined (__hpux)
extern void *alloca ();
#else /* Don't use void *.  */
extern char *alloca ();
#endif /* Don't use void *.  */
#endif /* Not _AIX */
#endif /* Not HAVE_ALLOCA_H */
#endif /* Not GNU C */
#endif /* alloca not defined */

/* HOST_BYTE_ORDER must be defined to one of these.  */

#ifdef HAVE_ENDIAN_H
#include <endian.h>
#endif

#if !defined (BIG_ENDIAN)
#define BIG_ENDIAN 4321
#endif

#if !defined (LITTLE_ENDIAN)
#define LITTLE_ENDIAN 1234
#endif

/* Dynamic target-system-dependent parameters for GDB. */
#include "gdbarch.h"
#if (GDB_MULTI_ARCH == 0)
/* Multi-arch targets _should_ be including "arch-utils.h" directly
   into their *-tdep.c file.  This is a prop to help old non-
   multi-arch targets to continue to compile. */
#include "arch-utils.h"
#endif

/* Static target-system-dependent parameters for GDB. */

/* Number of bits in a char or unsigned char for the target machine.
   Just like CHAR_BIT in <limits.h> but describes the target machine.  */
#if !defined (TARGET_CHAR_BIT)
#define TARGET_CHAR_BIT 8
#endif

/* Number of bits in a short or unsigned short for the target machine. */
#if !defined (TARGET_SHORT_BIT)
#define TARGET_SHORT_BIT (2 * TARGET_CHAR_BIT)
#endif

/* Number of bits in an int or unsigned int for the target machine. */
#if !defined (TARGET_INT_BIT)
#define TARGET_INT_BIT (4 * TARGET_CHAR_BIT)
#endif

/* Number of bits in a long or unsigned long for the target machine. */
#if !defined (TARGET_LONG_BIT)
#define TARGET_LONG_BIT (4 * TARGET_CHAR_BIT)
#endif

/* Number of bits in a long long or unsigned long long for the target machine. */
#if !defined (TARGET_LONG_LONG_BIT)
#define TARGET_LONG_LONG_BIT (2 * TARGET_LONG_BIT)
#endif

/* Number of bits in a float for the target machine. */
#if !defined (TARGET_FLOAT_BIT)
#define TARGET_FLOAT_BIT (4 * TARGET_CHAR_BIT)
#endif

/* Number of bits in a double for the target machine. */
#if !defined (TARGET_DOUBLE_BIT)
#define TARGET_DOUBLE_BIT (8 * TARGET_CHAR_BIT)
#endif

/* Number of bits in a long double for the target machine.  */
#if !defined (TARGET_LONG_DOUBLE_BIT)
#define TARGET_LONG_DOUBLE_BIT (2 * TARGET_DOUBLE_BIT)
#endif

/* Number of bits in a pointer for the target machine */
#if !defined (TARGET_PTR_BIT)
#define TARGET_PTR_BIT TARGET_INT_BIT
#endif

/* Number of bits in a BFD_VMA for the target object file format. */
#if !defined (TARGET_BFD_VMA_BIT)
#define TARGET_BFD_VMA_BIT TARGET_PTR_BIT
#endif

/* If we picked up a copy of CHAR_BIT from a configuration file
   (which may get it by including <limits.h>) then use it to set
   the number of bits in a host char.  If not, use the same size
   as the target. */

#if defined (CHAR_BIT)
#define HOST_CHAR_BIT CHAR_BIT
#else
#define HOST_CHAR_BIT TARGET_CHAR_BIT
#endif

/* The bit byte-order has to do just with numbering of bits in
   debugging symbols and such.  Conceptually, it's quite separate
   from byte/word byte order.  */

#if !defined (BITS_BIG_ENDIAN)
#define BITS_BIG_ENDIAN (TARGET_BYTE_ORDER == BIG_ENDIAN)
#endif

/* In findvar.c.  */

extern LONGEST extract_signed_integer (void *, int);

extern ULONGEST extract_unsigned_integer (void *, int);

extern int extract_long_unsigned_integer (void *, int, LONGEST *);

extern CORE_ADDR extract_address (void *, int);

extern CORE_ADDR extract_typed_address (void *buf, struct type *type);

extern void store_signed_integer (void *, int, LONGEST);

extern void store_unsigned_integer (void *, int, ULONGEST);

extern void store_address (void *, int, LONGEST);

extern void store_typed_address (void *buf, struct type *type, CORE_ADDR addr);

/* Setup definitions for host and target floating point formats.  We need to
   consider the format for `float', `double', and `long double' for both target
   and host.  We need to do this so that we know what kind of conversions need
   to be done when converting target numbers to and from the hosts DOUBLEST
   data type.  */

/* This is used to indicate that we don't know the format of the floating point
   number.  Typically, this is useful for native ports, where the actual format
   is irrelevant, since no conversions will be taking place.  */

extern const struct floatformat floatformat_unknown;

#if HOST_BYTE_ORDER == BIG_ENDIAN
#ifndef HOST_FLOAT_FORMAT
#define HOST_FLOAT_FORMAT &floatformat_ieee_single_big
#endif
#ifndef HOST_DOUBLE_FORMAT
#define HOST_DOUBLE_FORMAT &floatformat_ieee_double_big
#endif
#else /* LITTLE_ENDIAN */
#ifndef HOST_FLOAT_FORMAT
#define HOST_FLOAT_FORMAT &floatformat_ieee_single_little
#endif
#ifndef HOST_DOUBLE_FORMAT
#define HOST_DOUBLE_FORMAT &floatformat_ieee_double_little
#endif
#endif

#ifndef HOST_LONG_DOUBLE_FORMAT
#define HOST_LONG_DOUBLE_FORMAT &floatformat_unknown
#endif

/* Use `long double' if the host compiler supports it.  (Note that this is not
   necessarily any longer than `double'.  On SunOS/gcc, it's the same as
   double.)  This is necessary because GDB internally converts all floating
   point values to the widest type supported by the host.

   There are problems however, when the target `long double' is longer than the
   host's `long double'.  In general, we'll probably reduce the precision of
   any such values and print a warning.  */

#ifdef HAVE_LONG_DOUBLE
typedef long double DOUBLEST;
#else
typedef double DOUBLEST;
#endif

extern void floatformat_to_doublest (const struct floatformat *,
				     char *, DOUBLEST *);
extern void floatformat_from_doublest (const struct floatformat *,
				       DOUBLEST *, char *);
extern DOUBLEST extract_floating (void *, int);

extern void store_floating (void *, int, DOUBLEST);

/* On some machines there are bits in addresses which are not really
   part of the address, but are used by the kernel, the hardware, etc.
   for special purposes.  ADDR_BITS_REMOVE takes out any such bits
   so we get a "real" address such as one would find in a symbol
   table.  This is used only for addresses of instructions, and even then
   I'm not sure it's used in all contexts.  It exists to deal with there
   being a few stray bits in the PC which would mislead us, not as some sort
   of generic thing to handle alignment or segmentation (it's possible it
   should be in TARGET_READ_PC instead). */
#if !defined (ADDR_BITS_REMOVE)
#define ADDR_BITS_REMOVE(addr) (addr)
#endif /* No ADDR_BITS_REMOVE.  */

/* From valops.c */

extern CORE_ADDR push_bytes (CORE_ADDR, char *, int);

extern CORE_ADDR push_word (CORE_ADDR, ULONGEST);

extern int watchdog;

/* Hooks for alternate command interfaces.  */

#ifdef UI_OUT
/* The name of the interpreter if specified on the command line. */
extern char *interpreter_p;
#endif

/* If a given interpreter matches INTERPRETER_P then it should update
   command_loop_hook and init_ui_hook with the per-interpreter
   implementation. */
/* FIXME: command_loop_hook and init_ui_hook should be moved here. */

struct target_waitstatus;
struct cmd_list_element;

/* Should the asynchronous variant of the interpreter (using the
   event-loop) be enabled? */
extern int event_loop_p;

extern void (*init_ui_hook) (char *argv0);
extern void (*command_loop_hook) (void);
extern void (*show_load_progress) (const char *section,
				   unsigned long section_sent, 
				   unsigned long section_size, 
				   unsigned long total_sent, 
				   unsigned long total_size);
extern void (*print_frame_info_listing_hook) (struct symtab * s,
					      int line, int stopline,
					      int noerror);
extern struct frame_info *parse_frame_specification (char *frame_exp);
extern int (*query_hook) (const char *, va_list);
extern void (*warning_hook) (const char *, va_list);
extern void (*flush_hook) (struct ui_file * stream);
extern void (*create_breakpoint_hook) (struct breakpoint * b);
extern void (*delete_breakpoint_hook) (struct breakpoint * bpt);
extern void (*modify_breakpoint_hook) (struct breakpoint * bpt);
extern void (*interactive_hook) (void);
extern void (*registers_changed_hook) (void);
extern void (*readline_begin_hook) (char *,...);
extern char *(*readline_hook) (char *);
extern void (*readline_end_hook) (void);
extern void (*register_changed_hook) (int regno);
extern void (*memory_changed_hook) (CORE_ADDR addr, int len);
extern void (*context_hook) (int);
extern int (*target_wait_hook) (int pid, struct target_waitstatus * status);

extern void (*attach_hook) (void);
extern void (*detach_hook) (void);
extern void (*call_command_hook) (struct cmd_list_element * c,
				  char *cmd, int from_tty);

extern void (*set_hook) (struct cmd_list_element * c);

extern NORETURN void (*error_hook) (void) ATTR_NORETURN;

extern void (*error_begin_hook) (void);

extern int (*ui_load_progress_hook) (const char *section, unsigned long num);


/* Inhibit window interface if non-zero. */

extern int use_windows;

/* Symbolic definitions of filename-related things.  */
/* FIXME, this doesn't work very well if host and executable
   filesystems conventions are different.  */

#ifndef DIRNAME_SEPARATOR
#define DIRNAME_SEPARATOR ':'
#endif

#ifndef SLASH_P
#if defined(__GO32__)||defined(_WIN32)
#define SLASH_P(X) ((X)=='\\')
#else
#define SLASH_P(X) ((X)=='/')
#endif
#endif

#ifndef SLASH_CHAR
#if defined(__GO32__)||defined(_WIN32)
#define SLASH_CHAR '\\'
#else
#define SLASH_CHAR '/'
#endif
#endif

#ifndef SLASH_STRING
#if defined(__GO32__)||defined(_WIN32)
#define SLASH_STRING "\\"
#else
#define SLASH_STRING "/"
#endif
#endif

#ifndef ROOTED_P
#define ROOTED_P(X) (SLASH_P((X)[0]))
#endif

/* On some systems, PIDGET is defined to extract the inferior pid from
   an internal pid that has the thread id and pid in seperate bit
   fields.  If not defined, then just use the entire internal pid as
   the actual pid. */

#ifndef PIDGET
#define PIDGET(PID) (PID)
#define TIDGET(PID) 0
#define MERGEPID(PID, TID) (PID)
#endif

/* If under Cygwin, provide backwards compatibility with older
   Cygwin compilers that don't define the current cpp define. */
#ifdef __CYGWIN32__
#ifndef __CYGWIN__
#define __CYGWIN__
#endif
#endif

/* Define well known filenos if the system does not define them.  */
#ifndef STDIN_FILENO
#define STDIN_FILENO   0
#endif
#ifndef STDOUT_FILENO
#define STDOUT_FILENO  1
#endif
#ifndef STDERR_FILENO
#define STDERR_FILENO  2
#endif

/* If this definition isn't overridden by the header files, assume
   that isatty and fileno exist on this system.  */
#ifndef ISATTY
#define ISATTY(FP)	(isatty (fileno (FP)))
#endif


/* FIXME: cagney/1999-12-13: The following will be moved to gdb.h /
   libgdb.h or gdblib.h. */

/* Return-code (RC) from a gdb library call.  (The abreviation RC is
   taken from the sim/common directory.) */

enum gdb_rc {
  /* The operation failed.  The failure message can be fetched by
     calling ``char *error_last_message(void)''. The value is
     determined by the catch_errors() interface. */
  /* NOTE: Since ``defs.h:catch_errors()'' does not return an error /
     internal / quit indication it is not possible to return that
     here. */
  GDB_RC_FAIL = 0,
  /* No error occured but nothing happened. Due to the catch_errors()
     interface, this must be non-zero. */
  GDB_RC_NONE = 1,
  /* The operation was successful. Due to the catch_errors()
     interface, this must be non-zero. */
  GDB_RC_OK = 2
};


/* Print the specified breakpoint on GDB_STDOUT. (Eventually this
   function will ``print'' the object on ``output''). */
enum gdb_rc gdb_breakpoint_query (/* struct {ui,gdb}_out *output, */ int bnum);

/* Create a breakpoint at ADDRESS (a GDB source and line). */
enum gdb_rc gdb_breakpoint (char *address, char *condition,
			    int hardwareflag, int tempflag,
			    int thread, int ignore_count);
enum gdb_rc gdb_thread_select (/* output object */ char *tidstr);

#ifdef UI_OUT
/* Print a list of known thread ids. */
enum gdb_rc gdb_list_thread_ids (/* output object */);

/* Switch thread and print notification. */
#endif
#endif /* #ifndef DEFS_H */
