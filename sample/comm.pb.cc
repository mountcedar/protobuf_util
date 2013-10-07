// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "comm.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace comm {

namespace {

const ::google::protobuf::Descriptor* Message_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Message_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* Message_Type_descriptor_ = NULL;
const ::google::protobuf::Descriptor* Data_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Data_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_comm_2eproto() {
  protobuf_AddDesc_comm_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "comm.proto");
  GOOGLE_CHECK(file != NULL);
  Message_descriptor_ = file->message_type(0);
  static const int Message_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, data_),
  };
  Message_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Message_descriptor_,
      Message::default_instance_,
      Message_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Message));
  Message_Type_descriptor_ = Message_descriptor_->enum_type(0);
  Data_descriptor_ = file->message_type(1);
  static const int Data_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Data, status_),
  };
  Data_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Data_descriptor_,
      Data::default_instance_,
      Data_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Data, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Data, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Data));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_comm_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Message_descriptor_, &Message::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Data_descriptor_, &Data::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_comm_2eproto() {
  delete Message::default_instance_;
  delete Message_reflection_;
  delete Data::default_instance_;
  delete Data_reflection_;
}

void protobuf_AddDesc_comm_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\ncomm.proto\022\004comm\"}\n\007Message\022\n\n\002id\030\001 \002("
    "\005\022 \n\004type\030\002 \002(\0162\022.comm.Message.Type\022\030\n\004d"
    "ata\030\003 \003(\0132\n.comm.Data\"*\n\004Type\022\007\n\003ACK\020\000\022\r"
    "\n\tSUCCEEDED\020\001\022\n\n\006FAILED\020\002\"\026\n\004Data\022\016\n\006sta"
    "tus\030\001 \001(\t", 169);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "comm.proto", &protobuf_RegisterTypes);
  Message::default_instance_ = new Message();
  Data::default_instance_ = new Data();
  Message::default_instance_->InitAsDefaultInstance();
  Data::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_comm_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_comm_2eproto {
  StaticDescriptorInitializer_comm_2eproto() {
    protobuf_AddDesc_comm_2eproto();
  }
} static_descriptor_initializer_comm_2eproto_;


// ===================================================================

const ::google::protobuf::EnumDescriptor* Message_Type_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Message_Type_descriptor_;
}
bool Message_Type_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

#ifndef _MSC_VER
const Message_Type Message::ACK;
const Message_Type Message::SUCCEEDED;
const Message_Type Message::FAILED;
const Message_Type Message::Type_MIN;
const Message_Type Message::Type_MAX;
const int Message::Type_ARRAYSIZE;
#endif  // _MSC_VER
#ifndef _MSC_VER
const int Message::kIdFieldNumber;
const int Message::kTypeFieldNumber;
const int Message::kDataFieldNumber;
#endif  // !_MSC_VER

Message::Message()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Message::InitAsDefaultInstance() {
}

Message::Message(const Message& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Message::SharedCtor() {
  _cached_size_ = 0;
  id_ = 0;
  type_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Message::~Message() {
  SharedDtor();
}

void Message::SharedDtor() {
  if (this != default_instance_) {
  }
}

void Message::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Message::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Message_descriptor_;
}

const Message& Message::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_comm_2eproto();  return *default_instance_;
}

Message* Message::default_instance_ = NULL;

Message* Message::New() const {
  return new Message;
}

void Message::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    id_ = 0;
    type_ = 0;
  }
  data_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Message::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 id = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &id_)));
          set_has_id();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_type;
        break;
      }
      
      // required .comm.Message.Type type = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_type:
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::comm::Message_Type_IsValid(value)) {
            set_type(static_cast< ::comm::Message_Type >(value));
          } else {
            mutable_unknown_fields()->AddVarint(2, value);
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_data;
        break;
      }
      
      // repeated .comm.Data data = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_data:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_data()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_data;
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Message::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int32 id = 1;
  if (has_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->id(), output);
  }
  
  // required .comm.Message.Type type = 2;
  if (has_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      2, this->type(), output);
  }
  
  // repeated .comm.Data data = 3;
  for (int i = 0; i < this->data_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      3, this->data(i), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Message::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int32 id = 1;
  if (has_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->id(), target);
  }
  
  // required .comm.Message.Type type = 2;
  if (has_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      2, this->type(), target);
  }
  
  // repeated .comm.Data data = 3;
  for (int i = 0; i < this->data_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        3, this->data(i), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Message::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 id = 1;
    if (has_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->id());
    }
    
    // required .comm.Message.Type type = 2;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->type());
    }
    
  }
  // repeated .comm.Data data = 3;
  total_size += 1 * this->data_size();
  for (int i = 0; i < this->data_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->data(i));
  }
  
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Message::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Message* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Message*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Message::MergeFrom(const Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  data_.MergeFrom(from.data_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_id()) {
      set_id(from.id());
    }
    if (from.has_type()) {
      set_type(from.type());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Message::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Message::CopyFrom(const Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Message::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;
  
  return true;
}

void Message::Swap(Message* other) {
  if (other != this) {
    std::swap(id_, other->id_);
    std::swap(type_, other->type_);
    data_.Swap(&other->data_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Message::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Message_descriptor_;
  metadata.reflection = Message_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int Data::kStatusFieldNumber;
#endif  // !_MSC_VER

Data::Data()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Data::InitAsDefaultInstance() {
}

Data::Data(const Data& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Data::SharedCtor() {
  _cached_size_ = 0;
  status_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Data::~Data() {
  SharedDtor();
}

void Data::SharedDtor() {
  if (status_ != &::google::protobuf::internal::kEmptyString) {
    delete status_;
  }
  if (this != default_instance_) {
  }
}

void Data::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Data::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Data_descriptor_;
}

const Data& Data::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_comm_2eproto();  return *default_instance_;
}

Data* Data::default_instance_ = NULL;

Data* Data::New() const {
  return new Data;
}

void Data::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_status()) {
      if (status_ != &::google::protobuf::internal::kEmptyString) {
        status_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Data::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional string status = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_status()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->status().data(), this->status().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Data::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional string status = 1;
  if (has_status()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->status().data(), this->status().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->status(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Data::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional string status = 1;
  if (has_status()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->status().data(), this->status().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->status(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Data::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional string status = 1;
    if (has_status()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->status());
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Data::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Data* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Data*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Data::MergeFrom(const Data& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_status()) {
      set_status(from.status());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Data::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Data::CopyFrom(const Data& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Data::IsInitialized() const {
  
  return true;
}

void Data::Swap(Data* other) {
  if (other != this) {
    std::swap(status_, other->status_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Data::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Data_descriptor_;
  metadata.reflection = Data_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace comm

// @@protoc_insertion_point(global_scope)
