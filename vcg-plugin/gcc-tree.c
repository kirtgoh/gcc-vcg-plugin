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

static const char *ts_names[] = {
  "tree_base",
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
  "tree_label_decl",
  "tree_function_decl",
  "tree_translatin_unit_decl",
  "tree_type",
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

static gdl_node *create_tree_node (gdl_graph *graph, tree tn, char *name,
                                   int nested_level);

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

/* Create gdl node for tree common part COMMON and add it into GRAPH.  */

static gdl_node *
create_common_node (gdl_graph *graph, void *common,
                    enum tree_node_structure_enum tns, int nested_level)
{
  gdl_node *node, *anode;
  char *label;

  if (common == NULL)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  /* Avoid nested level is too deep.  */
  if (nested_level > 10)
    {
      gdl_set_node_label (node, "...");
      return node;
    }

  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("name: common\n");
  vcg_plugin_common.buf_print ("addr: 0x%x", (unsigned) common);
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);

  switch (tns)
    {
    case TS_BASE:
      break;
      
    case TS_COMMON:
      #define tx (*(struct tree_common *) common)
      anode = create_common_node (graph, &tx.base, TS_BASE, nested_level + 1);
      create_dashed_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.chain, "chain", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.type, "type", nested_level + 1);
      create_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_DECL_MINIMAL:
      #define tx (*(struct tree_decl_minimal *) common)
      anode = create_common_node (graph, &tx.common, TS_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.name, "name", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.context, "context", nested_level + 1);
      create_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_DECL_COMMON:
      #define tx (*(struct tree_decl_common *) common)
      anode = create_common_node (graph, &tx.common, TS_DECL_MINIMAL,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.size, "size", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.size_unit, "size_unit", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.initial, "initial", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.attributes, "attributes", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.abstract_origin, "abstract_origin", nested_level + 1);
      create_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_DECL_WRTL:
      #define tx (*(struct tree_decl_with_rtl *) common)
      anode = create_common_node (graph, &tx.common, TS_DECL_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_DECL_NON_COMMON:
      #define tx (*(struct tree_decl_non_common *) common)
      anode = create_common_node (graph, &tx.common, TS_DECL_WITH_VIS,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.saved_tree, "saved_tree", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.arguments, "arguments", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.result, "result", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.vindex, "vindex", nested_level + 1);
      create_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_DECL_WITH_VIS:
      #define tx (*(struct tree_decl_with_vis *) common)
      anode = create_common_node (graph, &tx.common, TS_DECL_WRTL,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.assembler_name, "assembler_name", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.section_name, "section_name", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.comdat_group, "comdat_group", nested_level + 1);
      create_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_INT_CST:
    case TS_REAL_CST:
    case TS_FIXED_CST:
    case TS_VECTOR:
    case TS_STRING:
    case TS_COMPLEX:
    case TS_IDENTIFIER:
    case TS_FIELD_DECL:
    case TS_VAR_DECL:
    case TS_PARM_DECL:
    case TS_LABEL_DECL:
    case TS_RESULT_DECL:
    case TS_CONST_DECL:
    case TS_TYPE_DECL:
    case TS_FUNCTION_DECL:
    case TS_TRANSLATION_UNIT_DECL:
    case TS_TYPE:
    case TS_LIST:
    case TS_VEC:
    case TS_EXP:
    case TS_SSA_NAME:
    case TS_BLOCK:
    case TS_BINFO:
    case TS_STATEMENT_LIST:
    case TS_CONSTRUCTOR:
    case TS_OMP_CLAUSE:
    case TS_OPTIMIZATION:
    case TS_TARGET_OPTION:
    case LAST_TS_ENUM:
      abort ();
      break;
    }

  return node;
}

/* Create a gdl node for tree TN and add it into GRAPH.  NESTED_LEVEL stands
   for the current nested level for gcc tree structure data.  */

static gdl_node *
create_tree_node (gdl_graph *graph, tree tn, char *name, int nested_level)
{
  gdl_node *node, *anode;
  enum tree_node_structure_enum tns;
  char buf[256];
  char *label;
  void **slot;
  int i;

  if (tn == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  /* Avoid nested level is too deep.  */
  if (nested_level > 10)
    {
      gdl_set_node_label (node, "...");
      return node;
    }

  tns = tree_node_structure (tn);
  
  vcg_plugin_common.buf_print ("%s\n", ts_names[tns]);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("name: %s\n", name);
  vcg_plugin_common.buf_print ("addr: 0x%x", (unsigned) tn);

  switch (tns)
    {
    case TS_BASE:
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      break;
      
    case TS_COMMON:
      #define tx tn->common
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.base, TS_BASE,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.chain, "chain", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.type, "type", nested_level + 1);
      create_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_INT_CST:
      #define tx tn->int_cst
      vcg_plugin_common.buf_print ("\nvalue: %ld", tx.int_cst);
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_REAL_CST:
      #define tx tn->real_cst
      vcg_plugin_common.buf_print ("\nreal_cst_ptr: 0x%x", (unsigned) tx.real_cst_ptr);
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_FIXED_CST:
      #define tx tn->fixed_cst
      vcg_plugin_common.buf_print ("\nfixed_cst_ptr: 0x%x", (unsigned) tx.fixed_cst_ptr);
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_VECTOR:
      #define tx tn->vector
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.elements, "elements",
                                nested_level + 1);
      create_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_STRING:
      #define tx tn->string
      sprintf (buf, "\nvalue: ");
      strncat (buf, tx.str, tx.length);
      vcg_plugin_common.buf_print ("%s", buf);
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_COMPLEX:
      #define tx tn->complex
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.real, "real", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.imag, "imag", nested_level + 1);
      create_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_IDENTIFIER:
      #define tx tn->identifier
      vcg_plugin_common.buf_print ("\nvalue: %s", tx.id.str);
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_DECL_MINIMAL:
    case TS_DECL_COMMON:
    case TS_DECL_WRTL:
    case TS_DECL_NON_COMMON:
    case TS_DECL_WITH_VIS:
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      break;
      
    case TS_FIELD_DECL:
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      break;
      
    case TS_VAR_DECL:
      #define tx tn->var_decl
      vcg_plugin_common.buf_print ("\nann = 0x%x", (unsigned) tx.ann);
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_DECL_WITH_VIS,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_PARM_DECL:
      #define tx tn->parm_decl
      vcg_plugin_common.buf_print ("\nann = 0x%x", (unsigned) tx.ann);
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_DECL_WRTL,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_LABEL_DECL:
      #define tx tn->result_decl
      vcg_plugin_common.buf_print ("\nann = 0x%x", (unsigned) tx.ann);
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_DECL_WRTL,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_RESULT_DECL:
      #define tx tn->result_decl
      vcg_plugin_common.buf_print ("\nann = 0x%x", (unsigned) tx.ann);
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_DECL_WRTL,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_CONST_DECL:
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      break;
      
    case TS_TYPE_DECL:
      #define tx tn->type_decl
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_DECL_NON_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_FUNCTION_DECL:
      #define tx tn->function_decl
      if (htab_find (tree_table, tn))
        {
          vcg_plugin_common.buf_print ("\nduplicated");
          label = vcg_plugin_common.buf_finish ();
          gdl_set_node_label (node, label);

          return node;
        }
      slot = htab_find_slot (tree_table, tn, INSERT);
      *slot = tn;
      vcg_plugin_common.buf_print ("\nf = 0x%x", (unsigned) tx.f);
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_DECL_NON_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.personality, "personality",
                                nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.function_specific_target,
                                "function_specific_target", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.function_specific_optimization,
                                "function_specific_optimization",
                                nested_level + 1);
      create_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_TRANSLATION_UNIT_DECL:
      #define tx tn->translation_unit_decl
      vcg_plugin_common.buf_print ("\nlanguage: %s", tx.language);
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_DECL_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_TYPE:
      #define tx tn->type
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      if (nested_level == 1)
        {
          anode = create_common_node (graph, &tx.common, TS_COMMON,
                                      nested_level + 1);
          create_dashed_edge (graph, node, anode);
          anode = create_tree_node (graph, tx.values, "values", nested_level + 1);
          create_edge (graph, node, anode);
          anode = create_tree_node (graph, tx.size, "size", nested_level + 1);
          create_edge (graph, node, anode);
          anode = create_tree_node (graph, tx.size_unit, "size_unit",
                                    nested_level + 1);
          create_edge (graph, node, anode);
          anode = create_tree_node (graph, tx.attributes, "attributes",
                                    nested_level + 1);
          create_edge (graph, node, anode);
          anode = create_tree_node (graph, tx.pointer_to, "pointer_to",
                                    nested_level + 1);
          create_edge (graph, node, anode);
          anode = create_tree_node (graph, tx.reference_to, "reference_to",
                                    nested_level + 1);
          create_edge (graph, node, anode);
          anode = create_tree_node (graph, tx.name, "name", nested_level + 1);
          create_edge (graph, node, anode);
          anode = create_tree_node (graph, tx.minval, "minval", nested_level + 1);
          create_edge (graph, node, anode);
          anode = create_tree_node (graph, tx.maxval, "maxval", nested_level + 1);
          create_edge (graph, node, anode);
          anode = create_tree_node (graph, tx.binfo, "binfo", nested_level + 1);
          create_edge (graph, node, anode);
          anode = create_tree_node (graph, tx.context, "context", nested_level + 1);
          create_edge (graph, node, anode);
          anode = create_tree_node (graph, tx.canonical, "canonical",
                                    nested_level + 1);
          create_edge (graph, node, anode);
        }
      #undef tx
      break;
      
    case TS_LIST:
      #define tx tn->list
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.purpose, "purpose", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.value, "value", nested_level + 1);
      create_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_VEC:
      #define tx tn->vec
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      for (i = 0; i < tx.length; i++)
        {
          sprintf (buf, "a[%d]", i);
          anode = create_tree_node (graph, tx.a[i], buf, nested_level + 1);
          create_edge (graph, node, anode);
        }
      #undef tx
      break;
      
    case TS_EXP:
      #define tx tn->exp
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.block, "block", nested_level + 1);
      create_edge (graph, node, anode);
      for (i = 0; i < TREE_OPERAND_LENGTH(tn); i++)
        {
          sprintf (buf, "operands[%d]", i);
          anode = create_tree_node (graph, tx.operands[i], buf, nested_level + 1);
          create_edge (graph, node, anode);
        }
      #undef tx
      break;
      
    case TS_SSA_NAME:
      #define tx tn->ssa_name
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.var, "var", nested_level + 1);
      create_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_BLOCK:
      #define tx tn->block
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.vars, "vars", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.subblocks, "subblocks", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.supercontext, "supercontext", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.abstract_origin, "abstract_origin", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.fragment_origin, "fragment_origin", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.fragment_chain, "fragment_chain", nested_level + 1);
      create_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_BINFO:
      #define tx tn->binfo
      vcg_plugin_common.buf_print ("\nbase_binfos: 0x%x", (unsigned) &tx.base_binfos);
      vcg_plugin_common.buf_print ("\nbase_accesses: 0x%x", (unsigned) &tx.base_accesses);
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.offset, "offset", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.vtable, "vtable", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.virtuals, "virtuals", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.vptr_field, "vptr_field", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.inheritance, "inheritance", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.vtt_subvtt, "vtt_subvtt", nested_level + 1);
      create_edge (graph, node, anode);
      anode = create_tree_node (graph, tx.vtt_vptr, "vtt_vptr", nested_level + 1);
      create_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_STATEMENT_LIST:
      #define tx tn->stmt_list
      vcg_plugin_common.buf_print ("\nhead: 0x%x", (unsigned) tx.head);
      vcg_plugin_common.buf_print ("\ntail: 0x%x", (unsigned) tx.tail);
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_CONSTRUCTOR:
      #define tx tn->constructor
      vcg_plugin_common.buf_print ("\nelts: 0x%x", (unsigned) tx.elts);
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_OMP_CLAUSE:
      #define tx tn->omp_clause
      vcg_plugin_common.buf_print ("\ncode: 0x%x", tx.code);
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_OPTIMIZATION:
      #define tx tn->optimization
      vcg_plugin_common.buf_print ("\nopts: 0x%x", (unsigned) &tx.opts);
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      #undef tx
      break;
      
    case TS_TARGET_OPTION:
      #define tx tn->target_option
      vcg_plugin_common.buf_print ("\nopts: 0x%x", (unsigned) &tx.opts);
      label = vcg_plugin_common.buf_finish ();
      gdl_set_node_label (node, label);
      anode = create_common_node (graph, &tx.common, TS_COMMON,
                                  nested_level + 1);
      create_dashed_edge (graph, node, anode);
      #undef tx
      break;
    case LAST_TS_ENUM:
      abort ();
    }
  return node;
}

/* Dump tree NODE into the file FNAME.  */

static void
dump_tree_to_file (char *fname, tree node)
{
  gdl_graph *graph;

  tree_table = htab_create (32, htab_hash_pointer, htab_eq_pointer, NULL);

  graph = vcg_plugin_common.top_graph;
  create_tree_node (graph, node, "tree", 1);
  vcg_plugin_common.dump (fname);

  htab_delete (tree_table);
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

