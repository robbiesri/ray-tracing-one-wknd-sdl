// Heavily inspired by
// http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/

#pragma once

#if defined(_DEBUG)
#define WKND_ASSERTS_ENABLED
#endif

namespace Assert {

enum class FailBehavior {
  Halt,
  Continue,
};

typedef FailBehavior (*Handler)(const char *condition, const char *msg,
                                const char *file, int line);

Handler GetHandler();
void SetHandler(Handler newHandler);

FailBehavior ReportFailure(const char *condition, const char *file, int line,
                           const char *msg, ...);

} // namespace Assert

#if defined(WIN32)
#define WKND_HALT() (__debugbreak())

#else
#if defined(CLANG) && (defined(X86) || defined(X64))
#define WKND_HALT() (__asm__ volatile("int $0x03"))
#elif defined(__APPLE__)
#define WKND_HALT() (raise(SIGTRAP))
#elif defined(GNUC)
#define WKND_HALT() (__builtin_trap())
#else
  // TODO ARM/Android?
  // TODO Static assert
#define WKND_HALT() ((void)0)
#endif
#endif

// TODO There's a future where we can forward the args to an empty function,
//      a la https://foonathan.net/2016/09/assertions/

#define WKND_UNUSED(x)                                                         \
  do {                                                                         \
    (void)sizeof(x);                                                           \
  } while (0)

#ifdef WKND_ASSERTS_ENABLED
#define WKND_ASSERT(cond)                                                      \
  do {                                                                         \
    if (!(cond)) {                                                             \
      if (Assert::ReportFailure(#cond, __FILE__, __LINE__, 0) ==         \
          Assert::FailBehavior::Halt)                                    \
        WKND_HALT();                                                           \
    }                                                                          \
  } while (0)

#define WKND_ASSERT_MSG(cond, msg, ...)                                        \
  do {                                                                         \
    if (!(cond)) {                                                             \
      if (Assert::ReportFailure(#cond, __FILE__, __LINE__, (msg),        \
                                      __VA_ARGS__) ==                          \
          Assert::FailBehavior::Halt)                                    \
        WKND_HALT();                                                           \
    }                                                                          \
  } while (0)

#define WKND_ASSERT_FAIL(msg, ...)                                             \
  do {                                                                         \
    if (Assert::ReportFailure(0, __FILE__, __LINE__, (msg),              \
                                    __VA_ARGS__) ==                            \
        Assert::FailBehavior::Halt)                                      \
      WKND_HALT();                                                             \
  } while (0)

#else
#define WKND_ASSERT(condition)                                                 \
  do {                                                                         \
    WKND_UNUSED(condition);                                                    \
  } while (0)
#define WKND_ASSERT_MSG(condition, msg, ...)                                   \
  do {                                                                         \
    WKND_UNUSED(condition);                                                    \
    WKND_UNUSED(msg);                                                          \
  } while (0)
#define WKND_ASSERT_FAIL(msg, ...)                                             \
  do {                                                                         \
    WKND_UNUSED(msg);                                                          \
  } while (0)
#endif

#define WKND_STATIC_ASSERT(x) typedef char wkndStaticAssert[(x) ? 1 : -1];