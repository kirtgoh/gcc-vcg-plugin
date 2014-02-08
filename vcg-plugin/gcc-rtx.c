/* Dump or view gcc rtx.

   Copyright (C) 2011 Mingjie Xing, mingjie.xing@gmail.com.

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

/* Temp file stream, used to get the rtx dump. */
static FILE *tmp_stream;
static char *tmp_buf;
static size_t tmp_buf_size;

static void
buf_print_rtx (const_rtx x)
{
  rewind (tmp_stream);
  print_rtl_single (tmp_stream, x);
  fflush(tmp_stream);
  vcg_plugin_common.buf_print ("%s", tmp_buf);

  return;
}

static gdl_node *
create_rtx_node (gdl_graph *graph, const_rtx x)
{
  gdl_node *node, *node_x;
  char *label;
  enum rtx_code code;
  const char *format_ptr;
  int i;
  const char *str;
  int value;
  rtx sub;
  basic_block bb;
  tree tn;

  if (x == 0)
    return NULL;

  node = gdl_new_graph_node (graph, NULL);

  code = GET_CODE (x);
  format_ptr = GET_RTX_FORMAT (code);
  
  buf_print_rtx (x);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("addr: 0x%x\n", (unsigned) x);
  vcg_plugin_common.buf_print ("format: %s\n", format_ptr);
  vcg_plugin_common.buf_print ("----------\n");
  vcg_plugin_common.buf_print ("code: %d\n", x->code);
  vcg_plugin_common.buf_print ("mode: %d\n", x->mode);
  vcg_plugin_common.buf_print ("jump: %d\n", x->jump);
  vcg_plugin_common.buf_print ("call: %d\n", x->call);
  vcg_plugin_common.buf_print ("unchanging: %d\n", x->unchanging);
  vcg_plugin_common.buf_print ("volatil: %d\n", x->volatil);
  vcg_plugin_common.buf_print ("in_struct: %d\n", x->in_struct);
  vcg_plugin_common.buf_print ("used: %d\n", x->used);
  vcg_plugin_common.buf_print ("frame_related: %d\n", x->frame_related);
  vcg_plugin_common.buf_print ("return_val: %d\n", x->return_val);
  vcg_plugin_common.buf_print ("u: ");
  vcg_plugin_common.buf_print ("fld[%d]", strlen (format_ptr));
  label = vcg_plugin_common.buf_finish ();
  gdl_set_node_label (node, label);
  
  for (i = 0; i < GET_RTX_LENGTH (code); i++)
    switch (*format_ptr++)
      {
      case 'T':
        str = XTMPL (x, i);
        goto string;
      case 'S':
      case 's':
        str = XSTR (x, i);
      string:
        node_x = gdl_new_graph_node (graph, NULL);
        if (str == 0)
          str = "";
        gdl_set_node_label (node_x, (char *) str);
        gdl_set_node_horizontal_order (node_x, i + 1);
        gdl_new_graph_edge (graph, gdl_get_node_title (node),
                            gdl_get_node_title (node_x));
        break;

      case '0':
        node_x = gdl_new_graph_node (graph, NULL);
        vcg_plugin_common.buf_print ("fld[%d]\n", i);
        vcg_plugin_common.buf_print ("format: 0\n");
        if (i == 1 && REG_P (x))
          {
            vcg_plugin_common.buf_print ("rt_uint: %d\n", ORIGINAL_REGNO (x));
            vcg_plugin_common.buf_print ("----------\n");
            vcg_plugin_common.buf_print ("ORIGINAL_REGNO");
            
          }
        else if (i == 1 && code == SYMBOL_REF)
          {
            vcg_plugin_common.buf_print ("rt_int: %d\n", SYMBOL_REF_FLAGS (x));
            vcg_plugin_common.buf_print ("----------\n");
            vcg_plugin_common.buf_print ("SYMBOL_REF_FLAGS");
          }
        else if (i == 2 && code == SYMBOL_REF)
          {
            vcg_plugin_common.buf_print ("rt_tree: 0x%x\n", SYMBOL_REF_DECL (x));
            vcg_plugin_common.buf_print ("----------\n");
            vcg_plugin_common.buf_print ("SYMBOL_REF_DECL");
          }
        else if (i == 4 && NOTE_P (x))
          {
            switch (NOTE_KIND (x))
              {
              case NOTE_INSN_EH_REGION_BEG:
              case NOTE_INSN_EH_REGION_END:
                vcg_plugin_common.buf_print ("rt_int: %d\n", NOTE_EH_HANDLER (x));
                vcg_plugin_common.buf_print ("----------\n");
                vcg_plugin_common.buf_print ("NOTE_EH_HANDLER");
                break;

              case NOTE_INSN_BLOCK_BEG:
              case NOTE_INSN_BLOCK_END:
                vcg_plugin_common.buf_print ("rt_tree: 0x%x\n", NOTE_BLOCK (x));
                vcg_plugin_common.buf_print ("----------\n");
                vcg_plugin_common.buf_print ("NOTE_BLOCK");
                break;

              case NOTE_INSN_BASIC_BLOCK:
              case NOTE_INSN_SWITCH_TEXT_SECTIONS:
                bb = NOTE_BASIC_BLOCK (x);
                vcg_plugin_common.buf_print ("rt_bb: 0x%x\n", bb);
                vcg_plugin_common.buf_print ("----------\n");
                if (bb)
                  vcg_plugin_common.buf_print ("bb index: %d\n", bb->index);
                vcg_plugin_common.buf_print ("NOTE_BASIC_BLOCK");
                break;
          
              case NOTE_INSN_DELETED_LABEL:
                vcg_plugin_common.buf_print ("rt_str: %s\n",
                                             NOTE_DELETED_LABEL_NAME (x));
                vcg_plugin_common.buf_print ("----------\n");
                vcg_plugin_common.buf_print ("NOTE_DELETED_LABEL_NAME");
                break;
       
              case NOTE_INSN_VAR_LOCATION:
                vcg_plugin_common.buf_print ("rt_rtx: 0x%x\n",
                                             NOTE_VAR_LOCATION (x));
                vcg_plugin_common.buf_print ("----------\n");
                vcg_plugin_common.buf_print ("NOTE_VAR_LOCATION");
                break;
       
              }
          }
        else if (i == 8 && JUMP_P (x) && JUMP_LABEL (x) != NULL)
          {
            sub = JUMP_LABEL (x);
            vcg_plugin_common.buf_print ("rt_rtx: 0x%x\n", sub);
            vcg_plugin_common.buf_print ("----------\n");
            vcg_plugin_common.buf_print ("insn id: %d", INSN_UID (sub));
          }
        else if (i == 0 && code == VALUE)
          {
            //cselib_val *val = CSELIB_VAL_PTR (x);
            vcg_plugin_common.buf_print ("rt_cselib: 0x%x\n", CSELIB_VAL_PTR (x));
            vcg_plugin_common.buf_print ("----------\n");
            //vcg_plugin_common.buf_print ("%u:%u", val->uid, val->hash);
          }
        else if (i == 0 && code == DEBUG_EXPR)
          {
            tn = DEBUG_EXPR_TREE_DECL (x);
            vcg_plugin_common.buf_print ("rt_tree: 0x%x\n", tn);
            vcg_plugin_common.buf_print ("----------\n");
            vcg_plugin_common.buf_print ("DEBUG_TEMP_UID: %i", DEBUG_TEMP_UID (tn));
          }
        /*
        else if (i == 0 && code == ENTRY_VALUE)
          {
            sub = ENTRY_VALUE_EXP (x);
            vcg_plugin_common.buf_print ("rt_rtx: 0x%x\n", sub);
            vcg_plugin_common.buf_print ("----------\n");
            buf_print_rtx (x);
          }
        */
       
        label = vcg_plugin_common.buf_finish ();
        gdl_set_node_label (node_x, label);
        gdl_set_node_horizontal_order (node_x, i + 1);
        gdl_new_graph_edge (graph, gdl_get_node_title (node),
                            gdl_get_node_title (node_x));

        break;

      case 'B':
        node_x = gdl_new_graph_node (graph, NULL);
        bb = XBBDEF (x, i);
        vcg_plugin_common.buf_print ("fld[%d]\n", i);
        vcg_plugin_common.buf_print ("format: B\n");
        vcg_plugin_common.buf_print ("rt_bb: 0x%x\n", bb);
        if (bb)
          {
            vcg_plugin_common.buf_print ("----------\n");
            vcg_plugin_common.buf_print ("bb index: %d", bb->index);
          }
        label = vcg_plugin_common.buf_finish ();
        gdl_set_node_label (node_x, label);
        gdl_set_node_horizontal_order (node_x, i + 1);
        gdl_new_graph_edge (graph, gdl_get_node_title (node),
                            gdl_get_node_title (node_x));

        break;

      case 'i':
        node_x = gdl_new_graph_node (graph, NULL);
        value = XINT (x, i);
        vcg_plugin_common.buf_print ("fld[%d]\n", i);
        vcg_plugin_common.buf_print ("format: i\n");
        vcg_plugin_common.buf_print ("rt_int: %d", value);
        label = vcg_plugin_common.buf_finish ();
        gdl_set_node_label (node_x, label);
        gdl_set_node_horizontal_order (node_x, i + 1);
        gdl_new_graph_edge (graph, gdl_get_node_title (node),
                            gdl_get_node_title (node_x));
        break;

      case 'e':
        node_x = gdl_new_graph_node (graph, NULL);
        sub = XEXP (x, i);
        vcg_plugin_common.buf_print ("fld[%d]\n", i);
        vcg_plugin_common.buf_print ("format: e\n");
        vcg_plugin_common.buf_print ("rt_rtx: 0x%x\n", sub);
        vcg_plugin_common.buf_print ("----------\n");
        buf_print_rtx (x);
        label = vcg_plugin_common.buf_finish ();
        gdl_set_node_label (node_x, label);
        gdl_set_node_horizontal_order (node_x, i + 1);
        gdl_new_graph_edge (graph, gdl_get_node_title (node),
                            gdl_get_node_title (node_x));
        break;

      case 'n':
        node_x = gdl_new_graph_node (graph, NULL);
        value = XINT (x, i);
        vcg_plugin_common.buf_print ("fld[%d]\n", i);
        vcg_plugin_common.buf_print ("format: n\n");
        vcg_plugin_common.buf_print ("rt_int: %d\n", value);
        vcg_plugin_common.buf_print ("----------\n");
        vcg_plugin_common.buf_print ("%s", GET_NOTE_INSN_NAME (value));
        label = vcg_plugin_common.buf_finish ();
        gdl_set_node_label (node_x, label);
        gdl_set_node_horizontal_order (node_x, i + 1);
        gdl_new_graph_edge (graph, gdl_get_node_title (node),
                            gdl_get_node_title (node_x));
        
        break;

      case 'u':
        node_x = gdl_new_graph_node (graph, NULL);
        sub = XEXP (x, i);
        vcg_plugin_common.buf_print ("fld[%d]\n", i);
        vcg_plugin_common.buf_print ("format: u\n");
        vcg_plugin_common.buf_print ("rt_rtx: 0x%x", (unsigned) sub);
        if (sub)
          {
            vcg_plugin_common.buf_print ("\n");
            vcg_plugin_common.buf_print ("----------\n");
            vcg_plugin_common.buf_print ("id: %d", INSN_UID (sub));
          }

        label = vcg_plugin_common.buf_finish ();
        gdl_set_node_label (node_x, label);
        gdl_set_node_horizontal_order (node_x, i + 1);
        gdl_new_graph_edge (graph, gdl_get_node_title (node),
                            gdl_get_node_title (node_x));
        break;

      }

  return node;
}

/* Dump rtx X into the file FNAME.  */

static void
dump_rtx_to_file (char *fname, const_rtx x)
{
  gdl_graph *graph;

  tmp_stream = open_memstream (&tmp_buf, &tmp_buf_size);

  graph = vcg_plugin_common.top_graph;
  create_rtx_node (graph, x);
  vcg_plugin_common.dump (fname, graph);

  fclose (tmp_stream);
  free (tmp_buf);
}

/* Public function to dump a gcc rtx X.  */

void
vcg_plugin_dump_rtx (const_rtx x)
{
  char *fname = "dump-rtx.vcg";

  vcg_plugin_common.init ();

  dump_rtx_to_file (fname, x);

  vcg_plugin_common.finish ();
}

/* Public function to view a gcc rtx X.  */

void
vcg_plugin_view_rtx (const_rtx x)
{
  char *fname = vcg_plugin_common.temp_file_name;

  vcg_plugin_common.init ();

  dump_rtx_to_file (fname, x);
  vcg_plugin_common.show (fname);

  vcg_plugin_common.finish ();
}

