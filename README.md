# Lem-in
42 school algorithm project.

Score 115/100


The goal of this project is to move your ant colony from one location to another. To make things more interesting, you are asked to move the ants in as few "turns" as possible. Basically the project consist of learning graph traversal algorithms to calculate maximum flow withing a flow network and then intelligently choose paths for ants to travel through.

### Features
- Overlapped adjacency lists
- Dinic's algorithm for maximum flow
- Bucket system for pathfinding
- "Smart" bruteforcing

### Running the program

![Usage](/images/map.jpg)

The program can either read the provided data from standard output or a file can be supplied as argument. See -h option to find out the full usage. What we see above is a simple .map file and below it, output of the program. There is a specific format to these files, which the lem-in requires. Each file must have the following: Number of ants, room ##start, room ##end and links connecting start and end room, otherwise no path can be found between start and end rooms. 

Ants must always come first, followed by rooms and links. Rooms have an id and coordinates, for example 10 10 10, where 10 is id as well as x and y coordinate. No two rooms may share an id or exactly same coordinates. Links are identified by "-" character between two room ids: 0-1. Comments and commands are allowed within the .map file, comments are marked with "#" and commands "##".

The output we see after the .map file itself, is divided in "turns". On each "turn" every ant may move once, through a link, to an empty room. Once every ant that is able to move has moved, new line is printed and that concludes one turn. The ants movements are always printed in the following format: L(ant id)-(room id), which ant is moving to which room. Two ants may never move to a same room and only one ant can move at a time.


### Data structure

![Struct](/images/struct.jpg)

The primary data structure used in the project is an adjacency list. In addition each room has their own little struct that holds the information of that particular room, its id, coordinates, visitation, flow, ant, level... The rooms are ordered not based on their ids, but simply in the order they appear in the .map file, with the exception of start and end rooms, which are always positioned to be the first and last room in the struct. 

The adjacency lists are 2d int arrays, where the 0 position always has the id of a parent room and starting from 1 to n, the parent rooms adjacents, meaning the rooms that are connected to the parent room. Since every parent and adjacent are identified by simply the order they appear in the .map file, that means that we can access rooms and their relevant data simply calling the room struct with the number we happen to be standing on. For example, if we wanted to find out the relevant data of room 5 adjacent 11, we can call ROOM[11] to access what we need, or if we wish to find out room 11 adjacents we can simply say row y we are currently on, is instead row x and we find ourselves at the start of that rooms adjacents. The secondary adjacency list holds flow data, it's format is exactly the same as the adjacency list with rooms, but the adjacents instead hold data of the direction of the flow. F1 for flow from parent to adjacent,  F0 for no flow and F-1 for flow from adjacent to parent.

### Dinic's algorithm

![Algo](/images/algo.jpg)

The basic concept of Dinic's algorithm is to have loop of Depth-first search(DFS) run inside a loop of Breadth-first search(BFS). Once we have the adjacency lists set up and data recorded, we can finally start figuring out, which rooms and links are good and can be used to find the way out. BFS assigns levels to rooms, based on how far away they are from the start room, this way, the shortest paths are found as the BFS traverses the graph with a queue system. BFS queues adjacents of a parent node, that are not in queue already and do not have established flow. 

After the BFS is done leveling the graph, DFS starts traversing the leveled paths always going +1 level upwards compared to the current parent node. If the DFS reaches the end room, we assign flow to all the links towards the end and negative flow backwards from the end. The negative flow is important because it allows us to re-use same paths when going to the other direction, this allows us to find out which paths would have multiple flows going through. Once the BFS can no longer assign a level to the end room, we know it is not possible to push more flow through.

### Pathfinding

![Paths](/images/paths.jpg)

Once the Dinic's algorithm has established the direction of the flow and all usable paths, it is time to use that information to find the best paths for the ants. This can be fairly tricky since the amount of the ants determines which collection of paths would be the best. For example if we have only 1 ant, the shortest path is always the best, however with 100 ants, using the shortest path may potentially block several slightly longer unique paths that would take less "turns" since we could divide the ants evenly to the slightly longer paths and end up with less "turns" taken.

The same Dinic's algorithm is used for pathfinding, with an important difference. This time, only the rooms that have flow coming to them are leveled. This way we end up with a graph where there are no dead ends and no loops and all paths are leading towards the end room. When DFS finds a path, we mark it visited and see if the BFS can still find more paths without using the same rooms DFS marked as visited. This is how we create a baseline for paths lengths. The first bucket of paths, generated this way is usually fairly good, sometimes reaching the minimum value of "turns" possible, but more often than not, It will simply give us an idea how many "turns" should it roughly take for ants to traverse through the graph.

Next we find out all different buckets starting with each adjacent room of the start room. The process is exactly the same as above, the only difference being that we start the leveling from a chosen adjacent of start room, instead of the start room. This way we get collections of buckets that each hold shortest paths based on the chosen adjacent node. Doing this is important because adjacency list as a data struct makes us favor rooms that are saved as first adjacents since we always start going through the adjacency list one by one starting from the first adjacent, until the last adjacent.

Now that we have generated several buckets of unique paths we should have fairly close value of optimal amount of "turns". At this point we consider the given .map file and try to determine if we should try and attempt to get even better "turn" value. So we count the forks in the directed graph. Forked room is any room with two or more paths coming in or two or more paths going out. If we find that there are not too many forks, we know we can attempt bruteforcing without sacrifising massive amounts of time. 

In the bruteforce approach we simply run one DFS search, only giving the flow direction as a guideline where to go. This kind of DFS will find all possible paths within the directed graph. The amount of paths can be huge so we attempt to remove as many paths as possible while we are pathfinding. The current best amount of turns, found by using the bucket BFS/DFS method is used as a point of comparison for path length, if the path is longer than our current best "turn" count, we ignore the path, since sending even 1 ant through it would take longer than the current best "turns".

Once all usefull paths are found, we start to compare then against each other one by one. Starting from the shortest, we mark it's rooms visited and try to find another path that is unique with the shortest, repeating this until there are no more paths left or we find paths with larger "turn" count than the current best. This process is done to every path starting from shortest. Whenever an acceptable path is added to the last bruteforced bucket of paths, we calculate the new best "turns" in an attempt to limit as many longer paths as possible so the bruteforce does not take too much time. Eventually all combinations are checked or the paths where the check should start are too long and the bruteforce stops.

All that is left is to check which of the buckets has shortest estimated "turn" value and move the ants through those paths.

## Note

The repository has the generator binary for generating various test maps. This is pre-compiled Mach-O 64-bit x86_64 executable and needs macOs to run.
