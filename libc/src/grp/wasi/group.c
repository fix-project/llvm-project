//===-- Synthetic WASI group identity ------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <errno.h>
#include <grp.h>
#include <stdint.h>
#include <string.h>

// WASI preview1 has no ownership model. struct stat reports gid 0, so expose
// one matching synthetic group for programs that print or resolve file owners.
static char root_name[] = "root";
static char empty[] = "";
static char *members[] = {0};
static struct group root_group = {root_name, empty, 0, members};

struct group *getgrgid(gid_t gid) { return gid == 0 ? &root_group : 0; }

struct group *getgrnam(const char *name) {
  return name != 0 && strcmp(name, root_name) == 0 ? &root_group : 0;
}

static int copy_group(struct group *source, struct group *group, char *buffer,
                      size_t size, struct group **result) {
  if (result == 0)
    return EINVAL;
  *result = 0;
  if (source == 0)
    return 0;
  if (group == 0 || buffer == 0)
    return EINVAL;

  uintptr_t start = (uintptr_t)buffer;
  uintptr_t aligned = (start + sizeof(char *) - 1) & ~(sizeof(char *) - 1);
  size_t padding = aligned - start;
  const size_t needed = sizeof(char *) + sizeof(root_name) + sizeof(empty);
  if (padding > size || size - padding < needed)
    return ERANGE;

  char **members_out = (char **)aligned;
  members_out[0] = 0;
  char *next = (char *)(members_out + 1);
  group->gr_name = next;
  memcpy(next, root_name, sizeof(root_name));
  next += sizeof(root_name);
  group->gr_passwd = next;
  memcpy(next, empty, sizeof(empty));
  group->gr_gid = source->gr_gid;
  group->gr_mem = members_out;
  *result = group;
  return 0;
}

int getgrgid_r(gid_t gid, struct group *group, char *buffer, size_t size,
               struct group **result) {
  return copy_group(getgrgid(gid), group, buffer, size, result);
}

int getgrnam_r(const char *name, struct group *group, char *buffer, size_t size,
               struct group **result) {
  return copy_group(getgrnam(name), group, buffer, size, result);
}

static int group_cursor;

void setgrent(void) { group_cursor = 0; }

struct group *getgrent(void) {
  if (group_cursor != 0)
    return 0;
  group_cursor = 1;
  return &root_group;
}

void endgrent(void) { group_cursor = 0; }

int initgroups(const char *user, gid_t group) {
  (void)user;
  (void)group;
  errno = ENOSYS;
  return -1;
}

int getgrouplist(const char *user, gid_t group, gid_t *groups, int *count) {
  (void)user;
  if (count == 0) {
    errno = EINVAL;
    return -1;
  }
  if (*count < 1 || groups == 0) {
    *count = 1;
    return -1;
  }
  groups[0] = group;
  *count = 1;
  return 1;
}
