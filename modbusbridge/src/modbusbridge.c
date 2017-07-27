#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>

#include <modbus/modbus.h>

#define ARGUMENTS_COUNT 8


modbus_t * rtu_context = NULL;
modbus_t * tcp_context = NULL;

void print_usage() {
    printf("Usage: "
           "modbusbridge DEVICE BAUD_RATE PARITY DATA_BITS STOP_BITS IP_ADDRESS PORT\n\n"
           "Example usage:\n"
           "modbusbridge /dev/ttyS0 115200 'N' 8 1 192.168.1.1 1502\n\n");
}


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

    if (modbus_connect(tcp_context) == -1) {
           printf("Modbus TCP connection failed: %s\n", modbus_strerror(errno));
           modbus_free(rtu_context);
           modbus_free(tcp_context);
           return 2;
    }


    return 0;
}
