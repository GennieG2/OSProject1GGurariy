To compile both the producer and the consumer use:


  g++ Producer.cpp -pthread -lrt -o producer
  
  g++ Consumer.cpp -pthread -lrt -o consumer

To run them use:
./producer & ./consumer &

(Please see example photo to know what the output should look like)

This is a producer-consumer problem in c++ using shared memory and semaphores. The producer writes data to a shared memory buffer and the consumer reads and processes the data.

These processes communicate through a shared memory buffer. The buffer size is set to 3 and the table can only hold two items at the same time. First I created and opened the shared memory buffer, then I manipulated the buffer via the producer and consumer processes (I wrote comments in the sourcecode for specifics). In the producer, after entering the critical section, I checked if the buffer is not full, produced a random item, added the item to the buffer, and updated the 'in' index in the bugger before exiting the critical section. In the consumer, after entering the critical section, I checked if the buffer is not , removed the item from the buffer if not, and updated the 'out' index in the buffer. Also, the consumer checks the terminate flag in the buffer and if it is true (20 items are produced) the consumer exits the critical section and exits the while loop, going straight to cleaning up the resources.

The semaphore ensures mutual exclusion because it limits access to the shared memory to one process at a time. I first created and open a semaphore, and used sem_wait(semaphore) to enter the critical section and sem_post(semaphore) to exit the critical section. The resources get cleaned up after finishing.

I used PRODUCED_COUNT to limit the total number of items that are produced to 20. Also, the terminate flag in the shared memory lets the consumer know when to stop. The producer sets terminate to true when it produces 20 items, which signals the consumer to terminate.
