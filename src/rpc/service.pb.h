// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: service.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_service_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_service_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3021000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3021011 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_service_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_service_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_service_2eproto;
namespace rpc {
class InputEcho;
struct InputEchoDefaultTypeInternal;
extern InputEchoDefaultTypeInternal _InputEcho_default_instance_;
class OutputEcho;
struct OutputEchoDefaultTypeInternal;
extern OutputEchoDefaultTypeInternal _OutputEcho_default_instance_;
class Request;
struct RequestDefaultTypeInternal;
extern RequestDefaultTypeInternal _Request_default_instance_;
class Response;
struct ResponseDefaultTypeInternal;
extern ResponseDefaultTypeInternal _Response_default_instance_;
}  // namespace rpc
PROTOBUF_NAMESPACE_OPEN
template<> ::rpc::InputEcho* Arena::CreateMaybeMessage<::rpc::InputEcho>(Arena*);
template<> ::rpc::OutputEcho* Arena::CreateMaybeMessage<::rpc::OutputEcho>(Arena*);
template<> ::rpc::Request* Arena::CreateMaybeMessage<::rpc::Request>(Arena*);
template<> ::rpc::Response* Arena::CreateMaybeMessage<::rpc::Response>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace rpc {

// ===================================================================

class Request final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:rpc.Request) */ {
 public:
  inline Request() : Request(nullptr) {}
  ~Request() override;
  explicit PROTOBUF_CONSTEXPR Request(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  Request(const Request& from);
  Request(Request&& from) noexcept
    : Request() {
    *this = ::std::move(from);
  }

  inline Request& operator=(const Request& from) {
    CopyFrom(from);
    return *this;
  }
  inline Request& operator=(Request&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const Request& default_instance() {
    return *internal_default_instance();
  }
  static inline const Request* internal_default_instance() {
    return reinterpret_cast<const Request*>(
               &_Request_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Request& a, Request& b) {
    a.Swap(&b);
  }
  inline void Swap(Request* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Request* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  Request* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<Request>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const Request& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const Request& from) {
    Request::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Request* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "rpc.Request";
  }
  protected:
  explicit Request(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kMethodFieldNumber = 1,
    kInputFieldNumber = 2,
  };
  // string method = 1;
  void clear_method();
  const std::string& method() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_method(ArgT0&& arg0, ArgT... args);
  std::string* mutable_method();
  PROTOBUF_NODISCARD std::string* release_method();
  void set_allocated_method(std::string* method);
  private:
  const std::string& _internal_method() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_method(const std::string& value);
  std::string* _internal_mutable_method();
  public:

  // bytes input = 2;
  void clear_input();
  const std::string& input() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_input(ArgT0&& arg0, ArgT... args);
  std::string* mutable_input();
  PROTOBUF_NODISCARD std::string* release_input();
  void set_allocated_input(std::string* input);
  private:
  const std::string& _internal_input() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_input(const std::string& value);
  std::string* _internal_mutable_input();
  public:

  // @@protoc_insertion_point(class_scope:rpc.Request)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr method_;
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr input_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_service_2eproto;
};
// -------------------------------------------------------------------

class Response final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:rpc.Response) */ {
 public:
  inline Response() : Response(nullptr) {}
  ~Response() override;
  explicit PROTOBUF_CONSTEXPR Response(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  Response(const Response& from);
  Response(Response&& from) noexcept
    : Response() {
    *this = ::std::move(from);
  }

  inline Response& operator=(const Response& from) {
    CopyFrom(from);
    return *this;
  }
  inline Response& operator=(Response&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const Response& default_instance() {
    return *internal_default_instance();
  }
  static inline const Response* internal_default_instance() {
    return reinterpret_cast<const Response*>(
               &_Response_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(Response& a, Response& b) {
    a.Swap(&b);
  }
  inline void Swap(Response* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Response* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  Response* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<Response>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const Response& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const Response& from) {
    Response::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Response* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "rpc.Response";
  }
  protected:
  explicit Response(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kMethodFieldNumber = 1,
    kOutputFieldNumber = 2,
    kIsErrorFieldNumber = 3,
  };
  // string method = 1;
  void clear_method();
  const std::string& method() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_method(ArgT0&& arg0, ArgT... args);
  std::string* mutable_method();
  PROTOBUF_NODISCARD std::string* release_method();
  void set_allocated_method(std::string* method);
  private:
  const std::string& _internal_method() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_method(const std::string& value);
  std::string* _internal_mutable_method();
  public:

  // bytes output = 2;
  void clear_output();
  const std::string& output() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_output(ArgT0&& arg0, ArgT... args);
  std::string* mutable_output();
  PROTOBUF_NODISCARD std::string* release_output();
  void set_allocated_output(std::string* output);
  private:
  const std::string& _internal_output() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_output(const std::string& value);
  std::string* _internal_mutable_output();
  public:

  // bool isError = 3;
  void clear_iserror();
  bool iserror() const;
  void set_iserror(bool value);
  private:
  bool _internal_iserror() const;
  void _internal_set_iserror(bool value);
  public:

  // @@protoc_insertion_point(class_scope:rpc.Response)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr method_;
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr output_;
    bool iserror_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_service_2eproto;
};
// -------------------------------------------------------------------

class InputEcho final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:rpc.InputEcho) */ {
 public:
  inline InputEcho() : InputEcho(nullptr) {}
  ~InputEcho() override;
  explicit PROTOBUF_CONSTEXPR InputEcho(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  InputEcho(const InputEcho& from);
  InputEcho(InputEcho&& from) noexcept
    : InputEcho() {
    *this = ::std::move(from);
  }

  inline InputEcho& operator=(const InputEcho& from) {
    CopyFrom(from);
    return *this;
  }
  inline InputEcho& operator=(InputEcho&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const InputEcho& default_instance() {
    return *internal_default_instance();
  }
  static inline const InputEcho* internal_default_instance() {
    return reinterpret_cast<const InputEcho*>(
               &_InputEcho_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(InputEcho& a, InputEcho& b) {
    a.Swap(&b);
  }
  inline void Swap(InputEcho* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(InputEcho* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  InputEcho* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<InputEcho>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const InputEcho& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const InputEcho& from) {
    InputEcho::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(InputEcho* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "rpc.InputEcho";
  }
  protected:
  explicit InputEcho(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kDataFieldNumber = 1,
  };
  // bytes data = 1;
  void clear_data();
  const std::string& data() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_data(ArgT0&& arg0, ArgT... args);
  std::string* mutable_data();
  PROTOBUF_NODISCARD std::string* release_data();
  void set_allocated_data(std::string* data);
  private:
  const std::string& _internal_data() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_data(const std::string& value);
  std::string* _internal_mutable_data();
  public:

  // @@protoc_insertion_point(class_scope:rpc.InputEcho)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr data_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_service_2eproto;
};
// -------------------------------------------------------------------

class OutputEcho final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:rpc.OutputEcho) */ {
 public:
  inline OutputEcho() : OutputEcho(nullptr) {}
  ~OutputEcho() override;
  explicit PROTOBUF_CONSTEXPR OutputEcho(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  OutputEcho(const OutputEcho& from);
  OutputEcho(OutputEcho&& from) noexcept
    : OutputEcho() {
    *this = ::std::move(from);
  }

  inline OutputEcho& operator=(const OutputEcho& from) {
    CopyFrom(from);
    return *this;
  }
  inline OutputEcho& operator=(OutputEcho&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const OutputEcho& default_instance() {
    return *internal_default_instance();
  }
  static inline const OutputEcho* internal_default_instance() {
    return reinterpret_cast<const OutputEcho*>(
               &_OutputEcho_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    3;

  friend void swap(OutputEcho& a, OutputEcho& b) {
    a.Swap(&b);
  }
  inline void Swap(OutputEcho* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(OutputEcho* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  OutputEcho* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<OutputEcho>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const OutputEcho& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const OutputEcho& from) {
    OutputEcho::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(OutputEcho* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "rpc.OutputEcho";
  }
  protected:
  explicit OutputEcho(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kDataFieldNumber = 1,
  };
  // bytes data = 1;
  void clear_data();
  const std::string& data() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_data(ArgT0&& arg0, ArgT... args);
  std::string* mutable_data();
  PROTOBUF_NODISCARD std::string* release_data();
  void set_allocated_data(std::string* data);
  private:
  const std::string& _internal_data() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_data(const std::string& value);
  std::string* _internal_mutable_data();
  public:

  // @@protoc_insertion_point(class_scope:rpc.OutputEcho)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr data_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_service_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Request

// string method = 1;
inline void Request::clear_method() {
  _impl_.method_.ClearToEmpty();
}
inline const std::string& Request::method() const {
  // @@protoc_insertion_point(field_get:rpc.Request.method)
  return _internal_method();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void Request::set_method(ArgT0&& arg0, ArgT... args) {
 
 _impl_.method_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:rpc.Request.method)
}
inline std::string* Request::mutable_method() {
  std::string* _s = _internal_mutable_method();
  // @@protoc_insertion_point(field_mutable:rpc.Request.method)
  return _s;
}
inline const std::string& Request::_internal_method() const {
  return _impl_.method_.Get();
}
inline void Request::_internal_set_method(const std::string& value) {
  
  _impl_.method_.Set(value, GetArenaForAllocation());
}
inline std::string* Request::_internal_mutable_method() {
  
  return _impl_.method_.Mutable(GetArenaForAllocation());
}
inline std::string* Request::release_method() {
  // @@protoc_insertion_point(field_release:rpc.Request.method)
  return _impl_.method_.Release();
}
inline void Request::set_allocated_method(std::string* method) {
  if (method != nullptr) {
    
  } else {
    
  }
  _impl_.method_.SetAllocated(method, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.method_.IsDefault()) {
    _impl_.method_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:rpc.Request.method)
}

// bytes input = 2;
inline void Request::clear_input() {
  _impl_.input_.ClearToEmpty();
}
inline const std::string& Request::input() const {
  // @@protoc_insertion_point(field_get:rpc.Request.input)
  return _internal_input();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void Request::set_input(ArgT0&& arg0, ArgT... args) {
 
 _impl_.input_.SetBytes(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:rpc.Request.input)
}
inline std::string* Request::mutable_input() {
  std::string* _s = _internal_mutable_input();
  // @@protoc_insertion_point(field_mutable:rpc.Request.input)
  return _s;
}
inline const std::string& Request::_internal_input() const {
  return _impl_.input_.Get();
}
inline void Request::_internal_set_input(const std::string& value) {
  
  _impl_.input_.Set(value, GetArenaForAllocation());
}
inline std::string* Request::_internal_mutable_input() {
  
  return _impl_.input_.Mutable(GetArenaForAllocation());
}
inline std::string* Request::release_input() {
  // @@protoc_insertion_point(field_release:rpc.Request.input)
  return _impl_.input_.Release();
}
inline void Request::set_allocated_input(std::string* input) {
  if (input != nullptr) {
    
  } else {
    
  }
  _impl_.input_.SetAllocated(input, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.input_.IsDefault()) {
    _impl_.input_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:rpc.Request.input)
}

// -------------------------------------------------------------------

// Response

// string method = 1;
inline void Response::clear_method() {
  _impl_.method_.ClearToEmpty();
}
inline const std::string& Response::method() const {
  // @@protoc_insertion_point(field_get:rpc.Response.method)
  return _internal_method();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void Response::set_method(ArgT0&& arg0, ArgT... args) {
 
 _impl_.method_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:rpc.Response.method)
}
inline std::string* Response::mutable_method() {
  std::string* _s = _internal_mutable_method();
  // @@protoc_insertion_point(field_mutable:rpc.Response.method)
  return _s;
}
inline const std::string& Response::_internal_method() const {
  return _impl_.method_.Get();
}
inline void Response::_internal_set_method(const std::string& value) {
  
  _impl_.method_.Set(value, GetArenaForAllocation());
}
inline std::string* Response::_internal_mutable_method() {
  
  return _impl_.method_.Mutable(GetArenaForAllocation());
}
inline std::string* Response::release_method() {
  // @@protoc_insertion_point(field_release:rpc.Response.method)
  return _impl_.method_.Release();
}
inline void Response::set_allocated_method(std::string* method) {
  if (method != nullptr) {
    
  } else {
    
  }
  _impl_.method_.SetAllocated(method, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.method_.IsDefault()) {
    _impl_.method_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:rpc.Response.method)
}

// bytes output = 2;
inline void Response::clear_output() {
  _impl_.output_.ClearToEmpty();
}
inline const std::string& Response::output() const {
  // @@protoc_insertion_point(field_get:rpc.Response.output)
  return _internal_output();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void Response::set_output(ArgT0&& arg0, ArgT... args) {
 
 _impl_.output_.SetBytes(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:rpc.Response.output)
}
inline std::string* Response::mutable_output() {
  std::string* _s = _internal_mutable_output();
  // @@protoc_insertion_point(field_mutable:rpc.Response.output)
  return _s;
}
inline const std::string& Response::_internal_output() const {
  return _impl_.output_.Get();
}
inline void Response::_internal_set_output(const std::string& value) {
  
  _impl_.output_.Set(value, GetArenaForAllocation());
}
inline std::string* Response::_internal_mutable_output() {
  
  return _impl_.output_.Mutable(GetArenaForAllocation());
}
inline std::string* Response::release_output() {
  // @@protoc_insertion_point(field_release:rpc.Response.output)
  return _impl_.output_.Release();
}
inline void Response::set_allocated_output(std::string* output) {
  if (output != nullptr) {
    
  } else {
    
  }
  _impl_.output_.SetAllocated(output, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.output_.IsDefault()) {
    _impl_.output_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:rpc.Response.output)
}

// bool isError = 3;
inline void Response::clear_iserror() {
  _impl_.iserror_ = false;
}
inline bool Response::_internal_iserror() const {
  return _impl_.iserror_;
}
inline bool Response::iserror() const {
  // @@protoc_insertion_point(field_get:rpc.Response.isError)
  return _internal_iserror();
}
inline void Response::_internal_set_iserror(bool value) {
  
  _impl_.iserror_ = value;
}
inline void Response::set_iserror(bool value) {
  _internal_set_iserror(value);
  // @@protoc_insertion_point(field_set:rpc.Response.isError)
}

// -------------------------------------------------------------------

// InputEcho

// bytes data = 1;
inline void InputEcho::clear_data() {
  _impl_.data_.ClearToEmpty();
}
inline const std::string& InputEcho::data() const {
  // @@protoc_insertion_point(field_get:rpc.InputEcho.data)
  return _internal_data();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void InputEcho::set_data(ArgT0&& arg0, ArgT... args) {
 
 _impl_.data_.SetBytes(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:rpc.InputEcho.data)
}
inline std::string* InputEcho::mutable_data() {
  std::string* _s = _internal_mutable_data();
  // @@protoc_insertion_point(field_mutable:rpc.InputEcho.data)
  return _s;
}
inline const std::string& InputEcho::_internal_data() const {
  return _impl_.data_.Get();
}
inline void InputEcho::_internal_set_data(const std::string& value) {
  
  _impl_.data_.Set(value, GetArenaForAllocation());
}
inline std::string* InputEcho::_internal_mutable_data() {
  
  return _impl_.data_.Mutable(GetArenaForAllocation());
}
inline std::string* InputEcho::release_data() {
  // @@protoc_insertion_point(field_release:rpc.InputEcho.data)
  return _impl_.data_.Release();
}
inline void InputEcho::set_allocated_data(std::string* data) {
  if (data != nullptr) {
    
  } else {
    
  }
  _impl_.data_.SetAllocated(data, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.data_.IsDefault()) {
    _impl_.data_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:rpc.InputEcho.data)
}

// -------------------------------------------------------------------

// OutputEcho

// bytes data = 1;
inline void OutputEcho::clear_data() {
  _impl_.data_.ClearToEmpty();
}
inline const std::string& OutputEcho::data() const {
  // @@protoc_insertion_point(field_get:rpc.OutputEcho.data)
  return _internal_data();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void OutputEcho::set_data(ArgT0&& arg0, ArgT... args) {
 
 _impl_.data_.SetBytes(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:rpc.OutputEcho.data)
}
inline std::string* OutputEcho::mutable_data() {
  std::string* _s = _internal_mutable_data();
  // @@protoc_insertion_point(field_mutable:rpc.OutputEcho.data)
  return _s;
}
inline const std::string& OutputEcho::_internal_data() const {
  return _impl_.data_.Get();
}
inline void OutputEcho::_internal_set_data(const std::string& value) {
  
  _impl_.data_.Set(value, GetArenaForAllocation());
}
inline std::string* OutputEcho::_internal_mutable_data() {
  
  return _impl_.data_.Mutable(GetArenaForAllocation());
}
inline std::string* OutputEcho::release_data() {
  // @@protoc_insertion_point(field_release:rpc.OutputEcho.data)
  return _impl_.data_.Release();
}
inline void OutputEcho::set_allocated_data(std::string* data) {
  if (data != nullptr) {
    
  } else {
    
  }
  _impl_.data_.SetAllocated(data, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.data_.IsDefault()) {
    _impl_.data_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:rpc.OutputEcho.data)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace rpc

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_service_2eproto
