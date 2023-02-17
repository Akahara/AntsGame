// ChatGPT magic, du moins en partie.
#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

class ProtocolHandler {
public:
  ProtocolHandler() : state_(kStateHeader), size_(0), header_read_(0) {}

  // Process the received data and return the extracted payloads.
  std::vector<std::vector<uint8_t>>
  ProcessData(const std::vector<uint8_t> &data) {
    std::vector<std::vector<uint8_t>> payloads;

    for (const auto byte : data) {
      switch (state_) {
      case kStateHeader:
        size_ = (size_ << 8) | byte;
        header_read_++;

        if (header_read_ == 4) {
          state_ = kStatePayload;
          payload_.reserve(size_);
        }
        break;
      case kStatePayload:
        payload_.push_back(byte);

        if (payload_.size() == size_) {
          payloads.push_back(payload_);
          payload_.clear();
          size_ = 0;
          header_read_ = 0;
          state_ = kStateHeader;
        }
        break;
      }
    }

    return payloads;
  }

private:
  // The states of the state machine.
  enum State { kStateHeader, kStatePayload };

  // The current state of the state machine.
  State state_;

  // The size of the next payload to be read.
  std::uint32_t size_;

  // The amount of bytes of the header read.
  unsigned int header_read_;

  // The buffer that stores the current payload being read.
  std::vector<uint8_t> payload_;
};