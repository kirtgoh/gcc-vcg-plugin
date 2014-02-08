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

#include "vcg-plugin.h"

static void
create_node_and_edges_callee (gdl_graph *graph, struct cgraph_node *node)
{
  struct cgraph_edge *edge;
  gdl_node *src, *dest;
  char *title_src, *title_dest;
  char *label;

  label = (char *) cgraph_node_name (node);
  title_src = label;
  src = gdl_new_graph_node (graph, title_src);
  gdl_set_node_label (src, label);

  for (edge = node->callees; edge; edge = edge->next_callee)
    {
      label = (char *) cgraph_node_name (edge->callee);
      vcg_plugin_common.buf_print ("%s.%s", title_src, label);
      title_dest = vcg_plugin_common.buf_finish ();
      if (gdl_find_edge (graph, title_src, title_dest) == NULL)
        {
          dest = gdl_new_graph_node (graph, title_dest);
          gdl_set_node_label (dest, label);
          gdl_new_graph_edge (graph, title_src, title_dest);
        }
    }
}

/* Dump callee graph into the file FNAME.  */

static void
dump_cgraph_callee_to_file (char *fname)
{
  gdl_graph *graph;
  struct cgraph_node *node;

  graph = vcg_plugin_common.top_graph;
  gdl_set_graph_orientation (graph, "left_to_right");

  for (node = cgraph_nodes; node; node = node->next)
    create_node_and_edges_callee (graph, node);

  vcg_plugin_common.dump (fname);
}

/* Public function to dump callee graph.  */

void
vcg_plugin_dump_cgraph_callee (void)
{
  vcg_plugin_common.init ();

  dump_cgraph_callee_to_file ("dump-cgraph-callee.vcg");

  vcg_plugin_common.finish ();
}

/* Public function to view callee graph.  */

void
vcg_plugin_view_cgraph_callee (void)
{
  vcg_plugin_common.init ();

  dump_cgraph_callee_to_file (vcg_plugin_common.temp_file_name);
  vcg_plugin_common.show (vcg_plugin_common.temp_file_name);

  vcg_plugin_common.finish ();
}

static void
create_node_and_edges (gdl_graph *graph, struct cgraph_node *node)
{
  struct cgraph_edge *edge;
  char *title, *title_a;

  title = (char *) cgraph_node_name (node);
  gdl_new_graph_node (graph, title);

  for (edge = node->callees; edge; edge = edge->next_callee)
    {
      title_a = (char *) cgraph_node_name (edge->callee);
      if (gdl_find_edge (graph, title, title_a))
        continue;
      gdl_new_graph_edge (graph, title, title_a);
    }
}

/* Dump call graph into the file FNAME.  */

static void
dump_cgraph_to_file (char *fname)
{
  gdl_graph *graph;
  struct cgraph_node *node;

  graph = vcg_plugin_common.top_graph;
  gdl_set_graph_orientation (graph, "left_to_right");

  for (node = cgraph_nodes; node; node = node->next)
    create_node_and_edges (graph, node);

  vcg_plugin_common.dump (fname);
}

/* Public function to dump call graph.  */

void
vcg_plugin_dump_cgraph (void)
{
  vcg_plugin_common.init ();

  dump_cgraph_to_file ("dump-cgraph.vcg");

  vcg_plugin_common.finish ();
}

/* Public function to view call graph.  */

void
vcg_plugin_view_cgraph (void)
{
  vcg_plugin_common.init ();

  dump_cgraph_to_file (vcg_plugin_common.temp_file_name);
  vcg_plugin_common.show (vcg_plugin_common.temp_file_name);

  vcg_plugin_common.finish ();
}


