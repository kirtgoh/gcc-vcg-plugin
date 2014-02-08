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

/* gcc's header files */

#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "toplev.h"
#include "gimple.h"
#include "tree-pass.h"
#include "rtl.h"
#include "intl.h"
#include "langhooks.h"
#include "cfghooks.h"

/* libgdl */
#include "gdl.h"

typedef struct
{
  char *plugin_name;
  char *version;
  char *info;
  char *vcg_viewer;
  char *temp_file_name;
  gdl_graph *top_graph;
  void (*init) (void);
  void (*tag) (char *str);
  void (*finish) (void);
  void (*error) (const char *format, ...);
  void (*dump) (char *fname, gdl_graph *graph);
  void (*show) (char *fname);
} vcg_plugin_common_t;

extern vcg_plugin_common_t vcg_plugin_common; 

/* These are available functins, which can be used to
   dump and view gcc internal data structures. */

extern void vcg_plugin_dump_function (tree fn); 
extern void vcg_plugin_view_function (tree fn); 


#endif
