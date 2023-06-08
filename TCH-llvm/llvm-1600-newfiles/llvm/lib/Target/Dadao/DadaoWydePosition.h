// The encoding used for conditional codes used in BR instructions

#ifndef LLVM_LIB_TARGET_DADAO_DADAOWYDEPOSITION_H
#define LLVM_LIB_TARGET_DADAO_DADAOWYDEPOSITION_H

#include "llvm/ADT/StringSwitch.h"
#include "llvm/Support/ErrorHandling.h"

namespace llvm {
namespace DDWP {
enum WydePosition {
  WPOS_0 = 0,   //  bits 15 - 0
  WPOS_1 = 1,   //  bits 31 - 16
  WPOS_2 = 2,   //  bits 47 - 32
  WPOS_3 = 3,   //  bits 63 - 48
  BEYOND
};

inline static StringRef dadaoWydePositionToString(DDWP::WydePosition WP) {
  switch (WP) {
  case DDWP::WPOS_0:
    return "w0";
  case DDWP::WPOS_1:
    return "w1";
  case DDWP::WPOS_2:
    return "w2";
  case DDWP::WPOS_3:
    return "w3";
  default:
    llvm_unreachable("Invalid wyde position");
  }
}

inline static WydePosition wposToDadaoWydePosition(StringRef S) {
  return StringSwitch<WydePosition>(S)
      .CaseLower("w0", DDWP::WPOS_0)
      .CaseLower("w1", DDWP::WPOS_1)
      .CaseLower("w2", DDWP::WPOS_2)
      .CaseLower("w3", DDWP::WPOS_3)
      .Default(DDWP::BEYOND);
}
} // namespace DDWP
} // namespace llvm

#endif // LLVM_LIB_TARGET_DADAO_DADAOWYDEPOSITION_H
