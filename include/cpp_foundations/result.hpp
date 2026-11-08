#pragma once

#include <concepts>     // constructible_from
#include <cstdlib>      // abort()
#include <type_traits>  // remove_cvref_t
#include <utility>      // move() & forward()
#include <variant>

namespace cf {
template <typename E>
class Unexpected {
 public:
  constexpr explicit Unexpected(E error) : error_(std::move(error)) {}

  // NOLINTBEGIN(readability-identifier-naming): matches std::expected
  [[nodiscard]] constexpr const E& error() const& { return error_; }
  [[nodiscard]] constexpr E&& error() && { return std::move(error_); }
  // NOLINTEND(readability-identifier-naming)

 private:
  E error_;
};

// Designed to resemble std::expected<T, E> to allow for ease of porting to
// C++23 in the future
template <typename T, typename E>
class [[nodiscard]] Result {
 public:
  // Constructor for value
  // Implicit so functions can `return value;` like std::expected.
  template <typename U = T>
    requires(!std::same_as<std::remove_cvref_t<U>, Result> &&
             std::constructible_from<T, U>)
  // NOLINTNEXTLINE(google-explicit-constructor,cppcoreguidelines-explicit-constructor)
  constexpr Result(U&& val)
      : data_(std::in_place_index<0>, std::forward<U>(val)) {}

  // NOLINTNEXTLINE(google-explicit-constructor,cppcoreguidelines-explicit-constructor)
  constexpr Result(Unexpected<E> unexpected)
      : data_(std::in_place_index<1>, std::move(unexpected).error()) {}

  // NOLINTBEGIN(readability-identifier-naming): matches std::expected
  // Validation checks
  [[nodiscard]] constexpr bool has_value() const noexcept {
    return data_.index() == 0;
  }

  constexpr explicit operator bool() const noexcept { return has_value(); }

  // Accessors
  // Calling value() when holding an error is a bug in the calling code
  // Not a runtime error, so abort program
  [[nodiscard]] constexpr T& value() & {
    if (!has_value()) {
      std::abort();
    }
    return std::get<0>(data_);
  }

  [[nodiscard]] constexpr const T& value() const& {
    if (!has_value()) {
      std::abort();
    }
    return std::get<0>(data_);
  }

  [[nodiscard]] constexpr T&& value() && {
    if (!has_value()) {
      std::abort();
    }
    return std::get<0>(std::move(data_));
  }

  [[nodiscard]] constexpr const T&& value() const&& {
    if (!has_value()) {
      std::abort();
    }
    return std::get<0>(std::move(data_));
  }

  // Calling error() when holding a value is a bug in the calling code
  // Not a runtime error, so abort program
  [[nodiscard]] constexpr E& error() & {
    if (has_value()) {
      std::abort();
    }
    return std::get<1>(data_);
  }

  [[nodiscard]] constexpr const E& error() const& {
    if (has_value()) {
      std::abort();
    }
    return std::get<1>(data_);
  }

  [[nodiscard]] constexpr E&& error() && {
    if (has_value()) {
      std::abort();
    }
    return std::get<1>(std::move(data_));
  }

  [[nodiscard]] constexpr const E&& error() const&& {
    if (has_value()) {
      std::abort();
    }
    return std::get<1>(std::move(data_));
  }

  // NOLINTEND(readability-identifier-naming)

 private:
  std::variant<T, E> data_;
};

// Specialized for operations that succeed with no value. `return {};` is
// success. Designed to resemble std::expected<void, E> to allow for ease of
// porting to C++23 in the future
template <typename E>
class [[nodiscard]] Result<void, E> {
 public:
  // Constructors
  constexpr Result() = default;

  // NOLINTNEXTLINE(google-explicit-constructor,cppcoreguidelines-explicit-constructor)
  constexpr Result(Unexpected<E> unexpected)
      : data_(std::in_place_index<1>, std::move(unexpected).error()) {}

  // NOLINTBEGIN(readability-identifier-naming): matches std::expected
  // Validation checks
  [[nodiscard]] constexpr bool has_value() const noexcept {
    return data_.index() == 0;
  }

  constexpr explicit operator bool() const noexcept { return has_value(); }

  // Accessors
  // Calling value() when holding an error is a bug in the calling code
  constexpr void value() const {
    if (!has_value()) {
      std::abort();
    }
  }

  // Calling error() when holding a value is a bug in the calling code
  // Not a runtime error, so abort program
  [[nodiscard]] constexpr E& error() & {
    if (has_value()) {
      std::abort();
    }
    return std::get<1>(data_);
  }

  [[nodiscard]] constexpr const E& error() const& {
    if (has_value()) {
      std::abort();
    }
    return std::get<1>(data_);
  }

  [[nodiscard]] constexpr E&& error() && {
    if (has_value()) {
      std::abort();
    }
    return std::get<1>(std::move(data_));
  }

  [[nodiscard]] constexpr const E&& error() const&& {
    if (has_value()) {
      std::abort();
    }
    return std::get<1>(std::move(data_));
  }
  // NOLINTEND(readability-identifier-naming)

 private:
  std::variant<std::monostate, E> data_;  // monostate = success
};

}  // namespace cf
