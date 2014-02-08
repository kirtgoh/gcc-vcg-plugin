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

/* plugin initialization */
int
plugin_init (struct plugin_name_args *plugin_info,
             struct plugin_gcc_version *version)
{
  int i;
  int argc = plugin_info->argc;
  struct plugin_argument *argv = plugin_info->argv;

  //if (!plugin_default_version_check (version, &gcc_version))
  //  return 1;

  /* Initialize the vcg plugin */
  for (i = 0; i < argc; i++)
    {
      printf ("key: %s\n", argv[i].key);
      printf ("value: %s\n", argv[i].value);
      /* Get the vcg viewer tool, default is "vcgview". */
      if (strcmp (argv[i].key, "viewer") == 0)
        {
          vcg_plugin_common.vcg_viewer = argv[i].value;
        }
    }
  vcg_plugin_common.info = concat ("GCC: (GNU) ", version->basever,
                                   " ", version->datestamp, " ",
                                   "(", version->devphase, ")\n", NULL);
  
  return 0;
}

