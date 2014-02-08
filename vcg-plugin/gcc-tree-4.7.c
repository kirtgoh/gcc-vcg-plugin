/* Dump or view gcc tree.

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

#if GCC_VERSION == 4007

static enum tree_node_structure_enum tns;
static char buf[256];
static void **slot;

/* Tree structure names based on treestruct.def.  */
static const char *ts_names[] = {
  "tree_base",
  "tree_typed",
  "tree_common",
  "tree_int_cst",
  "tree_real_cst",
  "tree_fixed_cst",
  "tree_vector",
  "tree_string",
  "tree_complex",
  "tree_identifier",
  "tree_decl_minimal",
  "tree_decl_common",
  "tree_decl_with_rtl",
  "tree_decl_non_common",
  "tree_decl_with_vis",
  "tree_field_decl",
  "tree_var_decl",
  "tree_parm_decl",
  "tree_label_decl",
  "tree_result_decl",
  "tree_const_decl",
  "tree_type_decl",
  "tree_function_decl",
  "tree_translatin_unit_decl",
  "tree_type_common",
  "tree_type_with_lang_specific",
  "tree_type_non_common",
  "tree_list",
  "tree_vec",
  "tree_exp",
  "tree_ssa_name",
  "tree_block",
  "tree_binfo",
  "tree_statement_list",
  "tree_constructor",
  "tree_omp_clause",
  "tree_optimization_option",
  "tree_target_option"
};

static htab_t tree_table;

static gdl_node *create_tree_node (gdl_graph *graph, tree tn,
                                   char *name, int level);

/* Create gdl edge and add it into GRAPH.  SN is source node,
   TN is target node.  */

static void
create_edge (gdl_graph *graph, gdl_node *sn, gdl_node *tn)
{
  if (tn == NULL || sn == NULL)
    return;

  gdl_new_graph_edge (graph, gdl_get_node_title (sn), gdl_get_node_title (tn));
}

/* Like create_edge, but also set the line style as "dashed".  */

static void
create_dashed_edge (gdl_graph *graph, gdl_node *sn, gdl_node *tn)
{
  gdl_edge *edge;

  if (tn == NULL || sn == NULL)
    return;

  edge = gdl_new_graph_edge (graph, gdl_get_node_title (sn),
                             gdl_get_node_title (tn));
  gdl_set_edge_linestyle (edge, "dashed");
}

static gdl_node *
create_ellipsis_node (gdl_graph *graph)
{
  gdl_node *node;

  node = gdl_new_graph_node (graph, NULL);
  gdl_set_node_label (node, "...");

  return node;
}

static gdl_node *
create_duplicated_node (gdl_graph *graph, tree tn)
{
  gdl_node *node;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  tns = tree_node_structure (tn);
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("duplicated");
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  return node;
}

/* Create gdl node for struct tree_base.  */

static gdl_node *
create_tree_base (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  tns = TS_BASE;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("...");
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  return node;
}

/* Create gdl node for struct tree_typed.  */

static gdl_node *
create_tree_typed (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->typed
  tns = TS_TYPED;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_base\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("type: %p", tx.type);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_node (graph, tx.type, "type", level - 1);
      create_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_common.  */

static gdl_node *
create_tree_common (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->common
  tns = TS_COMMON;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_typed\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("chain: %p", tx.chain);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_typed (graph, (tree) &tx.typed, "typed", level - 1);
      create_dashed_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.chain, "chain", level - 1);
      create_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_int_cst.  */

static gdl_node *
create_tree_int_cst (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->int_cst
  tns = TS_INT_CST;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_typed\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("int_cst: %ld", tx.int_cst);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_typed (graph, (tree) &tx.typed, "typed", level - 1);
      create_dashed_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_real_cst.  */

static gdl_node *
create_tree_real_cst (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->real_cst
  tns = TS_REAL_CST;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_typed\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("real_cst_ptr: %p", tx.real_cst_ptr);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_typed (graph, (tree) &tx.typed, "typed", level - 1);
      create_dashed_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_fixed_cst.  */

static gdl_node *
create_tree_fixed_cst (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->fixed_cst
  tns = TS_FIXED_CST;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_typed\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("fixed_cst_ptr: %p", tx.fixed_cst_ptr);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_typed (graph, (tree) &tx.typed, "typed", level - 1);
      create_dashed_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_string.  */

static gdl_node *
create_tree_string (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->string
  tns = TS_STRING;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_typed\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("length: %d\n", tx.length);
  strncpy (buf, tx.str, tx.length);
  vcg_plugin_common.buf_print ("str: %s", buf);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_typed (graph, (tree) &tx.typed, "typed", level - 1);
      create_dashed_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_complex.  */

static gdl_node *
create_tree_complex (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->complex
  tns = TS_COMPLEX;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_typed\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("real: %p\n", tx.real);
  vcg_plugin_common.buf_print ("imag: %p", tx.imag);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_typed (graph, (tree) &tx.typed, "typed", level - 1);
      create_dashed_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.real, "real", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.imag, "imag", level - 1);
      create_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_vector.  */

static gdl_node *
create_tree_vector (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->vector
  tns = TS_VECTOR;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_typed\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("elements: %p", tx.elements);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_typed (graph, (tree) &tx.typed, "typed", level - 1);
      create_dashed_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.elements, "elements", level - 1);
      create_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_identifier.  */

static gdl_node *
create_tree_identifier (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->identifier
  tns = TS_IDENTIFIER;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_common\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("&id: %p", &tx.id);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_common (graph, (tree) &tx.common, "common", level - 1);
      create_dashed_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_list.  */

static gdl_node *
create_tree_list (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->list
  tns = TS_LIST;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_common\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("purpose: %p\n", tx.purpose);
  vcg_plugin_common.buf_print ("value: %p", tx.value);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_common (graph, (tree) &tx.common, "common", level - 1);
      create_dashed_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.purpose, "purpose", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.value, "value", level - 1);
      create_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_vec.  */

static gdl_node *
create_tree_vec (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;
  int i, n;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->vec
  tns = TS_VEC;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_common\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("length: %d\n", tx.length);
  vcg_plugin_common.buf_print ("a[]: %p", tx.a);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_common (graph, (tree) &tx.common, "common", level - 1);
      create_dashed_edge (graph, node, anode);

      /* Avoid too many nodes.  */
      n = tx.length > 4 ? 4 : tx.length;

      for (i = 0; i < n; i++)
        {
          sprintf (buf, "a[%d]", i);
          anode = create_tree_node (graph, tx.a[i], buf, level - 1);
          create_edge (graph, node, anode);
          //if (anode)
          //  gdl_set_node_horizontal_order (anode, i + 1);
        }

      if (tx.length > n)
        {
          anode = create_ellipsis_node (graph);
          create_edge (graph, node, anode);
        }
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_constructor.  */

static gdl_node *
create_tree_constructor (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->constructor
  tns = TS_CONSTRUCTOR;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_typed\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("elts: %p", tx.elts);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_typed (graph, (tree) &tx.typed, "typed", level - 1);
      create_dashed_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_exp.  */

static gdl_node *
create_tree_exp (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;
  int i, n;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->exp
  tns = TS_EXP;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_typed\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("locus: %p\n", tx.locus);
  vcg_plugin_common.buf_print ("block: %p\n", tx.block);
  vcg_plugin_common.buf_print ("operands[]: %p", tx.operands);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_typed (graph, (tree) &tx.typed, "typed", level - 1);
      create_dashed_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.block, "block", level - 1);
      create_edge (graph, node, anode);

      /* Avoid too many nodes.  */
      n = TREE_OPERAND_LENGTH(tn) > 4 ? 4 : TREE_OPERAND_LENGTH(tn);

      for (i = 0; i < n; i++)
        {
          sprintf (buf, "operands[%d]", i);
          anode = create_tree_node (graph, tx.operands[i], buf, level - 1);
          create_edge (graph, node, anode);
          //if (anode)
          //  gdl_set_node_horizontal_order (anode, i + 1);
        }

      if (TREE_OPERAND_LENGTH(tn) > n)
        {
          anode = create_ellipsis_node (graph);
          create_edge (graph, node, anode);
        }
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_ssa_name.  */

static gdl_node *
create_tree_ssa_name (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->ssa_name
  tns = TS_SSA_NAME;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_typed\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("var: %p\n", tx.var);
  vcg_plugin_common.buf_print ("def_stmt: %p\n", tx.def_stmt);
  vcg_plugin_common.buf_print ("version: %d\n", tx.version);
  vcg_plugin_common.buf_print ("...");
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_typed (graph, (tree) &tx.typed, "typed", level - 1);
      create_dashed_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.var, "var", level - 1);
      create_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_omp_clause.  */

static gdl_node *
create_tree_omp_clause (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->omp_clause
  tns = TS_OMP_CLAUSE;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_common\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("ops[]: %p\n", tx.ops);
  vcg_plugin_common.buf_print ("...");
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_common (graph, (tree) &tx.common, "common", level - 1);
      create_dashed_edge (graph, node, anode);

//      for (i = 0; i < TREE_OPERAND_LENGTH(tn); i++)
//        {
//          sprintf (buf, "ops[%d]", i);
//          anode = create_tree_node (graph, tx.ops[i], buf, level - 1);
//          create_edge (graph, node, anode);
//        }
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_block.  */

static gdl_node *
create_tree_block (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->block
  tns = TS_BLOCK;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_base\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("vars: %p\n", tx.vars);
  vcg_plugin_common.buf_print ("subblocks: %p\n", tx.subblocks);
  vcg_plugin_common.buf_print ("supercontext: %p\n", tx.supercontext);
  vcg_plugin_common.buf_print ("abstract_origin: %p\n", tx.abstract_origin);
  vcg_plugin_common.buf_print ("fragment_origin: %p\n", tx.fragment_origin);
  vcg_plugin_common.buf_print ("fragment_chain: %p\n", tx.fragment_chain);
  vcg_plugin_common.buf_print ("...");
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_base (graph, (tree) &tx.base, "base", level - 1);
      create_dashed_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.vars, "vars", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.subblocks, "subblocks", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.supercontext, "supercontext",
                                level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.abstract_origin, "abstract_origin",
                                level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.fragment_origin, "fragment_origin",
                                level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.fragment_chain, "fragment_chain",
                                level - 1);
      create_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_type_common.  */

static gdl_node *
create_tree_type_common (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->type_common
  tns = TS_TYPE_COMMON;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_common\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("size: %p\n", tx.size);
  vcg_plugin_common.buf_print ("size_unit: %p\n", tx.size_unit);
  vcg_plugin_common.buf_print ("attributes: %p\n", tx.attributes);
  vcg_plugin_common.buf_print ("pointer_to: %p\n", tx.pointer_to);
  vcg_plugin_common.buf_print ("reference_to: %p\n", tx.reference_to);
  vcg_plugin_common.buf_print ("name: %p\n", tx.name);
  vcg_plugin_common.buf_print ("next_variant: %p\n", tx.next_variant);
  vcg_plugin_common.buf_print ("main_variant: %p\n", tx.main_variant);
  vcg_plugin_common.buf_print ("context: %p\n", tx.context);
  vcg_plugin_common.buf_print ("canonical: %p\n", tx.canonical);
  vcg_plugin_common.buf_print ("...");
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_common (graph, (tree) &tx.common, "common", level - 1);
      create_dashed_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.size, "size", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.size_unit, "size_unit", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.attributes, "attributes", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.pointer_to, "pointer_to", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.reference_to, "reference_to",
                                level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.name, "name", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.next_variant, "next_variant",
                                level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.main_variant, "main_variant",
                                level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.context, "context", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.canonical, "canonical", level - 1);
      create_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_type_with_lang_specific.  */

static gdl_node *
create_tree_type_with_lang_specific (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->type_with_lang_specific
  tns = TS_TYPE_WITH_LANG_SPECIFIC;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_type_common\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("lang_specific: %p", tx.lang_specific);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_type_common (graph, (tree) &tx.common, "common", level - 1);
      create_dashed_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_type_non_common.  */

static gdl_node *
create_tree_type_non_common (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->type_non_common
  tns = TS_TYPE_NON_COMMON;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_type_with_lang_specific\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("values: %p\n", tx.values);
  vcg_plugin_common.buf_print ("minval: %p\n", tx.minval);
  vcg_plugin_common.buf_print ("maxval: %p\n", tx.maxval);
  vcg_plugin_common.buf_print ("binfo: %p", tx.binfo);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_type_with_lang_specific (graph,
                                                   (tree) &tx.with_lang_specific,
                                                   "with_lang_specific",
                                                   level - 1);
      create_dashed_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.values, "values", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.minval, "minval", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.maxval, "maxval", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.binfo, "binfo", level - 1);
      create_edge (graph, node, anode);

    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_binfo.  */

static gdl_node *
create_tree_binfo (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->binfo
  tns = TS_BINFO;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_common\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("offset: %p\n", tx.offset);
  vcg_plugin_common.buf_print ("vtable: %p\n", tx.vtable);
  vcg_plugin_common.buf_print ("virtuals: %p\n", tx.virtuals);
  vcg_plugin_common.buf_print ("vptr_field: %p\n", tx.vptr_field);
  vcg_plugin_common.buf_print ("base_accesses: %p\n", tx.base_accesses);
  vcg_plugin_common.buf_print ("inheritance: %p\n", tx.inheritance);
  vcg_plugin_common.buf_print ("vtt_subvtt: %p\n", tx.vtt_subvtt);
  vcg_plugin_common.buf_print ("vtt_vptr: %p\n", tx.vtt_vptr);
  vcg_plugin_common.buf_print ("base_binfos: %p", tx.base_binfos);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_common (graph, (tree) &tx.common, "common", level - 1);
      create_dashed_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.offset, "offset", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.vtable, "vtable", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.virtuals, "virtuals", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.vptr_field, "vptr_field", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.inheritance, "inheritance", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.vtt_subvtt, "vtt_subvtt", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.vtt_vptr, "vtt_vptr", level - 1);
      create_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_decl_minimal.  */

static gdl_node *
create_tree_decl_minimal (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->decl_minimal
  tns = TS_DECL_MINIMAL;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_common\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("name: %p\n", tx.name);
  vcg_plugin_common.buf_print ("context: %p\n", tx.context);
  vcg_plugin_common.buf_print ("...");
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_common (graph, (tree) &tx.common, "common", level - 1);
      create_dashed_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.name, "name", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.context, "context", level - 1);
      create_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_decl_common.  */

static gdl_node *
create_tree_decl_common (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->decl_common
  tns = TS_DECL_COMMON;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_decl_minimal\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("size: %p\n", tx.size);
  vcg_plugin_common.buf_print ("size_unit: %p\n", tx.size_unit);
  vcg_plugin_common.buf_print ("initial: %p\n", tx.initial);
  vcg_plugin_common.buf_print ("attributes: %p\n", tx.attributes);
  vcg_plugin_common.buf_print ("abstract_origin: %p", tx.abstract_origin);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_decl_minimal (graph, (tree) &tx.common, "common", level - 1);
      create_dashed_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.size, "size", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.size_unit, "size_unit", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.initial, "initial", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.attributes, "attributes", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.abstract_origin, "abstract_origin",
                                level - 1);
      create_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_decl_with_rtl.  */

static gdl_node *
create_tree_decl_with_rtl (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->decl_with_rtl
  tns = TS_DECL_WRTL;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_decl_common\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("rtl: %p", tx.rtl);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_decl_common (graph, (tree) &tx.common, "common", level - 1);
      create_dashed_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_field_decl.  */

static gdl_node *
create_tree_field_decl (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->field_decl
  tns = TS_FIELD_DECL;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_decl_common\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("offset: %p\n", tx.offset);
  vcg_plugin_common.buf_print ("bit_field_type: %p\n", tx.bit_field_type);
  vcg_plugin_common.buf_print ("qualifier: %p\n", tx.qualifier);
  vcg_plugin_common.buf_print ("bit_offset: %p\n", tx.bit_offset);
  vcg_plugin_common.buf_print ("fcontext: %p", tx.fcontext);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_decl_common (graph, (tree) &tx.common, "common", level - 1);
      create_dashed_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.offset, "offset", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.bit_field_type, "bit_field_type",
                                level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.qualifier, "qualifier", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.bit_offset, "bit_offset", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.fcontext, "fcontext", level - 1);
      create_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_label_decl.  */

static gdl_node *
create_tree_label_decl (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->label_decl
  tns = TS_LABEL_DECL;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_decl_with_rtl\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("label_decl_uid: %d\n", tx.label_decl_uid);
  vcg_plugin_common.buf_print ("eh_landing_pad_nr: %d", tx.eh_landing_pad_nr);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_decl_with_rtl (graph, (tree) &tx.common, "common",
                                         level - 1);
      create_dashed_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_result_decl.  */

static gdl_node *
create_tree_result_decl (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->result_decl
  tns = TS_RESULT_DECL;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_decl_with_rtl\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("ann: %p", tx.ann);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_decl_with_rtl (graph, (tree) &tx.common, "common",
                                         level - 1);
      create_dashed_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_const_decl.  */

static gdl_node *
create_tree_const_decl (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->const_decl
  tns = TS_RESULT_DECL;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_decl_common");
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_decl_common (graph, (tree) &tx.common, "common", level - 1);
      create_dashed_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_parm_decl.  */

static gdl_node *
create_tree_parm_decl (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->parm_decl
  tns = TS_PARM_DECL;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_decl_with_rtl\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("incoming_rtl: %p\n", tx.incoming_rtl);
  vcg_plugin_common.buf_print ("ann: %p", tx.ann);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_decl_with_rtl (graph, (tree) &tx.common, "common", level - 1);
      create_dashed_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_decl_with_vis.  */

static gdl_node *
create_tree_decl_with_vis (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->decl_with_vis
  tns = TS_DECL_WITH_VIS;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_decl_with_rtl\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("assembler_name: %p\n", tx.assembler_name);
  vcg_plugin_common.buf_print ("section_name: %p\n", tx.section_name);
  vcg_plugin_common.buf_print ("comdat_group: %p\n", tx.comdat_group);
  vcg_plugin_common.buf_print ("...");
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_decl_with_rtl (graph, (tree) &tx.common, "common",
                                         level - 1);
      create_dashed_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.assembler_name, "assembler_name",
                                level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.section_name, "section_name",
                                level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.comdat_group, "comdat_group",
                                level - 1);
      create_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_var_decl.  */

static gdl_node *
create_tree_var_decl (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->var_decl
  tns = TS_VAR_DECL;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_decl_with_vis\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("ann: %p", tx.ann);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_decl_with_vis (graph, (tree) &tx.common, "common", level - 1);
      create_dashed_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_decl_non_common.  */

static gdl_node *
create_tree_decl_non_common (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->decl_non_common
  tns = TS_DECL_NON_COMMON;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_decl_with_vis\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("saved_tree: %p\n", tx.saved_tree);
  vcg_plugin_common.buf_print ("arguments: %p\n", tx.arguments);
  vcg_plugin_common.buf_print ("result: %p\n", tx.result);
  vcg_plugin_common.buf_print ("vindex: %p", tx.vindex);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_decl_with_vis (graph, (tree) &tx.common, "common",
                                         level - 1);
      create_dashed_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.saved_tree, "saved_tree", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.arguments, "arguments", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.result, "result", level - 1);
      create_edge (graph, node, anode);

      anode = create_tree_node (graph, tx.vindex, "vindex", level - 1);
      create_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_function_decl.  */

static gdl_node *
create_tree_function_decl (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->function_decl
  tns = TS_FUNCTION_DECL;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_decl_non_common\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("f: %p\n", tx.f);
  vcg_plugin_common.buf_print ("personality: %p\n", tx.personality);
  vcg_plugin_common.buf_print ("function_specific_target: %p\n",
                               tx.function_specific_target);
  vcg_plugin_common.buf_print ("function_specific_optimization: %p\n",
                               tx.function_specific_optimization);
  vcg_plugin_common.buf_print ("...");
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_decl_non_common (graph, (tree) &tx.common, "common", level - 1);
      create_dashed_edge (graph, node, anode);
    
      anode = create_tree_node (graph, tx.personality, "personality", level - 1);
      create_edge (graph, node, anode);
    
      anode = create_tree_node (graph, tx.function_specific_target,
                                "function_specific_target", level - 1);
      create_edge (graph, node, anode);
    
      anode = create_tree_node (graph, tx.function_specific_optimization,
                                "function_specific_optimization", level - 1);
      create_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_translation_unit_decl.  */

static gdl_node *
create_tree_translation_unit_decl (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->translation_unit_decl
  tns = TS_TRANSLATION_UNIT_DECL;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_decl_common\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("language: %s", tx.language);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_decl_common (graph, (tree) &tx.common, "common", level - 1);
      create_dashed_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_type_decl.  */

static gdl_node *
create_tree_type_decl (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->type_decl
  tns = TS_TYPE_DECL;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_decl_non_common");
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_decl_non_common (graph, (tree) &tx.common, "common", level - 1);
      create_dashed_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_statement_list.  */

static gdl_node *
create_tree_statement_list (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->stmt_list
  tns = TS_STATEMENT_LIST;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_typed\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("head: %p\n", tx.head);
  vcg_plugin_common.buf_print ("tail: %p", tx.tail);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_typed (graph, (tree) &tx.typed, "typed", level - 1);
      create_dashed_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_optimization_option.  */

static gdl_node *
create_tree_optimization_option (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->optimization
  tns = TS_OPTIMIZATION;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_common\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("&opts: %p", &tx.opts);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_common (graph, (tree) &tx.common, "common", level - 1);
      create_dashed_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create gdl node for struct tree_target_option.  */

static gdl_node *
create_tree_target_option (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node, *anode;
  char *label;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  #define tx tn->target_option
  tns = TS_TARGET_OPTION;
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("addr: %p\n", tn);
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("tree_common\n");
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("&opts: %p", &tx.opts);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  if (level > 0)
    {
      anode = create_tree_common (graph, (tree) &tx.common, "common", level - 1);
      create_dashed_edge (graph, node, anode);
    }
  #undef tx

  return node;
}

/* Create a gdl node for tree TN and add it into GRAPH.  LEVEL stands
   for the current nested level for gcc tree structure data.  */

static gdl_node *
create_tree_node (gdl_graph *graph, tree tn, char *name, int level)
{
  gdl_node *node;

  if (tn == 0)
    return NULL;

  if (htab_find (tree_table, tn))
    return create_duplicated_node (graph, tn);

  slot = htab_find_slot (tree_table, tn, INSERT);
  *slot = tn;

  tns = tree_node_structure (tn);
  switch (tns)
    {
    case TS_BASE:
      node = create_tree_base (graph, tn, name, level);
      break;
      
    case TS_TYPED:
      node = create_tree_typed (graph, tn, name, level);
      break;

    case TS_COMMON:
      node = create_tree_common (graph, tn, name, level);
      break;
      
    case TS_INT_CST:
      node = create_tree_int_cst (graph, tn, name, level);
      break;
      
    case TS_REAL_CST:
      node = create_tree_real_cst (graph, tn, name, level);
      break;
      
    case TS_FIXED_CST:
      node = create_tree_fixed_cst (graph, tn, name, level);
      break;
      
    case TS_VECTOR:
      node = create_tree_vector (graph, tn, name, level);
      break;
      
    case TS_STRING:
      node = create_tree_string (graph, tn, name, level);
      break;
      
    case TS_COMPLEX:
      node = create_tree_complex (graph, tn, name, level);
      break;
      
    case TS_IDENTIFIER:
      node = create_tree_identifier (graph, tn, name, level);
      break;
      
    case TS_DECL_MINIMAL:
      node = create_tree_decl_minimal (graph, tn, name, level);
      break;

    case TS_DECL_COMMON:
      node = create_tree_decl_common (graph, tn, name, level);
      break;

    case TS_DECL_WRTL:
      node = create_tree_decl_with_rtl (graph, tn, name, level);
      break;

    case TS_DECL_NON_COMMON:
      node = create_tree_decl_non_common (graph, tn, name, level);
      break;

    case TS_DECL_WITH_VIS:
      node = create_tree_decl_with_vis (graph, tn, name, level);
      break;
      
    case TS_FIELD_DECL:
      node = create_tree_field_decl (graph, tn, name, level);
      break;
      
    case TS_VAR_DECL:
      node = create_tree_var_decl (graph, tn, name, level);
      break;
      
    case TS_PARM_DECL:
      node = create_tree_parm_decl (graph, tn, name, level);
      break;
      
    case TS_LABEL_DECL:
      node = create_tree_label_decl (graph, tn, name, level);
      break;
      
    case TS_RESULT_DECL:
      node = create_tree_result_decl (graph, tn, name, level);
      break;
      
    case TS_CONST_DECL:
      node = create_tree_const_decl (graph, tn, name, level);
      break;
      
    case TS_TYPE_DECL:
      node = create_tree_type_decl (graph, tn, name, level);
      break;
      
    case TS_FUNCTION_DECL:
      node = create_tree_function_decl (graph, tn, name, level);
      break;
      
    case TS_TRANSLATION_UNIT_DECL:
      node = create_tree_translation_unit_decl (graph, tn, name, level);
      break;
      
    case TS_TYPE_COMMON:
      node = create_tree_type_common (graph, tn, name, level);
      break;
      
    case TS_TYPE_WITH_LANG_SPECIFIC:
      node = create_tree_type_with_lang_specific (graph, tn, name, level);
      break;
      
    case TS_TYPE_NON_COMMON:
      node = create_tree_type_non_common (graph, tn, name, level);
      break;
      
    case TS_LIST:
      node = create_tree_list (graph, tn, name, level);
      break;
      
    case TS_VEC:
      node = create_tree_vec (graph, tn, name, level);
      break;
      
    case TS_EXP:
      node = create_tree_exp (graph, tn, name, level);
      break;
      
    case TS_SSA_NAME:
      node = create_tree_ssa_name (graph, tn, name, level);
      break;
      
    case TS_BLOCK:
      node = create_tree_block (graph, tn, name, level);
      break;
      
    case TS_BINFO:
      node = create_tree_binfo (graph, tn, name, level);
      break;
      
    case TS_STATEMENT_LIST:
      node = create_tree_statement_list (graph, tn, name, level);
      break;
      
    case TS_CONSTRUCTOR:
      node = create_tree_constructor (graph, tn, name, level);
      break;
      
    case TS_OMP_CLAUSE:
      node = create_tree_omp_clause (graph, tn, name, level);
      break;
      
    case TS_OPTIMIZATION:
      node = create_tree_optimization_option (graph, tn, name, level);
      break;
      
    case TS_TARGET_OPTION:
      node = create_tree_target_option (graph, tn, name, level);
      break;

    case LAST_TS_ENUM:
      abort ();
    }
  return node;
}

/* Dump tree node NODE into the file FNAME.  */

static void
dump_tree_node_to_file (char *fname, tree node)
{
  gdl_graph *graph;

  tree_table = htab_create (32, htab_hash_pointer, htab_eq_pointer, NULL);

  graph = vcg_plugin_common.top_graph;
  create_tree_node (graph, node, "tree", 0);
  vcg_plugin_common.dump (fname);

  htab_delete (tree_table);
}

/* Dump tree NODE into the file FNAME.  */

static void
dump_tree_to_file (char *fname, tree node)
{
  gdl_graph *graph;

  tree_table = htab_create (32, htab_hash_pointer, htab_eq_pointer, NULL);

  graph = vcg_plugin_common.top_graph;
  create_tree_node (graph, node, "tree", 10);
  vcg_plugin_common.dump (fname);

  htab_delete (tree_table);
}

/* Public function to dump a gcc tree NODE.  */

void
vcg_plugin_dump_tree_node (tree node)
{
  vcg_plugin_common.init ();

  dump_tree_node_to_file ("dump-tree-node.vcg", node);

  vcg_plugin_common.finish ();
}

/* Public function to view a gcc tree NODE.  */

void
vcg_plugin_view_tree_node (tree node)
{
  vcg_plugin_common.init ();

  dump_tree_node_to_file (vcg_plugin_common.temp_file_name, node);
  vcg_plugin_common.show (vcg_plugin_common.temp_file_name);

  vcg_plugin_common.finish ();
}

/* Public function to dump a gcc tree NODE.  */

void
vcg_plugin_dump_tree (tree node)
{
  vcg_plugin_common.init ();

  dump_tree_to_file ("dump-tree.vcg", node);

  vcg_plugin_common.finish ();
}

/* Public function to view a gcc tree NODE.  */

void
vcg_plugin_view_tree (tree node)
{
  vcg_plugin_common.init ();

  dump_tree_to_file (vcg_plugin_common.temp_file_name, node);
  vcg_plugin_common.show (vcg_plugin_common.temp_file_name);

  vcg_plugin_common.finish ();
}

#endif

