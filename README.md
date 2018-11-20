# HNStat

# Subject

The goal of this test is to evaluate your ability to parse & process a large
amount of data, using the appropriate data structures and algorithms to solve a
simple challenge.
The provided data file is a TSV file listing all queries performed on HN Search
during a few days. Each line contains the timestamp and the query separated by
a tab. The goal is to extract the popular (most frequent) queries that have
been done during a specific time range.

# Build the project

## Requirements

To build this project, you should have:

 - CMake >= 3.9
 - g++ >= 8.2 or clang++ >= 7.0.0
 - Address Sanitizer for debug builds

## Instruction

This project uses CMake/Make as the build system.
Three build modes are possible:

 - Release: full optimization
 - RelWithDebInfo: some optimizations + benchmarks/logs + asserts + debug infos
 - Debug: no optimizations + benchmark/logs + asserts + debug infos + sanitizers
 linked + unit tests

You can do the following commands to build the project:
```bash
mkdir -p _build && \
cd _build && \
cmake -DCMAKE_BUILD_TYPE=BuildMode .. && \
make
```
If you prefer, you can just launch the bootstrap.sh script that will do all the
above in Release mode.

Remark: I coded some basic unit tests. You can launch them after building the
project in debug mode. Just call `make test`.

Now you can run the binary located in the `out` directory. Here is how
to use it:

```sh
Usage:
  ./hnStat distinct [--from TIMESTAMP] [--to TIMESTAMP] input_file
  ./hnStat top nb_queries [--from TIMESTAMP] [--to TIMESTAMP] input_file
```

The input\_file should be a tsv file formated as described in the subject.

## Integration

In the integration folder, there is a Dockerfile. You can build the image,
and launch a container with limited memory to test out the software execution.

```sh
cd integration && \
docker build -t hnstat . && \
docker run --rm --memory="512M" hnstat
```

# Data structures

## The Trie

I used a prefix tree instead of an hash map because it uses less memory.
Most of the time queries are quite similar most, and the trie permit to
reduce allocation of similar parts of a string.

The trie will then perform insertions in O(m) with m the length of the string
(we can then say it's constant time insertion) and we do not need to browse the
trie to extract top queries or distinct queries because we can keep track of
those infos during the insertion itself. The program itself will just iterate
through elements and get the requested information in O(n), with n the number
of queries (in fact, for the top, the algorithm run in O(l) with l the given
limit of the top. This makes the program run in O(n*l) for the top. But I
considere the limit as a constant).

A basic implementation of a trie was not suffisant. If each edge represented
only one letter, too much nodes / edges were allocated and memory usage grow
too quickly. As an optimization, I associated a string to each edge. At each
insertion, if we need to, we can split the edge in two and insert a node in
between. A lot of memory was saved this way (it reduced nearly by 50% the
memory usage on a million insertions).

## String Pool

After last updates, the performance bottleneck moved to edges split. At
each split, a string was deallocated and two others were allocated. This process
takes a lot of time. To change that, I made a string pool were all strings were
allocated only once, and for the lifetime of the trie. Edge does not contain
strings directly anymore but references on part of strings in the string pool.
At first I used C++17 string\_view, but later I optimized that with a raw char
pointer and an int to save 8bytes for each nodes.

## Vectors instead of maps

Each nodes and edges are stored in a vector to ensure locality. I benchmarked
the usage of an unordered\_map vs a vector as the container of edges and vector
performed approximatly 20% faster than the map.
As a later improvement, we could think to split the node vector in more small
vectors to prevent allocation of a too big chunk of memory, like kind of 
buckets.


# Memory usage

## Massif output

Using massif, we can have an idea of the heap memory usage of the program
during the insertions of nodes. Here we can see that nearly 100MB were used
to insert words contained in the example hn\_logs.txt.
```bash
    MB
106.5^                                                                       :
     |                                                          :@:@:@:::@::#:
     |                                                 @:@@:@:@:@@:@:@:::@::#:
     |                                          :::::::@:@@:@:@:@@:@:@:::@::#:
     |                                        ::: :::::@:@@:@:@:@@:@:@:::@::#:
     |                                    ::@@::: :::::@:@@:@:@:@@:@:@:::@::#:
     |                                   :::@ ::: :::::@:@@:@:@:@@:@:@:::@::#:
     |                                   :::@ ::: :::::@:@@:@:@:@@:@:@:::@::#:
     |                                   :::@ ::: :::::@:@@:@:@:@@:@:@:::@::#:
     |                                @@@:::@ ::: :::::@:@@:@:@:@@:@:@:::@::#:
     |                             @::@ @:::@ ::: :::::@:@@:@:@:@@:@:@:::@::#:
     |                          @@:@::@ @:::@ ::: :::::@:@@:@:@:@@:@:@:::@::#:
     |              @     ::@:::@@:@::@ @:::@ ::: :::::@:@@:@:@:@@:@:@:::@::#:
     |              @:::::::@:::@@:@::@ @:::@ ::: :::::@:@@:@:@:@@:@:@:::@::#:
     |              @:::::::@:::@@:@::@ @:::@ ::: :::::@:@@:@:@:@@:@:@:::@::#:
     |           :::@:::::::@:::@@:@::@ @:::@ ::: :::::@:@@:@:@:@@:@:@:::@::#:
     |        ::@:::@:::::::@:::@@:@::@ @:::@ ::: :::::@:@@:@:@:@@:@:@:::@::#:
     |        ::@:::@:::::::@:::@@:@::@ @:::@ ::: :::::@:@@:@:@:@@:@:@:::@::#:
     |    ::@@::@:::@:::::::@:::@@:@::@ @:::@ ::: :::::@:@@:@:@:@@:@:@:::@::#:
     | @@:::@ ::@:::@:::::::@:::@@:@::@ @:::@ ::: :::::@:@@:@:@:@@:@:@:::@::#:
   0 +----------------------------------------------------------------------->Gi
     0                                                                   7.361
```

## Size of nodes/edges

Nodes are composed of a parent index (4bytes), a frequency counter (4bytes) and a
vector of edges. The vector size is 24 bytes (3 pointers) and can't be reduced.
This makes the node size equal to 32bits.


Edges are composed of a reference to a string and the index of the child
(4bytes). The reference to string was at first a string\_view of size equal to
16bytes (two pointers). With the padding, sizeof(edge) corresponded to 24bytes.
In our case, a simple pointer and an int as the string length were suffisant.
This makes the final size of an edge equal to 8B + 4B + 4B = 16B, allowing us to
save 8B.
