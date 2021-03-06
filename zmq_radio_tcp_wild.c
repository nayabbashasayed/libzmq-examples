#define ZMQ_BUILD_DRAFT_API 1
#define IF_ADDR_LENGTH 32

#include <zmq.h>
#include <errno.h>
#include <string.h>

int main(void)
{
    void *ctx = zmq_ctx_new();
    int ret;

    /* Creating the socket */
    void *socket = zmq_socket(ctx, ZMQ_RADIO);
    if (!socket) {
        printf("Returned with %d at %d\n", errno, __LINE__);
        return errno;
    }

    /* Different ways to bind to local address */
    /* TCP port 5555 on all available interfaces (endpint address): */
    //     zmq_bind(socket, "tcp://*:5555");
    /* TCP port 5555 on the local loop-back interface on all platforms.
     *      zmq_bind(socket, "tcp://127.0.0.1:5555");
     * TCP port 5555 on the first Ethernet network interface on Linux.
     *      zmq_bind(socket, "tcp://eth0:5555");
     */

    ret = zmq_bind(socket, "tcp://*:5555");
    if (ret) {
        printf("Returned with %d at %d\n", errno, __LINE__);
        return errno;
    }

    char if_address[IF_ADDR_LENGTH];
    size_t if_length = IF_ADDR_LENGTH;

    memset(if_address, 0, sizeof(if_address));
    ret = zmq_getsockopt(socket, ZMQ_LAST_ENDPOINT, if_address, &if_length);

    if (ret) {
        printf("Returned with %d at %d\n", errno, __LINE__);
        return errno;
    }

    /* Stop accepting connections on a socket */
    ret = zmq_unbind(socket, if_address);
    if (ret) {
        printf("Returned with %d at %d\n", errno, __LINE__);
        return errno;
    }

    /* Closing the socket */
    ret = zmq_close(socket);
    if (ret) {
        printf("Returned with %d at %d\n", errno, __LINE__);
        return errno;
    }

    /* To free resources (context) allocated by zmq */
    ret = zmq_ctx_term(ctx);
    if (ret) {
        printf("Returned with %d at %d\n", errno, __LINE__);
        return ret;
    }
    return 0;
}
