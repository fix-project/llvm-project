# This file is licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

"""Expose compiler tools for the WASIp1 target toolchain."""

def _realpath(ctx, path):
    result = ctx.execute(["readlink", "-f", str(path)])
    if result.return_code:
        fail("Unable to resolve " + str(path))
    return result.stdout.strip()

def _tool(ctx, variable, default, cc_dir = None):
    value = ctx.os.environ.get(variable, "")
    if value:
        path = ctx.path(value) if value.startswith("/") else ctx.which(value)
    else:
        candidate = cc_dir.get_child(default) if cc_dir else None
        path = candidate if candidate and candidate.exists else ctx.which(default)
    if not path or not path.exists:
        fail("Set --repo_env={} to an executable path (could not find {})".format(variable, value or default))
    return path

def _wasi_toolchain_repository_impl(ctx):
    cc = _tool(ctx, "TARGET_CC", "clang")
    cc_dir = cc.dirname
    tools = {
        "cc": cc,
        "cxx": _tool(ctx, "TARGET_CXX", "clang++", cc_dir),
        "ld": _tool(ctx, "TARGET_LD", "wasm-ld", cc_dir),
        "ar": _tool(ctx, "TARGET_AR", "llvm-ar", cc_dir),
        "nm": _tool(ctx, "TARGET_NM", "llvm-nm", cc_dir),
        "objcopy": _tool(ctx, "TARGET_OBJCOPY", "llvm-objcopy", cc_dir),
        "objdump": _tool(ctx, "TARGET_OBJDUMP", "llvm-objdump", cc_dir),
        "strip": _tool(ctx, "TARGET_STRIP", "llvm-strip", cc_dir),
    }
    resource = ctx.execute([str(cc), "--print-resource-dir"])
    if resource.return_code:
        fail("TARGET_CC did not report its resource directory: " + resource.stderr)
    resource_dir = ctx.path(resource.stdout.strip())
    if not resource_dir.exists:
        fail("TARGET_CC resource directory does not exist: " + str(resource_dir))

    for name, path in tools.items():
        ctx.symlink(path, "tools/" + name)
    ctx.symlink(resource_dir, "resource")
    paths = [name.upper() + "_PATH = " + repr(str(path)) for name, path in tools.items()]
    paths += [
        "RESOURCE_DIR = " + repr(_realpath(ctx, resource_dir)),
    ]
    ctx.file("paths.bzl", "\n".join(paths) + "\n")
    ctx.file("BUILD.bazel", """
package(default_visibility = ["//visibility:public"])

TOOL_BINARIES = ["tools/cc", "tools/cxx", "tools/ld", "tools/ar",
                 "tools/nm", "tools/objcopy", "tools/objdump", "tools/strip"]
exports_files(TOOL_BINARIES)

filegroup(
    name = "files",
    srcs = TOOL_BINARIES + glob(["resource/**"]),
)

filegroup(
    name = "compiler_files",
    srcs = TOOL_BINARIES + glob(["resource/**"]),
)

filegroup(
    name = "linker_files",
    srcs = TOOL_BINARIES + glob(["resource/**"]),
)
""")

wasi_toolchain_repository = repository_rule(
    implementation = _wasi_toolchain_repository_impl,
    environ = ["TARGET_CC", "TARGET_CXX", "TARGET_LD", "TARGET_AR", "TARGET_NM", "TARGET_OBJCOPY", "TARGET_OBJDUMP", "TARGET_STRIP"],
    local = True,
)
