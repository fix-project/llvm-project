//===-- Compile time cpu feature detection ----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// This file lists target cpu features by introspecting compiler enabled
// preprocessor definitions.
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC___SUPPORT_MACROS_PROPERTIES_CPU_FEATURES_H
#define LLVM_LIBC_SRC___SUPPORT_MACROS_PROPERTIES_CPU_FEATURES_H

#include "architectures.h"

#if defined(__ARM_FEATURE_FP16_SCALAR_ARITHMETIC)
#define LIBC_TARGET_CPU_HAS_FULLFP16
#endif

#if defined(__ARM_FEATURE_SVE)
#define LIBC_TARGET_CPU_HAS_SVE
#endif

#if defined(__ARM_FEATURE_SVE2)
#define LIBC_TARGET_CPU_HAS_SVE2
#endif

#if defined(__ARM_FEATURE_MOPS)
#define LIBC_TARGET_CPU_HAS_MOPS
#endif

#if defined(__SSE2__)
#define LIBC_TARGET_CPU_HAS_SSE2
#define LIBC_TARGET_CPU_HAS_FPU_FLOAT
#define LIBC_TARGET_CPU_HAS_FPU_DOUBLE
#endif

#if defined(__SSE4_2__)
#define LIBC_TARGET_CPU_HAS_SSE4_2
#endif

#if defined(__AVX__)
#define LIBC_TARGET_CPU_HAS_AVX
#endif

#if defined(__AVX2__)
#define LIBC_TARGET_CPU_HAS_AVX2
#endif

#if defined(__AVX512F__)
#define LIBC_TARGET_CPU_HAS_AVX512F
#endif

#if defined(__AVX512BW__)
#define LIBC_TARGET_CPU_HAS_AVX512BW
#endif

#if defined(__AVX512F__) || defined(__AVX2__)
#define LIBC_TARGET_CPU_HAS_GATHER
#endif

#if defined(__ARM_FP)
#if (__ARM_FP & 0x2)
#define LIBC_TARGET_CPU_HAS_ARM_FPU_HALF
#define LIBC_TARGET_CPU_HAS_FPU_HALF
#endif // LIBC_TARGET_CPU_HAS_ARM_FPU_HALF
#if (__ARM_FP & 0x4)
#define LIBC_TARGET_CPU_HAS_ARM_FPU_FLOAT
#define LIBC_TARGET_CPU_HAS_FPU_FLOAT
#endif // LIBC_TARGET_CPU_HAS_ARM_FPU_FLOAT
#if (__ARM_FP & 0x8)
#define LIBC_TARGET_CPU_HAS_ARM_FPU_DOUBLE
#define LIBC_TARGET_CPU_HAS_FPU_DOUBLE
#endif // LIBC_TARGET_CPU_HAS_ARM_FPU_DOUBLE
#endif // __ARM_FP

#if defined(__ARM_NEON)
#define LIBC_TARGET_CPU_HAS_ARM_NEON
#endif

#if defined(__riscv_flen)
// https://github.com/riscv-non-isa/riscv-c-api-doc/blob/main/src/c-api.adoc
#if defined(__riscv_zfhmin)
#define LIBC_TARGET_CPU_HAS_RISCV_FPU_HALF
#define LIBC_TARGET_CPU_HAS_FPU_HALF
#endif // LIBC_TARGET_CPU_HAS_RISCV_FPU_HALF
#if (__riscv_flen >= 32)
#define LIBC_TARGET_CPU_HAS_RISCV_FPU_FLOAT
#define LIBC_TARGET_CPU_HAS_FPU_FLOAT
#endif // LIBC_TARGET_CPU_HAS_RISCV_FPU_FLOAT
#if (__riscv_flen >= 64)
#define LIBC_TARGET_CPU_HAS_RISCV_FPU_DOUBLE
#define LIBC_TARGET_CPU_HAS_FPU_DOUBLE
#endif // LIBC_TARGET_CPU_HAS_RISCV_FPU_DOUBLE
#endif // __riscv_flen

#if defined(__NVPTX__) || defined(__AMDGPU__) || defined(__SPIRV__)
#define LIBC_TARGET_CPU_HAS_FPU_FLOAT
#define LIBC_TARGET_CPU_HAS_FPU_DOUBLE
#endif

#if defined(__ARM_FEATURE_FMA) || (defined(__AVX2__) && defined(__FMA__)) ||   \
    defined(__NVPTX__) || defined(__AMDGPU__) || defined(__riscv_flen) ||      \
    defined(__SPIRV__)
#define LIBC_TARGET_CPU_HAS_FMA
// Provide a more fine-grained control of FMA instruction for ARM targets.
#if defined(LIBC_TARGET_CPU_HAS_FPU_HALF)
#define LIBC_TARGET_CPU_HAS_FMA_HALF
#endif // LIBC_TARGET_CPU_HAS_FMA_HALF
#if defined(LIBC_TARGET_CPU_HAS_FPU_FLOAT)
#define LIBC_TARGET_CPU_HAS_FMA_FLOAT
#endif // LIBC_TARGET_CPU_HAS_FMA_FLOAT
#if defined(LIBC_TARGET_CPU_HAS_FPU_DOUBLE)
#define LIBC_TARGET_CPU_HAS_FMA_DOUBLE
#endif // LIBC_TARGET_CPU_HAS_FMA_DOUBLE
#endif

#if defined(LIBC_TARGET_ARCH_IS_AARCH64) ||                                    \
    (defined(LIBC_TARGET_ARCH_IS_X86_64) &&                                    \
     defined(LIBC_TARGET_CPU_HAS_SSE4_2))
#define LIBC_TARGET_CPU_HAS_NEAREST_INT
#endif

#if defined(LIBC_TARGET_ARCH_IS_AARCH64) || defined(LIBC_TARGET_ARCH_IS_GPU)
#define LIBC_TARGET_CPU_HAS_FAST_FLOAT16_OPS
#endif

// WebAssembly exposes its optional (mostly standardized) ISA extensions through
// compiler-defined __wasm_*__ macros, exactly like x86/ARM expose theirs above.
// Gate optional features on these so that code requiring an extension is only
// built when the extension is actually enabled for the target.
#if defined(LIBC_TARGET_ARCH_IS_WASM)
#if defined(__wasm_atomics__)
#define LIBC_TARGET_CPU_HAS_WASM_ATOMICS
#endif
#if defined(__wasm_bulk_memory__)
#define LIBC_TARGET_CPU_HAS_WASM_BULK_MEMORY
#endif
#if defined(__wasm_exception_handling__)
#define LIBC_TARGET_CPU_HAS_WASM_EXCEPTION_HANDLING
#endif
#if defined(__wasm_multivalue__)
#define LIBC_TARGET_CPU_HAS_WASM_MULTIVALUE
#endif
#if defined(__wasm_mutable_globals__)
#define LIBC_TARGET_CPU_HAS_WASM_MUTABLE_GLOBALS
#endif
#if defined(__wasm_nontrapping_fptoint__)
#define LIBC_TARGET_CPU_HAS_WASM_NONTRAPPING_FPTOINT
#endif
#if defined(__wasm_reference_types__)
#define LIBC_TARGET_CPU_HAS_WASM_REFERENCE_TYPES
#endif
#if defined(__wasm_sign_ext__)
#define LIBC_TARGET_CPU_HAS_WASM_SIGN_EXT
#endif
#if defined(__wasm_simd128__)
#define LIBC_TARGET_CPU_HAS_WASM_SIMD128
#endif
#if defined(__wasm_tail_call__)
#define LIBC_TARGET_CPU_HAS_WASM_TAIL_CALL
#endif

// LLVM's built-in setjmp/longjmp lowering for WebAssembly is driven by the
// backend flag -mllvm -wasm-enable-sjlj (which requires the exception-handling,
// multivalue and reference-types features). The flag is opaque to the
// preprocessor, so the build system surfaces it through LIBC_WASM_ENABLE_SJLJ.
#if defined(LIBC_WASM_ENABLE_SJLJ) &&                                          \
    defined(LIBC_TARGET_CPU_HAS_WASM_EXCEPTION_HANDLING) &&                    \
    defined(LIBC_TARGET_CPU_HAS_WASM_MULTIVALUE) &&                            \
    defined(LIBC_TARGET_CPU_HAS_WASM_REFERENCE_TYPES)
#define LIBC_TARGET_CPU_HAS_WASM_SJLJ
#endif
#endif // LIBC_TARGET_ARCH_IS_WASM

#endif // LLVM_LIBC_SRC___SUPPORT_MACROS_PROPERTIES_CPU_FEATURES_H
