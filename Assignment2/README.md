﻿
# HOW TO COMPILE AND RUN

## PARALLEL KDTREE IMPLEMENTATION
- Be sure to be in Hybrid_version/ directory, then you can compile and run with only one command thanks to bash:
>  `qsub launcher.sh`

This will perform a single run with 10^8 points, for another kind of analysis there is more scripts on the tools directory
If you want to see an example of printing a tree, you can run:

>  `qsub print_a_tree.sh`

 - What does these scripts do? : Saves you a lot of time
 Well.. In fact calls PBS routines asking for 16 processors in each node (calling 2), setting a walltime only for 1 hour using the DSSC queue. 
 Then once it access to the requested node, it will access to the previous work directory, load the OPENMPI 4.1.1+GNU-9.3.0 module and compile the C program creating the executable **binary_tree** .

 - More information about the code and extras in the report
