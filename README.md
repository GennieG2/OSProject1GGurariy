To compile both the producer and the consumer use:
g++ Producer.cpp -pthread -lrt -o producer
g++ Consumer.cpp -pthread -lrt -o consumer

To run them use:
./producer & ./consumer &

(Please see example photo to know what the output should look like)

This is a producer-consumer problem in c++ using shared memory and semaphores. The producer writes data to a shared memory buffer and the consumer reads and processes the data.

# shared memory
These processes communicate through a shared memory buffer. The buffer size is set to 3 and the table can only hold two items at the same time. 

# semaphores
The semaphore ensures mutual exclusion because it limits access to the shared memory to one process at a time. 

# limiting the number of items and using terminate flag
I used PRODUCED_COUNT to limit the total number of items that are produced to 20. Also, the terminate flag in the shared memory lets the consumer know when to stop. The producer sets terminate to true when it produces 20 items, which signals the consumer to terminate.
