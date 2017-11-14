# Decomposition-of-Graphs

Given a set of C++ files (a.cpp, b.cpp, c.cpp, d.cpp), the C++ program reads all .cpp files in the given folder
and creates an adjacency list based graph, where each file is a vertex and a directed edge goes from node B to
node A, if a.cpp includes b.cpp. The goal here is to checks if the given files have a dependency bug. A dependency
bug happens when there is a cyclic dependency among the include files. To do this you can run the DFS on the
graph and test if the graph has any back edges. If there is no cyclic dependency bug (i.e the corresponding graph
has no cycles), then the program should list the .cpp files in a topologically sorted order. In other words if a
complier tried to compile these programs in the sorted order, then every .cpp file should be complied only after
all .cpp files included by it are already compiled.
