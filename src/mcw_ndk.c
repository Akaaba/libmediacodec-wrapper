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

#include "mcw_ndk.h"

#include <dlfcn.h>


#define LIBMEDIANDK "libmediandk.so"


/**
 * From NdkMediaError.h
 */

enum media_status {
	AMEDIA_OK = 0,

	AMEDIA_ERROR_BASE                  = -10000,
	AMEDIA_ERROR_UNKNOWN               = AMEDIA_ERROR_BASE,
	AMEDIA_ERROR_MALFORMED             = AMEDIA_ERROR_BASE - 1,
	AMEDIA_ERROR_UNSUPPORTED           = AMEDIA_ERROR_BASE - 2,
	AMEDIA_ERROR_INVALID_OBJECT        = AMEDIA_ERROR_BASE - 3,
	AMEDIA_ERROR_INVALID_PARAMETER     = AMEDIA_ERROR_BASE - 4,
};


/**
 * From NdkMediaFormat.h
 */

struct AMediaFormat;

typedef struct AMediaFormat *(*AMediaFormat_new_t)(
	void);
typedef enum media_status (*AMediaFormat_delete_t)(
	struct AMediaFormat *);
typedef const char *(*AMediaFormat_toString_t)(
	struct AMediaFormat *);
typedef bool (*AMediaFormat_getInt32_t)(
	struct AMediaFormat *,
	const char *name,
	int32_t *out);
typedef bool (*AMediaFormat_getInt64_t)(
	struct AMediaFormat *,
	const char *name,
	int64_t *out);
typedef bool (*AMediaFormat_getFloat_t)(
	struct AMediaFormat *,
	const char *name,
	float *out);
typedef bool (*AMediaFormat_getBuffer_t)(
	struct AMediaFormat *,
	const char *name,
	void **data,
	size_t *size);
typedef bool (*AMediaFormat_getString_t)(
	struct AMediaFormat *,
	const char *name,
	const char **out);
typedef void (*AMediaFormat_setInt32_t)(
	struct AMediaFormat *,
	const char *name,
	int32_t value);
typedef void (*AMediaFormat_setInt64_t)(
	struct AMediaFormat *,
	const char *name,
	int64_t value);
typedef void (*AMediaFormat_setFloat_t)(
	struct AMediaFormat *,
	const char *name,
	float value);
typedef void (*AMediaFormat_setString_t)(
	struct AMediaFormat *,
	const char *name,
	const char *value);
typedef void (*AMediaFormat_setBuffer_t)(
	struct AMediaFormat *,
	const char *name,
	void *data,
	size_t size);


/**
 * From NdkMediaCodec.h
 */

struct AMediaCodec;

struct AMediaCodecBufferInfo {
	int32_t offset;
	int32_t size;
	int64_t presentationTimeUs;
	uint32_t flags;
};

struct AMediaCrypto;

enum {
	AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM = 4,
	AMEDIACODEC_CONFIGURE_FLAG_ENCODE = 1,
	AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED = -3,
	AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED = -2,
	AMEDIACODEC_INFO_TRY_AGAIN_LATER = -1
};

typedef struct AMediaCodec *(*AMediaCodec_createCodecByName_t)(
	const char *name);
typedef struct AMediaCodec *(*AMediaCodec_createDecoderByType_t)(
	const char *mime_type);
typedef struct AMediaCodec *(*AMediaCodec_createEncoderByType_t)(
	const char *mime_type);
typedef enum media_status (*AMediaCodec_delete_t)(
	struct AMediaCodec *);
typedef enum media_status (*AMediaCodec_configure_t)(
	struct AMediaCodec *,
	const struct AMediaFormat *format,
	ANativeWindow *surface,
	struct AMediaCrypto *crypto,
	uint32_t flags);
typedef enum media_status (*AMediaCodec_start_t)(
	struct AMediaCodec *);
typedef enum media_status (*AMediaCodec_stop_t)(
	struct AMediaCodec *);
typedef enum media_status (*AMediaCodec_flush_t)(
	struct AMediaCodec *);
typedef uint8_t *(*AMediaCodec_getInputBuffer_t)(
	struct AMediaCodec *,
	size_t idx,
	size_t *out_size);
typedef uint8_t *(*AMediaCodec_getOutputBuffer_t)(
	struct AMediaCodec *,
	size_t idx,
	size_t *out_size);
typedef ssize_t (*AMediaCodec_dequeueInputBuffer_t)(
	struct AMediaCodec *,
	int64_t timeoutUs);
typedef enum media_status (*AMediaCodec_queueInputBuffer_t)(
	struct AMediaCodec *,
	size_t idx,
	off_t offset,
	size_t size,
	uint64_t time,
	uint32_t flags);
typedef ssize_t (*AMediaCodec_dequeueOutputBuffer_t)(
	struct AMediaCodec *,
	struct AMediaCodecBufferInfo *info,
	int64_t timeoutUs);
typedef struct AMediaFormat *(*AMediaCodec_getOutputFormat_t)(
	struct AMediaCodec *);
typedef enum media_status (*AMediaCodec_releaseOutputBuffer_t)(
	struct AMediaCodec *,
	size_t idx,
	bool render);
typedef enum media_status (*AMediaCodec_releaseOutputBufferAtTime_t)(
	struct AMediaCodec *,
	size_t idx,
	int64_t timestampNs);


/**
 * Implemetation
 */

static struct mcw_ndk {
	struct {
		AMediaFormat_new_t new;
		AMediaFormat_delete_t delete;
		AMediaFormat_toString_t toString;
		AMediaFormat_getInt32_t getInt32;
		AMediaFormat_getInt64_t getInt64;
		AMediaFormat_getFloat_t getFloat;
		AMediaFormat_getBuffer_t getBuffer;
		AMediaFormat_getString_t getString;
		AMediaFormat_setInt32_t setInt32;
		AMediaFormat_setInt64_t setInt64;
		AMediaFormat_setFloat_t setFloat;
		AMediaFormat_setString_t setString;
		AMediaFormat_setBuffer_t setBuffer;
		const char **KEY_AAC_PROFILE;
		const char **KEY_BIT_RATE;
		const char **KEY_CHANNEL_COUNT;
		const char **KEY_CHANNEL_MASK;
		const char **KEY_COLOR_FORMAT;
		const char **KEY_DURATION;
		const char **KEY_FLAC_COMPRESSION_LEVEL;
		const char **KEY_FRAME_RATE;
		const char **KEY_HEIGHT;
		const char **KEY_IS_ADTS;
		const char **KEY_IS_AUTOSELECT;
		const char **KEY_IS_DEFAULT;
		const char **KEY_IS_FORCED_SUBTITLE;
		const char **KEY_I_FRAME_INTERVAL;
		const char **KEY_LANGUAGE;
		const char **KEY_MAX_HEIGHT;
		const char **KEY_MAX_INPUT_SIZE;
		const char **KEY_MAX_WIDTH;
		const char **KEY_MIME;
		const char **KEY_PUSH_BLANK_BUFFERS_ON_STOP;
		const char **KEY_REPEAT_PREVIOUS_FRAME_AFTER;
		const char **KEY_SAMPLE_RATE;
		const char **KEY_WIDTH;
		const char **KEY_STRIDE;
	} AMediaFormat;
	struct {
		AMediaCodec_createCodecByName_t createCodecByName;
		AMediaCodec_createDecoderByType_t createDecoderByType;
		AMediaCodec_createEncoderByType_t createEncoderByType;
		AMediaCodec_delete_t delete;
		AMediaCodec_configure_t configure;
		AMediaCodec_start_t start;
		AMediaCodec_stop_t stop;
		AMediaCodec_flush_t flush;
		AMediaCodec_getInputBuffer_t getInputBuffer;
		AMediaCodec_getOutputBuffer_t getOutputBuffer;
		AMediaCodec_dequeueInputBuffer_t dequeueInputBuffer;
		AMediaCodec_queueInputBuffer_t queueInputBuffer;
		AMediaCodec_dequeueOutputBuffer_t dequeueOutputBuffer;
		AMediaCodec_getOutputFormat_t getOutputFormat;
		AMediaCodec_releaseOutputBuffer_t releaseOutputBuffer;
		AMediaCodec_releaseOutputBufferAtTime_t
			releaseOutputBufferAtTime;
	} AMediaCodec;
} mcw_ndk;


static const struct {
	const char *name;
	int offset;
} mcw_ndk_sym[] = {
	{ "AMediaFormat_new",
		offsetof(struct mcw_ndk,
		AMediaFormat.new) },
	{ "AMediaFormat_delete",
		offsetof(struct mcw_ndk,
		AMediaFormat.delete) },
	{ "AMediaFormat_toString",
		offsetof(struct mcw_ndk,
		AMediaFormat.toString) },
	{ "AMediaFormat_getInt32",
		offsetof(struct mcw_ndk,
		AMediaFormat.getInt32) },
	{ "AMediaFormat_getInt64",
		offsetof(struct mcw_ndk,
		AMediaFormat.getInt64) },
	{ "AMediaFormat_getFloat",
		offsetof(struct mcw_ndk,
		AMediaFormat.getFloat) },
	{ "AMediaFormat_getBuffer",
		offsetof(struct mcw_ndk,
		AMediaFormat.getBuffer) },
	{ "AMediaFormat_getString",
		offsetof(struct mcw_ndk,
		AMediaFormat.getString) },
	{ "AMediaFormat_setInt32",
		offsetof(struct mcw_ndk,
		AMediaFormat.setInt32) },
	{ "AMediaFormat_setInt64",
		offsetof(struct mcw_ndk,
		AMediaFormat.setInt64) },
	{ "AMediaFormat_setFloat",
		offsetof(struct mcw_ndk,
		AMediaFormat.setFloat) },
	{ "AMediaFormat_setString",
		offsetof(struct mcw_ndk,
		AMediaFormat.setString) },
	{ "AMediaFormat_setBuffer",
		offsetof(struct mcw_ndk,
		AMediaFormat.setBuffer) },
	{ "AMEDIAFORMAT_KEY_AAC_PROFILE",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_AAC_PROFILE) },
	{ "AMEDIAFORMAT_KEY_BIT_RATE",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_BIT_RATE) },
	{ "AMEDIAFORMAT_KEY_CHANNEL_COUNT",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_CHANNEL_COUNT) },
	{ "AMEDIAFORMAT_KEY_CHANNEL_MASK",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_CHANNEL_MASK) },
	{ "AMEDIAFORMAT_KEY_COLOR_FORMAT",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_COLOR_FORMAT) },
	{ "AMEDIAFORMAT_KEY_DURATION",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_DURATION) },
	{ "AMEDIAFORMAT_KEY_FLAC_COMPRESSION_LEVEL",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_FLAC_COMPRESSION_LEVEL) },
	{ "AMEDIAFORMAT_KEY_FRAME_RATE",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_FRAME_RATE) },
	{ "AMEDIAFORMAT_KEY_HEIGHT",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_HEIGHT) },
	{ "AMEDIAFORMAT_KEY_IS_ADTS",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_IS_ADTS) },
	{ "AMEDIAFORMAT_KEY_IS_AUTOSELECT",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_IS_AUTOSELECT) },
	{ "AMEDIAFORMAT_KEY_IS_DEFAULT",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_IS_DEFAULT) },
	{ "AMEDIAFORMAT_KEY_IS_FORCED_SUBTITLE",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_IS_FORCED_SUBTITLE) },
	{ "AMEDIAFORMAT_KEY_I_FRAME_INTERVAL",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_I_FRAME_INTERVAL) },
	{ "AMEDIAFORMAT_KEY_LANGUAGE",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_LANGUAGE) },
	{ "AMEDIAFORMAT_KEY_MAX_HEIGHT",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_MAX_HEIGHT) },
	{ "AMEDIAFORMAT_KEY_MAX_INPUT_SIZE",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_MAX_INPUT_SIZE) },
	{ "AMEDIAFORMAT_KEY_MAX_WIDTH",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_MAX_WIDTH) },
	{ "AMEDIAFORMAT_KEY_MIME",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_MIME) },
	{ "AMEDIAFORMAT_KEY_PUSH_BLANK_BUFFERS_ON_STOP",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_PUSH_BLANK_BUFFERS_ON_STOP) },
	{ "AMEDIAFORMAT_KEY_REPEAT_PREVIOUS_FRAME_AFTER",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_REPEAT_PREVIOUS_FRAME_AFTER) },
	{ "AMEDIAFORMAT_KEY_SAMPLE_RATE",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_SAMPLE_RATE) },
	{ "AMEDIAFORMAT_KEY_WIDTH",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_WIDTH) },
	{ "AMEDIAFORMAT_KEY_STRIDE",
		offsetof(struct mcw_ndk,
		AMediaFormat.KEY_STRIDE) },

	{ "AMediaCodec_createCodecByName",
		offsetof(struct mcw_ndk,
		AMediaCodec.createCodecByName) },
	{ "AMediaCodec_createDecoderByType",
		offsetof(struct mcw_ndk,
		AMediaCodec.createDecoderByType) },
	{ "AMediaCodec_createEncoderByType",
		offsetof(struct mcw_ndk,
		AMediaCodec.createEncoderByType) },
	{ "AMediaCodec_delete",
		offsetof(struct mcw_ndk,
		AMediaCodec.delete) },
	{ "AMediaCodec_configure",
		offsetof(struct mcw_ndk,
		AMediaCodec.configure) },
	{ "AMediaCodec_start",
		offsetof(struct mcw_ndk,
		AMediaCodec.start) },
	{ "AMediaCodec_stop",
		offsetof(struct mcw_ndk,
		AMediaCodec.stop) },
	{ "AMediaCodec_flush",
		offsetof(struct mcw_ndk,
		AMediaCodec.flush) },
	{ "AMediaCodec_getInputBuffer",
		offsetof(struct mcw_ndk,
		AMediaCodec.getInputBuffer) },
	{ "AMediaCodec_getOutputBuffer",
		offsetof(struct mcw_ndk,
		AMediaCodec.getOutputBuffer) },
	{ "AMediaCodec_dequeueInputBuffer",
		offsetof(struct mcw_ndk,
		AMediaCodec.dequeueInputBuffer) },
	{ "AMediaCodec_queueInputBuffer",
		offsetof(struct mcw_ndk,
		AMediaCodec.queueInputBuffer) },
	{ "AMediaCodec_dequeueOutputBuffer",
		offsetof(struct mcw_ndk,
		AMediaCodec.dequeueOutputBuffer) },
	{ "AMediaCodec_getOutputFormat",
		offsetof(struct mcw_ndk,
		AMediaCodec.getOutputFormat) },
	{ "AMediaCodec_releaseOutputBuffer",
		offsetof(struct mcw_ndk,
		AMediaCodec.releaseOutputBuffer) },
	{ "AMediaCodec_releaseOutputBufferAtTime",
		offsetof(struct mcw_ndk,
		AMediaCodec.releaseOutputBufferAtTime) },
};


static enum mcw_media_status mcw_ndk_map_status(
	enum media_status status)
{
	switch (status) {
	case AMEDIA_OK:
		return MCW_MEDIA_STATUS_OK;
	default:
	case AMEDIA_ERROR_UNKNOWN:
		return MCW_MEDIA_STATUS_ERROR_UNKNOWN;
	case AMEDIA_ERROR_MALFORMED:
		return MCW_MEDIA_STATUS_ERROR_MALFORMED;
	case AMEDIA_ERROR_UNSUPPORTED:
		return MCW_MEDIA_STATUS_ERROR_UNSUPPORTED;
	case AMEDIA_ERROR_INVALID_OBJECT:
		return MCW_MEDIA_STATUS_ERROR_INVALID_OBJECT;
	case AMEDIA_ERROR_INVALID_PARAMETER:
		return MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER;
	}
}


static struct mcw_mediaformat *mcw_ndk_mediaformat_new(
	void)
{
	return (struct mcw_mediaformat *)mcw_ndk.AMediaFormat.new();
}


static enum mcw_media_status mcw_ndk_mediaformat_delete(
	struct mcw_mediaformat *format)
{
	enum media_status status;
	status = mcw_ndk.AMediaFormat.delete((struct AMediaFormat *)format);
	return mcw_ndk_map_status(status);
}


static const char *mcw_ndk_mediaformat_to_string(
	struct mcw_mediaformat *format)
{
	return mcw_ndk.AMediaFormat.toString((struct AMediaFormat *)format);
}


static bool mcw_ndk_mediaformat_get_int32(
	struct mcw_mediaformat *format,
	const char *name,
	int32_t *out)
{
	return mcw_ndk.AMediaFormat.getInt32((struct AMediaFormat *)format,
		name, out);
}


static bool mcw_ndk_mediaformat_get_int64(
	struct mcw_mediaformat *format,
	const char *name,
	int64_t *out)
{
	return mcw_ndk.AMediaFormat.getInt64((struct AMediaFormat *)format,
		name, out);
}


static bool mcw_ndk_mediaformat_get_float(
	struct mcw_mediaformat *format,
	const char *name,
	float *out)
{
	return mcw_ndk.AMediaFormat.getFloat((struct AMediaFormat *)format,
		name, out);
}


static bool mcw_ndk_mediaformat_get_buffer(
	struct mcw_mediaformat *format,
	const char *name,
	void **data,
	size_t *size)
{
	return mcw_ndk.AMediaFormat.getBuffer((struct AMediaFormat *)format,
		name, data, size);
}


static bool mcw_ndk_mediaformat_get_string(
	struct mcw_mediaformat *format,
	const char *name,
	const char **out)
{
	return mcw_ndk.AMediaFormat.getString((struct AMediaFormat *)format,
		name, out);
}


static void mcw_ndk_mediaformat_set_int32(
	struct mcw_mediaformat *format,
	const char *name,
	int32_t value)
{
	mcw_ndk.AMediaFormat.setInt32((struct AMediaFormat *)format,
		name, value);
}


static void mcw_ndk_mediaformat_set_int64(
	struct mcw_mediaformat *format,
	const char *name,
	int64_t value)
{
	mcw_ndk.AMediaFormat.setInt64((struct AMediaFormat *)format,
		name, value);
}


static void mcw_ndk_mediaformat_set_float(
	struct mcw_mediaformat *format,
	const char *name,
	float value)
{
	mcw_ndk.AMediaFormat.setFloat((struct AMediaFormat *)format,
		name, value);
}


static void mcw_ndk_mediaformat_set_string(
	struct mcw_mediaformat *format,
	const char *name,
	const char *value)
{
	mcw_ndk.AMediaFormat.setString((struct AMediaFormat *)format,
		name, value);
}


static void mcw_ndk_mediaformat_set_buffer(
	struct mcw_mediaformat *format,
	const char *name,
	void *data,
	size_t size)
{
	mcw_ndk.AMediaFormat.setBuffer((struct AMediaFormat *)format,
		name, data, size);
}


static struct mcw_mediacodec *mcw_ndk_mediacodec_create_codec_by_name(
	const char *name)
{
	return (struct mcw_mediacodec *)mcw_ndk.AMediaCodec.createCodecByName(
		name);
}


static struct mcw_mediacodec *mcw_ndk_mediacodec_create_decoder_by_type(
	const char *mime_type)
{
	return (struct mcw_mediacodec *)mcw_ndk.AMediaCodec.createDecoderByType(
		mime_type);
}


static struct mcw_mediacodec *mcw_ndk_mediacodec_create_encoder_by_type(
	const char *mime_type)
{
	return (struct mcw_mediacodec *)mcw_ndk.AMediaCodec.createEncoderByType(
		mime_type);
}


static enum mcw_media_status mcw_ndk_mediacodec_delete(
	struct mcw_mediacodec *codec)
{
	enum media_status status;
	status = mcw_ndk.AMediaCodec.delete((struct AMediaCodec *)codec);
	return mcw_ndk_map_status(status);
}


static enum mcw_media_status mcw_ndk_mediacodec_configure(
	struct mcw_mediacodec *codec,
	const struct mcw_mediaformat *format,
	ANativeWindow *surface,
	struct mcw_mediacrypto *crypto,
	uint32_t flags)
{
	enum media_status status;
	status = mcw_ndk.AMediaCodec.configure((struct AMediaCodec *)codec,
		(struct AMediaFormat *)format, surface,
		(struct AMediaCrypto *)crypto, flags);
	return mcw_ndk_map_status(status);
}


static enum mcw_media_status mcw_ndk_mediacodec_start(
	struct mcw_mediacodec *codec)
{
	enum media_status status;
	status = mcw_ndk.AMediaCodec.start((struct AMediaCodec *)codec);
	return mcw_ndk_map_status(status);
}


static enum mcw_media_status mcw_ndk_mediacodec_stop(
	struct mcw_mediacodec *codec)
{
	enum media_status status;
	status = mcw_ndk.AMediaCodec.stop((struct AMediaCodec *)codec);
	return mcw_ndk_map_status(status);
}


static enum mcw_media_status mcw_ndk_mediacodec_flush(
	struct mcw_mediacodec *codec)
{
	enum media_status status;
	status = mcw_ndk.AMediaCodec.flush((struct AMediaCodec *)codec);
	return mcw_ndk_map_status(status);
}


static uint8_t *mcw_ndk_mediacodec_get_input_buffer(
	struct mcw_mediacodec *codec,
	size_t idx,
	size_t *out_size)
{
	return mcw_ndk.AMediaCodec.getInputBuffer(
		(struct AMediaCodec *)codec,
		idx, out_size);
}


static uint8_t *mcw_ndk_mediacodec_get_output_buffer(
	struct mcw_mediacodec *codec,
	size_t idx,
	size_t *out_size)
{
	return mcw_ndk.AMediaCodec.getOutputBuffer(
		(struct AMediaCodec *)codec,
		idx, out_size);
}


static ssize_t mcw_ndk_mediacodec_dequeue_input_buffer(
	struct mcw_mediacodec *codec,
	int64_t timeout_us)
{
	return mcw_ndk.AMediaCodec.dequeueInputBuffer(
		(struct AMediaCodec *)codec,
		timeout_us);
}


static enum mcw_media_status mcw_ndk_mediacodec_queue_input_buffer(
	struct mcw_mediacodec *codec,
	size_t idx,
	off_t offset,
	size_t size,
	uint64_t time,
	uint32_t flags)
{
	enum media_status status;
	status = mcw_ndk.AMediaCodec.queueInputBuffer(
		(struct AMediaCodec *)codec,
		idx, offset, size, time, flags);
	return mcw_ndk_map_status(status);
}


static ssize_t mcw_ndk_mediacodec_dequeue_output_buffer(
	struct mcw_mediacodec *codec,
	struct mcw_mediacodec_bufferinfo *info,
	int64_t timeout_us)
{
	struct AMediaCodecBufferInfo buf_info;
	ssize_t ret;

	ret = mcw_ndk.AMediaCodec.dequeueOutputBuffer(
		(struct AMediaCodec *)codec,
		&buf_info, timeout_us);

	info->offset = buf_info.offset;
	info->size = buf_info.size;
	info->presentation_time_us = buf_info.presentationTimeUs;
	info->flags = buf_info.flags;

	return ret;
}


static struct mcw_mediaformat *mcw_ndk_mediacodec_get_output_format(
	struct mcw_mediacodec *codec,
	size_t idx)
{
	return (struct mcw_mediaformat *)mcw_ndk.AMediaCodec.getOutputFormat(
		(struct AMediaCodec *)codec);
}


static enum mcw_media_status mcw_ndk_mediacodec_release_output_buffer(
	struct mcw_mediacodec *codec,
	size_t idx,
	bool render)
{
	enum media_status status;
	status = mcw_ndk.AMediaCodec.releaseOutputBuffer(
		(struct AMediaCodec *)codec, idx, render);
	return mcw_ndk_map_status(status);
}


static enum mcw_media_status mcw_ndk_mediacodec_release_output_buffer_at_time(
	struct mcw_mediacodec *codec,
	size_t idx,
	int64_t timestamp_ns)
{
	enum media_status status;
	status = mcw_ndk.AMediaCodec.releaseOutputBufferAtTime(
		(struct AMediaCodec *)codec, idx, timestamp_ns);
	return mcw_ndk_map_status(status);
}


int mcw_ndk_init(
	struct mcw *mcw)
{
	static int needs_init = 1;
	static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	int ret = 0;
	unsigned int i;
	void *handle = NULL;

	MCW_RETURN_ERR_IF_FAILED(mcw != NULL, -EINVAL);

	pthread_mutex_lock(&mutex);

	if (!needs_init)
		goto end;

	memset(&mcw_ndk, 0, sizeof(mcw_ndk));

	handle = dlopen(LIBMEDIANDK, RTLD_NOW);
	if (handle == NULL) {
		MCW_LOGI("dlopen() failed on '%s'", LIBMEDIANDK);
		ret = -ENOSYS;
		goto end2;
	}

	for (i = 0; i < MCW_SIZEOF_ARRAY(mcw_ndk_sym); i++) {
		void *sym = dlsym(handle, mcw_ndk_sym[i].name);
		if (sym == NULL) {
			MCW_LOGE("dlsym() failed on '%s'",
				mcw_ndk_sym[i].name);
			ret = -ENOSYS;
			goto end2;
		}
		*(void **)((uint8_t *)&mcw_ndk + mcw_ndk_sym[i].offset) = sym;
	}

	needs_init = 0;

end:
	mcw->mediaformat.nnew = mcw_ndk_mediaformat_new;
	mcw->mediaformat.ddelete = mcw_ndk_mediaformat_delete;
	mcw->mediaformat.to_string = mcw_ndk_mediaformat_to_string;
	mcw->mediaformat.get_int32 = mcw_ndk_mediaformat_get_int32;
	mcw->mediaformat.get_int64 = mcw_ndk_mediaformat_get_int64;
	mcw->mediaformat.get_float = mcw_ndk_mediaformat_get_float;
	mcw->mediaformat.get_buffer = mcw_ndk_mediaformat_get_buffer;
	mcw->mediaformat.get_string = mcw_ndk_mediaformat_get_string;
	mcw->mediaformat.set_int32 = mcw_ndk_mediaformat_set_int32;
	mcw->mediaformat.set_int64 = mcw_ndk_mediaformat_set_int64;
	mcw->mediaformat.set_float = mcw_ndk_mediaformat_set_float;
	mcw->mediaformat.set_string = mcw_ndk_mediaformat_set_string;
	mcw->mediaformat.set_buffer = mcw_ndk_mediaformat_set_buffer;
	mcw->mediaformat.KEY_AAC_PROFILE =
		*mcw_ndk.AMediaFormat.KEY_AAC_PROFILE;
	mcw->mediaformat.KEY_BIT_RATE =
		*mcw_ndk.AMediaFormat.KEY_BIT_RATE;
	mcw->mediaformat.KEY_CHANNEL_COUNT =
		*mcw_ndk.AMediaFormat.KEY_CHANNEL_COUNT;
	mcw->mediaformat.KEY_CHANNEL_MASK =
		*mcw_ndk.AMediaFormat.KEY_CHANNEL_MASK;
	mcw->mediaformat.KEY_COLOR_FORMAT =
		*mcw_ndk.AMediaFormat.KEY_COLOR_FORMAT;
	mcw->mediaformat.KEY_DURATION =
		*mcw_ndk.AMediaFormat.KEY_DURATION;
	mcw->mediaformat.KEY_FLAC_COMPRESSION_LEVEL =
		*mcw_ndk.AMediaFormat.KEY_FLAC_COMPRESSION_LEVEL;
	mcw->mediaformat.KEY_FRAME_RATE =
		*mcw_ndk.AMediaFormat.KEY_FRAME_RATE;
	mcw->mediaformat.KEY_HEIGHT =
		*mcw_ndk.AMediaFormat.KEY_HEIGHT;
	mcw->mediaformat.KEY_IS_ADTS =
		*mcw_ndk.AMediaFormat.KEY_IS_ADTS;
	mcw->mediaformat.KEY_IS_AUTOSELECT =
		*mcw_ndk.AMediaFormat.KEY_IS_AUTOSELECT;
	mcw->mediaformat.KEY_IS_DEFAULT =
		*mcw_ndk.AMediaFormat.KEY_IS_DEFAULT;
	mcw->mediaformat.KEY_IS_FORCED_SUBTITLE =
		*mcw_ndk.AMediaFormat.KEY_IS_FORCED_SUBTITLE;
	mcw->mediaformat.KEY_I_FRAME_INTERVAL =
		*mcw_ndk.AMediaFormat.KEY_I_FRAME_INTERVAL;
	mcw->mediaformat.KEY_LANGUAGE =
		*mcw_ndk.AMediaFormat.KEY_LANGUAGE;
	mcw->mediaformat.KEY_MAX_HEIGHT =
		*mcw_ndk.AMediaFormat.KEY_MAX_HEIGHT;
	mcw->mediaformat.KEY_MAX_INPUT_SIZE =
		*mcw_ndk.AMediaFormat.KEY_MAX_INPUT_SIZE;
	mcw->mediaformat.KEY_MAX_WIDTH =
		*mcw_ndk.AMediaFormat.KEY_MAX_WIDTH;
	mcw->mediaformat.KEY_MIME =
		*mcw_ndk.AMediaFormat.KEY_MIME;
	mcw->mediaformat.KEY_PUSH_BLANK_BUFFERS_ON_STOP =
		*mcw_ndk.AMediaFormat.KEY_PUSH_BLANK_BUFFERS_ON_STOP;
	mcw->mediaformat.KEY_REPEAT_PREVIOUS_FRAME_AFTER =
		*mcw_ndk.AMediaFormat.KEY_REPEAT_PREVIOUS_FRAME_AFTER;
	mcw->mediaformat.KEY_SAMPLE_RATE =
		*mcw_ndk.AMediaFormat.KEY_SAMPLE_RATE;
	mcw->mediaformat.KEY_WIDTH =
		*mcw_ndk.AMediaFormat.KEY_WIDTH;
	mcw->mediaformat.KEY_STRIDE =
		*mcw_ndk.AMediaFormat.KEY_STRIDE;
	mcw->mediacodec.create_codec_by_name =
		mcw_ndk_mediacodec_create_codec_by_name;
	mcw->mediacodec.create_decoder_by_type =
		mcw_ndk_mediacodec_create_decoder_by_type;
	mcw->mediacodec.create_encoder_by_type =
		mcw_ndk_mediacodec_create_encoder_by_type;
	mcw->mediacodec.ddelete = mcw_ndk_mediacodec_delete;
	mcw->mediacodec.configure = mcw_ndk_mediacodec_configure;
	mcw->mediacodec.start = mcw_ndk_mediacodec_start;
	mcw->mediacodec.stop = mcw_ndk_mediacodec_stop;
	mcw->mediacodec.flush = mcw_ndk_mediacodec_flush;
	mcw->mediacodec.get_input_buffer =
		mcw_ndk_mediacodec_get_input_buffer;
	mcw->mediacodec.get_output_buffer =
		mcw_ndk_mediacodec_get_output_buffer;
	mcw->mediacodec.dequeue_input_buffer =
		mcw_ndk_mediacodec_dequeue_input_buffer;
	mcw->mediacodec.queue_input_buffer =
		mcw_ndk_mediacodec_queue_input_buffer;
	mcw->mediacodec.dequeue_output_buffer =
		mcw_ndk_mediacodec_dequeue_output_buffer;
	mcw->mediacodec.get_output_format =
		mcw_ndk_mediacodec_get_output_format;
	mcw->mediacodec.release_output_buffer =
		mcw_ndk_mediacodec_release_output_buffer;
	mcw->mediacodec.release_output_buffer_at_time =
		mcw_ndk_mediacodec_release_output_buffer_at_time;

end2:
	if ((ret != 0) && (handle != NULL))
		dlclose(handle);

	pthread_mutex_unlock(&mutex);

	return ret;
}
