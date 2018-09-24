# Computer-Architechure
Codes in C

In Computer Architecture, there were five main Projects, each introducting us into a new aspect of coding in C.

PA1:  
  PA1 was constructed of 8 parts, starting with something as simple as compressing a string to creating a Binary Search
  Tree or Linked lIst by the end. 
      The eight codes:
          Anagram: sorts the inputed strings characters into alphabetical order.
          BST: takes in instructions to insert, search for, or delete nodes in a binary search tree.
          List: takes in instructions to insert or delete nodes, then prints the remaining list
          Mexp: Matrix exponentiation. Takes in a matrix and mltiplies it to a given power.
          Rle: Compresses a string, so aaabbb beomes a3b3. Only compresses if the new string is shorter than the original.
          Sudoku:Checks a 9x9 grid to see if it follows the Sudoku rules, and also can check to see if 
          a puzzle with one or two empty spaces is solvable.
          Table:A very simple hash table that takes in nubers to insert, and can check whether numbers are present.
          Yell: Returns the string in all Caps.
           
          
PA2 (learn.c): 
  "Given matrices X and Y , your program will compute (XT X)−1XT Y in order to learn W. This will require (1) multiplying, (2)   transposing, and (3) inverting matrices. Programming Assignment I already involved matrix multiplication; you may adapt your   implementation for this assignment.
  Transposing an m × n matrix produces an n × m matrix. Each row of the X becomes a column of XT ."
  
  Using simple machine learning, the program takes in data for the prices of houses, learns the equation, and uses it to         predict the prices of future houses.
  
  
  
PA3:
    Using GDB and a "bomb" written in Assembly Language, I had to figure out the inputs needed to prevent the "bomb" from         exploding (essentially it exploded if I got a question wrong. There was no program to write, but I was able to diffuse         stages as simple as entering a string stored in memory to entering an order of a linked list stored, all by decoding           Assembly using the GDB.
    
    
PA4 (truthtable.c): 
  This program takes in instructions for a truth table using anything from and, or, xor to decoders and multiplexes. Using       structs, I stored the input data and was able to print out truthtables whther or not the inputs were given in order, and       the code could easily handle temporary variables between the input and output.
  
PA5 (cachesim.c)
  This program takes in a document giving the size and type of cache to be implemented, then simulates using a cache on the     following memory lines given. The memory files could be up to a couple million lines of code long, and outputted is a tally   of cache hits and misses, both in a pre-fetch and non pre-fetch cache.
  
  
