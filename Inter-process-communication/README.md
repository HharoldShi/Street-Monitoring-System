# Inter-process-communication
The three process random-street-gen, graph-gen and shortest-path-finder are linked by the driver. Process random-street-gen generate radom streets specifications to be inputed to graph-gen. Process graph-gen then generate a graph, which is inputed into the shortest-path-finder to output shortest path between any two locations. 

There are two pipes and four processes. Pipe 1: write is stdout of random_street-gen, read is stdin of graph-gen. Pipe 2: write includes stdout of graph-gen and stdout of the driver, read is stdin of shortest-path-finder. 

The driver takes input of two locations, then output them to stdout. 
