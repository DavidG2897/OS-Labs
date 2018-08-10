# OperativeSystemsClass
All code for my Operative Systems class is in this repo.

# How to build and run
All code in here is programmed in C, preferably use GCC compiler to build.

  From terminal (UNIX) use the following command to build:
  gcc <input_file> -o <output_filename>
    Ex. user> gcc helloWorld.c -o helloWorld.o //Compiles file helloWorld.c and outputs the executable helloWorld.o
  From terminal/cmd use the folowing command to run:
  ./<output_filename>
    Ex. user> ./helloWorld.o
        user> "Hello World!"

# Performance summary conclusion
  # Challenge 1 (ch1.c)
    Since this code is running on a large computer and it is a fairly simple code there is no present problems with CPU performance, nevertheless execution time could be improved. O(n^2) time complexity.
    No memory issues, only using small 8bit variables (unsigned chars).
  # Challenge 2 (ch2.c)
    This code requires user input, so execution time could take as long as the user likes. It uses an insertion sort algorithm so O(n^2) time complexity as worst-case scenario.
    No memory issues, although it uses a dynamic array structure.
  # Challenge 3 (ch3.c)
    
