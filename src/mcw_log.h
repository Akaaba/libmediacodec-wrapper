/**
 * Copyright (c) 2017 Aurelien Barre
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the copyright holder nor the names of the
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MCW_LOG_H_
#define _MCW_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(BUILD_LIBULOG)

#define ULOG_TAG libmcw
#include <ulog.h>

#define MCW_LOGD(_fmt, ...) ULOGD(_fmt, ##__VA_ARGS__)
#define MCW_LOGI(_fmt, ...) ULOGI(_fmt, ##__VA_ARGS__)
#define MCW_LOGW(_fmt, ...) ULOGW(_fmt, ##__VA_ARGS__)
#define MCW_LOGE(_fmt, ...) ULOGE(_fmt, ##__VA_ARGS__)

#else /* !BUILD_LIBULOG */

#include <stdio.h>

#define MCW_LOG(_fmt, ...)  fprintf(stderr, _fmt "\n", ##__VA_ARGS__)
#define MCW_LOGD(_fmt, ...) MCW_LOG("[D]" _fmt, ##__VA_ARGS__)
#define MCW_LOGI(_fmt, ...) MCW_LOG("[I]" _fmt, ##__VA_ARGS__)
#define MCW_LOGW(_fmt, ...) MCW_LOG("[W]" _fmt, ##__VA_ARGS__)
#define MCW_LOGE(_fmt, ...) MCW_LOG("[E]" _fmt, ##__VA_ARGS__)

#endif /* !BUILD_LIBULOG */

/** Log error with errno */
#define MCW_LOG_ERRNO(_fct, _err) \
	MCW_LOGE("%s:%d: %s err=%d(%s)", __func__, __LINE__, \
			_fct, _err, strerror(_err))

/** Log error with fd and errno */
#define MCW_LOG_FD_ERRNO(_fct, _fd, _err) \
	MCW_LOGE("%s:%d: %s(fd=%d) err=%d(%s)", __func__, __LINE__, \
			_fct, _fd, _err, strerror(_err))

/** Log error if condition failed and return from function */
#define MCW_RETURN_IF_FAILED(_cond, _err) \
	do { \
		if (!(_cond)) { \
			MCW_LOGE("%s:%d: err=%d(%s)", __func__, __LINE__, \
					(_err), strerror(-(_err))); \
			return; \
		} \
	} while (0)

/** Log error if condition failed and return error from function */
#define MCW_RETURN_ERR_IF_FAILED(_cond, _err) \
	do { \
		if (!(_cond)) { \
			MCW_LOGE("%s:%d: err=%d(%s)", __func__, __LINE__, \
					(_err), strerror(-(_err))); \
			/* codecheck_ignore[RETURN_PARENTHESES] */ \
			return (_err); \
		} \
	} while (0)

/** Log error if condition failed and return value from function */
#define MCW_RETURN_VAL_IF_FAILED(_cond, _err, _val) \
	do { \
		if (!(_cond)) { \
			MCW_LOGE("%s:%d: err=%d(%s)", __func__, __LINE__, \
					(_err), strerror(-(_err))); \
			/* codecheck_ignore[RETURN_PARENTHESES] */ \
			return (_val); \
		} \
	} while (0)

/** Log error if condition failed and return error from function */
#define MCW_LOG_ERR_AND_RETURN_ERR_IF_FAILED(_cond, _err, _fmt, ...) \
	do { \
		if (!(_cond)) { \
			MCW_LOGE("%s:%d: err=%d(%s) " _fmt, \
				__func__, __LINE__, \
				(_err), strerror(-(_err)), ##__VA_ARGS__); \
			/* codecheck_ignore[RETURN_PARENTHESES] */ \
			return (_err); \
		} \
	} while (0)

/** Log error if condition failed and return value from function */
#define MCW_LOG_ERR_AND_RETURN_VAL_IF_FAILED(_cond, _err, _val, _fmt, ...) \
	do { \
		if (!(_cond)) { \
			MCW_LOGE("%s:%d: err=%d(%s) " _fmt, \
				__func__, __LINE__, \
				(_err), strerror(-(_err)), ##__VA_ARGS__); \
			/* codecheck_ignore[RETURN_PARENTHESES] */ \
			return (_val); \
		} \
	} while (0)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !_MCW_LOG_H_ */
