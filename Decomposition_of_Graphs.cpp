#include <iostream>
#include <sstream>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

// Graph vertex
class Vertex {
private:
    std::string name; // Name of vertex
    std::set<std::string> adj; // Adjacent vertices.
    
    // We'll use an unusual approach here:
    // record 'a { b, c }' (where a is name, b and c is in adjacent list)
    // notes that there are edges 'b -> a' and 'c -> a'. Not the reverse!
    // It would be convinient for us in the future.
    
public:
    // Overloading == operator so we can call std::find on a set of vertices
    inline bool operator == (const Vertex & v) const { return name == v.name; }
    
    // Constructor, takes only new name identifier
    Vertex(const std::string & newName) { name = newName; }
    
    // Insert new vertex into adjacency list
    void NewV(const std::string & name) { adj.insert(name); }
    
    // Delete vertex from adjacency list
    // If there is no vertex, would not do anything
    void DeleteV(const std::string & name) { adj.erase(name); }
    
    // Checks if vertex has no incoming edges
    bool IsIncomingEdges() { return !adj.empty(); }
    
    // Accessor. Get name of this vertex
    std::string Name() { return name; }
};

// Graph itself
class Graph {
private:
    std::vector<Vertex> v; // Vector of vertices
    // Actually can be better done via map, but for graph with 3 vertices it doesn't matter much
    
public:
    void TryInsert(const std::string & name);
    
    void InsertVertex(const std::string & name);
    void InsertPair(const std::string & from, const std::string & to);
    
    // Function tries to locate bugs in dependencies.
    // If there is a bug (i.e. cycle), returns true.
    // Otherwise, returns false, and output is set to the proper path of linking
    bool IsThereBugs(std::string & output);
};

// Insert new vertex (if it is not already present)
void Graph::InsertVertex(const std::string & name) {
    if (std::find(v.begin(), v.end(), Vertex(name)) == v.end())
        v.push_back(Vertex(name));
}

void Graph::InsertPair(const std::string & from, const std::string & to) {
    // Try to insert two vertices into graph
    // If they does not exist, no insertion would be made, nothing would change.
    InsertVertex(from);
    InsertVertex(to);
    
    // Next, we need to add "to" into adjacency list of "from"
    // Note that find would always return non-end, because in worst case it would
    // have been created just now.
    std::vector<Vertex>::iterator it = std::find(v.begin(), v.end(), Vertex(from));
    it->NewV(to);
}

bool Graph::IsThereBugs(std::string & output) {
    // We have to obtain a copy of graph, because we'll destroy it in the process
    Graph g = *this;
    output = "";
    
    // While our graph is not empty...
    while (!g.v.empty()) {
        bool success = false;
        std::string name;
        
        // Find a vertex with no incoming edges
        std::vector<Vertex>::iterator it, inner;
        for (it = g.v.begin(); it != g.v.end(); ++it) {
            if (!it->IsIncomingEdges()) {
                // Add this to string
                output += it->Name();
                
                // Set success flag
                success = true;
                
                // Retrieve name of this vertex
                name = it->Name();
                
                // Remove this vertex from graph
                g.v.erase(it);
                
                // Remove all entries of 'name' in all adjacency lists
                for (inner = g.v.begin(); inner != g.v.end(); ++inner)
                    inner->DeleteV(name);
                
                // Add a cool-looking arrow to the string
                if (!g.v.empty())
                    output += " -> ";
                
                break;
            }
        }
        
        // Unable to find vertices with no incoming edges.
        if (!success)
            return true;
    }
    
    return false;
}

// Deletes all occurencies of pattern in string str
void DeleteSubstrings(std::string & str, const std::string & pattern) {
    int i = str.find(pattern);
    
    while (i != std::string::npos) {
        str.erase(i, pattern.length());
        i = str.find(pattern, i);
    }
}

void InputVertex(Graph & g, const std::string & initial) {
    // Insert this vertex
    g.InsertVertex(initial);
    
    // Print initial file name
    std::cout << "Filename: " << initial << ".cpp\n";
    
    // Scan string with includes
    std::cout << "Files included: ";
    std::string includes;
    std::getline(std::cin, includes);
    std::cout << "\n";
    
    // Check if includes is blank
    if (includes == "-")
        return;
    
    // Remove all occurences of '.cpp' and ',' in source string
    DeleteSubstrings(includes, ".cpp");
    DeleteSubstrings(includes, ",");
    
    // Parse includes line
    std::istringstream ss(includes);
    std::string include;
    ss >> include;
    
    while (!ss.bad() && !ss.fail()) {
        g.InsertPair(initial, include);
        ss >> include;
    }
}

int main() {
    Graph g;
    
    // There are only 3 input vertices in this graph
    InputVertex(g, "a");
    InputVertex(g, "b");
    InputVertex(g, "c");
    
    // Evaluate order and print it
    std::string out;
    if (g.IsThereBugs(out))
        std::cout << "The files cannot be complied as they have a cyclic dependency\n";
    else
        std::cout << "The files can be compiled in the order " << out << "\n";
    
    return 0;
}


