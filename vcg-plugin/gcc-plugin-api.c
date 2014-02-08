/* GCC plugin APIs.

   Copyright (C) 2009, 2010, 2011 Mingjie Xing, mingjie.xing@gmail.com. 

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include "vcg-plugin.h"

/* plugin license check */
int plugin_is_GPL_compatible;

/* */
static void
help (void)
{
  printf (
"Usage: gcc -fplugin=/path/to/vcg_plugin.so -fplugin-arg-vcg_plugin-<option> ...\n" \
"Options:\n" \
"  cgraph               dump the call graph before IPA passes.\n" \
"  cgraph-callee        dump the callee graph for each function.\n" \
"  cgraph-caller        dump the caller graph for each function.\n" \
"  gimple-hierarchy     dump the gimple hierarchy graph.\n" \
"  help                 show this help.\n" \
"  passes               dump the passes graph.\n" \
"  pass-lists           dump the pass lists graph.\n" \
"  tree-hierarchy       dump the tree hierarchy graph.\n" \
"  viewer=name          set the vcg viewer, default is vcgview.\n" \
"  \n" \
"%s %s <http://code.google.com/p/gcc-vcg-plugin>\n",
 vcg_plugin_common.plugin_name, vcg_plugin_common.version);

  exit (0);
}

/* Plugin initialization.  */

int
plugin_init (struct plugin_name_args *plugin_info,
             struct plugin_gcc_version *version)
{
  int i;
  int argc = plugin_info->argc;
  struct plugin_argument *argv = plugin_info->argv;

  //if (!plugin_default_version_check (version, &gcc_version))
  //  return 1;

  vcg_plugin_common.info = concat ("GCC: (GNU) ", version->basever,
                                   " ", version->datestamp, " ",
                                   "(", version->devphase, ")\n", NULL);

  /* Initialize the vcg plugin */
  for (i = 0; i < argc; i++)
    {
      //printf ("key: %s\n", argv[i].key);
      //printf ("value: %s\n", argv[i].value);

      /* Get the vcg viewer tool, default is "vcgview". */
      if (strcmp (argv[i].key, "viewer") == 0)
        {
          vcg_plugin_common.vcg_viewer = argv[i].value;
        }

      /* Dump call graph.  */
      if (strcmp (argv[i].key, "cgraph") == 0)
        {
          register_callback (plugin_info->base_name,
                             PLUGIN_ALL_IPA_PASSES_START,
                             (plugin_callback_func) vcg_plugin_callback_cgraph,
                             NULL);
        }

      /* Dump callee graph.  */
      if (strcmp (argv[i].key, "cgraph-callee") == 0)
        {
          register_callback (plugin_info->base_name,
                             PLUGIN_ALL_IPA_PASSES_START,
                             (plugin_callback_func) vcg_plugin_callback_callee,
                             NULL);
        }

      /* Dump caller graph.  */
      if (strcmp (argv[i].key, "cgraph-caller") == 0)
        {
          register_callback (plugin_info->base_name,
                             PLUGIN_ALL_IPA_PASSES_START,
                             (plugin_callback_func) vcg_plugin_callback_caller,
                             NULL);
        }

      /* Dump passes.  */
      if (strcmp (argv[i].key, "passes") == 0)
        {
          register_callback (plugin_info->base_name,
                             PLUGIN_START_UNIT,
            	             (plugin_callback_func) vcg_plugin_callback_passes_start,
                             NULL);
          register_callback (plugin_info->base_name,
                             PLUGIN_OVERRIDE_GATE,
                             (plugin_callback_func) vcg_plugin_callback_pass,
                             NULL);
          register_callback (plugin_info->base_name,
                             PLUGIN_FINISH_UNIT,
                             (plugin_callback_func) vcg_plugin_callback_passes_finish,
                             NULL);
        }

      /* Dump gcc pass lists.  */
      if (strcmp (argv[i].key, "pass-lists") == 0)
        {
          register_callback (plugin_info->base_name,
                             PLUGIN_FINISH,
                             (plugin_callback_func) vcg_plugin_callback_pass_lists,
                             NULL);
        }

      /* Dump gimple hierarchy graph.  */
      if (strcmp (argv[i].key, "gimple-hierarchy") == 0)
        {
          register_callback (plugin_info->base_name,
                             PLUGIN_FINISH,
                             (plugin_callback_func) vcg_plugin_callback_gimple_hierarchy,
                             NULL);
        }

      /* Dump tree hierarchy graph.  */
      if (strcmp (argv[i].key, "tree-hierarchy") == 0)
        {
          register_callback (plugin_info->base_name,
                             PLUGIN_FINISH,
                             (plugin_callback_func) vcg_plugin_callback_tree_hierarchy,
                             NULL);
        }

      if (strcmp (argv[i].key, "help") == 0)
        {
          help (); 
        }
    }
  
  return 0;
}

