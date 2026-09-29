#include "hdr/errno_macros.h"
#include "src/arpa/inet/inet_ntoa.h"
#include "src/ftw/nftw.h"
#include "src/netdb/gethostbyname.h"
#include "src/netdb/getservbyname.h"
#include "src/pthread/pthread_mutex_consistent.h"
#include "src/pthread/pthread_mutex_timedlock.h"
#include "src/stdio/tmpfile.h"
#include "src/stdio/tmpnam.h"
#include "src/sys/stat/mkdir.h"
#include "src/sys/stat/mkfifo.h"
#include "src/sys/stat/mknodat.h"
#include "src/time/strptime.h"
#include "src/unistd/execvpe.h"
#include "src/unistd/getegid.h"
#include "src/unistd/rmdir.h"
#include "src/unistd/symlink.h"
#include "src/unistd/ttyname_r.h"
#include "src/unistd/unlink.h"
#include "test/UnitTest/Test.h"

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <ftw.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <time.h>

static_assert(ESHUTDOWN == 108);
static_assert(F_GETLK64 == F_GETLK);
static_assert(SO_KEEPALIVE == 9);
static_assert(MSG_TRUNC == 0x20);
static_assert(PTHREAD_PRIO_NONE == 0);

TEST(LlvmLibcWasiCompatTest, LegacyAndPortableFunctions) {
  EXPECT_EQ(LIBC_NAMESPACE::getegid(), static_cast<gid_t>(0));

  errno = 0;
  EXPECT_EQ(LIBC_NAMESPACE::gethostbyname("localhost"), nullptr);
  EXPECT_EQ(errno, ENOSYS);
  EXPECT_EQ(h_errno, NO_RECOVERY);

  struct in_addr addr = {htonl(0x7f000001)};
  EXPECT_STREQ(LIBC_NAMESPACE::inet_ntoa(addr), "127.0.0.1");

  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  struct timespec past = {0, 0};
  EXPECT_EQ(LIBC_NAMESPACE::pthread_mutex_timedlock(&mutex, &past), 0);
  EXPECT_EQ(LIBC_NAMESPACE::pthread_mutex_timedlock(&mutex, &past), ETIMEDOUT);
  EXPECT_EQ(LIBC_NAMESPACE::pthread_mutex_consistent(&mutex), EINVAL);

  FILE *stream = LIBC_NAMESPACE::tmpfile();
  ASSERT_NE(stream, nullptr);
  EXPECT_GE(fputs("ok", stream), 0);
  EXPECT_EQ(fclose(stream), 0);

  char path[L_tmpnam];
  ASSERT_NE(LIBC_NAMESPACE::tmpnam(path), nullptr);
  struct stat st;
  EXPECT_EQ(stat(path, &st), -1);
  EXPECT_EQ(errno, ENOENT);
  char second[L_tmpnam];
  ASSERT_NE(LIBC_NAMESPACE::tmpnam(second), nullptr);
  EXPECT_NE(strcmp(path, second), 0);
}

TEST(LlvmLibcWasiCompatTest, UnsupportedOperationsReportErrors) {
  errno = 0;
  EXPECT_EQ(LIBC_NAMESPACE::mkfifo("/tmp/llvm_libc_no_fifo", 0600), -1);
  EXPECT_EQ(errno, ENOSYS);
  errno = 0;
  EXPECT_EQ(LIBC_NAMESPACE::mknodat(AT_FDCWD, "/tmp/llvm_libc_no_node",
                                    S_IFIFO | 0600, 0),
            -1);
  EXPECT_EQ(errno, ENOSYS);
  errno = 0;
  EXPECT_EQ(LIBC_NAMESPACE::execvpe("missing", nullptr, nullptr), -1);
  EXPECT_EQ(errno, ENOSYS);
  errno = 0;
  EXPECT_EQ(LIBC_NAMESPACE::getservbyname("http", "tcp"), nullptr);
  EXPECT_EQ(errno, ENOSYS);
  struct tm result = {};
  errno = 0;
  EXPECT_EQ(LIBC_NAMESPACE::strptime("2026", "%Y", &result), nullptr);
  EXPECT_EQ(errno, ENOSYS);
  char name[32];
  int old_errno = errno;
  EXPECT_EQ(LIBC_NAMESPACE::ttyname_r(-1, name, sizeof(name)), EBADF);
  EXPECT_EQ(errno, old_errno);
}

namespace {
int visited = 0;
int visit(const char *, const struct stat *, int, struct FTW *) {
  ++visited;
  return visited > 20 ? 1 : 0;
}
} // namespace

TEST(LlvmLibcWasiCompatTest, DirectoryTraversal) {
  constexpr char DIR[] = "/tmp/llvm_libc_wasi_compat_test";
  constexpr char LINK[] = "/tmp/llvm_libc_wasi_compat_test/loop";
  LIBC_NAMESPACE::unlink(LINK);
  LIBC_NAMESPACE::rmdir(DIR);
  ASSERT_EQ(LIBC_NAMESPACE::mkdir(DIR, 0700), 0);
  ASSERT_EQ(LIBC_NAMESPACE::symlink(".", LINK), 0);
  visited = 0;
  EXPECT_EQ(LIBC_NAMESPACE::nftw(DIR, visit, 1, FTW_PHYS), 0);
  EXPECT_GE(visited, 2);
  visited = 0;
  EXPECT_EQ(LIBC_NAMESPACE::nftw(DIR, visit, 1, 0), 0);
  EXPECT_GE(visited, 1);
  EXPECT_EQ(LIBC_NAMESPACE::unlink(LINK), 0);
  EXPECT_EQ(LIBC_NAMESPACE::rmdir(DIR), 0);
}
