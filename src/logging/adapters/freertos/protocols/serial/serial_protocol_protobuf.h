#pragma once
#include "serial_protocol.h"

#include "crc32.h"
#include <PacketSerial.h>
#include <pb_decode.h>
#include <pb_encode.h>

// Template parameters:
// - TxMessage: The type of the transmit protobuf message.
// - RxMessage: The type of the receive protobuf message.
// - TxFields: The field descriptors for the transmit message.
// - RxFields: The field descriptors for the receive message.
// - MaxTxSize: Maximum size of the transmit buffer.
// - MaxRxSize: Maximum size of the receive buffer.

template <typename TxMessage, typename RxMessage, const pb_field_t *TxFields, const pb_field_t *RxFields,
          size_t MaxTxSize, size_t MaxRxSize>
class SerialProtocolProtobuf : public SerialProtocol {
  public:
    SerialProtocolProtobuf(Stream &stream, std::function<void(const RxMessage &)> message_callback);
    ~SerialProtocolProtobuf() {};
    void loop() override;

    // Method to send a protobuf message
    void send(const TxMessage &msg);

  private:
    Stream &stream_;
    PacketSerial_<> packet_serial_;
    std::function<void(const RxMessage &)> message_callback_;

    // Handle incoming packets
    void handlePacket(const uint8_t *buffer, size_t size);

    // Singleton instance for the packet handler (due to PacketSerial limitations)
    static SerialProtocolProtobuf *instance_;

    // Packet handler callback (static function)
    static void packetHandlerStatic(const uint8_t *buffer, size_t size);
};
