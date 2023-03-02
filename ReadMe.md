In this project, we implement both code compression and decompression using C, C++ for a given input file.
The task is as follows.

Assume that the dictionary can have eight entries (index 3 bits) and the eight entries are selected based on
frequency (the most frequent instruction should have index 000). If two entries have the same frequency,
the priority should be given to the one that appears first in the original program order. The original
code consists of 32-bit binaries. You are allowed to use only the following seven possible formats for
compression. Note that if one entry (32-bit binary) can be compressed in more than one way, choose the
most beneficial one i.e., the one that provides the shortest compressed pattern. If two formats produce
exactly the same compression, choose the one that appears earlier in the following listing (e.g., run-length
encoding appears earlier than direct matching). Also, if there is a scenario where you have two possible
ways of applying bitmasks to a 32-bit binary, always give preference to the scenario where the leftmost bit
‘1’ for the bitmask pattern (e.g., 11 is preferred over 01). Please count the starting location of a mismatch
from the leftmost (MSB) bit of the pattern – If the mismatch is at the MSB, Mismatch Location should
be 00000.
