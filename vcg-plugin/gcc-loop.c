/* Dump or view gcc loop.
 
   Copyright (C) 2011 Mingjie Xing, mingjie.xing@gmail.com.

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

/* Should be enough.  */
static char buf[512];

static char **bb_graph_title;
static char **bb_graph_label;
static char **bb_node_title;

/* Initialize all of the names.  */

static void
create_names (void)
{
  int i;
  char *func_name = (char *) current_function_name ();
  int bb_num = n_basic_blocks;
  
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
free_names (void)
{
  int i;
  int bb_num = n_basic_blocks;

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

/* Dump the loop into the file.  */

static void
dump_loop_to_file (char *fname, int loop_id)
{
  struct loop *loop;
  basic_block bb;
  edge e;
  edge_iterator ei;

  gdl_graph *graph, *bb_graph;
  gdl_edge *edge;

  if (loop_id >= number_of_loops ())
    goto done;

  graph = vcg_plugin_common.top_graph;

  loop = get_loop (loop_id);

  create_names ();

  mark_dfs_back_edges ();

  FOR_ALL_BB (bb)
    if (flow_bb_inside_loop_p (loop, bb))
      {
        bb_graph = create_bb_graph (bb);
        gdl_add_subgraph (graph, bb_graph);

        FOR_EACH_EDGE (e, ei, bb->succs)
          if (flow_bb_inside_loop_p (loop, e->dest))
            {
              edge = gdl_new_graph_edge (graph,
                                         bb_graph_title[e->src->index],
                                         bb_graph_title[e->dest->index]);
              if (e->flags & EDGE_DFS_BACK)
                gdl_set_edge_type (edge, GDL_BACKEDGE);
            }
      }

done:
  vcg_plugin_common.dump (fname);

  /* Free names for graphs and nodes.  */
  free_names ();
}

/* Public function to dump a loop.  */

void
vcg_plugin_dump_loop (unsigned loop_id)
{
  vcg_plugin_common.init ();

  dump_loop_to_file ("dump-loop.vcg", loop_id);

  vcg_plugin_common.finish ();
}

/* Public function to view a loop.  */

void
vcg_plugin_view_loop (unsigned loop_id)
{
  vcg_plugin_common.init ();

  dump_loop_to_file (vcg_plugin_common.temp_file_name, loop_id);
  vcg_plugin_common.show (vcg_plugin_common.temp_file_name);

  vcg_plugin_common.finish ();
}

