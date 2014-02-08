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

/* Use the unique id number to avoid the same pass names.  */
static int id;

/* Point to the focused pass.  */
static gdl_node *this_node;

/* The previous gdl node.  */
static gdl_node *prev_node;

static void
check_current_pass (struct opt_pass *pass, gdl_node *node)
{
  if (pass == current_pass)
    this_node = node;
}

/* Try to create a gdl edge.  */

static void
try_create_edge (gdl_graph *graph, gdl_node *src, gdl_node *dest)
{
  if (src == NULL)
    return;

  gdl_new_graph_edge (graph, gdl_get_node_title (src),
                      gdl_get_node_title (dest));
} 

/* Get the label name based on PASS.  It's juat the pass name.  */

static char *
get_label (struct opt_pass *pass)
{
  return (char *) pass->name;
}

/* Get the title name based on PASS.  Use the unique id number to avoid the
   same pass names.  */

static char *
get_title (struct opt_pass *pass, char *suffix)
{
  static char buf[1024];

  if (suffix)
    sprintf (buf, "%d. %s [%s] %s", ++id, pass->name,
             current_function_name (), suffix);
  else
    sprintf (buf, "%d. %s [%s]", ++id, pass->name,
             current_function_name ());

  return buf;
}

/* Create a gdl node based on PASS.  */

static gdl_node *
create_node (gdl_graph *graph, struct opt_pass *pass, char *suffix)
{
  char *title;
  gdl_node *node;

  title = get_title (pass, suffix);
  node = gdl_new_graph_node (graph, title);

  return node;
}

static gdl_node *
create_sub_pass_list_graph (gdl_graph *graph, struct opt_pass *pass_list,
                            gdl_node *prev_node)
{
  gdl_graph *subgraph;
  gdl_node *node = NULL, *prev = prev_node;
  char *title, *label;
  struct opt_pass *pass = pass_list;

  label = get_label (pass);
  title = get_title (pass, NULL);
  subgraph = gdl_new_graph (title);
  gdl_set_graph_label (subgraph, label);
  gdl_set_graph_folding (subgraph, 1);
  gdl_set_graph_shape (subgraph, "ellipse");
  gdl_add_subgraph (graph, subgraph);

  if (pass->execute)
    {
      node = create_node (subgraph, pass, NULL);
      gdl_set_node_label (node, label);
      check_current_pass (pass, node);
      try_create_edge (subgraph, prev, node);
      prev = node;
    }

  for (pass = pass->sub; pass; pass = pass->next)
    {
      if (pass->sub)
        prev = create_sub_pass_list_graph (subgraph, pass, prev);
      else
        {
          node = create_node (subgraph, pass, NULL);
          label = get_label (pass);
          gdl_set_node_label (node, label);
          check_current_pass (pass, node);
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
  struct opt_pass *pass;
  char *label;

  subgraph = gdl_new_graph (name);
  gdl_set_graph_label (subgraph, name);
  gdl_set_graph_folding (subgraph, 1);
  gdl_set_graph_shape (subgraph, "ellipse");
  gdl_add_subgraph (graph, subgraph);

  for (pass = pass_list; pass; pass = pass->next)
    {
      if (pass->sub)
        prev = create_sub_pass_list_graph (subgraph, pass, prev);
      else
        {
          label = get_label (pass);
          node = create_node (subgraph, pass, NULL);
          gdl_set_node_label (node, label);
          check_current_pass (pass, node);
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

/* Public function to dump the gcc pass lists.  */

void
vcg_plugin_dump_pass_lists (void)
{
  vcg_plugin_common.init ();

  dump_passes_to_file ("dump-pass-lists.vcg");

  vcg_plugin_common.finish ();
}

/* Public function to view the gcc pass lists.  */

void
vcg_plugin_view_pass_lists (void)
{
  vcg_plugin_common.init ();

  dump_passes_to_file (vcg_plugin_common.temp_file_name);
  vcg_plugin_common.show (vcg_plugin_common.temp_file_name);

  vcg_plugin_common.finish ();
}

/* Plugin callback function for PLUGIN_FINISH event.
   Dump gcc pass lists.  */

void *
vcg_plugin_callback_pass_lists (void *gcc_data, void *user_data)
{
  vcg_plugin_dump_pass_lists ();
  return NULL;
}

/* Plugin callback function for PLUGIN_START_UNIT event.
   Dump the pass list.  */

void *
vcg_plugin_callback_passes_start (void *gcc_data, void *user_data)
{
  /* Do some initialization.  */
  id = 0;
  this_node = NULL;
  prev_node = NULL;

  vcg_plugin_common.init ();
  return NULL;
}

/* Plugin callback function for PLUGIN_OVERRIDE_GATE event.  Create a node for
   the current pass, and set node color as green if the gate status value is
   true, grey for false.  */

void *
vcg_plugin_callback_pass (void *gcc_data, void *user_data)
{
  gdl_graph *graph;
  gdl_node *node;

  graph = vcg_plugin_common.top_graph;
  if (*(bool *)gcc_data)
    {
      node = create_node (graph, current_pass, "[enable]");
    }
  else
    {
      node = create_node (graph, current_pass, "[disable]");
      gdl_set_node_color (node, "lightgrey");
    }

  try_create_edge (graph, prev_node, node);
  prev_node = node;
  return NULL;
}

/* Plugin callback function for PLUGIN_FINISH_UNIT event.
   Dump the pass list.  */

void *
vcg_plugin_callback_passes_finish (void *gcc_data, void *user_data)
{
  char *fname;

  fname = concat (dump_base_name, ".passes.vcg", NULL);
  vcg_plugin_common.dump (fname);
  free (fname);

  vcg_plugin_common.finish ();
  return NULL;
}

