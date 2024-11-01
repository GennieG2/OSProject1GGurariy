#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>

#define SHM_NAME "/shm_gennie"
#define SEM_NAME "/sem_gennie"
#define BUFFER_SIZE 3
#define PRODUCED_COUNT 20

struct Buffer {
    int data[BUFFER_SIZE];
    int in, out;
    bool terminate;
};

int main() {
    std::cout << std::endl << "Producer will produce " << PRODUCED_COUNT << " items and then stop." << std::endl << std::endl;

    //create and open shared memory object
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

    //set the size of shared memory object
    ftruncate(shm_fd, sizeof(Buffer));

    //map the shared memory object into address space
    Buffer* buffer = (Buffer*) mmap(0, sizeof(Buffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    buffer->in = buffer->out = 0;
    buffer->terminate = false;
    
    sem_t* semaphore = sem_open(SEM_NAME, O_CREAT, 0644, 1);
    if (semaphore == SEM_FAILED) {
       perror("sem_open");
       return 1;
    }

    int produced = 0;
    while(true) {
        if (produced == PRODUCED_COUNT)
            break; 
    
        sem_wait(semaphore);  //enter critical section

        if ((buffer->in + 1) % BUFFER_SIZE != buffer->out) {  //check if buffer is not full
            int item = rand() % 100;  //produce a random item
            buffer->data[buffer->in] = item;  //add the item to the buffer
            buffer->in = (buffer->in + 1) % BUFFER_SIZE;  //update the 'in' index
            produced++;
            std::cout<< produced << " Produced: " << item << std::endl;
        }

        sem_post(semaphore);  //exit critical section
        sleep(1);  //simulate production time
    }

    //signaling to consumer to terminate
    sem_wait(semaphore);
    buffer->terminate = true; 
    sem_post(semaphore);

    sleep(4);

    //clean up resources
    sem_close(semaphore); 
    sem_unlink(SEM_NAME);
    munmap(buffer, sizeof(buffer));
    close(shm_fd);
    shm_unlink(SHM_NAME);

    std::cout << "--------------" << std::endl << "producer terminated" << std::endl;

    return 0;
}