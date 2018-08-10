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
# Performance summary conclusions
  # Challenge 1 (ch1.c)
    There is no CPU or memory issues regarding this program, nevertheless it has a time complexity of O(n^2), which can be improved on future commits.
  # Challenge 2 (ch2.c)
    There is no CPU or memory issues regarding this program, nevertheless it requires user input which can extend the execution time of the program as much as the user likes, also O(n^2) time complexity.
  # Challenge 3 (ch3.c)
    There is no CPU or memory issues regading this program, nevertheless it uses an insertion sort algorithm that causes a O(n^2) complexity as a worst-case scenario.
