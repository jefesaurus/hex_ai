#pragma once

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>
#include <experimental/filesystem>
#include <fstream>

#include "base/logging.h"

// Read a plain text file as text format protobuf of the given type.
template <class ProtoType>
std::optional<ProtoType> ReadTextProto(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.good()) {
    LOG(ERROR) << "Reading: '" << filename << "' failed: " << strerror(errno);
    return std::nullopt;
  }
  ProtoType proto;
  google::protobuf::io::IstreamInputStream stream(&file);
  if (!google::protobuf::TextFormat::Parse(&stream, &proto)) {
    LOG(ERROR) << "Could not parse file: " << filename
               << " as text proto of type: " << proto.GetTypeName();
    return std::nullopt;
  }
  return proto;
}

// Write a plain text file with the protobuf's text format.
bool WriteTextProto(const google::protobuf::Message& message,
                    const std::string& filename) {
  auto dir = std::experimental::filesystem::path(filename).parent_path();
  if (!dir.empty()) {
    std::experimental::filesystem::create_directory(dir);
  }

  std::ofstream file(filename);
  if (!file.good()) {
    LOG(ERROR) << "Couldn't create file: '" << filename
               << "' failed: " << strerror(errno);
    return false;
  }

  google::protobuf::io::OstreamOutputStream stream(&file);
  if (!google::protobuf::TextFormat::Print(message, &stream)) {
    LOG(ERROR) << "Could not print text proto.";
    return false;
  }
  return true;
}

// Read a binary file as a protobuf of the given type.
template <class ProtoType>
std::optional<ProtoType> ReadBinaryProto(const std::string& filename) {
  std::ifstream infile(filename, std::ifstream::binary);
  if (!infile.good()) {
    LOG(ERROR) << "Reading: '" << filename << "' failed: " << strerror(errno);
    return std::nullopt;
  }

  google::protobuf::io::IstreamInputStream input_stream(&infile);
  google::protobuf::io::CodedInputStream stream(&input_stream);

  ProtoType proto;
  static constexpr uint64_t bytes_limit = 1024 * 1024 * 1024;
  stream.SetTotalBytesLimit(bytes_limit, bytes_limit);
  if (!proto.ParseFromCodedStream(&stream) || !stream.ConsumedEntireMessage()) {
    LOG(ERROR) << "Could not parse file: " << filename
               << " as binary proto of type: " << proto.GetTypeName();
    return std::nullopt;
  }
  return proto;
}

// Write a binary file from the given protobuf.
bool WriteBinaryProto(const google::protobuf::Message& message,
                      const std::string& filename) {
  auto dir = std::experimental::filesystem::path(filename).parent_path();
  if (!dir.empty()) {
    std::experimental::filesystem::create_directory(dir);
  }
  std::ofstream file(filename, std::ofstream::binary);
  if (!file.good()) {
    LOG(ERROR) << "Couldn't create file: '" << filename
               << "' failed: " << strerror(errno);
    return false;
  }
  if (!message.SerializeToOstream(&file)) {
    LOG(ERROR) << "Could not write binary proto.";
    return false;
  }
  return true;
}
