/* Graph description languange.

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

/* Used to create a node/graph with no title.  */
static unsigned anonymous_title_id = 0;
static char buf[512];

/* Create a node for a given TITLE.  */

gdl_node *
gdl_new_node (char *title)
{
  gdl_node *node;

  node = XNEW (gdl_node);
  memset (node->set_p, 0, GDL_NODE_ATTR_MAX * sizeof (int));

  /* Duplicate the string.  */
  if (title == NULL)
    {
      sprintf (buf, "anonymous.%d", anonymous_title_id++);
      title = buf;
    }
  gdl_set_node_title (node, strdup (title));

  node->next = NULL;
  node->parent = NULL;
  
  return node;
}

gdl_node *
gdl_new_graph_node (gdl_graph *graph, char *title)
{
  gdl_node *node;

  assert (graph);

  node = gdl_new_node (title);
  gdl_add_node (graph, node);

  return node;
}

/* Create an edge for a given SOURCE and TARGET.  */

gdl_edge *
gdl_new_edge (char *source, char *target)
{
  gdl_edge *edge;

  edge = XNEW (gdl_edge);
  memset (edge->set_p, 0, GDL_EDGE_ATTR_MAX * sizeof (int));

  /* Duplicate the string.  */
  gdl_set_edge_sourcename (edge, strdup (source));
  gdl_set_edge_targetname (edge, strdup (target));

  edge->type = GDL_EDGE;
  edge->next = NULL;
  
  return edge;
}

gdl_edge *
gdl_new_graph_edge (gdl_graph *graph, char *source, char *target)
{
  gdl_edge *edge;

  assert (graph);

  edge = gdl_new_edge (source, target);
  gdl_add_edge (graph, edge);

  return edge;
}

/* Create a graph for a given TITLE.  */

gdl_graph *
gdl_new_graph (char *title)
{
  gdl_graph *graph;

  graph = XNEW (gdl_graph);
  memset (graph->set_p, 0, GDL_GRAPH_ATTR_MAX * sizeof (int));
  memset (graph->colorentry_set_p, 0, 256 * sizeof (int));

  /* Duplicate the string.  */
  if (title == NULL)
    {
      sprintf (buf, "anonymous.%d", anonymous_title_id++);
      title = buf;
    }
  gdl_set_graph_title (graph, strdup (title));

  graph->node = NULL;
  graph->last_node = NULL;
  graph->subgraph = NULL;
  graph->last_subgraph = NULL;
  graph->edge = NULL;
  graph->last_edge = NULL;
  graph->next = NULL;
  graph->parent = NULL;

  return graph;
}

gdl_graph *
gdl_new_graph_subgraph (gdl_graph *graph, char *title)
{
  gdl_graph *subgraph;

  assert (graph);

  subgraph = gdl_new_graph (title);
  gdl_add_subgraph (graph, subgraph);

  return subgraph;
}

/* Free the NODE.  */

void
gdl_free_node (gdl_node *node)
{
  free (node->title);
  free (node);
}

/* Free the EDGE.  */

void
gdl_free_edge (gdl_edge *edge)
{
  free (edge->sourcename);
  free (edge->targetname);
  free (edge);
}

/* Free the GRAPH.  */

void
gdl_free_graph (gdl_graph *graph)
{
  gdl_graph *subgraphs, *subgraph, *next_subgraph;
  gdl_node *nodes, *node, *next_node;
  gdl_edge *edges, *edge, *next_edge;

  /* Free the nodes.  */
  nodes = gdl_get_graph_node (graph);
  for (node = nodes; node != NULL;)
    {
      next_node = node->next;
      gdl_free_node (node);
      node = next_node;
    }

  /* Free the edges.  */
  edges = gdl_get_graph_edge (graph);
  for (edge = edges; edge != NULL;)
    {
      next_edge = edge->next;
      gdl_free_edge (edge);
      edge = next_edge;
    }

  /* Free the subgraphs.  */
  subgraphs = gdl_get_graph_subgraph (graph);
  for (subgraph = subgraphs; subgraph != NULL;)
    {
      next_subgraph = subgraph->next;
      gdl_free_graph (subgraph);
      subgraph = next_subgraph;
    }

  /* Free the graph.  */
  free (graph->title);
  free (graph);
}

/* Add NODE into GRAPH.  */

void 
gdl_add_node (gdl_graph *graph, gdl_node *node)
{
  if (graph->node == NULL)
    {
      graph->node = node;
      graph->last_node = node;
    }
  else
    {
      graph->last_node->next = node;
      graph->last_node = node;
    }
  node->parent = graph;
}

/* Add EDGE into GRAPH.  */

void 
gdl_add_edge (gdl_graph *graph, gdl_edge *edge)
{
  if (graph->edge == NULL)
    {
      graph->edge = edge;
      graph->last_edge = edge;
    }
  else
    {
      graph->last_edge->next = edge;
      graph->last_edge = edge;
    }
}

/* Add SUBGRAPH into GRAPH.  */

void 
gdl_add_subgraph (gdl_graph *graph, gdl_graph *subgraph)
{
  if (graph->subgraph == NULL)
    {
      graph->subgraph = subgraph;
      graph->last_subgraph = subgraph;
    }
  else
    {
      graph->last_subgraph->next = subgraph;
      graph->last_subgraph = subgraph;
    }
  subgraph->parent = graph;
}

/* Find the node in GRAPH for a given TITLE.  */

gdl_node *
gdl_find_node (gdl_graph *graph, char *title)
{
  gdl_node *nodes, *node;

  nodes = gdl_get_graph_node (graph); 
  for (node = nodes; node; node = node->next)
    if (!strcmp (gdl_get_node_title (node), title))
      return node;

  return NULL;
}

/* Find the edge in GRAPH for a given SOURCE and TARGET.  */

gdl_edge *
gdl_find_edge (gdl_graph *graph, char *source, char *target)
{
  gdl_edge *edges, *edge;
  char *srcname, *destname;

  edges = gdl_get_graph_edge (graph); 
  for (edge = edges; edge; edge = edge->next)
    {
      srcname = gdl_get_edge_sourcename (edge);
      destname = gdl_get_edge_targetname (edge);
      if (!strcmp (srcname, source) && !strcmp (destname, target))
        return edge;
    }

  return NULL;
}

/* Find the subgraph in GRAPH for a given TITLE.  */

gdl_graph *
gdl_find_subgraph (gdl_graph *graph, char *title)
{
  gdl_graph *subgraphs, *subgraph;
  
  subgraphs = gdl_get_graph_subgraph (graph);
  for (subgraph = subgraphs; subgraph; subgraph = subgraph->next)
    if (!strcmp (gdl_get_graph_title (subgraph), title))
      return subgraph;

  return NULL;
}

/* Print the string into the file and add a '\' before each '"'.  */

static inline void
print_string (FILE *fout, char *str)
{
  int i;

  for (i = 0; i < strlen (str); i++)
    {
      if (str[i] == '"')
        fprintf (fout,  "\\");
      fprintf (fout, "%c", str[i]);
    }
}

/* Dump NODE into the file.  */

void
gdl_dump_node (FILE *fout, gdl_node *node)
{
  fputs ("node: {\n", fout);

  /* Dump the attributes.  */

  if (node->set_p[GDL_NODE_ATTR_bordercolor])
    {
      fprintf (fout, "bordercolor: %s\n", node->bordercolor);
    }

  if (node->set_p[GDL_NODE_ATTR_borderwidth])
    {
      fprintf (fout, "borderwidth: %d\n", node->borderwidth);
    }

  if (node->set_p[GDL_NODE_ATTR_color])
    {
      fprintf (fout, "color: %s\n", node->color);
    }

  if (node->set_p[GDL_NODE_ATTR_horizontal_order])
    {
      fprintf (fout, "horizontal_order: %d\n", node->horizontal_order);
    }

  if (node->set_p[GDL_NODE_ATTR_label])
    {
      fprintf (fout, "label: \"");
      print_string (fout, node->label);
      fprintf (fout, "\"\n");
    }

  if (node->set_p[GDL_NODE_ATTR_title])
    {
      fprintf (fout, "title: \"");
      print_string (fout, node->title);
      fprintf (fout, "\"\n");
    }

  if (node->set_p[GDL_NODE_ATTR_vertical_order])
    {
      fprintf (fout, "vertical_order: %d\n", node->vertical_order);
    }

  fputs ("}\n", fout);
}

/* Dump EDGE into the file.  */

void
gdl_dump_edge (FILE *fout, gdl_edge *edge)
{
  switch (edge->type)
    {
    case GDL_EDGE:
      fputs ("edge: {\n", fout);
      break;
    case GDL_BACKEDGE:
      fputs ("backedge: {\n", fout);
      break;
    case GDL_NEAREDGE:
      fputs ("nearedge: {\n", fout);
      break;
    case GDL_LEFTNEAREDGE:
      fputs ("leftnearedge: {\n", fout);
      break;
    case GDL_RIGHTNEAREDGE:
      fputs ("rightnearedge: {\n", fout);
      break;
    case GDL_BENTNEAREDGE:
      fputs ("bentnearedge: {\n", fout);
      break;
    case GDL_LEFTBENTNEAREDGE:
      fputs ("leftbentnearedge: {\n", fout);
      break;
    case GDL_RIGHTBENTNEAREDGE:
      fputs ("rightbentnearedge: {\n", fout);
      break;
    default:
      abort ();
    }

  /* Dump the attributes.  */

  if (edge->set_p[GDL_EDGE_ATTR_label])
    {
      fprintf (fout, "label: \"");
      print_string (fout, edge->label);
      fprintf (fout, "\"\n");
    }

  if (edge->set_p[GDL_EDGE_ATTR_linestyle])
    {
      fprintf (fout, "linestyle: %s\n", edge->linestyle);
    }

  if (edge->set_p[GDL_EDGE_ATTR_sourcename])
    {
      fprintf (fout, "sourcename: \"");
      print_string (fout, edge->sourcename);
      fprintf (fout, "\"\n");
    }

  if (edge->set_p[GDL_EDGE_ATTR_targetname])
    {
      fprintf (fout, "targetname: \"");
      print_string (fout, edge->targetname);
      fprintf (fout, "\"\n");
    }

  if (edge->set_p[GDL_EDGE_ATTR_thickness])
    {
      fprintf (fout, "thickness: %d\n", edge->thickness);
    }

  fputs ("}\n", fout);
}

/* Dump GRAPH into the file.  */

void
gdl_dump_graph (FILE *fout, gdl_graph *graph)
{
  int i;
  gdl_node *nodes, *node;
  gdl_edge *edges, *edge;
  gdl_graph *subgraphs, *subgraph;

  fputs ("graph: {\n", fout);

  /* Dump the attributes.  */

  if (graph->set_p[GDL_GRAPH_ATTR_colorentry])
    {
      for (i = 0; i < 256; i++)
        if (graph->colorentry_set_p[i])
          fprintf (fout, "colorentry %d: %d %d %d\n", i,
                   graph->colorentry[i][0],
                   graph->colorentry[i][1],
                   graph->colorentry[i][2]);
    }

  if (graph->set_p[GDL_GRAPH_ATTR_color])
    {
      fprintf (fout, "color: %s\n", graph->color);
    }

  if (graph->set_p[GDL_GRAPH_ATTR_folding])
    {
      fprintf (fout, "folding: %d\n", graph->folding);
    }

  if (graph->set_p[GDL_GRAPH_ATTR_label])
    {
      fprintf (fout, "label: \"");
      print_string (fout, graph->label);
      fprintf (fout, "\"\n");
    }

  if (graph->set_p[GDL_GRAPH_ATTR_layout_algorithm])
    {
      fprintf (fout, "layout_algorithm: %s\n", graph->layout_algorithm);
    }

  if (graph->set_p[GDL_GRAPH_ATTR_near_edges])
    {
      fprintf (fout, "near_edges: %s\n", graph->near_edges);
    }

  if (graph->set_p[GDL_GRAPH_ATTR_orientation])
    {
      fprintf (fout, "orientation: %s\n", graph->orientation);
    }

  if (graph->set_p[GDL_GRAPH_ATTR_port_sharing])
    {
      fprintf (fout, "port_sharing: %s\n", graph->port_sharing);
    }

  if (graph->set_p[GDL_GRAPH_ATTR_shape])
    {
      fprintf (fout, "shape: %s\n", graph->shape);
    }

  if (graph->set_p[GDL_GRAPH_ATTR_title])
    {
      fprintf (fout, "title: \"");
      print_string (fout, graph->title);
      fprintf (fout, "\"\n");
    }

  if (graph->set_p[GDL_GRAPH_ATTR_vertical_order])
    {
      fprintf (fout, "vertical_order: %d\n", graph->vertical_order);
    }

  if (graph->set_p[GDL_GRAPH_ATTR_xspace])
    {
      fprintf (fout, "xspace: %d\n", graph->xspace);
    }

  if (graph->set_p[GDL_GRAPH_ATTR_yspace])
    {
      fprintf (fout, "yspace: %d\n", graph->yspace);
    }

  if (graph->set_p[GDL_GRAPH_ATTR_node_borderwidth])
    {
      fprintf (fout, "node.borderwidth: %d\n", graph->node_borderwidth);
    }

  if (graph->set_p[GDL_GRAPH_ATTR_node_color])
    {
      fprintf (fout, "node.color: %s\n", graph->node_color);
    }

  if (graph->set_p[GDL_GRAPH_ATTR_node_shape])
    {
      fprintf (fout, "node.shape: %s\n", graph->node_shape);
    }

  if (graph->set_p[GDL_GRAPH_ATTR_node_textcolor])
    {
      fprintf (fout, "node.textcolor: %s\n", graph->node_textcolor);
    }

  if (graph->set_p[GDL_GRAPH_ATTR_edge_color])
    {
      fprintf (fout, "edge.color: %s\n", graph->edge_color);
    }

  if (graph->set_p[GDL_GRAPH_ATTR_edge_thickness])
    {
      fprintf (fout, "edge.thickness: %d\n", graph->edge_thickness);
    }

  /* Dump the nodes.  */
  nodes = gdl_get_graph_node (graph);
  for (node = nodes; node != NULL; node = node->next)
    gdl_dump_node (fout, node);

  /* Dump the subgraphs.  */
  subgraphs = gdl_get_graph_subgraph (graph);
  for (subgraph = subgraphs; subgraph != NULL; subgraph = subgraph->next)
    gdl_dump_graph (fout, subgraph);

  /* Dump the edges.  */
  edges = gdl_get_graph_edge (graph);
  for (edge = edges; edge != NULL; edge = edge->next)
    gdl_dump_edge (fout, edge);

  fputs ("}\n", fout);
}

