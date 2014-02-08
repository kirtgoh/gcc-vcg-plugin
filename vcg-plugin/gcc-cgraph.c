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

/* String hash table used to handle names, titles, labels etc.  */
static htab_t string_table;

/* Should be enough.  */
static char buf[1024];

/* For the hash tables.  */

static int
string_hash_eq (const void *y1, const void *y2)
{
  return strcmp ((const char *) y1, (const char *) y2) == 0;
}

/* Find the string in the string hash table, which is combined with
   PREFIX and the name of NODE, insert the string into the string
   hash table if it's not found.  */

static char*
find_string (struct cgraph_node *node, char *prefix)
{
  char **slot;
  char *str;

  if (prefix == NULL)
    sprintf (buf, "%s", cgraph_node_name (node));
  else
    sprintf (buf, "%s.%s", prefix, (char *) cgraph_node_name (node));

  str = htab_find (string_table, buf);
  if (str == NULL)
    {
      vcg_plugin_common.buf_print ("%s", buf);
      str = vcg_plugin_common.buf_finish ();
      slot = (char **) htab_find_slot (string_table, buf, INSERT);
      *slot = str;
    }

  return str;
}

/* Create gdl edge based on EDGE and PREFIX if it does not exist.  */

static void
create_edge (gdl_graph *graph, struct cgraph_edge *edge, char *prefix,
             bool backedge_p)
{
  char *source_title, *target_title;
  struct cgraph_node *source, *target;
  gdl_edge *e;
  
  source = edge->caller;
  target = edge->callee;

  source_title = find_string (source, prefix);
  target_title = find_string (target, prefix);

  if (gdl_find_edge (graph, source_title, target_title))
    return;

  e = gdl_new_graph_edge (graph, source_title, target_title);
  if (backedge_p)
    gdl_set_edge_type (e, GDL_BACKEDGE);
}

/* Create gdl node based on NODE and PREFIX.  */

static void
create_node (gdl_graph *graph, struct cgraph_node *node, char *prefix)
{
  char *title, *label;
  gdl_node *gnode;

  title = find_string (node, prefix);
  label = find_string (node, NULL);
  gnode = gdl_new_graph_node (graph, title);
  gdl_set_node_label (gnode, label);
}

/* Create specific call graph start from NODE.  Callee graph when CALLEE_P
   is 1, caller graph when CALLEE_P is 0.  */

static void
create_node_and_edges_specific (gdl_graph *graph, struct cgraph_node *node, int callee_p)
{
  struct cgraph_node *source, *target, *node_x;
  struct cgraph_edge *edge;
  struct cgraph_edge **stack;
  char *prefix;
  int sp;

  /* Don't create the single node.  */
  if ((callee_p && !node->callees) || (!callee_p && !node->callers))
    return;

  prefix = find_string (node, NULL);
  create_node (graph, node, prefix);

  stack = XNEWVEC (struct cgraph_edge *, cgraph_n_nodes + 1);
  sp = 0;

  if (callee_p)
    edge = node->callees;
  else
    edge = node->callers;

  while (1)
    {
      while (edge)
        {
          source = edge->caller;
          target = edge->callee;

          if (callee_p)
            node_x = target;
          else
            node_x = source;

          create_edge (graph, edge, prefix, !callee_p);

          if (gdl_find_node (graph, find_string (node_x, prefix)) == NULL)
            {
              create_node (graph, node_x, prefix);

              stack[++sp] = edge;
              if (callee_p)
                edge = node_x->callees;
              else
                edge = node_x->callers;

              continue;
            }

          if (callee_p)
            edge = edge->next_callee;
          else
            edge = edge->next_caller;
        }

      if (sp == 0)
        break;

      edge = stack[sp--];
      if (callee_p)
        edge = edge->next_callee;
      else
        edge = edge->next_caller;
    }
  free (stack);
}

/* Dump specific call graph into the file FNAME.  */

static void
dump_cgraph_to_file_specific (char *fname, int callee_p)
{
  gdl_graph *graph;
  struct cgraph_node *node;

  graph = vcg_plugin_common.top_graph;
  gdl_set_graph_orientation (graph, "left_to_right");

  for (node = cgraph_nodes; node; node = node->next)
    create_node_and_edges_specific (graph, node, callee_p);

  vcg_plugin_common.dump (fname);
}

/* Dump call graph into the file FNAME.  */

static void
dump_cgraph_to_file (char *fname)
{
  gdl_graph *graph;
  struct cgraph_node *node;
  struct cgraph_edge *edge;

  graph = vcg_plugin_common.top_graph;
  gdl_set_graph_orientation (graph, "left_to_right");

  for (node = cgraph_nodes; node; node = node->next)
    {
      /* Don't create the single node.  */
      if (!node->callees && !node->callers)
        continue;

      create_node (graph, node, NULL);
      for (edge = node->callees; edge; edge = edge->next_callee)
        create_edge (graph, edge, NULL, 0);
    }

  vcg_plugin_common.dump (fname);
}

/* Local init.  */

static void
local_init (void)
{
  string_table = htab_create (10, htab_hash_string, string_hash_eq, NULL);
}

/* Local finish.  */

static void
local_finish (void)
{
  htab_delete (string_table);
}

/* Public function to dump caller graph.  */

void
vcg_plugin_dump_cgraph_caller (void)
{
  char *fname;

  vcg_plugin_common.init ();
  local_init ();

  fname = concat (dump_base_name, ".cgraph-caller.vcg", NULL);
  dump_cgraph_to_file_specific (fname, 0);
  free (fname);

  local_finish ();
  vcg_plugin_common.finish ();
}

/* Public function to view caller graph.  */

void
vcg_plugin_view_cgraph_caller (void)
{
  vcg_plugin_common.init ();
  local_init ();

  dump_cgraph_to_file_specific (vcg_plugin_common.temp_file_name, 0);
  vcg_plugin_common.show (vcg_plugin_common.temp_file_name);

  local_finish ();
  vcg_plugin_common.finish ();
}

/* Public function to dump callee graph.  */

void
vcg_plugin_dump_cgraph_callee (void)
{
  char *fname;

  vcg_plugin_common.init ();
  local_init ();

  fname = concat (dump_base_name, ".cgraph-callee.vcg", NULL);
  dump_cgraph_to_file_specific (fname, 1);
  free (fname);

  local_finish ();
  vcg_plugin_common.finish ();
}

/* Public function to view callee graph.  */

void
vcg_plugin_view_cgraph_callee (void)
{
  vcg_plugin_common.init ();
  local_init ();

  dump_cgraph_to_file_specific (vcg_plugin_common.temp_file_name, 1);
  vcg_plugin_common.show (vcg_plugin_common.temp_file_name);

  local_finish ();
  vcg_plugin_common.finish ();
}

/* Public function to dump call graph.  */

void
vcg_plugin_dump_cgraph (void)
{
  char *fname;

  vcg_plugin_common.init ();
  local_init ();

  fname = concat (dump_base_name, ".cgraph.vcg", NULL);
  dump_cgraph_to_file (fname);
  free (fname);

  local_finish ();
  vcg_plugin_common.finish ();
}

/* Public function to view call graph.  */

void
vcg_plugin_view_cgraph (void)
{
  vcg_plugin_common.init ();
  local_init ();

  dump_cgraph_to_file (vcg_plugin_common.temp_file_name);
  vcg_plugin_common.show (vcg_plugin_common.temp_file_name);

  local_finish ();
  vcg_plugin_common.finish ();
}

/* Plugin callback function for PLUGIN_ALL_IPA_PASSES_START event.  */

void *
vcg_plugin_callback_cgraph (void *gcc_data, void *user_data)
{
  vcg_plugin_dump_cgraph ();
  return NULL;
}

/* Plugin callback function for PLUGIN_ALL_IPA_PASSES_START event.  */

void *
vcg_plugin_callback_callee (void *gcc_data, void *user_data)
{
  vcg_plugin_dump_cgraph_callee ();
  return NULL;
}

/* Plugin callback function for PLUGIN_ALL_IPA_PASSES_START event.  */

void *
vcg_plugin_callback_caller (void *gcc_data, void *user_data)
{
  vcg_plugin_dump_cgraph_caller ();
  return NULL;
}


