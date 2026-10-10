# This file is licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

"""C++ toolchain configuration for a host Clang targeting WASIp1."""

load("@rules_cc//cc:action_names.bzl", "ACTION_NAMES")
load("@bazel_skylib//rules:common_settings.bzl", "BuildSettingInfo")
load("@rules_cc//cc:cc_toolchain_config_lib.bzl", "action_config", "feature", "flag_group", "flag_set", "make_variable", "tool", "tool_path")
load("@rules_cc//cc:defs.bzl", "CcToolchainConfigInfo")
load("@rules_cc//cc/common:cc_common.bzl", "cc_common")
load("@wasi_toolchain//:paths.bzl", "AR_PATH", "CC_PATH", "CXX_PATH", "LD_PATH", "NM_PATH", "OBJCOPY_PATH", "OBJDUMP_PATH", "RESOURCE_DIR", "STRIP_PATH")

_COMPILE_ACTIONS = [
    ACTION_NAMES.c_compile,
    ACTION_NAMES.cpp_compile,
    ACTION_NAMES.cpp_header_parsing,
    ACTION_NAMES.linkstamp_compile,
    ACTION_NAMES.assemble,
    ACTION_NAMES.preprocess_assemble,
]

_LINK_ACTIONS = [
    ACTION_NAMES.cpp_link_executable,
    ACTION_NAMES.cpp_link_dynamic_library,
    ACTION_NAMES.cpp_link_nodeps_dynamic_library,
]

def _sysroot_impl(ctx):
    return [BuildSettingInfo(value = ctx.build_setting_value)]

sysroot_flag = rule(
    implementation = _sysroot_impl,
    build_setting = config.string(flag = True),
)

def _impl(ctx):
    sysroot = ctx.attr._sysroot[BuildSettingInfo].value
    if not sysroot.startswith("/"):
        fail("Set --@llvm-project//wasi:sysroot=/absolute/path/to/wasi-sysroot")

    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        toolchain_identifier = "llvm-wasm32-wasip1",
        host_system_name = "linux",
        target_system_name = "wasm32-unknown-wasip1",
        target_cpu = "wasm32",
        target_libc = "wasi",
        compiler = "clang",
        abi_version = "wasip1",
        abi_libc_version = "wasip1",
        cxx_builtin_include_directories = [
            sysroot + "/include",
            sysroot + "/include/wasm32-wasip1",
            sysroot + "/include/c++/v1",
            RESOURCE_DIR + "/include",
        ],
        builtin_sysroot = sysroot,
        action_configs = [
            action_config(
                action_name = action,
                enabled = True,
                tools = [tool(path = CXX_PATH)],
            )
            for action in _LINK_ACTIONS
        ],
        tool_paths = [
            tool_path(name = "gcc", path = CC_PATH),
            tool_path(name = "cpp", path = CXX_PATH),
            # CXX drives links so it can supply C++ runtimes and startup files.
            tool_path(name = "ld", path = CXX_PATH),
            tool_path(name = "ar", path = AR_PATH),
            tool_path(name = "nm", path = NM_PATH),
            tool_path(name = "objcopy", path = OBJCOPY_PATH),
            tool_path(name = "objdump", path = OBJDUMP_PATH),
            tool_path(name = "strip", path = STRIP_PATH),
            tool_path(name = "gcov", path = CC_PATH),
        ],
        features = [
            feature(
                name = "opt",
                flag_sets = [flag_set(
                    actions = _COMPILE_ACTIONS,
                    flag_groups = [flag_group(flags = ["-O2", "-DNDEBUG"])],
                )],
            ),
            feature(
                name = "dbg",
                flag_sets = [flag_set(
                    actions = _COMPILE_ACTIONS,
                    flag_groups = [flag_group(flags = ["-O0", "-g"])],
                )],
            ),
            feature(
                name = "fastbuild",
                flag_sets = [flag_set(
                    actions = _COMPILE_ACTIONS,
                    flag_groups = [flag_group(flags = ["-O0"])],
                )],
            ),
            feature(
                name = "wasi_sections",
                enabled = True,
                flag_sets = [
                    flag_set(
                        actions = _COMPILE_ACTIONS,
                        flag_groups = [flag_group(flags = ["-ffunction-sections", "-fdata-sections"])],
                    ),
                    flag_set(
                        actions = _LINK_ACTIONS,
                        flag_groups = [flag_group(flags = ["-Wl,--gc-sections"])],
                    ),
                ],
            ),
            feature(
                name = "wasi_target",
                enabled = True,
                flag_sets = [
                    flag_set(
                        actions = _COMPILE_ACTIONS + _LINK_ACTIONS,
                        flag_groups = [flag_group(flags = [
                            "--target=wasm32-unknown-wasip1",
                            "--sysroot=" + sysroot,
                        ])],
                    ),
                    flag_set(
                        actions = _LINK_ACTIONS,
                        flag_groups = [flag_group(flags = ["-fuse-ld=" + LD_PATH])],
                    ),
                ],
            ),
            feature(name = "supports_pic", enabled = True),
            feature(name = "supports_start_end_lib", enabled = True),
        ],
        make_variables = [
            make_variable(name = "STACK_FRAME_UNLIMITED", value = "-Wno-frame-larger-than"),
        ],
    )

wasi_cc_toolchain_config = rule(
    implementation = _impl,
    attrs = {
        "_sysroot": attr.label(default = Label("//wasi:sysroot")),
        "clang": attr.label(allow_single_file = True, mandatory = True),
        "clangxx": attr.label(allow_single_file = True, mandatory = True),
        "ld": attr.label(allow_single_file = True, mandatory = True),
        "ar": attr.label(allow_single_file = True, mandatory = True),
        "nm": attr.label(allow_single_file = True, mandatory = True),
        "objcopy": attr.label(allow_single_file = True, mandatory = True),
        "objdump": attr.label(allow_single_file = True, mandatory = True),
        "strip": attr.label(allow_single_file = True, mandatory = True),
    },
    provides = [CcToolchainConfigInfo],
)
