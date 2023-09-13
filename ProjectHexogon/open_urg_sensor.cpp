#include "open_urg_sensor.h"

int open_urg_sensor(urg_t* urg)
{
    urg_connection_type_t connection_type = URG_ETHERNET;
    long baudrate_or_port = 10940;
    const char* ip_address = "192.168.3.202";

    printf("connection_type: %d\n", connection_type);

    // \~english Connection
    if (urg_open(urg, connection_type, ip_address, baudrate_or_port) < 0) {
        printf("urg_open: %s, %ld: %s\n",
            ip_address, baudrate_or_port, urg_error(urg));
        return -1;
    }

    return 0;
}
