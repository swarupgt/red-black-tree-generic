Generic implementation of Red Black Trees with iterators

project by Sathwik PK and Swarup Totloor

RBTree<T, binop_t> class has 3 main functions, insert, erase, and search.
	- T class is the type of data to be stored by each node in the tree.
	- The binop_t class is a predicate based on which the red black tree is sorted.

RBTree member functions - 

begin() -> returns an iterator to the beginning of the tree.

end() -> returns an iterator one past the end of the tree.

insert(const T& obj) -> creates a new node using the object and inserts it into the tree .
                - return value - iterator to the newly inserted node.
                - parameter - const ref to object of type T

                - example - RBTree<int>::Iterator x = tree.insert(10);

erase(T obj) -> removes the node with the given object from the tree. 
                - return value - iterator to one-past the deleted node, if the last node was deleted, it returns end()
                - parameter - Object of type T

                - example - RBTree<int>::Iterator x = tree.erase(20);

erase(Iterator it) -> removes the node pointed by the given iterator from the tree.
                - return value - iterator to one-past the deleted node, if the last node was deleted, it returns end()
                - parameter - Iterator of type RBTree<T>::Iterator

                - example - RBTree<int>::Iterator x = tree.erase(tree.begin());

erase(Iterator i1, Iterator i2) -> removes all nodes in the given range (non-inclusive) of iterators from the tree.
                - return value - iterator to one-past the last deleted node, if the last node was deleted, it returns end()
                - parameters - Iterator of type RBTree<T>::Iterator, Iterator of type RBTree<T>::Iterator

                - example - RBTree<int>::Iterator x = tree.erase(tree.begin(), tree.end());

search(const T& obj) -> searches for the given object as a node in the tree, if it exists.
                - return value - Iterator of type RBTree<T>::Iterator, tree.end() if object doesn't exist.
                - parameter - const ref to T obj

                - example - RBTree<int>::Iterator x = tree.search(10);

disp() -> displays the tree in-order, along with its color, parent and whether each node is a left node.
        - return value - void
        - parameter - none

        - example - tree.disp();

disp2d() -> displays the tree in 2D, tilted to the left.
		- return value - void
		- parameter - none


Ideally, class Node must not be used by user.

----------------------------------------------Iterators-------------------------------------------------------

Each iterator is a bi-directional iterator.

Operations supported by RBTree<T>::Iterator - 
    - * (rvalue dereference)
    - ++ (pre and post)
    - -- (pre and post)
    - == & != (equality)
