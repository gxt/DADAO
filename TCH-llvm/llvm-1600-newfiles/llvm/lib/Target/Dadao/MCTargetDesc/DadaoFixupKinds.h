//===-- DadaoFixupKinds.h - Dadao Specific Fixup Entries --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DADAO_MCTARGETDESC_DADAOFIXUPKINDS_H
#define LLVM_LIB_TARGET_DADAO_MCTARGETDESC_DADAOFIXUPKINDS_H

#include "llvm/MC/MCFixup.h"

namespace llvm {
namespace Dadao {
// Although most of the current fixup types reflect a unique relocation
// one can have multiple fixup types for a given relocation and thus need
// to be uniquely named.
//
// This table *must* be in the save order of
// MCFixupKindInfo Infos[Dadao::NumTargetFixupKinds]
// in DadaoAsmBackend.cpp.
//
enum Fixups {
  // Results in R_Dadao_NONE
  FIXUP_DADAO_NONE = FirstTargetFixupKind,

  FIXUP_DADAO_21,   // 21-bit symbol relocation
  FIXUP_DADAO_21_F, // 21-bit symbol relocation, last two bits masked to 0
  FIXUP_DADAO_25,   // 25-bit branch targets
  FIXUP_DADAO_32,   // general 32-bit relocation
  FIXUP_DADAO_HI16, // upper 16-bits of a symbolic relocation
  FIXUP_DADAO_LO16, // lower 16-bits of a symbolic relocation
  FIXUP_DADAO_ABS,  // absolute symbol relocation

  // Marker
  LastTargetFixupKind,
  NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
};
} // namespace Dadao
} // namespace llvm

#endif // LLVM_LIB_TARGET_DADAO_MCTARGETDESC_DADAOFIXUPKINDS_H
