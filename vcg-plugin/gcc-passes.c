/* Dump or view gcc passes.

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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */ 

#include "vcg-plugin.h"

#define DEF_PASS_LIST(LIST) #LIST,
/* Pass list names.  */
static char *pass_list_name[] =
{
  GCC_PASS_LISTS
  NULL
};
#undef DEF_PASS_LIST

static int id = 0;

/* Point to the focused pass.  */
static gdl_node *this_node = NULL;

static void
check_current_pass (struct opt_pass *pass, gdl_node *node)
{
  if (pass == current_pass)
    this_node = node;
}

static void
try_create_edge (gdl_graph *graph, gdl_node *src, gdl_node *dest)
{
  if (src == NULL)
    return;

  gdl_new_graph_edge (graph, gdl_get_node_title (src),
                      gdl_get_node_title (dest));
} 

static gdl_node *
create_sub_pass_list_graph (gdl_graph *graph, struct opt_pass *pass_list,
                            char *name, gdl_node *prev_node)
{
  gdl_graph *subgraph;
  gdl_node *node = NULL, *prev = prev_node;
  char *title;
  struct opt_pass *pass = pass_list;

  asprintf (&title, "%s.%d", name, id++);
  subgraph = gdl_new_graph (title);
  gdl_set_graph_label (subgraph, name);
  gdl_set_graph_folding (subgraph, 1);
  gdl_set_graph_shape (subgraph, "ellipse");
  gdl_add_subgraph (graph, subgraph);

  if (pass->execute)
    {
      asprintf (&title, "%s.%d", name, id++);
      node = gdl_new_node (title);
      check_current_pass (pass, node);
      gdl_set_node_label (node, name);
      gdl_add_node (subgraph, node);
      try_create_edge (subgraph, prev, node);
      prev = node;
    }

  for (pass = pass->sub; pass; pass = pass->next)
    {
      if (pass->sub)
        prev = create_sub_pass_list_graph (subgraph, pass,
                                           (char *) pass->name, prev);
      else
        {
          asprintf (&title, "%s.%d", pass->name, id++);
          node = gdl_new_node (title);
          gdl_set_node_label (node, (char *) pass->name);
          check_current_pass (pass, node);
          gdl_add_node (subgraph, node);
          try_create_edge (subgraph, prev, node);
          prev = node;
        }
    }
  return node;
}

static void
create_pass_list_graph (gdl_graph *graph, struct opt_pass *pass_list,
                        char *name, gdl_node *prev_node)
{
  gdl_graph *subgraph;
  gdl_node *node = NULL, *prev = prev_node;
  char *title;
  struct opt_pass *pass;

  subgraph = gdl_new_graph (name);
  gdl_set_graph_label (subgraph, name);
  gdl_set_graph_folding (subgraph, 1);
  gdl_set_graph_shape (subgraph, "ellipse");
  gdl_add_subgraph (graph, subgraph);

  for (pass = pass_list; pass; pass = pass->next)
    {
      if (pass->sub)
        prev = create_sub_pass_list_graph (subgraph, pass,
                                           (char *) pass->name, prev);
      else
        {
          asprintf (&title, "%s.%d", pass->name, id++);
          node = gdl_new_node (title);
          gdl_set_node_label (node, (char *) pass->name);
          check_current_pass (pass, node);
          gdl_add_node (subgraph, node);
          try_create_edge (subgraph, prev, node);
          prev = node;
        }
    }
}

static void
dump_passes_to_file (char *fname)
{
  gdl_graph *graph, *g;
  int i;

  graph = vcg_plugin_common.top_graph;
  gdl_set_graph_yspace (graph, 15);

  /* Do some initialization.  */
  id = 0;
  this_node = NULL;

  for (i = 0; i < PASS_LIST_NUM; i++)
    create_pass_list_graph (graph, *gcc_pass_lists[i], pass_list_name[i], NULL);

  if (this_node)
    {
      gdl_set_node_color (this_node, "red");
      g = gdl_get_node_parent (this_node);
      while (g && g != graph)
        {
          gdl_set_graph_color (g, "red");
          gdl_set_graph_folding (g, 0);
          g = gdl_get_graph_parent (g);
        }
    }
  vcg_plugin_common.dump (fname);
}

/* Public function to dump the gcc passes.  */

void
vcg_plugin_dump_passes (void)
{
  vcg_plugin_common.init ();

  dump_passes_to_file ("dump-passes.vcg");

  vcg_plugin_common.finish ();
}

/* Public function to view the gcc passes.  */

void
vcg_plugin_view_passes (void)
{
  vcg_plugin_common.init ();

  dump_passes_to_file (vcg_plugin_common.temp_file_name);
  vcg_plugin_common.show (vcg_plugin_common.temp_file_name);

  vcg_plugin_common.finish ();
}

