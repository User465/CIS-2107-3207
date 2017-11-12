/*
Abstract of Approach
With this project, I sought to create multiple worker threads that wait on a
socket to fill up with words so they can extract those words from the socket
and test to see if they are spelled correctly by accessing a shared dictionary
array. It then sends the results back into the socket to be echoed by the client

Techniques Used
I broke up the tasks into as may functions as possible, with many of the functions
grouped up so they can handle one large task at a time. For instance, I have
the queue functions bundled up at the bottom for easy access, and they each
handle the semaphore values for the sockets.

Outline of Main Tasks of Each Process/Thread
Each thread is in charge of waiting on the socket until a word enters the queue.
It then removes the word from the queue and checks the shared dictionary array
to see if the word is contained in the array. Whether it is or it isn't, the word
is concatenated with the result (OK or MISSPELLED) and sent back through the
socket to be echoed by the server.

Outline of Each Major Routine in Process/Thread
The threads must remove the words from the sockets, check the dictionary array,
and then send the result back through the socket to be echoed by the client so
it is either OK or MISSPELLED. It then clears the buffer string so multiple
processes can be run.

Input/Output of Each Function
**Include discussion of each possible function return value

Main function
Main function ties everything together. In a while loop it creates the worker
threads, accepts connection from the client, and adds the client to the socket
to be serviced by the worker threads.

void initQueue(queue *, int); //init queue
This function initializes the queue to be used in the rest of the program. This
is paired up with deinitQueue which does the exact opposite. It takes a queue
and an int as input and returns nothing.

void deinitQueue(queue *); //deinit queue
This function is the opposite of the previous function mentioned; this deinits
the queue (even though it is never called hehe).

void addToSocket(queue *, int); //add to socket
This function takes a queue as input and returns nothing. It merely adds the
client to the socket to be serviced by the worker threads.

int removeFromSocket(queue *); //remove from socket
This accepts the queue as input and returns an int to make sure the client was
successfully removed from the queue.

int getlistenfd(char *); //listen fd
This function takes a char pointer and returns an int to ensure that the server
is listening for a client connection correctly, and returns 1 if successful and
failure otherwise.

void *serviceClient(void *); //services the client with the thread
This function is the one the worker threads use to handle the words extracted
from the socket. It takes in a void pointer and returns a void pointer. It locks
the semaphore for one thread and works on it until it is done, and then frees
up the semaphore.

ssize_t readLine(int , void *, size_t); //read line
char **makeDictionary(char *); //makes the dictionary into pointer
This function takes in the dictionary file and writes all the words into a giant
array which will be accessed by a pointer. It takes in the dictionary file and
the size and returns a char pointer pointer.
*/


#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>

#define BUF_SIZE 256
#define READ 0
#define WRITE 1
#define TRUE 1
#define FALSE 0
#define BACKLOG 20
#define DEFAULT_PORT_STR "9999"
#define DEFAULT_DICTIONARY "dictionary.txt"
#define DICTIONARY_SIZE 99172
#define EXIT_USAGE_ERROR 1
#define EXIT_GETADDRINFO_ERROR 2
#define EXIT_BIND_FAILURE 3
#define EXIT_LISTEN_FAILURE 4
#define NUM_WORKERS 5
#define MAX_LINE 64

typedef struct addrinfo addrinfo;
typedef struct sockaddr_storage sockaddr_storage;
typedef struct sockaddr sockaddr;
typedef struct {
    int *buf; //buffer array
    int capacity; //max
    int front; //first
    int rear; //last
    int size; //size of queue
    sem_t mutex; //semaphore binary
    sem_t slots; //semaphore
    sem_t items; //semaphore
} queue;
typedef struct threadArg {
    queue *qu;
    char **dictionaryWords;
} threadArg;

void initQueue(queue *, int); //init queue
void deinitQueue(queue *); //deinit queue
void addToSocket(queue *, int); //add to socket
int removeFromSocket(queue *); //remove from socket
int getlistenfd(char *); //listen fd
void *serviceClient(void *); //services the client with the thread
ssize_t readLine(int , void *, size_t); //read line
char **makeDictionary(char *); //makes the dictionary into pointer


int main(int argc, char **argv) {
    pthread_t threads[NUM_WORKERS];
    threadArg threadArgument; //thread
    int newSocket, welcomeSocket; //listening and connected socket descriptor
    int i;
    sockaddr client_addr; //client address
    socklen_t client_addr_size; //size of client address
    char client_name[MAX_LINE]; //client name
    char client_port[MAX_LINE]; //client port
    char *port; //port
    char *dictionaryName; //dictionary name
    char **dictionaryWords; //list of dictionary words
    void *ret;
    queue q; //queue of socket descriptors

    if (argc < 2) { //neither given
        port = DEFAULT_PORT_STR;
        dictionaryName = DEFAULT_DICTIONARY;
    } else if (argc < 3) { //only dictionary name
        port = DEFAULT_PORT_STR;
        dictionaryName = argv[2];
    } else { //port and dictionary given
        port = argv[1];
        dictionaryName = argv[2];
    }

    newSocket = getlistenfd(port); //socket descriptor

    initQueue(&q, 20); //queue made

    if ((dictionaryWords = makeDictionary(dictionaryName)) == NULL) { //error opening dictionary
        fprintf(stderr, "Error: Couldn't open dictionary.\n"); //error message
        return EXIT_FAILURE; //exit failure notice
    }

    threadArgument.qu = &q; //worker thread queue
    threadArgument.dictionaryWords = dictionaryWords; //worker thread dictionary

    // create worker threads
    for (i = 0; i < NUM_WORKERS; i++) {
        if(pthread_create(&threads[i], NULL, serviceClient, &threadArgument) != 0) {
            fprintf(stderr, "error creating thread.\n");
            return EXIT_FAILURE;
        }
    }

    while (1) {
        client_addr_size = sizeof(client_addr);
        if((welcomeSocket = accept(newSocket, (sockaddr *) &client_addr, &client_addr_size)) == -1) { //accept connection
            fprintf(stderr, "accept error\n");
            continue;
        }

        if (getnameinfo((sockaddr *) &client_addr, client_addr_size, client_name, MAX_LINE, client_port, MAX_LINE, 0) != 0) { //name info of connection
            fprintf(stderr, "error getting name information about client\n");
        } else {
            printf("accepted connection from %s: %s\n", client_name, client_port);
        }

        addToSocket(&q, welcomeSocket); //add client to socket
    }

    // join threads
    for (i = 0; i < NUM_WORKERS; i++) {
        if (pthread_join(threads[i], &ret) != 0) {
            fprintf(stderr, "join error\n");
            return EXIT_FAILURE;
        }
    }

    free(dictionaryWords);

    return EXIT_SUCCESS;
}

/* given a port number or service as string,
returns a descriptor to pass on to accept() */
int getlistenfd(char *port) {
    int listenfd, status;
    addrinfo hints, *res, *p;

    memset(&hints, 0, sizeof(hints));
    // TCP
    hints.ai_socktype = SOCK_STREAM;
    // IPv4
    hints.ai_family = AF_INET;

    if ((status = getaddrinfo(NULL, port, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo error %s\n", gai_strerror(status));
        exit(EXIT_GETADDRINFO_ERROR);
    }

    /* try to bind to the first available address/port in the list
    if we fail, try the next one */
    for (p = res; p != NULL; p = p->ai_next) {
        if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;

        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break;
    }

    // free up memory
    freeaddrinfo(res);

    if (p == NULL)
        exit(EXIT_BIND_FAILURE);

    if (listen(listenfd, BACKLOG) < 0) {
        close(listenfd);
        exit(EXIT_LISTEN_FAILURE);
    }

    return listenfd;
}

/*worker thread function*/
void *serviceClient(void *arguments) {
    int welcomeSocket;
    ssize_t bytes_read;
    char line[MAX_LINE];
    char result[MAX_LINE];
    int ok = FALSE;

    threadArg *args = arguments;
    queue *qu = args->qu;
    char **dictionaryWords = args->dictionaryWords;

    while (1) {
        welcomeSocket = removeFromSocket(qu);
        while((bytes_read = readLine(welcomeSocket, line, MAX_LINE - 1)) > 0) {
            memset(result, 0, sizeof(result)); //empty the string
            int i;
            for (i = 0; dictionaryWords[i] != NULL; i++) {
                if (strncmp(dictionaryWords[i], line, strlen(line) - 2) == 0) { //checks to see if words match
                    strncpy(result, line, strlen(line) - 2); //copies result to line
                    ok = TRUE;
                    break;
                }
            }

            if (ok) {
                strcat(result, " OK\n"); //spelled correctly
            } else {
                strncpy(result, line, strlen(line) - 2);
                strcat(result, " MISSPELLED\n"); //misspelled
            }
            write(welcomeSocket, result, sizeof(result)); //sends result back to client
            ok = FALSE;
        }

        printf("connection closed\n");
        close(welcomeSocket);
    }
}

/* FROM KERRISK
   Read characters from 'fd' until a newline is encountered. If a newline
   character is not encountered in the first (n - 1) bytes, then the excess
   characters are discarded. The returned string placed in 'buf' is
   null-terminated and includes the newline character if it was read in the
   first (n - 1) bytes. The function return value is the number of bytes
   placed in buffer (which includes the newline character if encountered,
   but excludes the terminating null byte). */
ssize_t readLine(int fd, void *buffer, size_t n) {
    ssize_t numRead;                    /* # of bytes fetched by last read() */
    size_t totRead;                     /* Total bytes read so far */
    char *buf;
    char ch;

    if (n <= 0 || buffer == NULL) {
        errno = EINVAL;
        return -1;
    }

    buf = buffer;                       /* No pointer arithmetic on "void *" */

    totRead = 0;
    for (;;) {
        numRead = read(fd, &ch, 1);

        if (numRead == -1) {
            if (errno == EINTR)         /* Interrupted --> restart read() */
                continue;
            else
                return -1;              /* Some other error */

        } else if (numRead == 0) {      /* EOF */
            if (totRead == 0)           /* No bytes read; return 0 */
                return 0;
            else                        /* Some bytes read; add '\0' */
                break;

        } else {                        /* 'numRead' must be 1 if we get here */
        if (totRead < n - 1) {      /* Discard > (n - 1) bytes */
            totRead++;
            *buf++ = ch;
        }

        if (ch == '\n')
            break;
        }
    }

    *buf = '\0';
    return totRead;
}

//makes the dictionary that'll be shared by all threads
char **makeDictionary(char *dictionaryName) {
    FILE *fp;
    char line[MAX_LINE];
    char **dict;
    int i = 0;

    if ((dict = malloc(DICTIONARY_SIZE * sizeof(char *))) == NULL) { //allocating array
        fprintf(stderr, "error allocating dictionary array.\n");
        return NULL;
    }

    fp = fopen(dictionaryName, "r"); //opens file
    while (fgets(line, sizeof(line), fp)) {
        if ((dict[i] = malloc(strlen(line) * sizeof(char *) + 1)) == NULL)//puts word into array
        {
            fprintf(stderr, "error loading word into dict array.\n");
            return NULL;
        }

        strncpy(dict[i++], line, strlen(line) - 1); //takes away new line character
    }
    fclose(fp);
    dict[i] = NULL;
    return dict;
}

//initializes queue
void initQueue(queue *q, int n) {
    q->buf = calloc(n, sizeof(int)); // capacity is n
    q->capacity = n; // size is 0
    q->size = 0; // empty buffer if front == rear
    q->front = q->rear = 0; // binary sem for locking
    sem_init(&q->mutex, 0, 1); // initially buf has n empty slots
    sem_init(&q->slots, 0, n); // initially buf has 0 items
    sem_init(&q->items, 0, 0); //items set to 0
}

void deinitQueue(queue *q) { //frees queue
    free(q->buf);
}

void addToSocket(queue *q, int item) {
    sem_wait(&q->slots); //P() on slots
    sem_wait(&q->mutex); //P() on mutex
    q->buf[(++q->rear) % (q->capacity)] = item; //critical code
    q->size++; //critical code
    sem_post(&q->mutex); //V() on mutex to free it up
    sem_post(&q->items); //V() on items to free it up
}

int removeFromSocket(queue *q) { //removes from socket to service client
    int item;
    sem_wait(&q->items); //P() on items
    sem_wait(&q->mutex); //P() on mutex
    item = q->buf[(++q->front) % (q->capacity)]; //critical code
    q->size--; //critical code
    sem_post(&q->mutex); //V() on mutex to free it up for others
    sem_post(&q->slots); //V() on slots to free it up
    return item;
}
