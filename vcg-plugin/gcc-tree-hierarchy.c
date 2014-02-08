/* Dump or view gcc tree structure hierarchy.

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

static void
dump_tree_hierarchy_to_file_4_6 (char *fname)
{
  gdl_graph *graph;
  gdl_node *node;

  graph = vcg_plugin_common.top_graph;
  gdl_set_graph_orientation (graph, "left_to_right");

  #define NEW_NODE(name) \
  node = gdl_new_graph_node (graph, name); \
  gdl_set_node_label (node, name);

  NEW_NODE ("tree_base")
  NEW_NODE ("tree_common")
  NEW_NODE ("tree_int_cst")
  NEW_NODE ("tree_real_cst")
  NEW_NODE ("tree_fixed_cst")
  NEW_NODE ("tree_vector")
  NEW_NODE ("tree_string")
  NEW_NODE ("tree_complex")
  NEW_NODE ("tree_identifier")
  NEW_NODE ("tree_decl_minimal")
  NEW_NODE ("tree_decl_common")
  NEW_NODE ("tree_decl_with_rtl")
  NEW_NODE ("tree_decl_non_common")
  NEW_NODE ("tree_parm_decl")
  NEW_NODE ("tree_decl_with_vis")
  NEW_NODE ("tree_var_decl")
  NEW_NODE ("tree_field_decl")
  NEW_NODE ("tree_label_decl")
  NEW_NODE ("tree_result_decl")
  NEW_NODE ("tree_const_decl")
  NEW_NODE ("tree_type_decl")
  NEW_NODE ("tree_function_decl")
  NEW_NODE ("tree_translation_unit_decl")
  NEW_NODE ("tree_type")
  NEW_NODE ("tree_list")
  NEW_NODE ("tree_vec")
  NEW_NODE ("tree_exp")
  NEW_NODE ("tree_ssa_name")
  NEW_NODE ("tree_block")
  NEW_NODE ("tree_binfo")
  NEW_NODE ("tree_statement_list")
  NEW_NODE ("tree_constructor")
  NEW_NODE ("tree_omp_clause")
  NEW_NODE ("tree_optimization_option")
  NEW_NODE ("tree_target_option")

  #define NEW_EDGE(src, dest) \
  gdl_new_graph_edge (graph, src, dest);

  NEW_EDGE ("tree_base", "tree_common")
  NEW_EDGE ("tree_common", "tree_int_cst")
  NEW_EDGE ("tree_common", "tree_real_cst")
  NEW_EDGE ("tree_common", "tree_fixed_cst")
  NEW_EDGE ("tree_common", "tree_string")
  NEW_EDGE ("tree_common", "tree_complex")
  NEW_EDGE ("tree_common", "tree_vector")
  NEW_EDGE ("tree_common", "tree_identifier")
  NEW_EDGE ("tree_common", "tree_list")
  NEW_EDGE ("tree_common", "tree_vec")
  NEW_EDGE ("tree_common", "tree_constructor")
  NEW_EDGE ("tree_common", "tree_exp")
  NEW_EDGE ("tree_common", "tree_ssa_name")
  NEW_EDGE ("tree_common", "tree_omp_clause")
  NEW_EDGE ("tree_common", "tree_block")
  NEW_EDGE ("tree_common", "tree_type")
  NEW_EDGE ("tree_common", "tree_binfo")
  NEW_EDGE ("tree_common", "tree_decl_minimal")
  NEW_EDGE ("tree_decl_minimal", "tree_decl_common")
  NEW_EDGE ("tree_decl_common", "tree_decl_with_rtl")
  NEW_EDGE ("tree_decl_common", "tree_field_decl")
  NEW_EDGE ("tree_decl_with_rtl", "tree_label_decl")
  NEW_EDGE ("tree_decl_with_rtl", "tree_result_decl")
  NEW_EDGE ("tree_decl_with_rtl", "tree_const_decl")
  NEW_EDGE ("tree_decl_with_rtl", "tree_parm_decl")
  NEW_EDGE ("tree_decl_with_rtl", "tree_decl_with_vis")
  NEW_EDGE ("tree_decl_with_vis", "tree_var_decl")
  NEW_EDGE ("tree_decl_with_vis", "tree_decl_non_common")
  NEW_EDGE ("tree_decl_non_common", "tree_function_decl")
  NEW_EDGE ("tree_decl_common", "tree_translation_unit_decl")
  NEW_EDGE ("tree_decl_non_common", "tree_type_decl")
  NEW_EDGE ("tree_common", "tree_statement_list")
  NEW_EDGE ("tree_common", "tree_optimization_option")
  NEW_EDGE ("tree_common", "tree_target_option")

  vcg_plugin_common.dump (fname);
}

static void
dump_tree_hierarchy_to_file_4_7 (char *fname)
{
  gdl_graph *graph;
  gdl_node *node;

  graph = vcg_plugin_common.top_graph;
  gdl_set_graph_orientation (graph, "left_to_right");

  #define NEW_NODE(name) \
  node = gdl_new_graph_node (graph, name); \
  gdl_set_node_label (node, name);

  NEW_NODE ("tree_base")
  NEW_NODE ("tree_typed")
  NEW_NODE ("tree_common")
  NEW_NODE ("tree_int_cst")
  NEW_NODE ("tree_real_cst")
  NEW_NODE ("tree_fixed_cst")
  NEW_NODE ("tree_vector")
  NEW_NODE ("tree_string")
  NEW_NODE ("tree_complex")
  NEW_NODE ("tree_identifier")
  NEW_NODE ("tree_decl_minimal")
  NEW_NODE ("tree_decl_common")
  NEW_NODE ("tree_decl_with_rtl")
  NEW_NODE ("tree_decl_non_common")
  NEW_NODE ("tree_parm_decl")
  NEW_NODE ("tree_decl_with_vis")
  NEW_NODE ("tree_var_decl")
  NEW_NODE ("tree_field_decl")
  NEW_NODE ("tree_label_decl")
  NEW_NODE ("tree_result_decl")
  NEW_NODE ("tree_const_decl")
  NEW_NODE ("tree_type_decl")
  NEW_NODE ("tree_function_decl")
  NEW_NODE ("tree_translation_unit_decl")
  NEW_NODE ("tree_type_common")
  NEW_NODE ("tree_type_with_lang_specific")
  NEW_NODE ("tree_type_non_common")
  NEW_NODE ("tree_list")
  NEW_NODE ("tree_vec")
  NEW_NODE ("tree_exp")
  NEW_NODE ("tree_ssa_name")
  NEW_NODE ("tree_block")
  NEW_NODE ("tree_binfo")
  NEW_NODE ("tree_statement_list")
  NEW_NODE ("tree_constructor")
  NEW_NODE ("tree_omp_clause")
  NEW_NODE ("tree_optimization_option")
  NEW_NODE ("tree_target_option")

  #define NEW_EDGE(src, dest) \
  gdl_new_graph_edge (graph, src, dest);

  NEW_EDGE ("tree_base", "tree_typed")
  NEW_EDGE ("tree_typed", "tree_common")
  NEW_EDGE ("tree_typed", "tree_int_cst")
  NEW_EDGE ("tree_typed", "tree_real_cst")
  NEW_EDGE ("tree_typed", "tree_fixed_cst")
  NEW_EDGE ("tree_typed", "tree_string")
  NEW_EDGE ("tree_typed", "tree_complex")
  NEW_EDGE ("tree_typed", "tree_vector")
  NEW_EDGE ("tree_common", "tree_identifier")
  NEW_EDGE ("tree_common", "tree_list")
  NEW_EDGE ("tree_common", "tree_vec")
  NEW_EDGE ("tree_typed", "tree_constructor")
  NEW_EDGE ("tree_typed", "tree_exp")
  NEW_EDGE ("tree_typed", "tree_ssa_name")
  NEW_EDGE ("tree_common", "tree_omp_clause")
  NEW_EDGE ("tree_base", "tree_block")
  NEW_EDGE ("tree_common", "tree_type_common")
  NEW_EDGE ("tree_type_common", "tree_type_with_lang_specific")
  NEW_EDGE ("tree_type_with_lang_specific", "tree_type_non_common")
  NEW_EDGE ("tree_common", "tree_binfo")
  NEW_EDGE ("tree_common", "tree_decl_minimal")
  NEW_EDGE ("tree_decl_minimal", "tree_decl_common")
  NEW_EDGE ("tree_decl_common", "tree_decl_with_rtl")
  NEW_EDGE ("tree_decl_common", "tree_field_decl")
  NEW_EDGE ("tree_decl_with_rtl", "tree_label_decl")
  NEW_EDGE ("tree_decl_with_rtl", "tree_result_decl")
  NEW_EDGE ("tree_decl_common", "tree_const_decl")
  NEW_EDGE ("tree_decl_with_rtl", "tree_parm_decl")
  NEW_EDGE ("tree_decl_with_rtl", "tree_decl_with_vis")
  NEW_EDGE ("tree_decl_with_vis", "tree_var_decl")
  NEW_EDGE ("tree_decl_with_vis", "tree_decl_non_common")
  NEW_EDGE ("tree_decl_non_common", "tree_function_decl")
  NEW_EDGE ("tree_decl_common", "tree_translation_unit_decl")
  NEW_EDGE ("tree_decl_non_common", "tree_type_decl")
  NEW_EDGE ("tree_typed", "tree_statement_list")
  NEW_EDGE ("tree_common", "tree_optimization_option")
  NEW_EDGE ("tree_common", "tree_target_option")

  vcg_plugin_common.dump (fname);
}

/* Public function to dump the gcc tree structure hierarchy.  */

void
vcg_plugin_dump_tree_hierarchy_4_6 (void)
{
  vcg_plugin_common.init ();

  dump_tree_hierarchy_to_file_4_6 ("dump-tree-hierarchy-4.6.vcg");

  vcg_plugin_common.finish ();
}

/* Public function to dump the gcc tree structure hierarchy.  */

void
vcg_plugin_dump_tree_hierarchy_4_7 (void)
{
  vcg_plugin_common.init ();

  dump_tree_hierarchy_to_file_4_7 ("dump-tree-hierarchy-4.7.vcg");

  vcg_plugin_common.finish ();
}

/* Public function to dump the gcc tree structure hierarchy.  */

void
vcg_plugin_dump_tree_hierarchy (void)
{
  vcg_plugin_common.init ();

  if (!strncmp (vcg_plugin_common.gcc_basever, "4.6", 3))
    dump_tree_hierarchy_to_file_4_6 ("dump-tree-hierarchy-4.6.vcg");
  else if (!strncmp (vcg_plugin_common.gcc_basever, "4.7", 3))
    dump_tree_hierarchy_to_file_4_7 ("dump-tree-hierarchy-4.7.vcg");

  vcg_plugin_common.finish ();
}

/* Public function to view the gcc tree structure hierarchy.  */

void
vcg_plugin_view_tree_hierarchy_4_6 (void)
{
  vcg_plugin_common.init ();

  dump_tree_hierarchy_to_file_4_6 (vcg_plugin_common.temp_file_name);

  vcg_plugin_common.show (vcg_plugin_common.temp_file_name);

  vcg_plugin_common.finish ();
}

/* Public function to view the gcc tree structure hierarchy.  */

void
vcg_plugin_view_tree_hierarchy_4_7 (void)
{
  vcg_plugin_common.init ();

  dump_tree_hierarchy_to_file_4_7 (vcg_plugin_common.temp_file_name);

  vcg_plugin_common.show (vcg_plugin_common.temp_file_name);

  vcg_plugin_common.finish ();
}

/* Public function to view the gcc tree structure hierarchy.  */

void
vcg_plugin_view_tree_hierarchy (void)
{
  vcg_plugin_common.init ();

  if (!strncmp (vcg_plugin_common.gcc_basever, "4.6", 3))
    dump_tree_hierarchy_to_file_4_6 (vcg_plugin_common.temp_file_name);
  else if (!strncmp (vcg_plugin_common.gcc_basever, "4.7", 3))
    dump_tree_hierarchy_to_file_4_7 (vcg_plugin_common.temp_file_name);

  vcg_plugin_common.show (vcg_plugin_common.temp_file_name);

  vcg_plugin_common.finish ();
}

/* Plugin callback function for PLUGIN_FINISH event.  */

void *
vcg_plugin_callback_tree_hierarchy_4_6 (void *gcc_data, void *user_data)
{
  vcg_plugin_dump_tree_hierarchy_4_6 ();
  return NULL;
}

/* Plugin callback function for PLUGIN_FINISH event.  */

void *
vcg_plugin_callback_tree_hierarchy_4_7 (void *gcc_data, void *user_data)
{
  vcg_plugin_dump_tree_hierarchy_4_7 ();
  return NULL;
}

/* Plugin callback function for PLUGIN_FINISH event.  */

void *
vcg_plugin_callback_tree_hierarchy (void *gcc_data, void *user_data)
{
  vcg_plugin_dump_tree_hierarchy ();
  return NULL;
}

