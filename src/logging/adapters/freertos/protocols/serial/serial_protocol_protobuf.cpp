#include "serial_protocol_protobuf.h"

template <typename TxMessage, typename RxMessage, const pb_field_t *TxFields, const pb_field_t *RxFields,
          size_t MaxTxSize, size_t MaxRxSize>
SerialProtocolProtobuf<TxMessage, RxMessage, TxFields, RxFields, MaxTxSize, MaxRxSize>
    *SerialProtocolProtobuf<TxMessage, RxMessage, TxFields, RxFields, MaxTxSize, MaxRxSize>::instance_ = nullptr;

template <typename TxMessage, typename RxMessage, const pb_field_t *TxFields, const pb_field_t *RxFields,
          size_t MaxTxSize, size_t MaxRxSize>
SerialProtocolProtobuf<TxMessage, RxMessage, TxFields, RxFields, MaxTxSize, MaxRxSize>::SerialProtocolProtobuf(
    Stream &stream, std::function<void(const RxMessage &)> message_callback)
    : stream_(stream), message_callback_(message_callback) {
    packet_serial_.setStream(&stream_);

    // Due to PacketSerial limitations, we use a static instance
    instance_ = this;

    packet_serial_.setPacketHandler(&SerialProtocolProtobuf::packetHandlerStatic);
}

template <typename TxMessage, typename RxMessage, const pb_field_t *TxFields, const pb_field_t *RxFields,
          size_t MaxTxSize, size_t MaxRxSize>
void SerialProtocolProtobuf<TxMessage, RxMessage, TxFields, RxFields, MaxTxSize, MaxRxSize>::loop() {
    packet_serial_.update();
}

template <typename TxMessage, typename RxMessage, const pb_field_t *TxFields, const pb_field_t *RxFields,
          size_t MaxTxSize, size_t MaxRxSize>
void SerialProtocolProtobuf<TxMessage, RxMessage, TxFields, RxFields, MaxTxSize, MaxRxSize>::send(
    const TxMessage &msg) {
    uint8_t buffer[MaxTxSize + 4]; // Additional 4 bytes for CRC32

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer) - 4);

    if (!pb_encode(&stream, TxFields, &msg)) {
        // Handle encoding error
        Serial.println("Encoding failed");
        return;
    }

    // Compute and append CRC32
    uint32_t crc = 0;
    crc32(buffer, stream.bytes_written, &crc);
    buffer[stream.bytes_written + 0] = (crc >> 0) & 0xFF;
    buffer[stream.bytes_written + 1] = (crc >> 8) & 0xFF;
    buffer[stream.bytes_written + 2] = (crc >> 16) & 0xFF;
    buffer[stream.bytes_written + 3] = (crc >> 24) & 0xFF;

    // Send the packet
    packet_serial_.send(buffer, stream.bytes_written + 4);
    stream_.flush();
}

template <typename TxMessage, typename RxMessage, const pb_field_t *TxFields, const pb_field_t *RxFields,
          size_t MaxTxSize, size_t MaxRxSize>
void SerialProtocolProtobuf<TxMessage, RxMessage, TxFields, RxFields, MaxTxSize, MaxRxSize>::handlePacket(
    const uint8_t *buffer, size_t size) {
    if (size <= 4) {
        // Ignore packets that are too small to contain data and CRC32
        return;
    }

    // Verify CRC32
    uint32_t expected_crc = 0;
    crc32(buffer, size - 4, &expected_crc);

    uint32_t received_crc =
        buffer[size - 4] | (buffer[size - 3] << 8) | (buffer[size - 2] << 16) | (buffer[size - 1] << 24);

    if (expected_crc != received_crc) {
        // CRC mismatch
        Serial.println("CRC mismatch");
        return;
    }

    // Decode the protobuf message
    pb_istream_t stream = pb_istream_from_buffer(buffer, size - 4);
    RxMessage msg = {};

    if (!pb_decode(&stream, RxFields, &msg)) {
        // Decoding failed
        Serial.println("Decoding failed");
        return;
    }

    // Call the message callback
    message_callback_(msg);
}

template <typename TxMessage, typename RxMessage, const pb_field_t *TxFields, const pb_field_t *RxFields,
          size_t MaxTxSize, size_t MaxRxSize>
void SerialProtocolProtobuf<TxMessage, RxMessage, TxFields, RxFields, MaxTxSize, MaxRxSize>::packetHandlerStatic(
    const uint8_t *buffer, size_t size) {
    if (instance_) {
        instance_->handlePacket(buffer, size);
    }
}
