# Define vcg plugin commands used in gdb. #

define view-cgraph
  call vcg_plugin_view_cgraph ()
end
document view-cgraph
Usage: view-cgraph
View the current call graph.
end

define dump-cgraph
  call vcg_plugin_dump_cgraph ()
end
document dump-cgraph
Usage: dump-cgraph
Dump the current call graph.
end

define view-function
  call vcg_plugin_view_function ($arg0)
end
document view-function
Usage: view-function [tree FUNCTION_DECL_TREE]
View the control flow graph of FUNCTION_DECL_TREE.
end

define dump-function
  call vcg_plugin_dump_function ($arg0)
end
document dump-function
Usage: dump-function [tree FUNCTION_DECL_TREE]
Dump the control flow graph of FUNCTION_DECL_TREE.
end

define view-gimple-hierarchy
  call vcg_plugin_view_gimple_hierarchy ()
end
document view-gimple-hierarchy
Usage: view-gimple-hierarchy
View the graph of gcc gimple statement structure hierarchy.
end

define dump-gimple-hierarchy
  call vcg_plugin_dump_gimple_hierarchy ()
end
document dump-gimple-hierarchy
Usage: dump-gimple-hierarchy
Dump the graph of gcc gimple statement structure hierarchy.
end

define view-passes
  call vcg_plugin_view_passes ($arg0)
end
document view-passes
Usage: view-passes [struct opt_pass * PASS]
View the graph of gcc pass lists. If PASS exists, then show the PASS in red color.
end

define dump-passes
  call vcg_plugin_dump_passes ($arg0)
end
document dump-passes
Usage: dump-passes [struct opt_pass * PASS]
Dump the graph of gcc pass lists. If PASS exists, then show the PASS in red color.
end

define view-tree
  call vcg_plugin_view_tree ($arg0)
end
document view-tree
Usage: view-tree [tree TREE]
View the graph of TREE.
end

define dump-tree
  call vcg_plugin_dump_tree ($arg0)
end
document dump-tree
Usage: dump-tree [tree TREE]
Dump the graph of TREE.
end

define view-tree-hierarchy
  call vcg_plugin_view_tree_hierarchy ()
end
document view-tree-hierarchy
Usage: view-tree-hierarchy
View the graph of gcc tree structure hierarchy.
end

define dump-tree-hierarchy
  call vcg_plugin_dump_tree_hierarchy ()
end
document dump-tree-hierarchy
Usage: dump-tree-hierarchy
Dump the graph of gcc tree structure hierarchy.
end

