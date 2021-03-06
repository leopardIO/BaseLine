// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: SearchProtoMessage.proto

#ifndef PROTOBUF_SearchProtoMessage_2eproto__INCLUDED
#define PROTOBUF_SearchProtoMessage_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_SearchProtoMessage_2eproto();
void protobuf_AssignDesc_SearchProtoMessage_2eproto();
void protobuf_ShutdownFile_SearchProtoMessage_2eproto();

class SearchProtoMessage;

// ===================================================================

class SearchProtoMessage : public ::google::protobuf::Message {
 public:
  SearchProtoMessage();
  virtual ~SearchProtoMessage();
  
  SearchProtoMessage(const SearchProtoMessage& from);
  
  inline SearchProtoMessage& operator=(const SearchProtoMessage& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const SearchProtoMessage& default_instance();
  
  void Swap(SearchProtoMessage* other);
  
  // implements Message ----------------------------------------------
  
  SearchProtoMessage* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const SearchProtoMessage& from);
  void MergeFrom(const SearchProtoMessage& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required uint32 picture_length = 1;
  inline bool has_picture_length() const;
  inline void clear_picture_length();
  static const int kPictureLengthFieldNumber = 1;
  inline ::google::protobuf::uint32 picture_length() const;
  inline void set_picture_length(::google::protobuf::uint32 value);
  
  // required string picture_name = 2;
  inline bool has_picture_name() const;
  inline void clear_picture_name();
  static const int kPictureNameFieldNumber = 2;
  inline const ::std::string& picture_name() const;
  inline void set_picture_name(const ::std::string& value);
  inline void set_picture_name(const char* value);
  inline void set_picture_name(const char* value, size_t size);
  inline ::std::string* mutable_picture_name();
  inline ::std::string* release_picture_name();
  
  // required float start_time = 3;
  inline bool has_start_time() const;
  inline void clear_start_time();
  static const int kStartTimeFieldNumber = 3;
  inline float start_time() const;
  inline void set_start_time(float value);
  
  // required float end_time = 4;
  inline bool has_end_time() const;
  inline void clear_end_time();
  static const int kEndTimeFieldNumber = 4;
  inline float end_time() const;
  inline void set_end_time(float value);
  
  // required uint32 task_id = 5;
  inline bool has_task_id() const;
  inline void clear_task_id();
  static const int kTaskIdFieldNumber = 5;
  inline ::google::protobuf::uint32 task_id() const;
  inline void set_task_id(::google::protobuf::uint32 value);
  
  // @@protoc_insertion_point(class_scope:SearchProtoMessage)
 private:
  inline void set_has_picture_length();
  inline void clear_has_picture_length();
  inline void set_has_picture_name();
  inline void clear_has_picture_name();
  inline void set_has_start_time();
  inline void clear_has_start_time();
  inline void set_has_end_time();
  inline void clear_has_end_time();
  inline void set_has_task_id();
  inline void clear_has_task_id();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* picture_name_;
  ::google::protobuf::uint32 picture_length_;
  float start_time_;
  float end_time_;
  ::google::protobuf::uint32 task_id_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];
  
  friend void  protobuf_AddDesc_SearchProtoMessage_2eproto();
  friend void protobuf_AssignDesc_SearchProtoMessage_2eproto();
  friend void protobuf_ShutdownFile_SearchProtoMessage_2eproto();
  
  void InitAsDefaultInstance();
  static SearchProtoMessage* default_instance_;
};
// ===================================================================


// ===================================================================

// SearchProtoMessage

// required uint32 picture_length = 1;
inline bool SearchProtoMessage::has_picture_length() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void SearchProtoMessage::set_has_picture_length() {
  _has_bits_[0] |= 0x00000001u;
}
inline void SearchProtoMessage::clear_has_picture_length() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void SearchProtoMessage::clear_picture_length() {
  picture_length_ = 0u;
  clear_has_picture_length();
}
inline ::google::protobuf::uint32 SearchProtoMessage::picture_length() const {
  return picture_length_;
}
inline void SearchProtoMessage::set_picture_length(::google::protobuf::uint32 value) {
  set_has_picture_length();
  picture_length_ = value;
}

// required string picture_name = 2;
inline bool SearchProtoMessage::has_picture_name() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void SearchProtoMessage::set_has_picture_name() {
  _has_bits_[0] |= 0x00000002u;
}
inline void SearchProtoMessage::clear_has_picture_name() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void SearchProtoMessage::clear_picture_name() {
  if (picture_name_ != &::google::protobuf::internal::kEmptyString) {
    picture_name_->clear();
  }
  clear_has_picture_name();
}
inline const ::std::string& SearchProtoMessage::picture_name() const {
  return *picture_name_;
}
inline void SearchProtoMessage::set_picture_name(const ::std::string& value) {
  set_has_picture_name();
  if (picture_name_ == &::google::protobuf::internal::kEmptyString) {
    picture_name_ = new ::std::string;
  }
  picture_name_->assign(value);
}
inline void SearchProtoMessage::set_picture_name(const char* value) {
  set_has_picture_name();
  if (picture_name_ == &::google::protobuf::internal::kEmptyString) {
    picture_name_ = new ::std::string;
  }
  picture_name_->assign(value);
}
inline void SearchProtoMessage::set_picture_name(const char* value, size_t size) {
  set_has_picture_name();
  if (picture_name_ == &::google::protobuf::internal::kEmptyString) {
    picture_name_ = new ::std::string;
  }
  picture_name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* SearchProtoMessage::mutable_picture_name() {
  set_has_picture_name();
  if (picture_name_ == &::google::protobuf::internal::kEmptyString) {
    picture_name_ = new ::std::string;
  }
  return picture_name_;
}
inline ::std::string* SearchProtoMessage::release_picture_name() {
  clear_has_picture_name();
  if (picture_name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = picture_name_;
    picture_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// required float start_time = 3;
inline bool SearchProtoMessage::has_start_time() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void SearchProtoMessage::set_has_start_time() {
  _has_bits_[0] |= 0x00000004u;
}
inline void SearchProtoMessage::clear_has_start_time() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void SearchProtoMessage::clear_start_time() {
  start_time_ = 0;
  clear_has_start_time();
}
inline float SearchProtoMessage::start_time() const {
  return start_time_;
}
inline void SearchProtoMessage::set_start_time(float value) {
  set_has_start_time();
  start_time_ = value;
}

// required float end_time = 4;
inline bool SearchProtoMessage::has_end_time() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void SearchProtoMessage::set_has_end_time() {
  _has_bits_[0] |= 0x00000008u;
}
inline void SearchProtoMessage::clear_has_end_time() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void SearchProtoMessage::clear_end_time() {
  end_time_ = 0;
  clear_has_end_time();
}
inline float SearchProtoMessage::end_time() const {
  return end_time_;
}
inline void SearchProtoMessage::set_end_time(float value) {
  set_has_end_time();
  end_time_ = value;
}

// required uint32 task_id = 5;
inline bool SearchProtoMessage::has_task_id() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void SearchProtoMessage::set_has_task_id() {
  _has_bits_[0] |= 0x00000010u;
}
inline void SearchProtoMessage::clear_has_task_id() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void SearchProtoMessage::clear_task_id() {
  task_id_ = 0u;
  clear_has_task_id();
}
inline ::google::protobuf::uint32 SearchProtoMessage::task_id() const {
  return task_id_;
}
inline void SearchProtoMessage::set_task_id(::google::protobuf::uint32 value) {
  set_has_task_id();
  task_id_ = value;
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_SearchProtoMessage_2eproto__INCLUDED
