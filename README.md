Write a c program that connects to the listening service
at a given IP and PORT.

nc IP PORT // to see how the program runs

The service will return to you a list of some random number
of whole numbers separated by spaces. Then it will ask you
for either the minimum, the maximum, or the sum of the
given list of numbers.

Return to it the answer followed by a newline character.

It will test you with this protocol 200 times. Your
solution should be able to reach the end.

Example:

Service sends:
    800 20 3785
    Give me the maximum of all the numbers:\n

You respond:
    3785\n

Service sends:
    10 300 908510 1038
    Give me the minimum of all the numbers:\n

You respond:
    10\n

Service sends:
    10 20 30 40 50
    Give me the sum of all the numbers:\n

You respond:
    150\n

Upon completing the challenge you will see a message
notifying you of your success.

Once you have solved this problem, please email @sigovs.com with your solution C file and an explanation of what it does.
This is not meant to be a challenging problem, but there are some gotchas. Pay attention to your buffers :)

Please let us know in your explanation how you built your program.

i.e. gcc or visual studio version, what flags you use to compile it.  If you have a make file that builds it, that is fine too.
 

