#include <pb_encode.h>
#include "trashscan_protocol.pb.h"

void setup() {
    Serial.begin(9600);
}

void loop() {
    BIN_STATUS bin_status = BIN_STATUS_init_default;
    bin_status.SENSOR_1 = 12.34;
    bin_status.SENSOR_2 = 23.45;
    bin_status.SENSOR_3 = 34.56;
    bin_status.SENSOR_4 = 45.67;

    uint8_t buffer[TRASHSCAN_PROTOCOL_PB_H_MAX_SIZE];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    if (pb_encode(&stream, BIN_STATUS_fields, &bin_status)) {
        Serial.write(buffer, stream.bytes_written);
    } else {
        Serial.print("Encoding failed: ");
        Serial.println(PB_GET_ERROR(&stream));
    }

    delay(1000);
}
