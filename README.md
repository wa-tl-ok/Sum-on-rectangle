# Sum on rectangle
Quick search for the sum of numbers under a rectangle with changes using 2D Segment Tree

Let us be given a rectangle of size n*m. Each cell contains a number (initially all numbers are equal to zero)
Let's learn how to do the following operations:

! x1 y1 x2 y2 X - add the number X to each cell of the rectangle with corners at points A(x1, y1) and B(x2, y2).

? x1 y1 x2 y2 - find the sum of numbers under a rectangle with corners at points A(x1, y1) and B(x2, y2).

Please note that push notifications cannot be used directly. Compared to a one-dimensional Segment Tree, it is impossible to push and stack segments.

! Obviously, a two-dimensional data structure will be needed.

We will store Segment Tree as follows: (below is an example for a special case of the original rectangle)

    -------------------------
    | 2 | 3 | 6 | 9 | 2 | 5 |
    ------------------------- =>> | 3 | 7 | 8 | 9 | 9 | 8 |
    | 1 | 4 | 2 | 0 | 7 | 3 |
    -------------------------                          =>> | 8 | 16 | 16 | 13 | 18 | 14 |
    | 2 | 7 | 1 | 4 | 8 | 0 |
    ------------------------- =>> | 5 | 9 | 8 | 4 | 9 | 6 |
    | 3 | 2 | 7 | 0 | 1 | 6 |
    -------------------------


Let's transform the drawing:

    ---------------------------------
    | 2 | 3 | 6 | 9 | 2 | 5 | 0 | 0 |
    --------------------------------- =>> | 3 | 7 | 8 | 9 | 9 | 8 | 0 | 0 |
    | 1 | 4 | 2 | 0 | 7 | 3 | 0 | 0 |
    ---------------------------------                                =>> | 8 | 16 | 16 | 13 | 18 | 14 | 0 | 0 |
    | 2 | 7 | 1 | 4 | 8 | 0 | 0 | 0 |
    --------------------------------- =>> | 5 | 9 | 8 | 4 | 9 | 6 | 0 | 0 |
    | 3 | 2 | 7 | 0 | 1 | 6 | 0 | 0 |
    ---------------------------------

Note!) The size of each converted string does not exceed 2*m.

Note!) The size of Segment Tree in lines does not exceed 4*m.

Let's now notice that after redrawn the Segment Tree a little, we get:

                                                       | 8 | 16 | 16 | 13 | 18 | 14 | 0 | 0 |
                                                         /                                \
                                                        /                                  \
                                                       /                                    \
                                    | 3 | 7 | 8 | 9 | 9 | 8 | 0 | 0 |                  | 5 | 9 | 8 | 4 | 9 | 6 | 0 | 0 |
                                     /                            \                     /                             \
                                    /                              \                   /                               \
                                   /                                \                 /                                 \
    | 2 | 3 | 6 | 9 | 2 | 5 | 0 | 0 |     | 1 | 4 | 2 | 0 | 7 | 3 | 0 | 0 |     | 2 | 7 | 1 | 4 | 8 | 0 | 0 | 0 |     | 3 | 2 | 7 | 0 | 1 | 6 | 0 | 0 |

Note!) The i-th cell of the root stores information about the amount in the i-th column.

Note!) The leafs will store the rows of the original table.

Note!) There are no more than 4*n nodes in total.

Suppose we need to get an answer to some kind of query like "?". How to do it? It is enough to look at those nodes of the Segment Tree that satisfy the first coordinate, then element-by-element iterate through all the cells (satisfying the second coordinate) of each matching array, updating the sum. This solution works in O(m*log(n)).

Let's try to improve the asymptotics. Note that we don’t necessarily have to go through element by element, counting the sum each time. Instead, you can build Segment Tree on each such array, responsible for the second coordinate. Then searching and updating the sum will be the same as in the one-dimensional case without pushes. For each of these arrays we will do Segment Tree on sum and add; Let's learn how to process queries like "!" And "?" for a two-dimensional data structure (in our case, 2D Segment Tree).

1) Query "!": let's learn how to process this request not using "push". So, let’s say we need to add X on the segment [l, r]
To do this, for all the Segment Trees visited during the traversal, the node recalculates sum(v) (node v “owns the piece [L:R]”):

        1a)  R>r && l>L: sum(v)+=(r-l)*X
        1b)  R>r && L>l: sum(v)+=(r-L)*X
        1c)  r>R && l>L: sum(v)+=(R-l)*X
        1d)  r>R && L>l: sum(v)+=(R-L)*X

From these cases the formula obviously follows: sum(v) = sum(v) + (min(r, R) - max(l, L)) * X.

When traversing the Segment Tree, we will also update add at the nodes (increase add(v) by X for all nodes that are under the current one).

Note!) It is easy to notice that after changing add(v) for all nodes that are under the current one, further traversal to the Segment Tree does not make sense, since we have already touched on all the necessary nodes.

2) Query "?": it will be possible to collect all add(V) for all parents. Obviously, this value needs to be multiplied by the length of approximately the segment.
We will maintain stability on the way from the root to the nodes and will not forget about the sum (v).

! Total memory costs: 16nm.

! Total costs by time: log(n)log(m).
