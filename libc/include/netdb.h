//===-- POSIX header <netdb.h> ------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===---------------------------------------------------------------------===//

#ifndef _LLVM_LIBC_NETDB_H
#define _LLVM_LIBC_NETDB_H

#include "__llvm-libc-common.h"
#include "llvm-libc-types/socklen_t.h"
#include "llvm-libc-types/struct_addrinfo.h"
#include "llvm-libc-types/struct_sockaddr.h"

#include <sys/socket.h>

__BEGIN_C_DECLS

#define EAI_BADFLAGS (-1)
#define EAI_NONAME (-2)
#define EAI_AGAIN (-3)
#define EAI_FAIL (-4)
#define EAI_FAMILY (-6)
#define EAI_SOCKTYPE (-7)
#define EAI_SERVICE (-8)
#define EAI_MEMORY (-10)
#define EAI_SYSTEM (-11)
#define EAI_OVERFLOW (-12)
#define EAI_NODATA (-5)

#define AI_PASSIVE 0x0001
#define AI_CANONNAME 0x0002
#define AI_NUMERICHOST 0x0004
#define AI_NUMERICSERV 0x0400
#define AI_ALL 0x0100
#define AI_ADDRCONFIG 0x0020
#define AI_V4MAPPED 0x0080

#define NI_NOFQDN 0x01
#define NI_NUMERICHOST 0x02
#define NI_NAMEREQD 0x04
#define NI_NUMERICSERV 0x08
#define NI_DGRAM 0x10

#define NI_MAXHOST 1025
#define NI_MAXSERV 32

#ifdef __wasi__
#define HOST_NOT_FOUND 1
#define TRY_AGAIN 2
#define NO_RECOVERY 3
#define NO_DATA 4

struct hostent {
  char *h_name;
  char **h_aliases;
  int h_addrtype;
  int h_length;
  char **h_addr_list;
};
#define h_addr h_addr_list[0]

struct servent {
  char *s_name;
  char **s_aliases;
  int s_port;
  char *s_proto;
};

struct protoent {
  char *p_name;
  char **p_aliases;
  int p_proto;
};

extern int h_errno;

struct hostent *gethostbyname(const char *) __NOEXCEPT;
struct hostent *gethostbyaddr(const void *, socklen_t, int) __NOEXCEPT;
struct servent *getservent(void) __NOEXCEPT;
struct servent *getservbyname(const char *, const char *) __NOEXCEPT;
struct servent *getservbyport(int, const char *) __NOEXCEPT;
struct protoent *getprotobyname(const char *) __NOEXCEPT;
void setservent(int) __NOEXCEPT;
void endservent(void) __NOEXCEPT;
#endif // __wasi__

int getaddrinfo(const char *__restrict, const char *__restrict,
                const struct addrinfo *__restrict,
                struct addrinfo **__restrict) __NOEXCEPT;

void freeaddrinfo(struct addrinfo *) __NOEXCEPT;

int getnameinfo(const struct sockaddr *__restrict, socklen_t, char *__restrict,
                socklen_t, char *__restrict, socklen_t, int) __NOEXCEPT;

const char *gai_strerror(int) __NOEXCEPT;

__END_C_DECLS

#endif // _LLVM_LIBC_NETDB_H
