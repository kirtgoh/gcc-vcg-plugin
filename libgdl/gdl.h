/* The structures according to graph description language (gdl).

   Copyright (C) 2009, 2010, 2011 Mingjie Xing, mingjie.xing@gmail.com. 

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

#ifndef GDL_H
#define GDL_H

#include <assert.h>

/* shape */

#define GDL_BOX			"box"
#define GDL_RHOMB		"rhomb"
#define GDL_ELLIPSE		"ellipse"
#define GDL_TRIANGLE		"triangle"

/* color */

#define GDL_COLOR_BLACK		"black"
#define GDL_COLOR_BLUE		"blue"
#define GDL_COLOR_LIGHTBLUE	"lightblue"
#define GDL_COLOR_RED		"red"
#define GDL_COLOR_GREEN		"green"
#define GDL_COLOR_YELLOW	"yellow"
#define GDL_COLOR_WHITE		"white"
#define GDL_COLOR_LIGHTGREY	"lightgrey"

/* line style */

#define GDL_CONTINUOUS		"continuous"
#define GDL_DASHED		"dashed"
#define GDL_DOTTED		"dotted"
#define GDL_INVISIBLE		"invisible"

/* layout algorithm */

#define GDL_MAX_DEPTH		"max_depth"
#define GDL_TREE		"tree"

typedef struct gdl_node gdl_node;
typedef struct gdl_edge gdl_edge;
typedef struct gdl_graph gdl_graph;

enum gdl_node_attr
{
  GDL_NODE_ATTR_bordercolor,
  GDL_NODE_ATTR_borderwidth,
  GDL_NODE_ATTR_color,
  GDL_NODE_ATTR_horizontal_order,
  GDL_NODE_ATTR_label,
  GDL_NODE_ATTR_title,
  GDL_NODE_ATTR_vertical_order,
  GDL_NODE_ATTR_MAX
};

struct gdl_node 
{
  char *bordercolor;
  int borderwidth;
  char *color;
  int horizontal_order;
  char *label;
  char *title;
  int vertical_order;
  /* The value is 1 if the attribute is set.  */
  int set_p[GDL_NODE_ATTR_MAX];

  gdl_node *next;
  /* The graph who it belongs to.  */
  gdl_graph *parent;
};

typedef enum
{
  GDL_EDGE,
  GDL_BACKEDGE,
  GDL_NEAREDGE,
  GDL_LEFTNEAREDGE,
  GDL_RIGHTNEAREDGE,
  GDL_BENTNEAREDGE,
  GDL_LEFTBENTNEAREDGE,
  GDL_RIGHTBENTNEAREDGE 
} gdl_edge_type;

enum gdl_edge_attr
{
  GDL_EDGE_ATTR_label,
  GDL_EDGE_ATTR_linestyle,
  GDL_EDGE_ATTR_sourcename,
  GDL_EDGE_ATTR_targetname,
  GDL_EDGE_ATTR_thickness,
  GDL_EDGE_ATTR_MAX
};

struct gdl_edge
{
  char *label;
  char *linestyle;
  char *sourcename;
  char *targetname;
  int thickness;
  /* The value is 1 if the attribute is set.  */
  int set_p[GDL_EDGE_ATTR_MAX];

  gdl_edge_type type;
  gdl_edge *next;
};

enum gdl_graph_attr
{
  GDL_GRAPH_ATTR_color,
  GDL_GRAPH_ATTR_colorentry,
  GDL_GRAPH_ATTR_folding,
  GDL_GRAPH_ATTR_label,
  GDL_GRAPH_ATTR_layout_algorithm,
  GDL_GRAPH_ATTR_near_edges,
  GDL_GRAPH_ATTR_node_alignment,
  GDL_GRAPH_ATTR_orientation,
  GDL_GRAPH_ATTR_port_sharing,
  GDL_GRAPH_ATTR_shape,
  GDL_GRAPH_ATTR_splines,
  GDL_GRAPH_ATTR_title,
  GDL_GRAPH_ATTR_vertical_order,
  GDL_GRAPH_ATTR_xspace,
  GDL_GRAPH_ATTR_yspace,

  GDL_GRAPH_ATTR_node_borderwidth,
  GDL_GRAPH_ATTR_node_color,
  GDL_GRAPH_ATTR_node_shape,
  GDL_GRAPH_ATTR_node_textcolor,

  GDL_GRAPH_ATTR_edge_color,
  GDL_GRAPH_ATTR_edge_thickness,
  GDL_GRAPH_ATTR_MAX
};

struct gdl_graph
{
  char *color;
  int colorentry[256][3];
  int folding;
  char *label;
  char *layout_algorithm;
  char *near_edges;
  char *node_alignment;
  char *orientation;
  char *port_sharing;
  char *shape;
  char *splines;
  char *title;
  int vertical_order;
  int xspace;
  int yspace;

  int node_borderwidth;
  char *node_color;
  char *node_shape;
  char *node_textcolor;

  char *edge_color;
  int edge_thickness;
  /* The value is 1 if the attribute is set.  */
  int set_p[GDL_GRAPH_ATTR_MAX];
  int colorentry_set_p[256];

  /* nodes or subgraphs */
  int node_num;
  int subgraph_num;
  int edge_num;
  gdl_node *node;
  gdl_node *last_node;
  gdl_graph *subgraph;
  gdl_graph *last_subgraph;
  gdl_edge *edge;
  gdl_edge *last_edge;
  gdl_graph *next;
  /* The graph who it belongs to.  */
  gdl_graph *parent;
};

extern char *gdl_get_node_bordercolor (gdl_node *node);
extern int gdl_get_node_borderwidth (gdl_node *node);
extern char *gdl_get_node_color (gdl_node *node);
extern char *gdl_get_node_label (gdl_node *node);
extern char *gdl_get_node_title (gdl_node *node);
extern int gdl_get_node_vertical_order (gdl_node *node);
extern char *gdl_get_edge_label (gdl_edge *edge);
extern char *gdl_get_edge_linestyle (gdl_edge *edge);
extern char *gdl_get_edge_sourcename (gdl_edge *edge);
extern char *gdl_get_edge_targetname (gdl_edge *edge);
extern int gdl_get_edge_thickness (gdl_edge *edge);
extern char *gdl_get_graph_color (gdl_graph *graph);
extern int gdl_get_graph_folding (gdl_graph *graph);
extern char *gdl_get_graph_label (gdl_graph *graph);
extern char *gdl_get_graph_layout_algorithm (gdl_graph *graph);
extern char *gdl_get_graph_near_edges (gdl_graph *graph);
extern char *gdl_get_graph_orientation (gdl_graph *graph);
extern char *gdl_get_graph_port_sharing (gdl_graph *graph);
extern char *gdl_get_graph_shape (gdl_graph *graph);
extern char *gdl_get_graph_splines (gdl_graph *graph);
extern char *gdl_get_graph_title (gdl_graph *graph);
extern int gdl_get_graph_vertical_order (gdl_graph *graph);
extern int gdl_get_graph_xspace (gdl_graph *graph);
extern int gdl_get_graph_yspace (gdl_graph *graph);
extern int gdl_get_graph_node_borderwidth (gdl_graph *graph);
extern char *gdl_get_graph_node_color (gdl_graph *graph);
extern char *gdl_get_graph_node_shape (gdl_graph *graph);
extern char *gdl_get_graph_edge_color (gdl_graph *graph);
extern int gdl_get_graph_edge_thickness (gdl_graph *graph);
extern gdl_node *gdl_get_graph_node (gdl_graph *graph);
extern gdl_edge *gdl_get_graph_edge (gdl_graph *graph);
extern gdl_graph *gdl_get_graph_subgraph (gdl_graph *graph);
extern gdl_graph *gdl_get_graph_parent (gdl_graph *graph);
extern gdl_graph *gdl_get_node_parent (gdl_node *node);
extern gdl_edge_type gdl_get_edge_type (gdl_edge *edge);

extern void gdl_set_node_bordercolor (gdl_node *node, char *value);
extern void gdl_set_node_borderwidth (gdl_node *node, int value);
extern void gdl_set_node_color (gdl_node *node, char *value);
extern void gdl_set_node_horizontal_order (gdl_node *node, int value);
extern void gdl_set_node_label (gdl_node *node, char *value);
extern void gdl_set_node_title (gdl_node *node, char *value);
extern void gdl_set_node_vertical_order (gdl_node *node, int value);
extern void gdl_set_edge_label (gdl_edge *edge, char *value);
extern void gdl_set_edge_linestyle (gdl_edge *edge, char *value);
extern void gdl_set_edge_sourcename (gdl_edge *edge, char *value);
extern void gdl_set_edge_targetname (gdl_edge *edge, char *value);
extern void gdl_set_edge_thickness (gdl_edge *edge, int value);
extern void gdl_set_graph_color (gdl_graph *graph, char *value);
extern void gdl_set_graph_colorentry (gdl_graph *graph, int id,
                                      int r, int g, int b);
extern void gdl_set_graph_folding (gdl_graph *graph, int value);
extern void gdl_set_graph_label (gdl_graph *graph, char *value);
extern void gdl_set_graph_layout_algorithm (gdl_graph *graph, char *value);
extern void gdl_set_graph_near_edges (gdl_graph *graph, char *value);
extern void gdl_set_graph_node_alignment (gdl_graph *graph, char *value);
extern void gdl_set_graph_orientation (gdl_graph *graph, char *value);
extern void gdl_set_graph_port_sharing (gdl_graph *graph, char *value);
extern void gdl_set_graph_shape (gdl_graph *graph, char *value);
extern void gdl_set_graph_splines (gdl_graph *graph, char *value);
extern void gdl_set_graph_title (gdl_graph *graph, char *value);
extern void gdl_set_graph_vertical_order (gdl_graph *graph, int value);
extern void gdl_set_graph_xspace (gdl_graph *graph, int value);
extern void gdl_set_graph_yspace (gdl_graph *graph, int value);
extern void gdl_set_graph_node_borderwidth (gdl_graph *graph, int value);
extern void gdl_set_graph_node_color (gdl_graph *graph, char *value);
extern void gdl_set_graph_node_shape (gdl_graph *graph, char *value);
extern void gdl_set_graph_node_textcolor (gdl_graph *graph, char *value);
extern void gdl_set_graph_edge_color (gdl_graph *graph, char *value);
extern void gdl_set_graph_edge_thickness (gdl_graph *graph, int value);
extern void gdl_set_edge_type (gdl_edge *edge, gdl_edge_type type);

extern gdl_graph *gdl_new_graph (char *title); 
extern gdl_node *gdl_new_node (char *title);
extern gdl_node *gdl_new_graph_node (gdl_graph *graph, char *title);
extern gdl_edge *gdl_new_edge (char *source, char *target);
extern gdl_edge *gdl_new_graph_edge (gdl_graph *graph,
                                     char *source, char *target);
extern gdl_graph *gdl_new_graph (char *title); 
extern gdl_graph *gdl_new_graph_subgraph (gdl_graph *graph, char *title); 

extern void gdl_free_node (gdl_node *node);
extern void gdl_free_edge (gdl_edge *edge);
extern void gdl_free_graph (gdl_graph *graph);

extern void gdl_add_node (gdl_graph *graph, gdl_node *node); 
extern void gdl_add_edge (gdl_graph *graph, gdl_edge *edge);
extern void gdl_add_subgraph (gdl_graph *graph, gdl_graph *subgraph);

extern gdl_node *gdl_find_node (gdl_graph *graph, char *title);
extern gdl_edge *gdl_find_edge (gdl_graph *graph, char *source, char *target);
extern gdl_graph *gdl_find_subgraph (gdl_graph *graph, char *title);

extern void gdl_dump_node (FILE *fout, gdl_node *node);
extern void gdl_dump_edge (FILE *fout, gdl_edge *edge);
extern void gdl_dump_graph (FILE *fout, gdl_graph *graph);

#endif
