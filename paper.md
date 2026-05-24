# Creating Signal Processing Library for In-Memory Financial Applications

## Purpose
Python is being increasingly utilized for applications in quantitative finance. Most people currently use pandas or polars which are both performant in-memory data processing libraries. However, these libraries are more general and therefore include more features than necessary for most quantitative finance needs leading to bloat. The goal of this library is to be a lightweight, efficient library similiar to pandas but optimized for quantitative signal processing applications.

At the low level, it is designed to implement these operations efficiently:
  - addition (+)
  - subtraction (-)
  - multiplication (*)
  - division (/)
  - exp (^)
  - delay ([attribute_name]_[delay])

## Underlying Data Structure
The underlying data structure is a b+ tree with the leaf nodes forming a doubly linked list. Within the leaf nodes, the data is stored in the form of a 2D vector with each row vector being aligned with the provided attribute list and each value in the row vector representing a attribute val for its given index.

$$
\mathbf{keys} = \left[ 
  \begin{array}{c|c|c}
  k_1 & ... & k_n \\
  \end{array} 
\right],\\
[
  \mathbf{attribute_1} = \left[ 
    \begin{array}{c|c|c}
    x_1 & ... & x_n \\
    \end{array} 
  \right],\\
  ...,\\
  \mathbf{attribute_n} = \left[ 
    \begin{array}{c|c|c}
    y_1 & ... & y_n \\
    \end{array} 
  \right]
]
$$

# Future
- verify functionality with testing
- algorithm development
- backtesting
- Add markov analaysis
- Add fourier analysis

