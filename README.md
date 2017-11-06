# libmediacodec-wrapper - Android MediaCodec API wrapper library

libmediacodec-wrapper (MCW) is a C library to wrap Android's NDK C
AMediaCodec API and Android's SDK Java MediaCodec API through JNI.

The library automatically detects at runtime if the NDK API is available
(Android 5.0+, API level 21) and if the Java API is available (Android 4.1+,
API level 16).

The user can either force the use of the NDK or JNI implementations or let
the library choose automatically. In auto mode the NDK implementation is
used in priority if available.