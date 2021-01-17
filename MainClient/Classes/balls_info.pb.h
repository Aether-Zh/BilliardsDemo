// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: balls_info.proto

#ifndef PROTOBUF_balls_5finfo_2eproto__INCLUDED
#define PROTOBUF_balls_5finfo_2eproto__INCLUDED

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
void  protobuf_AddDesc_balls_5finfo_2eproto();
void protobuf_AssignDesc_balls_5finfo_2eproto();
void protobuf_ShutdownFile_balls_5finfo_2eproto();

class BallInfo;
class BallsInfo;

// ===================================================================

class BallInfo : public ::google::protobuf::Message {
 public:
  BallInfo();
  virtual ~BallInfo();
  
  BallInfo(const BallInfo& from);
  
  inline BallInfo& operator=(const BallInfo& from) {
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
  static const BallInfo& default_instance();
  
  void Swap(BallInfo* other);
  
  // implements Message ----------------------------------------------
  
  BallInfo* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const BallInfo& from);
  void MergeFrom(const BallInfo& from);
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
  
  // required int32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);
  
  // required double r = 2;
  inline bool has_r() const;
  inline void clear_r();
  static const int kRFieldNumber = 2;
  inline double r() const;
  inline void set_r(double value);
  
  // required double x = 3;
  inline bool has_x() const;
  inline void clear_x();
  static const int kXFieldNumber = 3;
  inline double x() const;
  inline void set_x(double value);
  
  // required double y = 4;
  inline bool has_y() const;
  inline void clear_y();
  static const int kYFieldNumber = 4;
  inline double y() const;
  inline void set_y(double value);
  
  // @@protoc_insertion_point(class_scope:BallInfo)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_r();
  inline void clear_has_r();
  inline void set_has_x();
  inline void clear_has_x();
  inline void set_has_y();
  inline void clear_has_y();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  double r_;
  double x_;
  double y_;
  ::google::protobuf::int32 id_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void  protobuf_AddDesc_balls_5finfo_2eproto();
  friend void protobuf_AssignDesc_balls_5finfo_2eproto();
  friend void protobuf_ShutdownFile_balls_5finfo_2eproto();
  
  void InitAsDefaultInstance();
  static BallInfo* default_instance_;
};
// -------------------------------------------------------------------

class BallsInfo : public ::google::protobuf::Message {
 public:
  BallsInfo();
  virtual ~BallsInfo();
  
  BallsInfo(const BallsInfo& from);
  
  inline BallsInfo& operator=(const BallsInfo& from) {
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
  static const BallsInfo& default_instance();
  
  void Swap(BallsInfo* other);
  
  // implements Message ----------------------------------------------
  
  BallsInfo* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const BallsInfo& from);
  void MergeFrom(const BallsInfo& from);
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
  
  // repeated .BallInfo ball = 1;
  inline int ball_size() const;
  inline void clear_ball();
  static const int kBallFieldNumber = 1;
  inline const ::BallInfo& ball(int index) const;
  inline ::BallInfo* mutable_ball(int index);
  inline ::BallInfo* add_ball();
  inline const ::google::protobuf::RepeatedPtrField< ::BallInfo >&
      ball() const;
  inline ::google::protobuf::RepeatedPtrField< ::BallInfo >*
      mutable_ball();
  
  // @@protoc_insertion_point(class_scope:BallsInfo)
 private:
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::RepeatedPtrField< ::BallInfo > ball_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_balls_5finfo_2eproto();
  friend void protobuf_AssignDesc_balls_5finfo_2eproto();
  friend void protobuf_ShutdownFile_balls_5finfo_2eproto();
  
  void InitAsDefaultInstance();
  static BallsInfo* default_instance_;
};
// ===================================================================


// ===================================================================

// BallInfo

// required int32 id = 1;
inline bool BallInfo::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void BallInfo::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void BallInfo::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void BallInfo::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 BallInfo::id() const {
  return id_;
}
inline void BallInfo::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
}

// required double r = 2;
inline bool BallInfo::has_r() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void BallInfo::set_has_r() {
  _has_bits_[0] |= 0x00000002u;
}
inline void BallInfo::clear_has_r() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void BallInfo::clear_r() {
  r_ = 0;
  clear_has_r();
}
inline double BallInfo::r() const {
  return r_;
}
inline void BallInfo::set_r(double value) {
  set_has_r();
  r_ = value;
}

// required double x = 3;
inline bool BallInfo::has_x() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void BallInfo::set_has_x() {
  _has_bits_[0] |= 0x00000004u;
}
inline void BallInfo::clear_has_x() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void BallInfo::clear_x() {
  x_ = 0;
  clear_has_x();
}
inline double BallInfo::x() const {
  return x_;
}
inline void BallInfo::set_x(double value) {
  set_has_x();
  x_ = value;
}

// required double y = 4;
inline bool BallInfo::has_y() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void BallInfo::set_has_y() {
  _has_bits_[0] |= 0x00000008u;
}
inline void BallInfo::clear_has_y() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void BallInfo::clear_y() {
  y_ = 0;
  clear_has_y();
}
inline double BallInfo::y() const {
  return y_;
}
inline void BallInfo::set_y(double value) {
  set_has_y();
  y_ = value;
}

// -------------------------------------------------------------------

// BallsInfo

// repeated .BallInfo ball = 1;
inline int BallsInfo::ball_size() const {
  return ball_.size();
}
inline void BallsInfo::clear_ball() {
  ball_.Clear();
}
inline const ::BallInfo& BallsInfo::ball(int index) const {
  return ball_.Get(index);
}
inline ::BallInfo* BallsInfo::mutable_ball(int index) {
  return ball_.Mutable(index);
}
inline ::BallInfo* BallsInfo::add_ball() {
  return ball_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::BallInfo >&
BallsInfo::ball() const {
  return ball_;
}
inline ::google::protobuf::RepeatedPtrField< ::BallInfo >*
BallsInfo::mutable_ball() {
  return &ball_;
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_balls_5finfo_2eproto__INCLUDED