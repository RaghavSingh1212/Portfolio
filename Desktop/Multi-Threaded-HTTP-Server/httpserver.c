#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>
#include <ctype.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/stat.h>
#include "queue.h"
#include "linkedList.h"
#include "rwlock.h"
#include "protocol.h"
#include "debug.h"
#include "asgn2_helper_funcs.h"

#define BUFFER_SIZE 8000

List l;
queue_t *q;
pthread_mutex_t mutex;

struct add {
    char method[9];
    char URI[65];
    char version[9];
    char key[129];
    char value[129];
    char mkc[129];
    int r_val;
};

// Function prototypes
void *workerFunction(void *arg);
int handleConnection(int connfd, char *buffer, struct add *r);
rwlock_t *manage_locks(struct add *r);
void handle_get_request(int connfd, struct add *r, rwlock_t *rwlock);
void handle_put_request(
    int connfd, char *buffer, struct add *r, rwlock_t *rwlock, int lol, int base_val);
void errror_handle(int connfd, struct add *r);

// Existing function definitions
void extractHeader(const char *cursor, regmatch_t *header_match, struct add *r, const char *type);
int verifyHeaderCompletion(regex_t *rorgj, const char *cursor);
void decodeRequestLine(struct add *r, const char *buffer, regmatch_t *pmatch);
void processHeader(const char *cursor, regmatch_t *header_match, struct add *r);
int process_request(struct add *r, char *buffer);
int parseCommandLineArgs(int argc, char **argv, int *threads, int *port);
void initializeServer(int threads);
void createWorkerThreads(pthread_t *workers, int threads);
int initializeListener(Listener_Socket *listener, int port);
void serverMainLoop(Listener_Socket *listener);
int main(int argc, char **argv);

void extractHeader(const char *cursor, regmatch_t *header_match, struct add *r, const char *type) {
    int key_length = header_match[1].rm_eo - header_match[1].rm_so;
    int value_length = header_match[2].rm_eo - header_match[2].rm_so;
    char value_buffer[129] = { 0 };

    if (strcmp(type, "Content-Length") == 0) {
        memcpy(r->key, cursor + header_match[1].rm_so, key_length);
        memcpy(r->value, cursor + header_match[2].rm_so, value_length);
    } else if (strcmp(type, "Request-Id") == 0) {
        memcpy(r->mkc, cursor + header_match[1].rm_so, key_length);
        memcpy(value_buffer, cursor + header_match[2].rm_so, value_length);
        r->r_val = atoi(value_buffer);
    }
}

int verifyHeaderCompletion(regex_t *rorgj, const char *cursor) {
    regmatch_t rorgj_match[2];
    if (regexec(rorgj, cursor, 2, rorgj_match, 0) == 0) {
        if (cursor != cursor + rorgj_match[0].rm_so) {
            fprintf(stderr, "error extracting message-body\n");
            return -1;
        }
        return 0;
    } else {
        return -1;
    }
}

void decodeRequestLine(struct add *r, const char *buffer, regmatch_t *pmatch) {
    int method_length = pmatch[1].rm_eo - pmatch[1].rm_so;
    memcpy(r->method, buffer + pmatch[1].rm_so, method_length);

    int uri_length = pmatch[2].rm_eo - pmatch[2].rm_so;
    memcpy(r->URI, buffer + pmatch[2].rm_so, uri_length);

    int version_length = pmatch[3].rm_eo - pmatch[3].rm_so;
    memcpy(r->version, buffer + pmatch[3].rm_so, version_length);
}

void processHeader(const char *cursor, regmatch_t *header_match, struct add *r) {
    char header_type[128];
    memset(header_type, '\0', sizeof(header_type));
    strncpy(
        header_type, cursor + header_match[1].rm_so, header_match[1].rm_eo - header_match[1].rm_so);

    if (strcmp(header_type, "Content-Length") == 0 || strcmp(header_type, "Request-Id") == 0) {
        extractHeader(cursor, header_match, r, header_type);
    }
}

int process_request(struct add *r, char *buffer) {
    regex_t regex;
    if (regcomp(&regex, REQUEST_LINE_REGEX, REG_EXTENDED) != 0) {
        return -1;
    }
    regmatch_t pmatch[4];
    if (regexec(&regex, buffer, 4, pmatch, 0) != 0) {
        regfree(&regex);
        return -1;
    }
    decodeRequestLine(r, buffer, pmatch);
    regex_t header;
    if (regcomp(&header, HEADER_FIELD_REGEX, REG_EXTENDED) != 0) {
        return -1;
    }
    char *cursor = buffer + pmatch[0].rm_eo;
    int finalpos = pmatch[0].rm_eo;
    regex_t rorgj;
    const char *rorgj_pattern = "\r\n(.*)";
    if (regcomp(&rorgj, rorgj_pattern, REG_EXTENDED) != 0) {
        return -1;
    }
    regmatch_t header_match[3];
    while (regexec(&header, cursor, 3, header_match, 0) == 0) {
        processHeader(cursor, header_match, r);
        cursor += header_match[0].rm_eo;
        finalpos += header_match[0].rm_eo;
    }
    int result = verifyHeaderCompletion(&rorgj, cursor);
    if (result != 0) {
        regfree(&regex);
        regfree(&header);
        regfree(&rorgj);
        return -1;
    }
    regfree(&regex);
    regfree(&header);
    regfree(&rorgj);
    return finalpos + 2;
}

int parseCommandLineArgs(int argc, char **argv, int *threads, int *port) {
    int obs;
    while ((obs = getopt(argc, argv, "t:")) != -1) {
        if (obs == 't') {
            *threads = atoi(optarg);
        } else {
            fprintf(stderr, "Invalid thread count\n");
            return -1;
        }
    }
    if (optind >= argc) {
        fprintf(stderr, "Port not specified\n");
        return -1;
    }
    *port = atoi(argv[optind]);
    return 0;
}

void initializeServer(int threads) {
    l = newList();
    pthread_mutex_init(&mutex, NULL);
    q = queue_new(threads);
}

void createWorkerThreads(pthread_t *workers, int threads) {
    for (int i = 0; i < threads; ++i) {
        pthread_create(&workers[i], NULL, workerFunction, NULL);
    }
}

int initializeListener(Listener_Socket *listener, int port) {
    int listening = listener_init(listener, port);
    if (listening != 0) {
        fprintf(stderr, "Invalid Port\n");
        return 1;
    }
    return 0;
}

void serverMainLoop(Listener_Socket *listener) {
    while (1) {
        uintptr_t socket = listener_accept(listener);
        queue_push(q, (void *) socket);
    }
}

void *workerFunction(void *arg) {
    (void) arg;

    char buffer[BUFFER_SIZE];
    while (1) {
        int connfd;
        queue_pop(q, (void **) &connfd);
        memset(buffer, 0, BUFFER_SIZE);
        int lol = read_until(connfd, buffer, 2048, "\r\n\r\n");
        struct add r;
        memset(&r, 0, sizeof(struct add));
        int base_val = handleConnection(connfd, buffer, &r);
        if (base_val == -1) {
            close(connfd);
            continue;
        }
        rwlock_t *rwlock = manage_locks(&r);
        if (strcmp(r.method, "GET") == 0) {
            handle_get_request(connfd, &r, rwlock);
        } else if (strcmp(r.method, "PUT") == 0) {
            handle_put_request(connfd, buffer, &r, rwlock, lol, base_val);
        } else {
            const char *response
                = "HTTP/1.1 501 Not Implemented\r\nContent-Length: 16\r\n\r\nNot Implemented\n";
            write(connfd, response, strlen(response));
            fprintf(stderr, "%s,/%s,%d,%d\n", r.method, r.URI, 501, r.r_val);
        }
        memset(buffer, 0, BUFFER_SIZE);
        memset(&r, 0, sizeof(struct add));
        close(connfd);
    }
    return NULL;
}

void errror_handle(int connfd, struct add *r) {
    switch (errno) {
    case ENOENT: {
        const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 10\r\n\r\nNot Found\n";
        int responselen = strlen(response);
        write(connfd, response, responselen);
        fprintf(stderr, "%s,/%s,%d,%d\n", r->method, r->URI, 404, r->r_val);
        break;
    }
    case EACCES:
    case EISDIR: {
        printf("Detected a directory file in errror_handle.");
        const char *response = "HTTP/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n";
        int responselen = strlen(response);
        write(connfd, response, responselen);
        fprintf(stderr, "%s,/%s,%d,%d\n", r->method, r->URI, 403, r->r_val);
        break;
    }
    default: {
        const char *response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: "
                               "22\r\n\r\nInternal Server Error\n";
        int responselen = strlen(response);
        write(connfd, response, responselen);
        fprintf(stderr, "%s,/%s,%d,%d\n", r->method, r->URI, 500, r->r_val);
        break;
    }
    }
}

int handleConnection(int connfd, char *buffer, struct add *r) {
    int base_val = process_request(r, buffer);
    if (base_val > 2048) {
        const char *response
            = "HTTP/1.1 400 Bad Request\r\nContent-Length: 12\r\n\r\nBad Request\n";
        write(connfd, response, strlen(response));
        memset(buffer, '\0', BUFFER_SIZE);
        read_n_bytes(connfd, buffer, BUFFER_SIZE);
        fprintf(stderr, "Zero Buffer is %s\n", buffer);
        close(connfd);
        return -1;
    }
    if (base_val == -1) {
        const char *response
            = "HTTP/1.1 400 Bad Request\r\nContent-Length: 12\r\n\r\nBad Request\n";
        write(connfd, response, strlen(response));
        close(connfd);
        return -1;
    }
    return base_val;
}

rwlock_t *manage_locks(struct add *r) {
    rwlock_t *rwlock = NULL;
    pthread_mutex_lock(&mutex);
    moveFront(l);
    while (curr_index(l) != -1) {
        if (strcmp((char *) getfile(l), r->URI) == 0) {
            rwlock = (rwlock_t *) getlock(l);
            break;
        }
        moveNext(l);
    }
    if (rwlock == NULL) {
        rwlock = rwlock_new(2, 1);
        append(l, r->URI, rwlock);
    }
    pthread_mutex_unlock(&mutex);
    return rwlock;
}

void handle_get_request(int connfd, struct add *r, rwlock_t *rwlock) {
    reader_lock(rwlock);
    if (strcmp(r->version, "HTTP/1.1") != 0) {
        const char *response = "HTTP/1.1 505 Version Not Supported\r\nContent-Length: "
                               "22\r\n\r\nVersion Not Supported\n";
        write(connfd, response, strlen(response));
        fprintf(stderr, "%s,/%s,%d,%d\n", r->method, r->URI, 505, r->r_val);
        reader_unlock(rwlock);
        close(connfd);
        return;
    }
    int fd = open(r->URI, O_RDWR);
    if (fd == -1) {
        errror_handle(connfd, r);
        reader_unlock(rwlock);
        close(connfd);
        return;
    }
    struct stat fileStat;
    fstat(fd, &fileStat);
    uint64_t filesize = fileStat.st_size;

    char fsize[129] = { "\0" };

    char *response = "HTTP/1.1 200 OK\r\nContent-Length: ";
    write_n_bytes(connfd, response, strlen(response));
    write_n_bytes(connfd, fsize, strlen(fsize));
    write_n_bytes(connfd, "\r\n\r\n", 4);
    pass_n_bytes(fd, connfd, filesize);
    fprintf(stderr, "%s,/%s,%d,%d\n", r->method, r->URI, 200, r->r_val);
    reader_unlock(rwlock);
    close(fd);
}

void handle_put_request(
    int connfd, char *buffer, struct add *r, rwlock_t *rwlock, int lol, int base_val) {
    writer_lock(rwlock);
    if (strcmp(r->version, "HTTP/1.1") != 0) {
        const char *response = "HTTP/1.1 505 Version Not Supported\r\nContent-Length: "
                               "22\r\n\r\nVersion Not Supported\n";
        write(connfd, response, strlen(response));
        fprintf(stderr, "%s,/%s,%d,%d\n", r->method, r->URI, 505, r->r_val);
        writer_unlock(rwlock);
        close(connfd);
        return;
    }
    if (strcmp(r->key, "Content-Length") != 0) {
        const char *response
            = "HTTP/1.1 400 Bad Request\r\nContent-Length: 12\r\n\r\nBad Request\n";
        write(connfd, response, strlen(response));
        fprintf(stderr, "%s,/%s,%d,%d\n", r->method, r->URI, 400, r->r_val);
        writer_unlock(rwlock);
        close(connfd);
        return;
    }
    bool decode = false;
    int fd = open(r->URI, O_WRONLY | O_TRUNC, 0666);
    if (fd == -1) {
        fd = open(r->URI, O_CREAT | O_WRONLY | O_TRUNC, 0666);
        if (fd == -1) {
            errror_handle(connfd, r);
            writer_unlock(rwlock);
            close(connfd);
            return;
        }
        decode = true;
    }
    int spp = lol - base_val;
    int pqlw = write_n_bytes(fd, buffer + base_val, spp);
    int num_pqlw = atoi(r->value);
    pass_n_bytes(connfd, fd, num_pqlw - pqlw);
    if (decode) {
        const char *response = "HTTP/1.1 201 Created\r\nContent-Length: 8\r\n\r\nCreated\n";
        int response_length = strlen(response);
        write(connfd, response, response_length);
        fprintf(stderr, "%s,/%s,%d,%d\n", r->method, r->URI, 201, r->r_val);
    } else {
        const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 3\r\n\r\nOK\n";
        int response_length = strlen(response);
        write(connfd, response, response_length);
        fprintf(stderr, "%s,/%s,%d,%d\n", r->method, r->URI, 200, r->r_val);
    }
    writer_unlock(rwlock);
    close(fd);
}

int main(int argc, char **argv) {
    int threads = 4; // Default thread count
    int port;

    if (parseCommandLineArgs(argc, argv, &threads, &port) != 0) {
        return -1;
    }

    initializeServer(threads);

    pthread_t workers[threads]; // Array to hold thread IDs
    createWorkerThreads(workers, threads);

    Listener_Socket listener;
    if (initializeListener(&listener, port) != 0) {
        return 1;
    }

    serverMainLoop(&listener);

    for (int i = 0; i < threads; ++i) {
        pthread_join(workers[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}
