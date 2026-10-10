# This file is licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

"""Declare a WASIp1 C++ toolchain from Bazel-built inputs.

The sysroot and resource_dir labels may produce directory artifacts or file
groups. Their contents are toolchain inputs of every compile and link action.
For a header-only resource file group, the common directory is "include".
Tool paths require a Linux execution platform with /proc/self/cwd.
"""

load("@rules_cc//cc:defs.bzl", "cc_toolchain")
load(":cc_toolchain_config.bzl", "wasi_cc_toolchain_config")

def _files_impl(ctx):
    return [DefaultInfo(files = depset(transitive = [dep[DefaultInfo].files for dep in ctx.attr.srcs]))]

_files = rule(
    implementation = _files_impl,
    attrs = {"srcs": attr.label_list(allow_files = True, cfg = "exec")},
)

def wasi_cc_toolchain_from_targets(
        name,
        clang,
        clangxx,
        ld,
        ar,
        nm,
        objcopy,
        objdump,
        strip,
        resource_dir,
        sysroot,
        exec_compatible_with = ["@platforms//os:linux", "@platforms//cpu:x86_64"],
        visibility = None):
    """Register a WASIp1 compiler whose tools and data are Bazel targets."""
    tools = [clang, clangxx, ld, ar, nm, objcopy, objdump, strip]
    _files(
        name = name + "_files",
        srcs = tools + [resource_dir, sysroot],
    )
    wasi_cc_toolchain_config(
        name = name + "_config",
        source_built = True,
        clang = clang,
        clangxx = clangxx,
        ld = ld,
        ar = ar,
        nm = nm,
        objcopy = objcopy,
        objdump = objdump,
        strip = strip,
        resource_dir = resource_dir,
        sysroot = sysroot,
    )
    cc_toolchain(
        name = name + "_impl",
        toolchain_identifier = name,
        toolchain_config = name + "_config",
        all_files = name + "_files",
        ar_files = name + "_files",
        as_files = name + "_files",
        compiler_files = name + "_files",
        dwp_files = name + "_files",
        linker_files = name + "_files",
        objcopy_files = name + "_files",
        strip_files = name + "_files",
        supports_param_files = 1,
    )
    native.toolchain(
        name = name,
        toolchain = name + "_impl",
        toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
        exec_compatible_with = exec_compatible_with,
        target_compatible_with = ["@platforms//os:wasi", "@platforms//cpu:wasm32"],
        visibility = visibility,
    )
