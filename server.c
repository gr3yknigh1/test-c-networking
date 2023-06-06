#include <memory.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define RET_ERR -1
#define LISTEN_PORT 8080
#define CONNECTION_TRYIES 5

static bool IS_SERVER_RUNNING = true;


int main() {
    int socket_fd;
    int ret_code, on = 1;

    // NOTE: Creating socket
    socket_fd = socket(AF_INET6, SOCK_STREAM, 0);
    if (socket_fd == RET_ERR) {
        fprintf(stderr, "socket() failed: %i\n", socket_fd);
        return EXIT_FAILURE;
    }

    // NOTE: Setting socket to be reusable
    ret_code = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (ret_code == RET_ERR) {
        fprintf(stderr, "setsockopt() failed: %i\n", ret_code);
        return EXIT_FAILURE;
    }

    // NOTE: Bind socket
    struct sockaddr_in6 addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
    addr.sin6_port = htons(LISTEN_PORT);

    ret_code = bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret_code == RET_ERR) {
        fprintf(stderr, "bind() failed");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    ret_code = listen(socket_fd, CONNECTION_TRYIES);
    if (ret_code == RET_ERR) {
        fprintf(stderr, "listen() failed: %i\n", ret_code);
        close(socket_fd);
        return EXIT_FAILURE;
    }

    fprintf(stdout, "Server successfully binded to %i\n", LISTEN_PORT);

    while (IS_SERVER_RUNNING) {
        fprintf(stdout, "Waiting for request...\n");
        int accept_fd = accept(socket_fd, NULL, NULL);

        if (accept_fd == RET_ERR) {
            fprintf(stderr, "accept() failed: %i\n", accept_fd);
            continue;
        }

        fprintf(stdout, "Accepted request!\n");

        char req_buffer[1024];
        ret_code = recv(accept_fd, req_buffer, sizeof(req_buffer), 0);
        if (ret_code == RET_ERR) {
            fprintf(stderr, "recv() failed: %i\n", ret_code);
            continue;
        }

        fprintf(stdout, "Request:\n%s\n\n", req_buffer);

        char res_buffer[1024] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 5\r\n\r\nHihi\n";
        ret_code = send(accept_fd, res_buffer, sizeof(res_buffer), 0);
        if (ret_code == RET_ERR) {
            fprintf(stderr, "send() failed: %i\n", ret_code);
            close(socket_fd);
            close(accept_fd);
            return EXIT_FAILURE;
        }

        close(accept_fd);
        break;
    }

    close(socket_fd);

    return EXIT_SUCCESS;
}

