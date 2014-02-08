# Define vcg plugin commands used in gdb. #

define view-bb
  call vcg_plugin_view_bb ("$arg0")
end
document view-bb
Usage: view-bb index
View the basic block which is specified by index.
end

define dump-bb
  call vcg_plugin_dump_bb ("$arg0")
end
document dump-bb
Usage: dump-bb index
Dump the basic block which is specified by index.
end

define view-bbs
  call vcg_plugin_view_bb ($arg0)
end
document view-bbs
Usage: view-bbs "index string"
View the basic blocks which are specified by index string.
end

define dump-bbs
  call vcg_plugin_dump_bb ($arg0)
end
document dump-bbs
Usage: dump-bb "index string"
Dump the basic blocks which are specified by index string.
end

define view-dominance
  call vcg_plugin_view_dominance ()
end
document view-dominance
Usage: view-dominance
View the current dominance graph.
end

define dump-dominance
  call vcg_plugin_dump_dominance ()
end
document dump-dominance
Usage: dump-dominance
Dump the current dominance graph.
end

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
  call vcg_plugin_view_function ()
end
document view-function
Usage: view-function 
View the control flow graph of the current function.
end

define dump-function
  call vcg_plugin_dump_function ()
end
document dump-function
Usage: dump-function 
Dump the control flow graph of the current function.
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

define view-rtx
  call vcg_plugin_view_rtx ($arg0)
end
document view-rtx
Usage: view-rtx [rtx RTX]
View the graph of RTX.
end

define dump-rtx
  call vcg_plugin_dump_rtx ($arg0)
end
document dump-rtx
Usage: dump-rtx [rtx RTX]
Dump the graph of RTX.
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

