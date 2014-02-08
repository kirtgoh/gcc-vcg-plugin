/* vcg plugin

   Copyright (C) 2009, 2010, 2011 Mingjie Xing, mingjie.xing@gmail.com. 

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef VCG_PLUGIN_H
#define VCG_PLUGIN_H

/* GNU extensions, asprintf */
#define _GNU_SOURCE

#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>

/* gcc's header files */

#include "gcc-plugin.h"
#include "plugin.h"
#include "plugin-version.h"

#include "gimple.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "cfgloop.h"
#include "cgraph.h"

/* libgdl */
#include "gdl.h"

/* The common data and functions for each dump/view command.  */

typedef struct
{
  /* The plugin name.  */
  char *plugin_name;

  /* The plugin version.  */
  char *version;

  /* Other information.  */
  char *info;

  /* The name of the vcg viewer tool.  */
  char *vcg_viewer;

  /* The top graph.  */
  gdl_graph *top_graph;

  /* Temp file name to dump/view a graph.  */
  char *temp_file_name;

  /* Temp stream to get gcc dump.  */
  FILE *stream;
  char *stream_buf;
  size_t stream_buf_size;

  void (*init) (void);
  void (*finish) (void);
  void (*error) (const char *format, ...);
  void (*dump) (char *fname);
  void (*show) (char *fname);
  void (*buf_print) (char *fmt, ...);
  char *(*buf_finish) (void);
} vcg_plugin_common_t;

extern vcg_plugin_common_t vcg_plugin_common; 

/* These are available functins, which can be used to
   dump and view gcc internal data structures. */

extern void vcg_plugin_dump_bb (char *list);
extern void vcg_plugin_view_bb (char *list);

extern void vcg_plugin_dump_cgraph (void);
extern void vcg_plugin_view_cgraph (void);

extern void vcg_plugin_dump_dominance (void);
extern void vcg_plugin_view_dominance (void);

extern void vcg_plugin_dump_function (void); 
extern void vcg_plugin_view_function (void); 

extern void vcg_plugin_dump_gimple_hierarchy (void);
extern void vcg_plugin_view_gimple_hierarchy (void);

extern void vcg_plugin_dump_loop (unsigned loop_id);
extern void vcg_plugin_view_loop (unsigned loop_id);

extern void vcg_plugin_dump_passes (void);
extern void vcg_plugin_view_passes (void);

extern void vcg_plugin_dump_rtx (const_rtx x);
extern void vcg_plugin_view_rtx (const_rtx x);

extern void vcg_plugin_dump_tree_hierarchy (void);
extern void vcg_plugin_view_tree_hierarchy (void);

extern void vcg_plugin_dump_tree (tree node);
extern void vcg_plugin_view_tree (tree node);

/* These are callback functins, which can be registered to dump
   gcc internal data structures. */

extern void *vcg_plugin_callback_cgraph (void *, void *);
extern void *vcg_plugin_callback_callee (void *, void *);
extern void *vcg_plugin_callback_caller (void *, void *);

extern void *vcg_plugin_callback_passes_start (void *, void *);
extern void *vcg_plugin_callback_pass (void *, void *);
extern void *vcg_plugin_callback_passes_finish (void *, void *);
extern void *vcg_plugin_callback_pass_lists (void *, void *);

extern void *vcg_plugin_callback_gimple_hierarchy (void *, void *);

extern void *vcg_plugin_callback_tree_hierarchy (void *, void *);

#endif
