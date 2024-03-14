//===-- DadaoELFObjectWriter.cpp - Dadao ELF Writer -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/DadaoBaseInfo.h"
#include "MCTargetDesc/DadaoFixupKinds.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {

class DadaoELFObjectWriter : public MCELFObjectTargetWriter {
public:
  explicit DadaoELFObjectWriter(uint8_t OSABI);

  ~DadaoELFObjectWriter() override = default;

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
  bool needsRelocateWithSymbol(const MCSymbol &SD,
                               unsigned Type) const override;
};

} // end anonymous namespace

DadaoELFObjectWriter::DadaoELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(/*Is64Bit_=*/true, OSABI, ELF::EM_DADAO,
                              /*HasRelocationAddend_=*/true) {}

unsigned DadaoELFObjectWriter::getRelocType(MCContext & /*Ctx*/,
                                            const MCValue & /*Target*/,
                                            const MCFixup &Fixup,
                                            bool /*IsPCRel*/) const {
  unsigned Type;
  unsigned Kind = static_cast<unsigned>(Fixup.getKind());
  switch (Kind) {
  case Dadao::FIXUP_DADAO_21:
    Type = ELF::R_DADAO_21;
    break;
  case Dadao::FIXUP_DADAO_21_F:
    Type = ELF::R_DADAO_21_F;
    break;
  case Dadao::FIXUP_DADAO_25:
    Type = ELF::R_DADAO_25;
    break;
  case Dadao::FIXUP_DADAO_32:
  case FK_Data_4:
    Type = ELF::R_DADAO_32;
    break;
  case Dadao::FIXUP_DADAO_HI16:
    Type = ELF::R_DADAO_HI16;
    break;
  case Dadao::FIXUP_DADAO_LO16:
    Type = ELF::R_DADAO_LO16;
    break;
  case Dadao::FIXUP_DADAO_ABS:
    Type = ELF::R_DADAO_ABS;
    break;
  case Dadao::FIXUP_DADAO_NONE:
    Type = ELF::R_DADAO_NONE;
    break;

  default:
    llvm_unreachable("Invalid fixup kind!");
  }
  return Type;
}

bool DadaoELFObjectWriter::needsRelocateWithSymbol(const MCSymbol & /*SD*/,
                                                   unsigned Type) const {
  switch (Type) {
  case ELF::R_DADAO_21:
  case ELF::R_DADAO_21_F:
  case ELF::R_DADAO_25:
  case ELF::R_DADAO_32:
  case ELF::R_DADAO_HI16:
  case ELF::R_DADAO_ABS:
    return true;
  default:
    return false;
  }
}

std::unique_ptr<MCObjectTargetWriter>
llvm::createDadaoELFObjectWriter(uint8_t OSABI) {
  return std::make_unique<DadaoELFObjectWriter>(OSABI);
}
