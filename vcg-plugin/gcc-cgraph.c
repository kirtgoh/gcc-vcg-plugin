/* Dump or view gcc call graph.

   Copyright (C) 2010, 2011 Mingjie Xing, mingjie.xing@gmail.com.

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

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "gcc-plugin.h"
#include "plugin.h"
#include "plugin-version.h"

#include "cgraph.h"
#include "vcg-plugin.h"

static void
create_node_and_edges (gdl_graph *graph, struct cgraph_node *node)
{
  struct cgraph_edge *edge;
  gdl_node *node_g;
  gdl_edge *edge_g;
  char *title, *title_a;

  title = cgraph_node_name (node);
  node_g = gdl_new_node (title);
  gdl_add_node (graph, node_g);

  for (edge = node->callees; edge; edge = edge->next_callee)
    {
      title_a = cgraph_node_name (edge->callee);
      if (gdl_find_edge (graph, title, title_a))
        continue;
      edge_g = gdl_new_edge (title, title_a);
      gdl_add_edge (graph, edge_g);
    }
}

/* Dump call graph into the file FNAME.  */

static void
dump_cgraph_to_file (char *fname)
{
  gdl_graph *graph;
  struct cgraph_node *node;

  graph = vcg_plugin_common.top_graph;
  for (node = cgraph_nodes; node; node = node->next)
    create_node_and_edges (graph, node);

  vcg_plugin_common.dump (fname, graph);
}

/* Public function to dump call graph.  */

void
vcg_plugin_dump_cgraph (void)
{
  char *fname = "dump-cgraph.vcg";

  vcg_plugin_common.init ();

  /* Create the dump file name.  */
  dump_cgraph_to_file (fname);

  vcg_plugin_common.finish ();
}

/* Public function to view call graph.  */

void
vcg_plugin_view_cgraph (void)
{
  char *fname;

  vcg_plugin_common.init ();

  /* Get the temp file name.  */
  fname = vcg_plugin_common.temp_file_name;
  dump_cgraph_to_file (fname);
  vcg_plugin_common.show (fname);

  vcg_plugin_common.finish ();
}


