// Polyfill for std::identity when compiling as C++17.
// adapted from LLVM libc++ with the following notice:

//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//


#ifndef IDENTITY_POLYFILL_H
#define IDENTITY_POLYFILL_H

#include <utility>

namespace std {

struct identity {
  template <class _Tp>
  [[nodiscard]] constexpr _Tp&& operator()(_Tp&& __t) const noexcept {
    return std::forward<_Tp>(__t);
  }

  using is_transparent = void;
};

}

#endif
