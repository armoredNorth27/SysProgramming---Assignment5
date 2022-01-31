Compilation instructions:
    There isn't anything complicated for compiling
    the code. Run 'make main' in order to compile
    and create all necessary executable files.
    You can run 'make clean' to remove all .o and 
    executable files.

NOTE:
I've added functionality to check if the server has won
after it does its move. The control flow that was given
in the assignment specifications didn't ask for this,
but I went to confirm with the Professor if this was 
needed and she said that I should include it, so I did.
I've also modified the server.h file by adding a function
that checks if the game board if full. This is because
there are 2 ways for the find_winner function to return
a value of 'N'. I needed a way to be able to distinguish
between an 'N' where the board is full, and an 'N' where
there are still moves to be played. Having this extra
function just helps to clean up the code a little bit more.

One last thing to note is that when I was testing my code
in the virtual machine, I would get a "cannot bind socket"
error when I run the server more than once. I think this
is because the socket is in a TIME_WAIT state but I'm not
100% sure if this is the case. Seeing as I'm closing the
sockets the same way that we've been doing with some of the
other material in the course I've decided to leave it as is
and I'm hoping that this won't impact your marking too much. 
After waiting for a few minutes I could then run the server again.
