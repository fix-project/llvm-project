//===-- Synthetic WASI user identity -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <pwd.h>
#include <errno.h>
#include <string.h>

// WASI preview1 has no ownership model. struct stat reports uid 0, so expose
// one matching synthetic user for programs that print or resolve file owners.
static char root_name[] = "root";
static char empty[] = "";
static char root_dir[] = "/";
static char shell[] = "/bin/sh";
static struct passwd root_user = {root_name, empty, 0, 0, empty, root_dir,
                                  shell};

struct passwd *getpwuid(uid_t uid) { return uid == 0 ? &root_user : 0; }

struct passwd *getpwnam(const char *name) {
  return name != 0 && strcmp(name, root_name) == 0 ? &root_user : 0;
}

static int copy_passwd(struct passwd *source, struct passwd *pwd, char *buffer,
                       size_t size, struct passwd **result) {
  if (result == 0)
    return EINVAL;
  *result = 0;
  if (source == 0)
    return 0;
  if (pwd == 0 || buffer == 0)
    return EINVAL;

  const size_t needed = sizeof(root_name) + sizeof(empty) + sizeof(empty) +
                        sizeof(root_dir) + sizeof(shell);
  if (size < needed)
    return ERANGE;

  char *next = buffer;
  pwd->pw_name = next;
  memcpy(next, root_name, sizeof(root_name));
  next += sizeof(root_name);
  pwd->pw_passwd = next;
  memcpy(next, empty, sizeof(empty));
  next += sizeof(empty);
  pwd->pw_gecos = next;
  memcpy(next, empty, sizeof(empty));
  next += sizeof(empty);
  pwd->pw_dir = next;
  memcpy(next, root_dir, sizeof(root_dir));
  next += sizeof(root_dir);
  pwd->pw_shell = next;
  memcpy(next, shell, sizeof(shell));
  pwd->pw_uid = source->pw_uid;
  pwd->pw_gid = source->pw_gid;
  *result = pwd;
  return 0;
}

int getpwuid_r(uid_t uid, struct passwd *pwd, char *buffer, size_t size,
               struct passwd **result) {
  return copy_passwd(getpwuid(uid), pwd, buffer, size, result);
}

int getpwnam_r(const char *name, struct passwd *pwd, char *buffer, size_t size,
               struct passwd **result) {
  return copy_passwd(getpwnam(name), pwd, buffer, size, result);
}

static int passwd_cursor;

void setpwent(void) { passwd_cursor = 0; }

struct passwd *getpwent(void) {
  if (passwd_cursor != 0)
    return 0;
  passwd_cursor = 1;
  return &root_user;
}

void endpwent(void) { passwd_cursor = 0; }
