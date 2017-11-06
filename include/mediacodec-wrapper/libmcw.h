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

#ifndef _LIBMCW_H_
#define _LIBMCW_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <inttypes.h>
#include <unistd.h>
#include <stdbool.h>
#include <android/native_window.h>


enum mcw_media_status {
	MCW_MEDIA_STATUS_OK = 0,

	MCW_MEDIA_STATUS_ERROR_BASE =
		-10000,
	MCW_MEDIA_STATUS_ERROR_UNKNOWN =
		MCW_MEDIA_STATUS_ERROR_BASE,
	MCW_MEDIA_STATUS_ERROR_MALFORMED =
		MCW_MEDIA_STATUS_ERROR_BASE - 1,
	MCW_MEDIA_STATUS_ERROR_UNSUPPORTED =
		MCW_MEDIA_STATUS_ERROR_BASE - 2,
	MCW_MEDIA_STATUS_ERROR_INVALID_OBJECT =
		MCW_MEDIA_STATUS_ERROR_BASE - 3,
	MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER =
		MCW_MEDIA_STATUS_ERROR_BASE - 4,
};


#define MCW_BUFFER_FLAG_END_OF_STREAM 	4
#define MCW_CONFIGURE_FLAG_ENCODE	1
#define MCW_INFO_OUTPUT_BUFFERS_CHANGED	-3
#define MCW_INFO_OUTPUT_FORMAT_CHANGED	-2
#define MCW_INFO_TRY_AGAIN_LATER	-1


struct mcw_mediaformat;


typedef struct mcw_mediaformat *(*mcw_mediaformat_new_t)(
	void);


typedef enum mcw_media_status (*mcw_mediaformat_delete_t)(
	struct mcw_mediaformat *format);


typedef const char *(*mcw_mediaformat_to_string_t)(
	struct mcw_mediaformat *format);


typedef bool (*mcw_mediaformat_get_int32_t)(
	struct mcw_mediaformat *format,
	const char *name,
	int32_t *out);


typedef bool (*mcw_mediaformat_get_int64_t)(
	struct mcw_mediaformat *format,
	const char *name,
	int64_t *out);


typedef bool (*mcw_mediaformat_get_float_t)(
	struct mcw_mediaformat *format,
	const char *name,
	float *out);


typedef bool (*mcw_mediaformat_get_buffer_t)(
	struct mcw_mediaformat *format,
	const char *name,
	void **data,
	size_t *size);


typedef bool (*mcw_mediaformat_get_string_t)(
	struct mcw_mediaformat *format,
	const char *name,
	const char **out);


typedef void (*mcw_mediaformat_set_int32_t)(
	struct mcw_mediaformat *format,
	const char *name,
	int32_t value);


typedef void (*mcw_mediaformat_set_int64_t)(
	struct mcw_mediaformat *format,
	const char *name,
	int64_t value);


typedef void (*mcw_mediaformat_set_float_t)(
	struct mcw_mediaformat *format,
	const char *name,
	float value);


typedef void (*mcw_mediaformat_set_string_t)(
	struct mcw_mediaformat *format,
	const char *name,
	const char *value);


typedef void (*mcw_mediaformat_set_buffer_t)(
	struct mcw_mediaformat *format,
	const char *name,
	void *data,
	size_t size);


struct mcw_mediacodec;


struct mcw_mediacrypto;


struct mcw_mediacodec_bufferinfo {
	int32_t offset;
	int32_t size;
	int64_t presentation_time_us;
	uint32_t flags;
};


typedef struct mcw_mediacodec *(*mcw_mediacodec_create_codec_by_name_t)(
	const char *name);


typedef struct mcw_mediacodec *(*mcw_mediacodec_create_decoder_by_type_t)(
	const char *mime_type);


typedef struct mcw_mediacodec *(*mcw_mediacodec_create_encoder_by_type_t)(
	const char *mime_type);


typedef enum mcw_media_status (*mcw_mediacodec_delete_t)(
	struct mcw_mediacodec *codec);


typedef enum mcw_media_status (*mcw_mediacodec_configure_t)(
	struct mcw_mediacodec *codec,
	const struct mcw_mediaformat *format,
	ANativeWindow *surface,
	struct mcw_mediacrypto *crypto,
	uint32_t flags);


typedef enum mcw_media_status (*mcw_mediacodec_start_t)(
	struct mcw_mediacodec *codec);


typedef enum mcw_media_status (*mcw_mediacodec_stop_t)(
	struct mcw_mediacodec *codec);


typedef enum mcw_media_status (*mcw_mediacodec_flush_t)(
	struct mcw_mediacodec *codec);


typedef uint8_t *(*mcw_mediacodec_get_input_buffer_t)(
	struct mcw_mediacodec *codec,
	size_t idx,
	size_t *out_size);


typedef uint8_t *(*mcw_mediacodec_get_output_buffer_t)(
	struct mcw_mediacodec *codec,
	size_t idx,
	size_t *out_size);


typedef ssize_t (*mcw_mediacodec_dequeue_input_buffer_t)(
	struct mcw_mediacodec *codec,
	int64_t timeout_us);


typedef enum mcw_media_status (*mcw_mediacodec_queue_input_buffer_t)(
	struct mcw_mediacodec *codec,
	size_t idx,
	off_t offset,
	size_t size,
	uint64_t time,
	uint32_t flags);


typedef ssize_t (*mcw_mediacodec_dequeue_output_buffer_t)(
	struct mcw_mediacodec *codec,
	struct mcw_mediacodec_bufferinfo *info,
	int64_t timeout_us);


typedef struct mcw_mediaformat *(*mcw_mediacodec_get_output_format_t)(
	struct mcw_mediacodec *codec,
	size_t idx);


typedef enum mcw_media_status (*mcw_mediacodec_release_output_buffer_t)(
	struct mcw_mediacodec *codec,
	size_t idx,
	bool render);


typedef enum mcw_media_status (*mcw_mediacodec_release_output_buffer_at_time_t)(
	struct mcw_mediacodec *codec,
	size_t idx,
	int64_t timestamp_ns);


struct mcw {
	struct {
		mcw_mediaformat_new_t nnew;
		mcw_mediaformat_delete_t ddelete;
		mcw_mediaformat_to_string_t to_string;
		mcw_mediaformat_get_int32_t get_int32;
		mcw_mediaformat_get_int64_t get_int64;
		mcw_mediaformat_get_float_t get_float;
		mcw_mediaformat_get_buffer_t get_buffer;
		mcw_mediaformat_get_string_t get_string;
		mcw_mediaformat_set_int32_t set_int32;
		mcw_mediaformat_set_int64_t set_int64;
		mcw_mediaformat_set_float_t set_float;
		mcw_mediaformat_set_string_t set_string;
		mcw_mediaformat_set_buffer_t set_buffer;
		const char *KEY_AAC_PROFILE;
		const char *KEY_BIT_RATE;
		const char *KEY_CHANNEL_COUNT;
		const char *KEY_CHANNEL_MASK;
		const char *KEY_COLOR_FORMAT;
		const char *KEY_DURATION;
		const char *KEY_FLAC_COMPRESSION_LEVEL;
		const char *KEY_FRAME_RATE;
		const char *KEY_HEIGHT;
		const char *KEY_IS_ADTS;
		const char *KEY_IS_AUTOSELECT;
		const char *KEY_IS_DEFAULT;
		const char *KEY_IS_FORCED_SUBTITLE;
		const char *KEY_I_FRAME_INTERVAL;
		const char *KEY_LANGUAGE;
		const char *KEY_MAX_HEIGHT;
		const char *KEY_MAX_INPUT_SIZE;
		const char *KEY_MAX_WIDTH;
		const char *KEY_MIME;
		const char *KEY_PUSH_BLANK_BUFFERS_ON_STOP;
		const char *KEY_REPEAT_PREVIOUS_FRAME_AFTER;
		const char *KEY_SAMPLE_RATE;
		const char *KEY_WIDTH;
		const char *KEY_STRIDE;
	} mediaformat;
	struct {
		mcw_mediacodec_create_codec_by_name_t create_codec_by_name;
		mcw_mediacodec_create_decoder_by_type_t create_decoder_by_type;
		mcw_mediacodec_create_encoder_by_type_t create_encoder_by_type;
		mcw_mediacodec_delete_t ddelete;
		mcw_mediacodec_configure_t configure;
		mcw_mediacodec_start_t start;
		mcw_mediacodec_stop_t stop;
		mcw_mediacodec_flush_t flush;
		mcw_mediacodec_get_input_buffer_t get_input_buffer;
		mcw_mediacodec_get_output_buffer_t get_output_buffer;
		mcw_mediacodec_dequeue_input_buffer_t dequeue_input_buffer;
		mcw_mediacodec_queue_input_buffer_t queue_input_buffer;
		mcw_mediacodec_dequeue_output_buffer_t dequeue_output_buffer;
		mcw_mediacodec_get_output_format_t get_output_format;
		mcw_mediacodec_release_output_buffer_t release_output_buffer;
		mcw_mediacodec_release_output_buffer_at_time_t
			release_output_buffer_at_time;
	} mediacodec;
};


struct mcw *mcw_new(
	void);


int mcw_destroy(
	struct mcw *self);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !_LIBMCW_H_ */
