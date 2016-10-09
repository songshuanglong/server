#ifndef PLATFORM_H
#define PLATFORM_H
#define ASPLATFORM_H
#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#  define PLATFORM_DARWIN
#  define PLATFORM_BSD4
#  ifdef __LP64__
#    define PLATFORM_DARWIN64
#  else
#    define PLATFORM_DARWIN32
#  endif
#elif defined(__CYGWIN__)
#  define PLATFORM_CYGWIN
#elif defined(MSDOS) || defined(_MSDOS)
#  define PLATFORM_MSDOS
#elif defined(__OS2__)
#  if defined(__EMX__)
#    define PLATFORM_OS2EMX
#  else
#    define PLATFORM_OS2
#  endif
#elif !defined(SAG_COM) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#  define PLATFORM_WIN32
#  define PLATFORM_WIN64
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#  if defined(WINCE) || defined(_WIN32_WCE)
#    define PLATFORM_WINCE
#  else
#    define PLATFORM_WIN32
#  endif
#elif defined(__MWERKS__) && defined(__INTEL__)
#  define PLATFORM_WIN32
#elif defined(__sun) || defined(sun)
#  define PLATFORM_SOLARIS
#elif defined(hpux) || defined(__hpux)
#  define PLATFORM_HPUX
#elif defined(__ultrix) || defined(ultrix)
#  define PLATFORM_ULTRIX
#elif defined(sinix)
#  define PLATFORM_RELIANT
#elif defined(__native_client__)
#  define PLATFORM_NACL
#elif defined(__linux__) || defined(__linux)
#  define PLATFORM_LINUX
#elif defined(__FreeBSD__) || defined(__DragonFly__)
#  define PLATFORM_FREEBSD
#  define PLATFORM_BSD4
#elif defined(__NetBSD__)
#  define PLATFORM_NETBSD
#  define PLATFORM_BSD4
#elif defined(__OpenBSD__)
#  define PLATFORM_OPENBSD
#  define PLATFORM_BSD4
#elif defined(__bsdi__)
#  define PLATFORM_BSDI
#  define PLATFORM_BSD4
#elif defined(__sgi)
#  define PLATFORM_IRIX
#elif defined(__osf__)
#  define PLATFORM_OSF
#elif defined(_AIX)
#  define PLATFORM_AIX
#elif defined(__Lynx__)
#  define PLATFORM_LYNX
#elif defined(__GNU__)
#  define PLATFORM_HURD
#elif defined(__DGUX__)
#  define PLATFORM_DGUX
#elif defined(__QNXNTO__)
#  define PLATFORM_QNX
#elif defined(_SEQUENT_)
#  define PLATFORM_DYNIX
#elif defined(_SCO_DS) /* SCO OpenServer 5 + GCC */
#  define PLATFORM_SCO
#elif defined(__USLC__) /* all SCO platforms + UDK or OUDK */
#  define PLATFORM_UNIXWARE
#elif defined(__svr4__) && defined(i386) /* Open UNIX 8 + GCC */
#  define PLATFORM_UNIXWARE
#elif defined(__INTEGRITY)
#  define PLATFORM_INTEGRITY
#elif defined(VXWORKS) /* there is no "real" VxWorks define - this has to be set in the mkspec! */
#  define PLATFORM_VXWORKS
#elif defined(__MAKEDEPEND__)
#else
#  error "Unsupported platform!"
#endif

#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64) || defined(PLATFORM_WINCE)
#  define PLATFORM_WIN
#endif

#if defined(PLATFORM_DARWIN)
#  define PLATFORM_MAC
#  define PLATFORM_MACX
#  if defined(PLATFORM_DARWIN64)
#     define PLATFORM_MAC64
#  elif defined(PLATFORM_DARWIN32)
#     define PLATFORM_MAC32
#  endif
#endif
#endif // PLATFORM_H
