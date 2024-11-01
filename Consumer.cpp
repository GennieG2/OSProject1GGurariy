#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>

#define SHM_NAME "/shm_gennie"
#define SEM_NAME "/sem_gennie"
#define BUFFER_SIZE 3

struct Buffer {
    int data[BUFFER_SIZE];
    int in, out;
    bool terminate;
};

int main() {
    //open the semaphore
    sem_t* semaphore = sem_open(SEM_NAME, 0);
     if (semaphore == SEM_FAILED) {
       perror("sem_open");
       return 1; 
    }
    //open the shared memory object
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    //map the shared memory object into the address space
    Buffer* buffer = (Buffer*) mmap(0, sizeof(Buffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
   
    int consumed = 0;
    while (true) {
        sem_wait(semaphore);  //enter critical section  

        if (buffer->in != buffer->out) {  //check if buffer is not empty          
            int item = buffer->data[buffer->out];  //remove the item from the buffer          
            buffer->out = (buffer->out + 1) % BUFFER_SIZE;  //update the 'out' index       
            consumed++; 

            std::cout << consumed << " Consumed: " << item << std::endl;
        }

        if (buffer->terminate) {
            sem_post(semaphore);
            break;
        }     
         
        sem_post(semaphore);  //exit critical section
        sleep(1);  //simulate consumption time
    }

    //clean up resources
    sem_close(semaphore); 
    sem_unlink(SEM_NAME);
    munmap(buffer, sizeof(Buffer));
    close(shm_fd);

    std::cout << "--------------" << std::endl << "consumer terminated" << std::endl;

    return 0;
}
