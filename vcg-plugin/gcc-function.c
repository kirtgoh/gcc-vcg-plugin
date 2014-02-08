/* Dump or view gcc function.
 
   Copyright (C) 2010, 2011 Mingjie Xing, mingjie.xing@gmail.com.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "vcg-plugin.h"

static char buf[32]; /* Should be enough.  */
static char **bb_graph_title;
static char **bb_graph_label;
static char **bb_node_title;

/* Initialize all of the names.  */
static void
create_names (void)
{
  int i;
  int bb_num = n_basic_blocks;
  const char *func_name = current_function_name ();
  
  bb_graph_title = XNEWVEC (char *, bb_num);
  bb_graph_label = XNEWVEC (char *, bb_num);
  bb_node_title = XNEWVEC (char *, bb_num);

  for (i = 0; i < bb_num; i++)
    {
      sprintf (buf, "%d", i);

      /* format: func_name.bb_index */
      bb_graph_title[i] = concat (func_name, ".", buf, NULL);

      /* format: ENTRY | EXIT | bb bb_index */
      if (i == 0)
        bb_graph_label[i] = "ENTRY";
      else if (i == 1)
        bb_graph_label[i] = "EXIT";
      else
        bb_graph_label[i] = concat ("bb ", buf, NULL);

      /* format: ENTRY | EXIT | bb.bb_index */
      if (i == 0)
        bb_node_title[i] = "ENTRY";
      else if (i == 1)
        bb_node_title[i] = "EXIT";
      else
        bb_node_title[i] = concat ("bb.", buf, NULL);
    }
}

static void
free_names (int bb_num)
{
  int i;

  for (i = 0; i < bb_num; i++)
    {
      free (bb_graph_title[i]);
      if (i > 1)
        free (bb_graph_label[i]);
      if (i > 1)
        free (bb_node_title[i]);
    }
  free (bb_graph_title);
  free (bb_graph_label);
  free (bb_node_title);
}

/* Create a graph from the basic block bb. */

static gdl_graph *
create_bb_graph (basic_block bb)
{
  gdl_graph *g;
  gdl_node *n;
  char *str;
  int i;

  g = gdl_new_graph (bb_graph_title[bb->index]);
  gdl_set_graph_label (g, bb_graph_label[bb->index]);
  gdl_set_graph_folding (g, 1);
  gdl_set_graph_shape (g, "ellipse");

  rewind (vcg_plugin_common.stream);
  gimple_dump_bb (bb, vcg_plugin_common.stream, 0, TDF_VOPS|TDF_MEMSYMS|TDF_BLOCKS);
  i = vcg_plugin_common.stream_buf_size;
  while (i > 1 && ISSPACE (vcg_plugin_common.stream_buf[i - 1])) i--;
  str = xstrndup (vcg_plugin_common.stream_buf, i);
  n = gdl_new_graph_node (g, bb_node_title[bb->index]);
  gdl_set_node_label (n, str);

  return g;
}

static int
set_vertical_order_1 (gdl_graph *graph, int *distance, basic_block bb)
{
  int val, max = 0;
  edge e;
  edge_iterator ei;
  gdl_graph *subgraph;
  gdl_node *node;

  if (distance[bb->index] != 0)
    return distance[bb->index];

  FOR_EACH_EDGE (e, ei, bb->preds)
    {
      if (e->flags & EDGE_DFS_BACK)
        continue;

      val = set_vertical_order_1 (graph, distance, e->src);
      max = max > val ? max : val;
    }
  distance[bb->index] = max + 1;
  subgraph = gdl_find_subgraph (graph, bb_graph_title[bb->index]); 
  gdl_set_graph_vertical_order (subgraph, distance[bb->index]);
  node = gdl_get_graph_node (subgraph);
  gdl_set_node_vertical_order (node, distance[bb->index]);
 
  return distance[bb->index];
}

static void
set_vertical_order (gdl_graph *graph)
{
  int val, max = 0;
  int *distance;
  basic_block bb;
  gdl_graph *subgraph;
  gdl_node *node;

  distance = XCNEWVEC (int, n_basic_blocks);

  max = set_vertical_order_1 (graph, distance, EXIT_BLOCK_PTR);
  FOR_EACH_BB (bb)
    {
      if (distance[bb->index] == 0)
        {
          val = set_vertical_order_1 (graph, distance, bb);
          max = max > val ? max : val + 1;
        }
    }
  subgraph = gdl_find_subgraph (graph,
                                bb_graph_title[EXIT_BLOCK_PTR->index]); 
  gdl_set_graph_vertical_order (subgraph, max);
  node = gdl_get_graph_node (subgraph);
  gdl_set_node_vertical_order (node, max);

  free (distance);
}

static void
dump_function_to_file (char *fname)
{
  basic_block bb;
  edge e;
  edge_iterator ei;

  gdl_graph *graph, *bb_graph;
  gdl_edge *edge;

  /* Create names for graphs and nodes.  */
  create_names ();

  graph = vcg_plugin_common.top_graph;

  mark_dfs_back_edges ();

  FOR_ALL_BB (bb)
    {
      bb_graph = create_bb_graph (bb);
      gdl_add_subgraph (graph, bb_graph);

      FOR_EACH_EDGE (e, ei, bb->succs)
        {
          edge = gdl_new_graph_edge (graph, bb_graph_title[e->src->index],
                              bb_graph_title[e->dest->index]);
              if (e->flags & EDGE_DFS_BACK)
                gdl_set_edge_type (edge, GDL_BACKEDGE);
        }
    }

  /* Optimize the graph layout.  */
  set_vertical_order (graph);

  vcg_plugin_common.dump (fname);

  /* Free names for graphs and nodes.  */
  free_names (n_basic_blocks);
}

/* Public function to dump a gcc function FN.  */

void
vcg_plugin_dump_function (void)
{
  vcg_plugin_common.init ();

  dump_function_to_file ("dump-function.vcg");

  vcg_plugin_common.finish ();
}

/* Public function to view a gcc function FN.  */

void
vcg_plugin_view_function (void)
{
  vcg_plugin_common.init ();

  dump_function_to_file (vcg_plugin_common.temp_file_name);
  vcg_plugin_common.show (vcg_plugin_common.temp_file_name);

  vcg_plugin_common.finish ();
}

