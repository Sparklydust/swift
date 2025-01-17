//===--- SILArgumentConvention.h --------------------------------*- C++ -*-===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2017 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://swift.org/LICENSE.txt for license information
// See https://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//

#ifndef SWIFT_SIL_SILARGUMENTCONVENTION_H
#define SWIFT_SIL_SILARGUMENTCONVENTION_H

#include "swift/AST/Types.h"

namespace swift {

/// Conventions for apply operands and function-entry arguments in SIL.
///
/// This is simply a union of ParameterConvention and ResultConvention
/// (ParameterConvention + Indirect_Out) for convenience when visiting all
/// arguments.
struct SILArgumentConvention {
  enum ConventionType : uint8_t {
    Indirect_In,
    Indirect_In_Constant,
    Indirect_In_Guaranteed,
    Indirect_Inout,
    Indirect_InoutAliasable,
    Indirect_Out,
    Direct_Owned,
    Direct_Unowned,
    Direct_Guaranteed,
  } Value;

  SILArgumentConvention(decltype(Value) NewValue) : Value(NewValue) {}

  /// Turn a ParameterConvention into a SILArgumentConvention.
  explicit SILArgumentConvention(ParameterConvention Conv) : Value() {
    switch (Conv) {
    case ParameterConvention::Indirect_In:
      Value = SILArgumentConvention::Indirect_In;
      return;
    case ParameterConvention::Indirect_In_Constant:
      Value = SILArgumentConvention::Indirect_In_Constant;
      return;
    case ParameterConvention::Indirect_Inout:
      Value = SILArgumentConvention::Indirect_Inout;
      return;
    case ParameterConvention::Indirect_InoutAliasable:
      Value = SILArgumentConvention::Indirect_InoutAliasable;
      return;
    case ParameterConvention::Indirect_In_Guaranteed:
      Value = SILArgumentConvention::Indirect_In_Guaranteed;
      return;
    case ParameterConvention::Direct_Unowned:
      Value = SILArgumentConvention::Direct_Unowned;
      return;
    case ParameterConvention::Direct_Guaranteed:
      Value = SILArgumentConvention::Direct_Guaranteed;
      return;
    case ParameterConvention::Direct_Owned:
      Value = SILArgumentConvention::Direct_Owned;
      return;
    }
    llvm_unreachable("covered switch isn't covered?!");
  }

  operator ConventionType() const { return Value; }

  bool isIndirectConvention() const {
    return Value <= SILArgumentConvention::Indirect_Out;
  }

  bool isInoutConvention() const {
    switch (Value) {
      case SILArgumentConvention::Indirect_Inout:
      case SILArgumentConvention::Indirect_InoutAliasable:
        return true;
      case SILArgumentConvention::Indirect_In_Guaranteed:
      case SILArgumentConvention::Indirect_In:
      case SILArgumentConvention::Indirect_In_Constant:
      case SILArgumentConvention::Indirect_Out:
      case SILArgumentConvention::Direct_Unowned:
      case SILArgumentConvention::Direct_Owned:
      case SILArgumentConvention::Direct_Guaranteed:
        return false;
    }
    llvm_unreachable("covered switch isn't covered?!");
  }

  bool isOwnedConvention() const {
    switch (Value) {
    case SILArgumentConvention::Indirect_In:
    case SILArgumentConvention::Indirect_In_Constant:
    case SILArgumentConvention::Direct_Owned:
      return true;
    case SILArgumentConvention::Indirect_In_Guaranteed:
    case SILArgumentConvention::Direct_Guaranteed:
    case SILArgumentConvention::Indirect_Inout:
    case SILArgumentConvention::Indirect_Out:
    case SILArgumentConvention::Indirect_InoutAliasable:
    case SILArgumentConvention::Direct_Unowned:
      return false;
    }
    llvm_unreachable("covered switch isn't covered?!");
  }

  bool isGuaranteedConvention() const {
    switch (Value) {
    case SILArgumentConvention::Indirect_In_Guaranteed:
    case SILArgumentConvention::Direct_Guaranteed:
      return true;
    case SILArgumentConvention::Indirect_Inout:
    case SILArgumentConvention::Indirect_In:
    case SILArgumentConvention::Indirect_In_Constant:
    case SILArgumentConvention::Indirect_Out:
    case SILArgumentConvention::Indirect_InoutAliasable:
    case SILArgumentConvention::Direct_Unowned:
    case SILArgumentConvention::Direct_Owned:
      return false;
    }
    llvm_unreachable("covered switch isn't covered?!");
  }

  /// Returns true if \p Value is a non-aliasing indirect parameter.
  bool isExclusiveIndirectParameter() {
    switch (Value) {
    case SILArgumentConvention::Indirect_In:
    case SILArgumentConvention::Indirect_In_Constant:
    case SILArgumentConvention::Indirect_Out:
    case SILArgumentConvention::Indirect_In_Guaranteed:
    case SILArgumentConvention::Indirect_Inout:
      return true;

    case SILArgumentConvention::Indirect_InoutAliasable:
    case SILArgumentConvention::Direct_Unowned:
    case SILArgumentConvention::Direct_Guaranteed:
    case SILArgumentConvention::Direct_Owned:
      return false;
    }
    llvm_unreachable("covered switch isn't covered?!");
  }
};

} // namespace swift

#endif
