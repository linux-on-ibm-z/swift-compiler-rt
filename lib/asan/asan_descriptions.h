//===-- asan_descriptions.h -------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file is a part of AddressSanitizer, an address sanity checker.
//
// ASan-private header for asan_descriptions.cc.
// TODO(filcab): Most struct definitions should move to the interface headers.
//===----------------------------------------------------------------------===//

#include "asan_internal.h"
#include "asan_thread.h"
#include "sanitizer_common/sanitizer_common.h"
#include "sanitizer_common/sanitizer_report_decorator.h"

namespace __asan {

void DescribeThread(AsanThreadContext *context);
static inline void DescribeThread(AsanThread *t) {
  if (t) DescribeThread(t->context());
}
const char *ThreadNameWithParenthesis(AsanThreadContext *t, char buff[],
                                      uptr buff_len);
const char *ThreadNameWithParenthesis(u32 tid, char buff[], uptr buff_len);

class Decorator : public __sanitizer::SanitizerCommonDecorator {
 public:
  Decorator() : SanitizerCommonDecorator() {}
  const char *Access() { return Blue(); }
  const char *EndAccess() { return Default(); }
  const char *Location() { return Green(); }
  const char *EndLocation() { return Default(); }
  const char *Allocation() { return Magenta(); }
  const char *EndAllocation() { return Default(); }

  const char *ShadowByte(u8 byte) {
    switch (byte) {
      case kAsanHeapLeftRedzoneMagic:
      case kAsanHeapRightRedzoneMagic:
      case kAsanArrayCookieMagic:
        return Red();
      case kAsanHeapFreeMagic:
        return Magenta();
      case kAsanStackLeftRedzoneMagic:
      case kAsanStackMidRedzoneMagic:
      case kAsanStackRightRedzoneMagic:
      case kAsanStackPartialRedzoneMagic:
        return Red();
      case kAsanStackAfterReturnMagic:
        return Magenta();
      case kAsanInitializationOrderMagic:
        return Cyan();
      case kAsanUserPoisonedMemoryMagic:
      case kAsanContiguousContainerOOBMagic:
      case kAsanAllocaLeftMagic:
      case kAsanAllocaRightMagic:
        return Blue();
      case kAsanStackUseAfterScopeMagic:
        return Magenta();
      case kAsanGlobalRedzoneMagic:
        return Red();
      case kAsanInternalHeapMagic:
        return Yellow();
      case kAsanIntraObjectRedzone:
        return Yellow();
      default:
        return Default();
    }
  }
  const char *EndShadowByte() { return Default(); }
  const char *MemoryByte() { return Magenta(); }
  const char *EndMemoryByte() { return Default(); }
};

enum ShadowKind : u8 {
  kShadowKindLow,
  kShadowKindGap,
  kShadowKindHigh,
};
static const char *const ShadowNames[] = {"low shadow", "shadow gap",
                                          "high shadow"};

struct ShadowAddressDescription {
  uptr addr;
  ShadowKind kind;
  u8 shadow_byte;
};

bool GetShadowAddressInformation(uptr addr, ShadowAddressDescription *descr);
bool DescribeAddressIfShadow(uptr addr);

}  // namespace __asan
