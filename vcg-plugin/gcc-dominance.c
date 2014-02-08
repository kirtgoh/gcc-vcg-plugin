/* Dump or view gcc dominance graph.

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

/* Dump dominance graph into the file FNAME.  */

static void
dump_dominance_to_file (char *fname)
{
  gdl_graph *graph;
  basic_block bb, bb2;
  char buf[32];
  char buf2[32];

  graph = vcg_plugin_common.top_graph;

  FOR_EACH_BB (bb)
    if ((bb2 = get_immediate_dominator (1, bb)))
      {
        sprintf (buf, "bb %d", bb->index);
        sprintf (buf2, "bb %d", bb2->index);

        if (!gdl_find_node (graph, buf))
          gdl_new_graph_node (graph, buf);

        if (!gdl_find_node (graph, buf2))
          gdl_new_graph_node (graph, buf2);

        gdl_new_graph_edge (graph, buf2, buf);
      }

  vcg_plugin_common.dump (fname, graph);
}

/* Public function to dump dominance graph.  */

void
vcg_plugin_dump_dominance (void)
{
  char *fname = "dump-dominance.vcg";

  vcg_plugin_common.init ();

  dump_dominance_to_file (fname);

  vcg_plugin_common.finish ();
}

/* Public function to view dominance graph.  */

void
vcg_plugin_view_dominance (void)
{
  char *fname = vcg_plugin_common.temp_file_name;

  vcg_plugin_common.init ();

  dump_dominance_to_file (fname);
  vcg_plugin_common.show (fname);

  vcg_plugin_common.finish ();
}


