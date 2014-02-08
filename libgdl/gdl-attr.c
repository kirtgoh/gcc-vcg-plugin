/* Functions to access gdl structure attributes and other fields.

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <error.h>
#include <libiberty.h>

#include "gdl.h"

/* Functions to get the attributes.  */

char *
gdl_get_node_bordercolor (gdl_node *node)
{
  return node->bordercolor;
}

int
gdl_get_node_borderwidth (gdl_node *node)
{
  return node->borderwidth;
}

char *
gdl_get_node_color (gdl_node *node)
{
  return node->color;
}

char *
gdl_get_node_label (gdl_node *node)
{
  return node->label;
}

char *
gdl_get_node_title (gdl_node *node)
{
  return node->title;
}

int
gdl_get_node_vertical_order (gdl_node *node)
{
  return node->vertical_order;
}

char *
gdl_get_edge_label (gdl_edge *edge)
{
  return edge->label;
}

char *
gdl_get_edge_linestyle (gdl_edge *edge)
{
  return edge->linestyle;
}

char *
gdl_get_edge_sourcename (gdl_edge *edge)
{
  return edge->sourcename;
}

char *
gdl_get_edge_targetname (gdl_edge *edge)
{
  return edge->targetname;
}

int
gdl_get_edge_thickness (gdl_edge *edge)
{
  return edge->thickness;
}

char *
gdl_get_graph_color (gdl_graph *graph)
{
  return graph->color;
}

int
gdl_get_graph_folding (gdl_graph *graph)
{
  return graph->folding;
}

char *
gdl_get_graph_label (gdl_graph *graph)
{
  return graph->label;
}

char *
gdl_get_graph_layout_algorithm (gdl_graph *graph)
{
  return graph->layout_algorithm;
}

char *
gdl_get_graph_near_edges (gdl_graph *graph)
{
  return graph->near_edges;
}

char *
gdl_get_graph_orientation (gdl_graph *graph)
{
  return graph->orientation;
}

char *
gdl_get_graph_port_sharing (gdl_graph *graph)
{
  return graph->port_sharing;
}

char *
gdl_get_graph_shape (gdl_graph *graph)
{
  return graph->shape;
}

char *
gdl_get_graph_splines (gdl_graph *graph)
{
  return graph->splines;
}

char *
gdl_get_graph_title (gdl_graph *graph)
{
  return graph->title;
}

int
gdl_get_graph_vertical_order (gdl_graph *graph)
{
  return graph->vertical_order;
}

int
gdl_get_graph_xspace (gdl_graph *graph)
{
  return graph->xspace;
}

int
gdl_get_graph_yspace (gdl_graph *graph)
{
  return graph->yspace;
}

int
gdl_get_graph_node_borderwidth (gdl_graph *graph)
{
  return graph->node_borderwidth;
}

char *
gdl_get_graph_node_color (gdl_graph *graph)
{
  return graph->node_color;
}

char *
gdl_get_graph_node_shape (gdl_graph *graph)
{
  return graph->node_shape;
}

char *
gdl_get_graph_edge_color (gdl_graph *graph)
{
  return graph->edge_color;
}

int
gdl_get_graph_edge_thickness (gdl_graph *graph)
{
  return graph->edge_thickness;
}

/* Functions to set the attributes.  */

void
gdl_set_node_bordercolor (gdl_node *node, char *value)
{
  node->set_p[GDL_NODE_ATTR_bordercolor] = 1;
  node->bordercolor = value;
}

void
gdl_set_node_borderwidth (gdl_node *node, int value)
{
  node->set_p[GDL_NODE_ATTR_borderwidth] = 1;
  node->borderwidth = value;
}

void
gdl_set_node_color (gdl_node *node, char *value)
{
  node->set_p[GDL_NODE_ATTR_color] = 1;
  node->color = value;
}

void
gdl_set_node_horizontal_order (gdl_node *node, int value)
{
  node->set_p[GDL_NODE_ATTR_horizontal_order] = 1;
  node->horizontal_order = value;
}

void
gdl_set_node_label (gdl_node *node, char *value)
{
  node->set_p[GDL_NODE_ATTR_label] = 1;
  node->label = value;
}

void
gdl_set_node_title (gdl_node *node, char *value)
{
  node->set_p[GDL_NODE_ATTR_title] = 1;
  node->title = value;
}

void
gdl_set_node_vertical_order (gdl_node *node, int value)
{
  node->set_p[GDL_NODE_ATTR_vertical_order] = 1;
  node->vertical_order = value;
}

void
gdl_set_edge_label (gdl_edge *edge, char *value)
{
  edge->set_p[GDL_EDGE_ATTR_label] = 1;
  edge->label = value;
}

void
gdl_set_edge_linestyle (gdl_edge *edge, char *value)
{
  edge->set_p[GDL_EDGE_ATTR_linestyle] = 1;
  edge->linestyle = value;
}

void
gdl_set_edge_sourcename (gdl_edge *edge, char *value)
{
  edge->set_p[GDL_EDGE_ATTR_sourcename] = 1;
  edge->sourcename = value;
}

void
gdl_set_edge_targetname (gdl_edge *edge, char *value)
{
  edge->set_p[GDL_EDGE_ATTR_targetname] = 1;
  edge->targetname = value;
}

void
gdl_set_edge_thickness (gdl_edge *edge, int value)
{
  edge->set_p[GDL_EDGE_ATTR_thickness] = 1;
  edge->thickness = value;
}

void
gdl_set_graph_color (gdl_graph *graph, char *value)
{
  graph->set_p[GDL_GRAPH_ATTR_color] = 1;
  graph->color = value;
}

void
gdl_set_graph_colorentry (gdl_graph *graph, int id, int r, int g, int b)
{
  assert (id >= 0 && id < 256);

  graph->set_p[GDL_GRAPH_ATTR_colorentry] = 1;
  graph->colorentry_set_p[id] = 1;
  graph->colorentry[id][0] = r;
  graph->colorentry[id][1] = g;
  graph->colorentry[id][2] = b;
}

void
gdl_set_graph_folding (gdl_graph *graph, int value)
{
  graph->set_p[GDL_GRAPH_ATTR_folding] = 1;
  graph->folding = value;
}

void
gdl_set_graph_label (gdl_graph *graph, char *value)
{
  graph->set_p[GDL_GRAPH_ATTR_label] = 1;
  graph->label = value;
}

void
gdl_set_graph_layout_algorithm (gdl_graph *graph, char *value)
{
  graph->set_p[GDL_GRAPH_ATTR_layout_algorithm] = 1;
  graph->layout_algorithm = value;
}

void
gdl_set_graph_near_edges (gdl_graph *graph, char *value)
{
  graph->set_p[GDL_GRAPH_ATTR_near_edges] = 1;
  graph->near_edges = value;
}

void
gdl_set_graph_orientation (gdl_graph *graph, char *value)
{
  graph->set_p[GDL_GRAPH_ATTR_orientation] = 1;
  graph->orientation = value;
}

void
gdl_set_graph_port_sharing (gdl_graph *graph, char *value)
{
  graph->set_p[GDL_GRAPH_ATTR_port_sharing] = 1;
  graph->port_sharing = value;
}

void
gdl_set_graph_shape (gdl_graph *graph, char *value)
{
  graph->set_p[GDL_GRAPH_ATTR_shape] = 1;
  graph->shape = value;
}

void
gdl_set_graph_splines (gdl_graph *graph, char *value)
{
  graph->set_p[GDL_GRAPH_ATTR_splines] = 1;
  graph->splines = value;
}

void
gdl_set_graph_title (gdl_graph *graph, char *value)
{
  graph->set_p[GDL_GRAPH_ATTR_title] = 1;
  graph->title = value;
}

void
gdl_set_graph_vertical_order (gdl_graph *graph, int value)
{
  graph->set_p[GDL_GRAPH_ATTR_vertical_order] = 1;
  graph->vertical_order = value;
}

void
gdl_set_graph_xspace (gdl_graph *graph, int value)
{
  graph->set_p[GDL_GRAPH_ATTR_xspace] = 1;
  graph->xspace = value;
}

void
gdl_set_graph_yspace (gdl_graph *graph, int value)
{
  graph->set_p[GDL_GRAPH_ATTR_yspace] = 1;
  graph->yspace = value;
}

void
gdl_set_graph_node_borderwidth (gdl_graph *graph, int value)
{
  graph->set_p[GDL_GRAPH_ATTR_node_borderwidth] = 1;
  graph->node_borderwidth = value;
}

void
gdl_set_graph_node_color (gdl_graph *graph, char *value)
{
  graph->set_p[GDL_GRAPH_ATTR_node_color] = 1;
  graph->node_color = value;
}

void
gdl_set_graph_node_shape (gdl_graph *graph, char *value)
{
  graph->set_p[GDL_GRAPH_ATTR_node_shape] = 1;
  graph->node_shape = value;
}

void
gdl_set_graph_node_textcolor (gdl_graph *graph, char *value)
{
  graph->set_p[GDL_GRAPH_ATTR_node_textcolor] = 1;
  graph->node_textcolor = value;
}

void
gdl_set_graph_edge_color (gdl_graph *graph, char *value)
{
  graph->set_p[GDL_GRAPH_ATTR_edge_color] = 1;
  graph->edge_color = value;
}

void
gdl_set_graph_edge_thickness (gdl_graph *graph, int value)
{
  graph->set_p[GDL_GRAPH_ATTR_edge_thickness] = 1;
  graph->edge_thickness = value;
}

/* Other functions to access structure fields.  */

gdl_node *
gdl_get_graph_node (gdl_graph *graph)
{
  return graph->node;
}

gdl_edge *
gdl_get_graph_edge (gdl_graph *graph)
{
  return graph->edge;
}

gdl_graph *
gdl_get_graph_subgraph (gdl_graph *graph)
{
  return graph->subgraph;
}

gdl_graph *
gdl_get_graph_parent (gdl_graph *graph)
{
  return graph->parent;
}

gdl_graph *
gdl_get_node_parent (gdl_node *node)
{
  return node->parent;
}

gdl_edge_type 
gdl_get_edge_type (gdl_edge *edge)
{
  return edge->type;
}

void
gdl_set_edge_type (gdl_edge *edge, gdl_edge_type type)
{
  edge->type = type;
}

