#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include <modbus/modbus.h>

#define ARGUMENTS_COUNT 8

#define TCP_MAX_CONNECTIONS 512


modbus_t * rtu_context = NULL;
modbus_t * tcp_context = NULL;


bool is_valid_baud_rate(int baud_rate) {
    return  baud_rate == 9600  ||
            baud_rate == 19200 ||
            baud_rate == 57600 ||
            baud_rate == 115200;
}

bool is_valid_parity(char parity) {
    return  parity == 'N' ||
            parity == 'E' ||
            parity == 'O';
}


bool is_valid_data_bits(unsigned char data_bits) {
    return data_bits >= 5 && data_bits <= 8;
}


bool is_valid_stop_bits(unsigned char stop_bits) {
    return  stop_bits == 0 ||
            stop_bits == 1;
}


bool is_valid_ip_address(const char * ip_address) {
    struct in_addr temp_address;
    return inet_pton(AF_INET, ip_address, &temp_address);
}


bool is_valid_port(unsigned int port) {
    return port >= 0 && port <= 65535;
}


void print_usage() {
    printf("Usage: "
           "modbusbridge DEVICE BAUD_RATE PARITY DATA_BITS STOP_BITS IP_ADDRESS PORT\n\n"
           "Example usage:\n"
           "modbusbridge /dev/ttyS0 115200 'N' 8 1 192.168.1.1 1502\n\n");
}


void termination_handler(int signum) {
    modbus_close(rtu_context);
    modbus_free(rtu_context);
    rtu_context = NULL;

    modbus_close(tcp_context);
    modbus_free(tcp_context);
    tcp_context = NULL;

    exit(signum);
}


int main(int argc, char * argv[]) {
    if (argc != ARGUMENTS_COUNT) {
        printf("Invalid arguments count.\n\n");
        print_usage();
        return 1;
    }

    //RTU
    const char * device;
    int baud_rate;
    char parity;
    unsigned char data_bits;
    unsigned char stop_bits;

    //TCP/IP
    const char * ip_address;
    unsigned int port;


    //RTU arguments validation
    device = argv[1];

    baud_rate = atoi(argv[2]);
    if (is_valid_baud_rate(baud_rate) == false) {
        printf("%s is not a valid baud rate.\n", argv[2]);
        return 1;
    }

    parity = argv[3][0];
    if (is_valid_parity(parity) == false) {
        printf("%s is not a valid parity value.\n", argv[3]);
        return 1;
    }

    data_bits = atoi(argv[4]);
    if (is_valid_data_bits(data_bits) == false) {
        printf("%s is not a valid data_bits value.\n", argv[4]);
        return 1;
    }

    stop_bits = atoi(argv[5]);
    if (is_valid_stop_bits(stop_bits) == false) {
        printf("%s is not a valid stop_bits value.\n", argv[5]);
        return 1;
    }


    //TCP/IP arguments validation
    ip_address = argv[6];
    if (is_valid_ip_address(ip_address) == false) {
        printf("%s is not a valid ip address.\n", argv[6]);
        return 1;
    }

    port = atoi(argv[7]);
    if (is_valid_port(port) == false) {
        printf ("%s is not a valid port.\n", argv[7]);
        return 1;
    }


    //RTU context allocation
    rtu_context = modbus_new_rtu(device, baud_rate, parity, data_bits, stop_bits);

    if (rtu_context == NULL) {
        printf("Unable to create the Modbus RTU context: %s\n", modbus_strerror(errno));
        return 2;
    }

    if (modbus_connect(rtu_context) == -1) {
           printf("Modbus RTU connection failed: %s\n", modbus_strerror(errno));
           modbus_free(rtu_context);
           return 2;
    }


    //TCP context allocation
    tcp_context = modbus_new_tcp(ip_address, port);

    if (tcp_context == NULL) {
        printf("Unable to create Modbus TCP context: %s\n", modbus_strerror(errno));
        return 2;
    }


    struct sigaction signal_action;
    signal_action.sa_handler = termination_handler;
    sigemptyset (&signal_action.sa_mask);
    signal_action.sa_flags = 0;

    sigaction(SIGINT, &signal_action, NULL);
    sigaction(SIGTERM, &signal_action, NULL);
    sigaction(SIGHUP, &signal_action, NULL);


    int server_socket;
    //also clear the fd sets
    fd_set connections;
    FD_ZERO(&connections);

    fd_set active_connections;
    FD_ZERO(&active_connections);

    struct sockaddr_in address;
    socklen_t address_length = sizeof(address);

    int new_fd;

    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
    int query_length;
    int raw_query_length;


    uint8_t response[MODBUS_RTU_MAX_ADU_LENGTH];
    int response_length;
    int raw_response_length;

    int slave_id;
    int data_start_index;
    int exception;


    server_socket = modbus_tcp_listen(tcp_context, TCP_MAX_CONNECTIONS);

    if (server_socket == -1) {
        printf("Modbus TCP listen failed: %s\n", modbus_strerror(errno));
        termination_handler(3);
    }

    FD_SET(server_socket, &active_connections);

    while (1) {
        connections = active_connections;

        if (select(FD_SETSIZE, &connections, NULL, NULL, NULL) == -1) {
            printf("Select error: %s\n", strerror(errno));
            termination_handler(4);
        }

        for (int i = 0; i < FD_SETSIZE; ++i) {
            if (FD_ISSET(i, &connections)) {

                //Connection on server socket -
                //in other words new client is asking for a new connection
                if (i == server_socket) {

                    //clear the temporary address variable
                    memset(&address, 0, address_length);
                    new_fd = accept(server_socket, (struct sockaddr *)&address, &address_length);

                    if (new_fd == -1) {
                        printf("Accept error: %s\n", strerror(errno));
                    } else {
                        FD_SET(new_fd, &active_connections);

                        printf("New connection from %s:%d on socket %d\n", inet_ntoa(address.sin_addr), address.sin_port, new_fd);
                    }

                // Data arriving on an already-connected socket.
                } else {
                    //reset the exection value
                    exception = 0;

                    modbus_set_socket(tcp_context, i);
                    query_length = modbus_receive(tcp_context, query);

                    //Close the connection
                    if (query_length == -1) {
                        printf("Connection closed on socker %d\n", i);

                        //close the socket and remove it from fd set
                        close(i);
                        FD_CLR(i, &active_connections);

                    //Parse the tcp request and send it through the rtu context
                    } else {
                        data_start_index = modbus_get_header_length(tcp_context) - 1;

                        slave_id = query[data_start_index];
                        raw_query_length = query_length - data_start_index - modbus_get_checksum_length(tcp_context);

                        modbus_flush(rtu_context);
                        modbus_set_slave(rtu_context, slave_id);

                        if (modbus_send_raw_request(rtu_context, query + data_start_index, raw_query_length) == -1) {
                            printf("Modbus RTU send request error: %s\n", modbus_strerror(errno));
                            exception = errno;
                        } else {
                            response_length = modbus_receive_confirmation(rtu_context, response);

                            if (response_length == -1) {
                                printf("Modbus RTU receive responsed error: %s\n", modbus_strerror(errno));
                                exception = errno;
                            } else {
                                data_start_index = modbus_get_header_length(rtu_context) - 1;
                                raw_response_length = response_length - data_start_index - modbus_get_checksum_length(rtu_context);
                                modbus_reply_raw_response(tcp_context, query, query_length, response + data_start_index, response_length);
                            }
                        }

                        //If exists an exception send it through the tcp context
                        if (exception != 0) {
                            if (exception > MODBUS_ENOBASE && MODBUS_ENOBASE < (MODBUS_ENOBASE + MODBUS_EXCEPTION_MAX)) {
                                exception -= MODBUS_ENOBASE;
                            } else {
                                exception = EMBXSFAIL;
                            }

                            modbus_reply_exception(tcp_context, query, exception);
                        }
                    }
                }
            }
        }
    }

    termination_handler(0);
}
