# HNStat

# data structures

## The Trie

I used a prefix tree instead of an hash map because it uses a lot less memory.
This is due to the fact that words are quite similar most of the time and in a
try we don't need to allocate two times similar part of a string.

The trie will then perform insertions in O(m) with m the length of the string
(we can then say it's constant time insertion) and we do not need to browse the
trie to extract top queries or distinct queries because we can keep track of
those infos during the insertion itself. The program itself will just iterate
through elements and get the requested information in O(n), with n the number
of queries (in fact, for the top, the algorithm run in O(l) with l the given
limit of the top. This make the program run in O(n*l) for the top. But I
considere the limit as a constant).

A basic implementation of a trie was not suffisant. If each edge represented
only one letter, too much nodes / edges were allocated and memory usage grow
too quickly. As an optimization, I associated a string to each edge. At each
insertion, if we need to, we can split the edge in two and insert a node in
between. A lot of memory was saved this way (It reduced nearly by 50% the
memory usage on a million insertions).

## String Pool

After last updates, the performance bottleneck moved to edges split. At
each split, a string was deallocated and two others were allocated. This process
takes a lot of time. To change that, I made a string pool were all strings were
allocated only one, and for the lifetime of the trie. Edge does not contain
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


# memory usage

## massif output

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

## size of nodes/edges

Nodes are composed of a parent index (4bytes), a frequency counter (4bytes) and a
vector of edges. The vector size is 24 bytes (3 pointers) and can't be reduced.
This make the node size equal to 32bits.


Edges are composed of a reference to a string and the index of the child
(4bytes). The reference to string was at first a string\_view of size equal to
16bytes (two pointers). With the padding, sizeof(edge) corresponded to 24bytes.
In our case, a simple pointer and an int as the string length were suffisant.
This make the final size of an edge equal to 8B + 4B + 4B = 16B, making use save
8B.
