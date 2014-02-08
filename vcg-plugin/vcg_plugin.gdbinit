# Define vcg plugin commands used in gdb. #

define view-bb
  call vcg_plugin_view_bb ("$arg0")
end
document view-bb
Usage: view-bb [int INDEX]
View the basic block which is specified by INDEX.
end

define dump-bb
  call vcg_plugin_dump_bb ("$arg0")
end
document dump-bb
Usage: dump-bb [int INDEX]
Dump the basic block which is specified by INDEX.
end

define view-bbs
  call vcg_plugin_view_bb ($arg0)
end
document view-bbs
Usage: view-bbs [string INDEX_STRING]
View the basic blocks which are specified by INDEX_STRING.
end

define dump-bbs
  call vcg_plugin_dump_bb ($arg0)
end
document dump-bbs
Usage: dump-bbs [string INDEX_STRING]
Dump the basic blocks which are specified by INDEX_STRING.
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

define view-cgraph-callee
  call vcg_plugin_view_cgraph_callee ()
end
document view-cgraph-callee
Usage: view-cgraph-callee
View the current callee graph.
end

define dump-cgraph-callee
  call vcg_plugin_dump_cgraph_callee ()
end
document dump-cgraph-callee
Usage: dump-cgraph-callee
Dump the current callee graph.
end

define view-cgraph-caller
  call vcg_plugin_view_cgraph_caller ()
end
document view-cgraph-caller
Usage: view-cgraph-caller
View the current caller graph.
end

define dump-cgraph-caller
  call vcg_plugin_dump_cgraph_caller ()
end
document dump-cgraph-caller
Usage: dump-cgraph-caller
Dump the current caller graph.
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

define view-loop
  call vcg_plugin_view_loop ($arg0)
end
document view-loop
Usage: view-loop [int LOOP_ID]
View the graph of loop.
end

define dump-loop
  call vcg_plugin_dump_loop ($arg0)
end
document dump-loop
Usage: dump-loop [int LOOP_ID]
Dump the graph of loop.
end

define view-pass-lists
  call vcg_plugin_view_pass_lists ()
end
document view-pass_lists
Usage: view-pass-lists
View the graph of gcc pass lists. If current_pass exists, then show the it in red color.
end

define dump-pass-lists
  call vcg_plugin_dump_pass_lists ()
end
document dump-pass-lists
Usage: dump-pass-lists
Dump the graph of gcc pass lists. If current_pass exists, then show the it in red color.
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

