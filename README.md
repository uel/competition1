## Competition task algorithms and datastructures



### Instructions
*The task in the competition task is to design an effective program that can solve the puzzle "rectangles".

*The puzzle is entered in a rectangular field. The field contains blank fields and fields with numeric information. The task is to divide the entire area of ​​the *given puzzle into rectangles so that:

*the rectangles covered the entire specified area,
*the rectangles did not overlap,
*each completed field was in a different rectangle,
*the size of the rectangle (area) was equal to the number of the completed field.
*When solving, you know that the playing area has a size of at most 32x32 squares, the total number of filled squares is at most 200 and the filled numbers are in *the range of 1 to 99.

*On the standard input, the program receives the puzzle entry, the entry ends with the end of std. input (active EOF). The exact input format can be seen in the *sample run below.

*The program analyzes the given puzzle and decides which of the variants occurred:
*entry is invalid a puzzle was entered on the input, which has only one valid solution. In this case, the program displays this solution.
*The input was a puzzle that cannot be assembled (0 solutions). The program displays information about the failed attempt.
*The given puzzle has many solutions (more than one), in which case the program calculates how many different ways the area can be divided. By different methods *of division, it is understood that the arrangement of the dividing rectangles differs in the location of at least one boundary line.
*The program must check the validity of the input data. If incorrect, meaningless or inconsistent values ​​are entered on the input, the program detects this, *displays an error message and exits. The format of the error message is shown in the sample run below. The following is considered an error:

*missing, irregular or incomplete field frame,
*the content of the box is different from the space and the number 1-99,
*missing indication of the corners of the boxes,
*fields larger than 32 x 32,
*number of fields filled in more than 200.
*The program is tested in a limited environment. It is limited by the amount of available memory and run time. Both limits can be seen in the sample test log. In *general, the task is not memory intensive. However, it is very CPU intensive. If the puzzle has multiple solutions, exploring all the options can be very time *consuming. It is necessary to design an efficient algorithm that will not waste time unnecessarily going through attempts that do not lead to the goal.

*The task is evaluated in the competition mode. This means that it is more demanding than standard tasks. Requires a combination of programming and algorithmic skills. It is assumed that the task will be solved by students who have some knowledge of programming, ie students for whom standard tasks are boring. The evaluation consists of a guaranteed point gain (success in non-competitive tests) and a point gain obtained on the basis of placement in the competition with other students (these points will be credited after the end of the competition).

### Example

#### Input
+--+--+--+--+--+
| 2  3        2|
+  +  +  +  +  +
|       2  3   |
+  +  +  +  +  +
|    3         |
+  +  +  +  +  +
|       2      |
+  +  +  +  +  +
| 2        3  3|
+--+--+--+--+--+

#### Output
One Answer:
+--+--+--+--+--+
| 2| 3      | 2|
+  +--+--+--+  +
|  |    2| 3|  |
+--+--+--+  +--+
|    3   |  |  |
+--+--+--+  +  +
|  |    2|  |  |
+  +--+--+--+  +
| 2|       3| 3|
+--+--+--+--+--+
