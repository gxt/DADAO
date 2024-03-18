//===--- Dadao.h - Declare Dadao target feature support ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares Dadao TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_BASIC_TARGETS_DADAO_H
#define LLVM_CLANG_LIB_BASIC_TARGETS_DADAO_H

#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Support/Compiler.h"

namespace clang {
namespace targets {

class LLVM_LIBRARY_VISIBILITY DadaoTargetInfo : public TargetInfo {
  // Class for Dadao (32-bit).
  // The CPU profiles supported by the Dadao backend
  enum CPUKind {
    DADAO_NONE,
    DADAO_WUMING,
  } CPU;

  static const TargetInfo::GCCRegAlias GCCRegAliases[];
  static const char *const GCCRegNames[];

public:
  DadaoTargetInfo(const llvm::Triple &Triple, const TargetOptions &)
      : TargetInfo(Triple) {
    // The default is PointerWidth = PointerAlign = 32.
    PointerWidth = PointerAlign = 64;
    // TODO: There are probably other options in TargetInfo that are used by clang frontend
    // but unaffected by resetDataLayout. We should check the settings of all of them
    // in TargetInfo::TargetInfo.

    // Description string has to be kept in sync with backend.
    resetDataLayout("e"        // Little endian
                    "-m:e"     // ELF name manging
                    "-p:64:64" // 64 bit pointers, 64 bit aligned
                    "-i64:64"  // 64 bit integers, 64 bit aligned
                    "-a:0:64"  // 64 bit alignment of objects of aggregate type
                    "-n64"     // 64 bit native integer width
                    "-S128"    // 128 bit natural stack alignment
    );

    // Setting RegParmMax equal to what mregparm was set to in the old
    // toolchain
    RegParmMax = 4;

    // Set the default CPU to WUMING
    CPU = DADAO_WUMING;

    // Temporary approach to make everything at least word-aligned and allow for
    // safely casting between pointers with different alignment requirements.
    // TODO: Remove this when there are no more cast align warnings on the
    // firmware.
    MinGlobalAlign = 32;
  }

  void getTargetDefines(const LangOptions &Opts,
                        MacroBuilder &Builder) const override;

  bool isValidCPUName(StringRef Name) const override;

  void fillValidCPUList(SmallVectorImpl<StringRef> &Values) const override;

  bool setCPU(const std::string &Name) override;

  bool hasFeature(StringRef Feature) const override;

  ArrayRef<const char *> getGCCRegNames() const override;

  ArrayRef<TargetInfo::GCCRegAlias> getGCCRegAliases() const override;

  BuiltinVaListKind getBuiltinVaListKind() const override {
    return TargetInfo::VoidPtrBuiltinVaList;
  }

  ArrayRef<Builtin::Info> getTargetBuiltins() const override {
    return std::nullopt;
  }

  bool validateAsmConstraint(const char *&Name,
                             TargetInfo::ConstraintInfo &info) const override {
    return false;
  }

  const char *getClobbers() const override { return ""; }

  bool hasBitIntType() const override { return true; }
};
} // namespace targets
} // namespace clang

#endif // LLVM_CLANG_LIB_BASIC_TARGETS_DADAO_H
