# Street Monitoring System
The project is to construct an optimized traffic system to help the local police install security cameras at traffic intersections. The idea is to minimize the number of cameras to install, while being as effective as possible with monitoring. The street maps are converted into undirected graphs, and the problem is reduced to solve the minimum vertex cover problem. We implemented three algorithms to solve the problem, and we compare the time-efficiencies and approximation ratios between them.

## Algorithms
The first algorithm is called CNF-SAT-VC. A polynomial-time reduction is implemented to map an instance of Vertex-Cover to a formula of CNF-SAT. The CNF-SAT is then solved by a SAT solver to find the minimum vertex cover. It can find a minimum vertex cover for any graph, but SAT solver normally has bad time-efficiency.

The second algorithm is an approximation algorithm, called APPROX-VC-1, which utilizes a greedy method. It picks a vertex of the highest degree (most incident edges), adds it to the vertex cover and throws away all the edges incident on that vertex. Then, it repeats till no edges remain.

The Third algorithm is named as APPROX-VC-2. It picks an edge ⟨u,v⟩, and adds both u and v to the vertex cover. Then, it throws away all edges attached to u and v, and repeat till no edges remain.

The three algorithms are implemented in C++, and concurrently run in three threads within the same process. To compare efficiencies between algorithms, we measure the running times and approximation ratios. The latter is the ratio of the size of the computed vertex cover to the output of CNF-SAT-VC, since CNF-SAT-VC guarantees to output the minimum vertex cover. 