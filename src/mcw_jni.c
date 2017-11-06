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

#include "mcw_jni.h"

#include <jni.h>


static const char *mcw_jni_mediaformat_KEY_AAC_PROFILE =
	"aac-profile";				/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_BIT_RATE =
	"bitrate";				/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_CHANNEL_COUNT =
	"channel-count";			/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_CHANNEL_MASK =
	"channel-mask";				/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_COLOR_FORMAT =
	"color-format";				/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_DURATION =
	"durationUs";				/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_FLAC_COMPRESSION_LEVEL =
	"flac-compression-level";		/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_FRAME_RATE =
	"frame-rate";				/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_HEIGHT =
	"height";				/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_IS_ADTS =
	"is-adts";				/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_IS_AUTOSELECT =
	"is-autoselect";			/* API level 19 */
static const char *mcw_jni_mediaformat_KEY_IS_DEFAULT =
	"is-default";				/* API level 19 */
static const char *mcw_jni_mediaformat_KEY_IS_FORCED_SUBTITLE =
	"is-forced-subtitle";			/* API level 19 */
static const char *mcw_jni_mediaformat_KEY_I_FRAME_INTERVAL =
	"i-frame-interval";			/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_LANGUAGE =
	"language";				/* API level 19 */
static const char *mcw_jni_mediaformat_KEY_MAX_HEIGHT =
	"max-height";				/* API level 19 */
static const char *mcw_jni_mediaformat_KEY_MAX_INPUT_SIZE =
	"max-input-size";			/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_MAX_WIDTH =
	"max-width";				/* API level 19 */
static const char *mcw_jni_mediaformat_KEY_MIME =
	"mime";					/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_PUSH_BLANK_BUFFERS_ON_STOP =
	"push-blank-buffers-on-shutdown";	/* API level 19 */
static const char *mcw_jni_mediaformat_KEY_REPEAT_PREVIOUS_FRAME_AFTER =
	"repeat-previous-frame-after";		/* API level 19 */
static const char *mcw_jni_mediaformat_KEY_SAMPLE_RATE =
	"sample-rate";				/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_STRIDE =
	"stride";				/* API level 23 */
static const char *mcw_jni_mediaformat_KEY_WIDTH =
	"width";				/* API level 16 */


static struct mcw_jni {
	JavaVM *jvm;
	struct {
		jclass clazz;
		jmethodID new;				/* API level 16 */
		jfieldID flags;				/* API level 16 */
		jfieldID offset;			/* API level 16 */
		jfieldID presentationTimeUs;		/* API level 16 */
		jfieldID size;				/* API level 16 */
	} BufferInfo;
	struct {
		jclass clazz;
		jmethodID new;				/* API level 16 */
		jmethodID toString;			/* API level 16 */
		jmethodID getInteger;			/* API level 16 */
		jmethodID getLong;			/* API level 16 */
		jmethodID getFloat;			/* API level 16 */
		jmethodID getByteBuffer;		/* API level 16 */
		jmethodID getString;			/* API level 16 */
		jmethodID setInteger;			/* API level 16 */
		jmethodID setLong;			/* API level 16 */
		jmethodID setFloat;			/* API level 16 */
		jmethodID setByteBuffer;		/* API level 16 */
		jmethodID setString;			/* API level 16 */
	} MediaFormat;
	struct {
		jclass clazz;
		jmethodID createByCodecName;		/* API level 16 */
		jmethodID createDecoderByType;		/* API level 16 */
		jmethodID createEncoderByType;		/* API level 16 */
		jmethodID release;			/* API level 16 */
		jmethodID configure;			/* API level 16 */
		jmethodID start;			/* API level 16 */
		jmethodID stop;				/* API level 16 */
		jmethodID flush;			/* API level 16 */
		jmethodID getInputBuffer;		/* API level 21 */
		jmethodID getInputBuffers;		/* API level 16 */
		jmethodID getOutputBuffer;		/* API level 21 */
		jmethodID getOutputBuffers;		/* API level 16 */
		jmethodID dequeueInputBuffer;		/* API level 16 */
		jmethodID queueInputBuffer;		/* API level 16 */
		jmethodID dequeueOutputBuffer;		/* API level 16 */
		jmethodID getOutputFormat;		/* API level 16 */
		jmethodID releaseOutputBuffer;		/* API level 16 */
		jmethodID releaseOutputBufferAtTime;	/* API level 21 */
	} MediaCodec;
} mcw_jni;


static const struct {
	const char *name;
	int offset;
} mcw_jni_class[] = {
	{ "android/media/MediaCodec$BufferInfo",
		offsetof(struct mcw_jni, BufferInfo.clazz) },
	{ "android/media/MediaFormat",
		offsetof(struct mcw_jni, MediaFormat.clazz) },
	{ "android/media/MediaCodec",
		offsetof(struct mcw_jni, MediaCodec.clazz) },
};


enum mcw_jni_member_type {
	MCW_JNI_MEMBER_TYPE_METHOD = 0,
	MCW_JNI_MEMBER_TYPE_STATIC_METHOD,
	MCW_JNI_MEMBER_TYPE_FIELD,
};

static const struct {
	enum mcw_jni_member_type type;
	const char *name;
	const char *signature;
	int class_offset;
	int offset;
	int critical;
} mcw_jni_member[] = {
	{ MCW_JNI_MEMBER_TYPE_METHOD, "<init>",
		"()V",
		offsetof(struct mcw_jni, BufferInfo.clazz),
		offsetof(struct mcw_jni, BufferInfo.new), 1 },
	{ MCW_JNI_MEMBER_TYPE_FIELD, "flags",
		"I",
		offsetof(struct mcw_jni, BufferInfo.clazz),
		offsetof(struct mcw_jni, BufferInfo.flags), 1 },
	{ MCW_JNI_MEMBER_TYPE_FIELD, "offset",
		"I",
		offsetof(struct mcw_jni, BufferInfo.clazz),
		offsetof(struct mcw_jni, BufferInfo.offset), 1 },
	{ MCW_JNI_MEMBER_TYPE_FIELD, "presentationTimeUs",
		"J",
		offsetof(struct mcw_jni, BufferInfo.clazz),
		offsetof(struct mcw_jni, BufferInfo.presentationTimeUs), 1 },
	{ MCW_JNI_MEMBER_TYPE_FIELD, "size",
		"I",
		offsetof(struct mcw_jni, BufferInfo.clazz),
		offsetof(struct mcw_jni, BufferInfo.size), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "<init>",
		"()V",
		offsetof(struct mcw_jni, MediaFormat.clazz),
		offsetof(struct mcw_jni, MediaFormat.new), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "toString",
		"()Ljava/lang/String;",
		offsetof(struct mcw_jni, MediaFormat.clazz),
		offsetof(struct mcw_jni, MediaFormat.toString), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "getInteger",
		"(Ljava/lang/String;)I",
		offsetof(struct mcw_jni, MediaFormat.clazz),
		offsetof(struct mcw_jni, MediaFormat.getInteger), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "getLong",
		"(Ljava/lang/String;)J",
		offsetof(struct mcw_jni, MediaFormat.clazz),
		offsetof(struct mcw_jni, MediaFormat.getLong), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "getFloat",
		"(Ljava/lang/String;)F",
		offsetof(struct mcw_jni, MediaFormat.clazz),
		offsetof(struct mcw_jni, MediaFormat.getFloat), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "getByteBuffer",
		"(Ljava/lang/String;)Ljava/nio/ByteBuffer;",
		offsetof(struct mcw_jni, MediaFormat.clazz),
		offsetof(struct mcw_jni, MediaFormat.getByteBuffer), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "getString",
		"(Ljava/lang/String;)Ljava/lang/String;",
		offsetof(struct mcw_jni, MediaFormat.clazz),
		offsetof(struct mcw_jni, MediaFormat.getString), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "setInteger",
		"(Ljava/lang/String;I)V",
		offsetof(struct mcw_jni, MediaFormat.clazz),
		offsetof(struct mcw_jni, MediaFormat.setInteger), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "setLong",
		"(Ljava/lang/String;J)V",
		offsetof(struct mcw_jni, MediaFormat.clazz),
		offsetof(struct mcw_jni, MediaFormat.setLong), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "setFloat",
		"(Ljava/lang/String;F)V",
		offsetof(struct mcw_jni, MediaFormat.clazz),
		offsetof(struct mcw_jni, MediaFormat.setFloat), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "setByteBuffer",
		"(Ljava/lang/String;Ljava/nio/ByteBuffer;)V",
		offsetof(struct mcw_jni, MediaFormat.clazz),
		offsetof(struct mcw_jni, MediaFormat.setByteBuffer), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "setString",
		"(Ljava/lang/String;Ljava/lang/String;)V",
		offsetof(struct mcw_jni, MediaFormat.clazz),
		offsetof(struct mcw_jni, MediaFormat.setString), 1 },
	{ MCW_JNI_MEMBER_TYPE_STATIC_METHOD, "createByCodecName",
		"(Ljava/lang/String;)Landroid/media/MediaCodec;",
		offsetof(struct mcw_jni, MediaCodec.clazz),
		offsetof(struct mcw_jni, MediaCodec.createByCodecName), 1 },
	{ MCW_JNI_MEMBER_TYPE_STATIC_METHOD, "createDecoderByType",
		"(Ljava/lang/String;)Landroid/media/MediaCodec;",
		offsetof(struct mcw_jni, MediaCodec.clazz),
		offsetof(struct mcw_jni, MediaCodec.createDecoderByType), 1 },
	{ MCW_JNI_MEMBER_TYPE_STATIC_METHOD, "createEncoderByType",
		"(Ljava/lang/String;)Landroid/media/MediaCodec;",
		offsetof(struct mcw_jni, MediaCodec.clazz),
		offsetof(struct mcw_jni, MediaCodec.createEncoderByType), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "release",
		"()V",
		offsetof(struct mcw_jni, MediaCodec.clazz),
		offsetof(struct mcw_jni, MediaCodec.release), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "configure",
		"(Landroid/media/MediaFormat;Landroid/view/Surface;"
			"Landroid/media/MediaCrypto;I)V",
		offsetof(struct mcw_jni, MediaCodec.clazz),
		offsetof(struct mcw_jni, MediaCodec.configure), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "start",
		"()V",
		offsetof(struct mcw_jni, MediaCodec.clazz),
		offsetof(struct mcw_jni, MediaCodec.start), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "stop",
		"()V",
		offsetof(struct mcw_jni, MediaCodec.clazz),
		offsetof(struct mcw_jni, MediaCodec.stop), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "flush",
		"()V",
		offsetof(struct mcw_jni, MediaCodec.clazz),
		offsetof(struct mcw_jni, MediaCodec.flush), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "getInputBuffer",
		"(I)Ljava/nio/ByteBuffer;",
		offsetof(struct mcw_jni, MediaCodec.clazz),
		offsetof(struct mcw_jni, MediaCodec.getInputBuffer), 0 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "getInputBuffers",
		"()[Ljava/nio/ByteBuffer;",
		offsetof(struct mcw_jni, MediaCodec.clazz),
		offsetof(struct mcw_jni, MediaCodec.getInputBuffers), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "getOutputBuffer",
		"(I)Ljava/nio/ByteBuffer;",
		offsetof(struct mcw_jni, MediaCodec.clazz),
		offsetof(struct mcw_jni, MediaCodec.getOutputBuffer), 0 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "getOutputBuffers",
		"()[Ljava/nio/ByteBuffer;",
		offsetof(struct mcw_jni, MediaCodec.clazz),
		offsetof(struct mcw_jni, MediaCodec.getOutputBuffers), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "dequeueInputBuffer",
		"(J)I",
		offsetof(struct mcw_jni, MediaCodec.clazz),
		offsetof(struct mcw_jni, MediaCodec.dequeueInputBuffer), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "queueInputBuffer",
		"(IIIJI)V",
		offsetof(struct mcw_jni, MediaCodec.clazz),
		offsetof(struct mcw_jni, MediaCodec.queueInputBuffer), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "dequeueOutputBuffer",
		"(Landroid/media/MediaCodec$BufferInfo;J)I",
		offsetof(struct mcw_jni, MediaCodec.clazz),
		offsetof(struct mcw_jni, MediaCodec.dequeueOutputBuffer), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "getOutputFormat",
		"()Landroid/media/MediaFormat;",
		offsetof(struct mcw_jni, MediaCodec.clazz),
		offsetof(struct mcw_jni, MediaCodec.getOutputFormat), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "releaseOutputBuffer",
		"(IZ)V",
		offsetof(struct mcw_jni, MediaCodec.clazz),
		offsetof(struct mcw_jni, MediaCodec.releaseOutputBuffer), 1 },
	{ MCW_JNI_MEMBER_TYPE_METHOD, "releaseOutputBuffer",
		"(IJ)V",
		offsetof(struct mcw_jni, MediaCodec.clazz),
		offsetof(struct mcw_jni,
			MediaCodec.releaseOutputBufferAtTime), 0 },
};


struct mcw_jni_mediaformat {
	jobject mediaformat;
};


struct mcw_jni_mediacodec {
	jobject mediacodec;
	jobject bufferinfo;
	jobject in_buffers;
	jobject out_buffers;
};


static inline bool mcw_jni_attach_and_get_env(
	JNIEnv **env)
{
	bool attached = false;
	JNIEnv *_env = NULL;
	jint status = (*mcw_jni.jvm)->GetEnv(mcw_jni.jvm,
		(void **)&_env, JNI_VERSION_1_6);
	if (status == JNI_EDETACHED) {
		jint ret = (*mcw_jni.jvm)->AttachCurrentThread(mcw_jni.jvm,
			&_env, NULL);
		if (ret != JNI_OK) {
			MCW_LOGE("failed to attach thread to VM (%d)", ret);
			return false;
		}
		attached = true;
	} else if (status != JNI_OK) {
		MCW_LOGE("failed to get JNI environment (%d)", status);
		return false;
	}

	if (env)
		*env = _env;
	return attached;
}


static inline void mcw_jni_detach(
	bool attached)
{
	if (attached) {
		jint ret = (*mcw_jni.jvm)->DetachCurrentThread(mcw_jni.jvm);
		if (ret != JNI_OK)
			MCW_LOGE("failed to detach thread from VM (%d)", ret);
	}
}


static inline jstring mcw_jni_string_new(
	JNIEnv *env,
	const char *str)
{
	jstring jstr = (*env)->NewStringUTF(env, str);
	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionClear(env);
		return NULL;
	} else
		return jstr;
}


static void mcw_jni_mediacodec_get_input_buffers(
	JNIEnv *env,
	struct mcw_jni_mediacodec *mc)
{
	jobject j_buffers = NULL;

	if (mcw_jni.MediaCodec.getInputBuffers) {
		j_buffers = (*env)->CallObjectMethod(env, mc->mediacodec,
			mcw_jni.MediaCodec.getInputBuffers);

		if ((*env)->ExceptionCheck(env)) {
			(*env)->ExceptionDescribe(env);
			(*env)->ExceptionClear(env);
			goto end;
		}

		mc->in_buffers = (*env)->NewGlobalRef(env, j_buffers);
	}

end:
	if (j_buffers)
		(*env)->DeleteLocalRef(env, j_buffers);
}


static void mcw_jni_mediacodec_get_output_buffers(
	JNIEnv *env,
	struct mcw_jni_mediacodec *mc)
{
	jobject j_buffers = NULL;

	if (mcw_jni.MediaCodec.getOutputBuffers) {
		j_buffers = (*env)->CallObjectMethod(env, mc->mediacodec,
			mcw_jni.MediaCodec.getOutputBuffers);

		if ((*env)->ExceptionCheck(env)) {
			(*env)->ExceptionDescribe(env);
			(*env)->ExceptionClear(env);
			goto end;
		}

		mc->out_buffers = (*env)->NewGlobalRef(env, j_buffers);
	}

end:
	if (j_buffers)
		(*env)->DeleteLocalRef(env, j_buffers);
}


static struct mcw_mediaformat *mcw_jni_mediaformat_new(
	void)
{
	int error = 0;
	struct mcw_jni_mediaformat *fmt = NULL;
	jobject j_fmt = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	fmt = calloc(1, sizeof(*fmt));
	MCW_RETURN_VAL_IF_FAILED(fmt != NULL, -ENOMEM, NULL);

	attached = mcw_jni_attach_and_get_env(&env);
	if (!env) {
		error = 1;
		goto end;
	}

	j_fmt = (*env)->NewObject(env,
		mcw_jni.MediaFormat.clazz,
		mcw_jni.MediaFormat.new);
	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		error = 1;
		goto end;
	}
	if (!j_fmt) {
		MCW_LOGE("format creation failed");
		error = 1;
		goto end;
	}
	fmt->mediaformat = (*env)->NewGlobalRef(env, j_fmt);

end:
	if (j_fmt)
		(*env)->DeleteLocalRef(env, j_fmt);

	mcw_jni_detach(attached);

	if (error) {
		free(fmt);
		return NULL;
	} else
		return (struct mcw_mediaformat *)fmt;
}


static enum mcw_media_status mcw_jni_mediaformat_delete(
	struct mcw_mediaformat *format)
{
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		MCW_LOGE("invalid format parameter");
		return MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_VAL_IF_FAILED(env != NULL, -ENODEV,
		MCW_MEDIA_STATUS_ERROR_UNKNOWN);

	(*env)->DeleteGlobalRef(env, fmt->mediaformat);

	free(fmt);

	mcw_jni_detach(attached);

	return MCW_MEDIA_STATUS_OK;
}


static const char *mcw_jni_mediaformat_to_string(
	struct mcw_mediaformat *format)
{
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jobject j_str = NULL;
	const char *str = NULL, *ret_str = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		MCW_LOGE("invalid format parameter");
		return NULL;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_VAL_IF_FAILED(env != NULL, -ENODEV, NULL);

	j_str = (*env)->CallObjectMethod(env, fmt->mediaformat,
		mcw_jni.MediaFormat.toString);
	if (!j_str)
		goto end;

	str = (*env)->GetStringUTFChars(env, j_str, NULL);
	if (!str)
		goto end;

	ret_str = strdup(str);

end:
	if (str)
		(*env)->ReleaseStringUTFChars(env, j_str, str);

	if (j_str)
		(*env)->DeleteLocalRef(env, j_str);

	mcw_jni_detach(attached);

	return ret_str;
}


static bool mcw_jni_mediaformat_get_int32(
	struct mcw_mediaformat *format,
	const char *name,
	int32_t *out)
{
	int error = 0;
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_name = NULL;
	jint ret = 0;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		MCW_LOGE("invalid format parameter");
		return false;
	}
	if (!name) {
		MCW_LOGE("invalid name parameter");
		return false;
	}
	if (!out) {
		MCW_LOGE("invalid output parameter");
		return false;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_VAL_IF_FAILED(env != NULL, -ENODEV, false);

	j_name = mcw_jni_string_new(env, name);
	if (!j_name) {
		MCW_LOGE("string creation failed");
		error = 1;
		goto end;
	}

	ret = (*env)->CallIntMethod(env, fmt->mediaformat,
		mcw_jni.MediaFormat.getInteger, j_name);

end:
	if (j_name)
		(*env)->DeleteLocalRef(env, j_name);

	if (!error)
		*out = (int32_t)ret;

	mcw_jni_detach(attached);

	return (error) ? false : true;
}


static bool mcw_jni_mediaformat_get_int64(
	struct mcw_mediaformat *format,
	const char *name,
	int64_t *out)
{
	int error = 0;
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_name = NULL;
	jlong ret = 0;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		MCW_LOGE("invalid format parameter");
		return false;
	}
	if (!name) {
		MCW_LOGE("invalid name parameter");
		return false;
	}
	if (!out) {
		MCW_LOGE("invalid output parameter");
		return false;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_VAL_IF_FAILED(env != NULL, -ENODEV, false);

	j_name = mcw_jni_string_new(env, name);
	if (!j_name) {
		MCW_LOGE("string creation failed");
		error = 1;
		goto end;
	}

	ret = (*env)->CallLongMethod(env, fmt->mediaformat,
		mcw_jni.MediaFormat.getLong, j_name);

end:
	if (j_name)
		(*env)->DeleteLocalRef(env, j_name);

	if (!error)
		*out = (int64_t)ret;

	mcw_jni_detach(attached);

	return (error) ? false : true;
}


static bool mcw_jni_mediaformat_get_float(
	struct mcw_mediaformat *format,
	const char *name,
	float *out)
{
	int error = 0;
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_name = NULL;
	jfloat ret = 0;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		MCW_LOGE("invalid format parameter");
		return false;
	}
	if (!name) {
		MCW_LOGE("invalid name parameter");
		return false;
	}
	if (!out) {
		MCW_LOGE("invalid output parameter");
		return false;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_VAL_IF_FAILED(env != NULL, -ENODEV, false);

	j_name = mcw_jni_string_new(env, name);
	if (!j_name) {
		MCW_LOGE("string creation failed");
		error = 1;
		goto end;
	}

	ret = (*env)->CallFloatMethod(env, fmt->mediaformat,
		mcw_jni.MediaFormat.getFloat, j_name);

end:
	if (j_name)
		(*env)->DeleteLocalRef(env, j_name);

	if (!error)
		*out = (float)ret;

	mcw_jni_detach(attached);

	return (error) ? false : true;
}


static bool mcw_jni_mediaformat_get_buffer(
	struct mcw_mediaformat *format,
	const char *name,
	void **data,
	size_t *size)
{
	int error = 0;
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_name = NULL;
	jobject j_buf = NULL;
	void *ptr = NULL;
	size_t sz = 0;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		MCW_LOGE("invalid format parameter");
		return false;
	}
	if (!name) {
		MCW_LOGE("invalid name parameter");
		return false;
	}
	if (!data) {
		MCW_LOGE("invalid output data parameter");
		return false;
	}
	if (!size) {
		MCW_LOGE("invalid output size parameter");
		return false;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_VAL_IF_FAILED(env != NULL, -ENODEV, false);

	j_name = mcw_jni_string_new(env, name);
	if (!j_name) {
		MCW_LOGE("string creation failed");
		error = 1;
		goto end;
	}

	j_buf = (*env)->CallObjectMethod(env, fmt->mediaformat,
		mcw_jni.MediaFormat.getByteBuffer, j_name);

	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		error = 1;
		goto end;
	}
	if (!j_buf) {
		error = 1;
		goto end;
	}

	ptr = (*env)->GetDirectBufferAddress(env, j_buf);
	sz = (*env)->GetDirectBufferCapacity(env, j_buf);

end:
	if (j_name)
		(*env)->DeleteLocalRef(env, j_name);
	if (j_buf)
		(*env)->DeleteLocalRef(env, j_buf);

	if (!error) {
		*data = ptr;
		*size = sz;
	}

	mcw_jni_detach(attached);

	return (error) ? false : true;
}


static bool mcw_jni_mediaformat_get_string(
	struct mcw_mediaformat *format,
	const char *name,
	const char **out)
{
	int error = 0;
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_name = NULL;
	jobject j_str = 0;
	const char *str = NULL, *ret_str = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		MCW_LOGE("invalid format parameter");
		return false;
	}
	if (!name) {
		MCW_LOGE("invalid name parameter");
		return false;
	}
	if (!out) {
		MCW_LOGE("invalid output parameter");
		return false;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_VAL_IF_FAILED(env != NULL, -ENODEV, false);

	j_name = mcw_jni_string_new(env, name);
	if (!j_name) {
		MCW_LOGE("string creation failed");
		error = 1;
		goto end;
	}

	j_str = (*env)->CallObjectMethod(env, fmt->mediaformat,
		mcw_jni.MediaFormat.getString, j_name);
	if (!j_str) {
		error = 1;
		goto end;
	}

	str = (*env)->GetStringUTFChars(env, j_str, NULL);
	if (!str) {
		error = 1;
		goto end;
	}

	ret_str = strdup(str);

end:
	if (str)
		(*env)->ReleaseStringUTFChars(env, j_str, str);
	if (j_str)
		(*env)->DeleteLocalRef(env, j_str);
	if (j_name)
		(*env)->DeleteLocalRef(env, j_name);

	if (!error)
		*out = ret_str;
	else
		free((char *)ret_str);

	mcw_jni_detach(attached);

	return (error) ? false : true;
}


static void mcw_jni_mediaformat_set_int32(
	struct mcw_mediaformat *format,
	const char *name,
	int32_t value)
{
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_name = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		MCW_LOGE("invalid format parameter");
		return;
	}
	if (!name) {
		MCW_LOGE("invalid name parameter");
		return;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_IF_FAILED(env != NULL, -ENODEV);

	j_name = mcw_jni_string_new(env, name);
	if (!j_name) {
		MCW_LOGE("string creation failed");
		goto end;
	}

	(*env)->CallVoidMethod(env, fmt->mediaformat,
		mcw_jni.MediaFormat.setInteger, j_name, (jint)value);

end:
	if (j_name)
		(*env)->DeleteLocalRef(env, j_name);

	mcw_jni_detach(attached);
}


static void mcw_jni_mediaformat_set_int64(
	struct mcw_mediaformat *format,
	const char *name,
	int64_t value)
{
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_name = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		MCW_LOGE("invalid format parameter");
		return;
	}
	if (!name) {
		MCW_LOGE("invalid name parameter");
		return;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_IF_FAILED(env != NULL, -ENODEV);

	j_name = mcw_jni_string_new(env, name);
	if (!j_name) {
		MCW_LOGE("string creation failed");
		goto end;
	}

	(*env)->CallVoidMethod(env, fmt->mediaformat,
		mcw_jni.MediaFormat.setLong, j_name, (jlong)value);

end:
	if (j_name)
		(*env)->DeleteLocalRef(env, j_name);

	mcw_jni_detach(attached);
}


static void mcw_jni_mediaformat_set_float(
	struct mcw_mediaformat *format,
	const char *name,
	float value)
{
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_name = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		MCW_LOGE("invalid format parameter");
		return;
	}
	if (!name) {
		MCW_LOGE("invalid name parameter");
		return;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_IF_FAILED(env != NULL, -ENODEV);

	j_name = mcw_jni_string_new(env, name);
	if (!j_name) {
		MCW_LOGE("string creation failed");
		goto end;
	}

	(*env)->CallVoidMethod(env, fmt->mediaformat,
		mcw_jni.MediaFormat.setInteger, j_name, (jfloat)value);

end:
	if (j_name)
		(*env)->DeleteLocalRef(env, j_name);

	mcw_jni_detach(attached);
}


static void mcw_jni_mediaformat_set_string(
	struct mcw_mediaformat *format,
	const char *name,
	const char *value)
{
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_name = NULL;
	jstring j_val = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		MCW_LOGE("invalid format parameter");
		return;
	}
	if (!name) {
		MCW_LOGE("invalid name parameter");
		return;
	}
	if (!value) {
		MCW_LOGE("invalid value parameter");
		return;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_IF_FAILED(env != NULL, -ENODEV);

	j_name = mcw_jni_string_new(env, name);
	if (!j_name) {
		MCW_LOGE("string creation failed");
		goto end;
	}

	j_val = mcw_jni_string_new(env, value);
	if (!j_val) {
		MCW_LOGE("string creation failed");
		goto end;
	}

	(*env)->CallVoidMethod(env, fmt->mediaformat,
		mcw_jni.MediaFormat.setString, j_name, j_val);

end:
	if (j_val)
		(*env)->DeleteLocalRef(env, j_val);
	if (j_name)
		(*env)->DeleteLocalRef(env, j_name);

	mcw_jni_detach(attached);
}


static void mcw_jni_mediaformat_set_buffer(
	struct mcw_mediaformat *format,
	const char *name,
	void *data,
	size_t size)
{
	int error = 0;
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_name = NULL;
	jobject j_buf = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		MCW_LOGE("invalid format parameter");
		return;
	}
	if (!name) {
		MCW_LOGE("invalid name parameter");
		return;
	}
	if (!data) {
		MCW_LOGE("invalid data parameter");
		return;
	}
	if (size == 0) {
		MCW_LOGE("invalid size parameter");
		return;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_IF_FAILED(env != NULL, -ENODEV);

	j_name = mcw_jni_string_new(env, name);
	if (!j_name) {
		MCW_LOGE("string creation failed");
		error = 1;
		goto end;
	}

	j_buf = (*env)->NewDirectByteBuffer(env, data, size);
	if (!j_buf) {
		MCW_LOGE("buffer creation failed");
		error = 1;
		goto end;
	}

	(*env)->CallVoidMethod(env, fmt->mediaformat,
		mcw_jni.MediaFormat.setByteBuffer, j_name, j_buf);

	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		error = 1;
		goto end;
	}

end:
	if (j_name)
		(*env)->DeleteLocalRef(env, j_name);
	if (j_buf)
		(*env)->DeleteLocalRef(env, j_buf);

	mcw_jni_detach(attached);
}


static struct mcw_mediacodec *mcw_jni_mediacodec_create_codec_by_name(
	const char *name)
{
	int error = 0;
	struct mcw_jni_mediacodec *mc = NULL;
	jobject j_mc = NULL;
	jobject j_buf_info = NULL;
	jstring j_name = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	MCW_RETURN_VAL_IF_FAILED(name != NULL, -EINVAL, NULL);

	mc = calloc(1, sizeof(*mc));
	MCW_RETURN_VAL_IF_FAILED(mc != NULL, -ENOMEM, NULL);

	attached = mcw_jni_attach_and_get_env(&env);
	if (!env) {
		error = 1;
		goto end;
	}

	j_name = mcw_jni_string_new(env, name);
	if (!j_name) {
		MCW_LOGE("string creation failed");
		error = 1;
		goto end;
	}

	j_mc = (*env)->CallStaticObjectMethod(env,
		mcw_jni.MediaCodec.clazz,
		mcw_jni.MediaCodec.createByCodecName, j_name);
	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		error = 1;
		goto end;
	}
	if (!j_mc) {
		MCW_LOGE("codec creation failed (name: %s)", name);
		error = 1;
		goto end;
	}
	mc->mediacodec = (*env)->NewGlobalRef(env, j_mc);

	j_buf_info = (*env)->NewObject(env,
		mcw_jni.BufferInfo.clazz, mcw_jni.BufferInfo.new);
	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		error = 1;
		goto end;
	}
	if (!j_buf_info) {
		MCW_LOGE("buffer info object creation failed");
		error = 1;
		goto end;
	}
	mc->bufferinfo = (*env)->NewGlobalRef(env, j_buf_info);

end:
	if (j_buf_info)
		(*env)->DeleteLocalRef(env, j_buf_info);
	if (j_name)
		(*env)->DeleteLocalRef(env, j_name);

	mcw_jni_detach(attached);

	if (error) {
		free(mc);
		return NULL;
	} else
		return (struct mcw_mediacodec *)mc;
}


static struct mcw_mediacodec *mcw_jni_mediacodec_create_decoder_by_type(
	const char *mime_type)
{
	int error = 0;
	struct mcw_jni_mediacodec *mc = NULL;
	jobject j_mc = NULL;
	jobject j_buf_info = NULL;
	jstring j_mime_type = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	MCW_RETURN_VAL_IF_FAILED(mime_type != NULL, -EINVAL, NULL);

	mc = calloc(1, sizeof(*mc));
	MCW_RETURN_VAL_IF_FAILED(mc != NULL, -ENOMEM, NULL);

	attached = mcw_jni_attach_and_get_env(&env);
	if (!env) {
		error = 1;
		goto end;
	}

	j_mime_type = mcw_jni_string_new(env, mime_type);
	if (!j_mime_type) {
		MCW_LOGE("string creation failed");
		error = 1;
		goto end;
	}

	j_mc = (*env)->CallStaticObjectMethod(env,
		mcw_jni.MediaCodec.clazz,
		mcw_jni.MediaCodec.createDecoderByType, j_mime_type);
	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		error = 1;
		goto end;
	}
	if (!j_mc) {
		MCW_LOGE("codec creation failed (mime type: %s)", mime_type);
		error = 1;
		goto end;
	}
	mc->mediacodec = (*env)->NewGlobalRef(env, j_mc);

	j_buf_info = (*env)->NewObject(env,
		mcw_jni.BufferInfo.clazz, mcw_jni.BufferInfo.new);
	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		error = 1;
		goto end;
	}
	if (!j_buf_info) {
		MCW_LOGE("buffer info object creation failed");
		error = 1;
		goto end;
	}
	mc->bufferinfo = (*env)->NewGlobalRef(env, j_buf_info);

end:
	if (j_buf_info)
		(*env)->DeleteLocalRef(env, j_buf_info);
	if (j_mime_type)
		(*env)->DeleteLocalRef(env, j_mime_type);

	mcw_jni_detach(attached);

	if (error) {
		free(mc);
		return NULL;
	} else
		return (struct mcw_mediacodec *)mc;
}


static struct mcw_mediacodec *mcw_jni_mediacodec_create_encoder_by_type(
	const char *mime_type)
{
	int error = 0;
	struct mcw_jni_mediacodec *mc = NULL;
	jobject j_mc = NULL;
	jobject j_buf_info = NULL;
	jstring j_mime_type = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	MCW_RETURN_VAL_IF_FAILED(mime_type != NULL, -EINVAL, NULL);

	mc = calloc(1, sizeof(*mc));
	MCW_RETURN_VAL_IF_FAILED(mc != NULL, -ENOMEM, NULL);

	attached = mcw_jni_attach_and_get_env(&env);
	if (!env) {
		error = 1;
		goto end;
	}

	j_mime_type = mcw_jni_string_new(env, mime_type);
	if (!j_mime_type) {
		MCW_LOGE("string creation failed");
		error = 1;
		goto end;
	}

	j_mc = (*env)->CallStaticObjectMethod(env,
		mcw_jni.MediaCodec.clazz,
		mcw_jni.MediaCodec.createEncoderByType, j_mime_type);
	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		error = 1;
		goto end;
	}
	if (!j_mc) {
		MCW_LOGE("codec creation failed (mime type: %s)", mime_type);
		error = 1;
		goto end;
	}
	mc->mediacodec = (*env)->NewGlobalRef(env, j_mc);

	j_buf_info = (*env)->NewObject(env,
		mcw_jni.BufferInfo.clazz, mcw_jni.BufferInfo.new);
	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		error = 1;
		goto end;
	}
	if (!j_buf_info) {
		MCW_LOGE("buffer info object creation failed");
		error = 1;
		goto end;
	}
	mc->bufferinfo = (*env)->NewGlobalRef(env, j_buf_info);

end:
	if (j_buf_info)
		(*env)->DeleteLocalRef(env, j_buf_info);
	if (j_mime_type)
		(*env)->DeleteLocalRef(env, j_mime_type);

	mcw_jni_detach(attached);

	if (error) {
		free(mc);
		return NULL;
	} else
		return (struct mcw_mediacodec *)mc;
}


static enum mcw_media_status mcw_jni_mediacodec_delete(
	struct mcw_mediacodec *codec)
{
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		MCW_LOGE("invalid codec parameter");
		return MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_VAL_IF_FAILED(env != NULL, -ENODEV,
		MCW_MEDIA_STATUS_ERROR_UNKNOWN);

	(*env)->CallVoidMethod(env, mc->mediacodec,
		mcw_jni.MediaCodec.release);

	(*env)->DeleteGlobalRef(env, mc->mediacodec);

	(*env)->DeleteGlobalRef(env, mc->bufferinfo);

	if (mc->in_buffers)
		(*env)->DeleteGlobalRef(env, mc->in_buffers);

	if (mc->out_buffers)
		(*env)->DeleteGlobalRef(env, mc->out_buffers);

	free(mc);

	mcw_jni_detach(attached);

	return MCW_MEDIA_STATUS_OK;
}


static enum mcw_media_status mcw_jni_mediacodec_configure(
	struct mcw_mediacodec *codec,
	const struct mcw_mediaformat *format,
	ANativeWindow *surface,
	struct mcw_mediacrypto *crypto,
	uint32_t flags)
{
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jobject j_surface = NULL;
	enum mcw_media_status ret = MCW_MEDIA_STATUS_OK;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		MCW_LOGE("invalid codec parameter");
		return MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_VAL_IF_FAILED(env != NULL, -ENODEV,
		MCW_MEDIA_STATUS_ERROR_UNKNOWN);

#if 0
	/* TODO: ANativeWindow_toSurface is introduced in API 26 */
	if (surface)
		j_surface = ANativeWindow_toSurface(env, surface);
#endif

	(*env)->CallVoidMethod(env, mc->mediacodec,
		mcw_jni.MediaCodec.configure,
		(fmt) ? fmt->mediaformat : NULL,
		j_surface,
		NULL, /* crypto is not supported */
		(jint)flags);

	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
	}

	if (j_surface)
		(*env)->DeleteLocalRef(env, j_surface);

	mcw_jni_detach(attached);

	return ret;
}


static enum mcw_media_status mcw_jni_mediacodec_start(
	struct mcw_mediacodec *codec)
{
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	enum mcw_media_status ret = MCW_MEDIA_STATUS_OK;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		MCW_LOGE("invalid codec parameter");
		return MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_VAL_IF_FAILED(env != NULL, -ENODEV,
		MCW_MEDIA_STATUS_ERROR_UNKNOWN);

	(*env)->CallVoidMethod(env, mc->mediacodec,
		mcw_jni.MediaCodec.start);

	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
		goto end;
	}

	mcw_jni_mediacodec_get_input_buffers(env, mc);
	mcw_jni_mediacodec_get_output_buffers(env, mc);

end:
	mcw_jni_detach(attached);

	return ret;
}


static enum mcw_media_status mcw_jni_mediacodec_stop(
	struct mcw_mediacodec *codec)
{
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	enum mcw_media_status ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		MCW_LOGE("invalid codec parameter");
		return MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_VAL_IF_FAILED(env != NULL, -ENODEV,
		MCW_MEDIA_STATUS_ERROR_UNKNOWN);

	(*env)->CallVoidMethod(env, mc->mediacodec,
		mcw_jni.MediaCodec.stop);

	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
	} else
		ret = MCW_MEDIA_STATUS_OK;

	mcw_jni_detach(attached);

	return ret;
}


static enum mcw_media_status mcw_jni_mediacodec_flush(
	struct mcw_mediacodec *codec)
{
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	enum mcw_media_status ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		MCW_LOGE("invalid codec parameter");
		return MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_VAL_IF_FAILED(env != NULL, -ENODEV,
		MCW_MEDIA_STATUS_ERROR_UNKNOWN);

	(*env)->CallVoidMethod(env, mc->mediacodec,
		mcw_jni.MediaCodec.flush);

	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
	} else
		ret = MCW_MEDIA_STATUS_OK;

	mcw_jni_detach(attached);

	return ret;
}


static uint8_t *mcw_jni_mediacodec_get_input_buffer(
	struct mcw_mediacodec *codec,
	size_t idx,
	size_t *out_size)
{
	int error = 0;
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	jobject j_buf = NULL;
	uint8_t *ptr = NULL;
	size_t sz = 0;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		MCW_LOGE("invalid codec parameter");
		return NULL;
	}
	if (!out_size) {
		MCW_LOGE("invalid output size parameter");
		return NULL;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_VAL_IF_FAILED(env != NULL, -ENODEV, NULL);

	if (mc->in_buffers) {
		j_buf = (*env)->GetObjectArrayElement(env,
			mc->in_buffers, (jint)idx);
	} else {
		j_buf = (*env)->CallObjectMethod(env, mc->mediacodec,
			mcw_jni.MediaCodec.getInputBuffer, (jint)idx);

		if ((*env)->ExceptionCheck(env)) {
			(*env)->ExceptionDescribe(env);
			(*env)->ExceptionClear(env);
			error = 1;
			goto end;
		}
	}

	if (!j_buf) {
		error = 1;
		goto end;
	}

	ptr = (*env)->GetDirectBufferAddress(env, j_buf);
	sz = (*env)->GetDirectBufferCapacity(env, j_buf);

end:
	if (j_buf)
		(*env)->DeleteLocalRef(env, j_buf);

	mcw_jni_detach(attached);

	if (error)
		return NULL;
	else {
		if (out_size)
			*out_size = sz;
		return ptr;
	}
}


static uint8_t *mcw_jni_mediacodec_get_output_buffer(
	struct mcw_mediacodec *codec,
	size_t idx,
	size_t *out_size)
{
	int error = 0;
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	jobject j_buf = NULL;
	uint8_t *ptr = NULL;
	size_t sz = 0;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		MCW_LOGE("invalid codec parameter");
		return NULL;
	}
	if (!out_size) {
		MCW_LOGE("invalid output size parameter");
		return NULL;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_VAL_IF_FAILED(env != NULL, -ENODEV, NULL);

	if (mc->out_buffers) {
		j_buf = (*env)->GetObjectArrayElement(env,
			mc->out_buffers, (jint)idx);
	} else {
		j_buf = (*env)->CallObjectMethod(env, mc->mediacodec,
			mcw_jni.MediaCodec.getOutputBuffer, (jint)idx);

		if ((*env)->ExceptionCheck(env)) {
			(*env)->ExceptionDescribe(env);
			(*env)->ExceptionClear(env);
			error = 1;
			goto end;
		}
	}

	if (!j_buf) {
		error = 1;
		goto end;
	}

	ptr = (*env)->GetDirectBufferAddress(env, j_buf);
	sz = (*env)->GetDirectBufferCapacity(env, j_buf);

end:
	if (j_buf)
		(*env)->DeleteLocalRef(env, j_buf);

	mcw_jni_detach(attached);

	if (error)
		return NULL;
	else {
		if (out_size)
			*out_size = sz;
		return ptr;
	}
}


static ssize_t mcw_jni_mediacodec_dequeue_input_buffer(
	struct mcw_mediacodec *codec,
	int64_t timeout_us)
{
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	jint ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		MCW_LOGE("invalid codec parameter");
		return -1;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_VAL_IF_FAILED(env != NULL, -ENODEV, -1);

	ret = (*env)->CallIntMethod(env, mc->mediacodec,
		mcw_jni.MediaCodec.dequeueInputBuffer, (jlong)timeout_us);

	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		ret = -1;
	}

	mcw_jni_detach(attached);

	return (ssize_t)ret;
}


static enum mcw_media_status mcw_jni_mediacodec_queue_input_buffer(
	struct mcw_mediacodec *codec,
	size_t idx,
	off_t offset,
	size_t size,
	uint64_t time,
	uint32_t flags)
{
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	enum mcw_media_status ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		MCW_LOGE("invalid codec parameter");
		return MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_VAL_IF_FAILED(env != NULL, -ENODEV,
		MCW_MEDIA_STATUS_ERROR_UNKNOWN);

	(*env)->CallVoidMethod(env, mc->mediacodec,
		mcw_jni.MediaCodec.queueInputBuffer, (jint)idx, (jint)offset,
		(jint)size, (jlong)time, (jint)flags);

	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
	} else
		ret = MCW_MEDIA_STATUS_OK;

	mcw_jni_detach(attached);

	return ret;
}


static ssize_t mcw_jni_mediacodec_dequeue_output_buffer(
	struct mcw_mediacodec *codec,
	struct mcw_mediacodec_bufferinfo *info,
	int64_t timeout_us)
{
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	jint ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		MCW_LOGE("invalid codec parameter");
		return -1;
	}
	if (!info) {
		MCW_LOGE("invalid info parameter");
		return -1;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_VAL_IF_FAILED(env != NULL, -ENODEV, -1);

	ret = (*env)->CallIntMethod(env, mc->mediacodec,
		mcw_jni.MediaCodec.dequeueOutputBuffer,
		mc->bufferinfo, (jlong)timeout_us);

	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		ret = -1;
		goto end;
	}

	if (ret == MCW_INFO_OUTPUT_BUFFERS_CHANGED)
		mcw_jni_mediacodec_get_output_buffers(env, mc);

	info->offset = (*env)->GetIntField(env,
		mc->bufferinfo, mcw_jni.BufferInfo.offset);
	info->size = (*env)->GetIntField(env,
		mc->bufferinfo, mcw_jni.BufferInfo.size);
	info->presentation_time_us = (*env)->GetLongField(env,
		mc->bufferinfo, mcw_jni.BufferInfo.presentationTimeUs);
	info->flags = (*env)->GetIntField(env,
		mc->bufferinfo, mcw_jni.BufferInfo.flags);

end:
	mcw_jni_detach(attached);

	return (ssize_t)ret;
}


static struct mcw_mediaformat *mcw_jni_mediacodec_get_output_format(
	struct mcw_mediacodec *codec,
	size_t idx)
{
	int error = 0;
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	struct mcw_jni_mediaformat *fmt = NULL;
	jobject j_fmt = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_VAL_IF_FAILED(env != NULL, -ENODEV, NULL);

	fmt = calloc(1, sizeof(*fmt));
	MCW_RETURN_VAL_IF_FAILED(fmt != NULL, -ENOMEM, NULL);

	j_fmt = (*env)->CallObjectMethod(env, mc->mediacodec,
		mcw_jni.MediaCodec.getOutputFormat, (jint)idx);
	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		error = 1;
		goto end;
	}
	if (!j_fmt) {
		error = 1;
		goto end;
	}
	fmt->mediaformat = (*env)->NewGlobalRef(env, j_fmt);

end:
	if (j_fmt)
		(*env)->DeleteLocalRef(env, j_fmt);

	mcw_jni_detach(attached);

	if (error) {
		free(fmt);
		return NULL;
	} else
		return (struct mcw_mediaformat *)fmt;
}


static enum mcw_media_status mcw_jni_mediacodec_release_output_buffer(
	struct mcw_mediacodec *codec,
	size_t idx,
	bool render)
{
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	enum mcw_media_status ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		MCW_LOGE("invalid codec parameter");
		return MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_VAL_IF_FAILED(env != NULL, -ENODEV,
		MCW_MEDIA_STATUS_ERROR_UNKNOWN);

	(*env)->CallVoidMethod(env, mc->mediacodec,
		mcw_jni.MediaCodec.releaseOutputBuffer,
		(jint)idx, (jboolean)render);

	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
	} else
		ret = MCW_MEDIA_STATUS_OK;

	mcw_jni_detach(attached);

	return ret;
}


static enum mcw_media_status mcw_jni_mediacodec_release_output_buffer_at_time(
	struct mcw_mediacodec *codec,
	size_t idx,
	int64_t timestamp_ns)
{
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	enum mcw_media_status ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		MCW_LOGE("invalid codec parameter");
		return MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	MCW_RETURN_VAL_IF_FAILED(env != NULL, -ENODEV,
		MCW_MEDIA_STATUS_ERROR_UNKNOWN);

	(*env)->CallVoidMethod(env, mc->mediacodec,
		mcw_jni.MediaCodec.releaseOutputBuffer,
		(jint)idx, (jlong)timestamp_ns);

	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
	} else
		ret = MCW_MEDIA_STATUS_OK;

	mcw_jni_detach(attached);

	return ret;
}


int mcw_jni_init(
	struct mcw *mcw,
	struct mcw_jnienv *jnienv)
{
	static int needs_init = 1;
	static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	int ret = 0;
	unsigned int i;
	JNIEnv *env = (JNIEnv *)jnienv;
	jclass clazz;
	jint res;

	MCW_RETURN_ERR_IF_FAILED(mcw != NULL, -EINVAL);
	MCW_RETURN_ERR_IF_FAILED(jnienv != NULL, -EINVAL);

	pthread_mutex_lock(&mutex);

	if (!needs_init)
		goto end;

	memset(&mcw_jni, 0, sizeof(mcw_jni));

	res = (*env)->GetJavaVM(env, &mcw_jni.jvm);
	if (res < 0) {
		ret = -EINVAL;
		goto end;
	}

	for (i = 0; i < MCW_SIZEOF_ARRAY(mcw_jni_class); i++) {
		clazz = (*env)->FindClass(env, mcw_jni_class[i].name);
		if ((*env)->ExceptionCheck(env)) {
			(*env)->ExceptionClear(env);
			ret = -ENOSYS;
			goto end;
		}
		*(jclass *)((uint8_t *)&mcw_jni + mcw_jni_class[i].offset) =
			(jclass)(*env)->NewGlobalRef(env, clazz);
	}

	for (i = 0; i < MCW_SIZEOF_ARRAY(mcw_jni_member); i++) {
		clazz = *(jclass *)((uint8_t *)&mcw_jni +
			mcw_jni_member[i].class_offset);
		switch (mcw_jni_member[i].type) {
		case MCW_JNI_MEMBER_TYPE_METHOD:
			*(jmethodID *)((uint8_t *)&mcw_jni +
				mcw_jni_member[i].offset) =
				(*env)->GetMethodID(env, clazz,
					mcw_jni_member[i].name,
					mcw_jni_member[i].signature);
			break;
		case MCW_JNI_MEMBER_TYPE_STATIC_METHOD:
			*(jmethodID *)((uint8_t *)&mcw_jni +
				mcw_jni_member[i].offset) =
				(*env)->GetStaticMethodID(env, clazz,
					mcw_jni_member[i].name,
					mcw_jni_member[i].signature);
			break;
		case MCW_JNI_MEMBER_TYPE_FIELD:
			*(jfieldID *)((uint8_t *)&mcw_jni +
				mcw_jni_member[i].offset) =
				(*env)->GetFieldID(env, clazz,
					mcw_jni_member[i].name,
					mcw_jni_member[i].signature);
			break;
		default:
			break;
		}
		if ((*env)->ExceptionCheck(env)) {
			(*env)->ExceptionClear(env);
			if (mcw_jni_member[i].critical) {
				ret = -ENOSYS;
				goto end;
			}
		}
	}

	if ((mcw_jni.MediaCodec.getInputBuffer) &&
		(mcw_jni.MediaCodec.getOutputBuffer)) {
		mcw_jni.MediaCodec.getInputBuffers = NULL;
		mcw_jni.MediaCodec.getOutputBuffers = NULL;
	} else if ((!mcw_jni.MediaCodec.getInputBuffers) &&
		(!mcw_jni.MediaCodec.getOutputBuffers)) {
		MCW_LOGE("Could not find methods "
			"getInputBuffer(s)/getOutputBuffer(s)");
		ret = -ENOSYS;
		goto end;
	}

	needs_init = 0;

end:
	mcw->mediaformat.nnew = mcw_jni_mediaformat_new;
	mcw->mediaformat.ddelete = mcw_jni_mediaformat_delete;
	mcw->mediaformat.to_string = mcw_jni_mediaformat_to_string;
	mcw->mediaformat.get_int32 = mcw_jni_mediaformat_get_int32;
	mcw->mediaformat.get_int64 = mcw_jni_mediaformat_get_int64;
	mcw->mediaformat.get_float = mcw_jni_mediaformat_get_float;
	mcw->mediaformat.get_buffer = mcw_jni_mediaformat_get_buffer;
	mcw->mediaformat.get_string = mcw_jni_mediaformat_get_string;
	mcw->mediaformat.set_int32 = mcw_jni_mediaformat_set_int32;
	mcw->mediaformat.set_int64 = mcw_jni_mediaformat_set_int64;
	mcw->mediaformat.set_float = mcw_jni_mediaformat_set_float;
	mcw->mediaformat.set_string = mcw_jni_mediaformat_set_string;
	mcw->mediaformat.set_buffer = mcw_jni_mediaformat_set_buffer;
	mcw->mediaformat.KEY_AAC_PROFILE =
		mcw_jni_mediaformat_KEY_AAC_PROFILE;
	mcw->mediaformat.KEY_BIT_RATE =
		mcw_jni_mediaformat_KEY_BIT_RATE;
	mcw->mediaformat.KEY_CHANNEL_COUNT =
		mcw_jni_mediaformat_KEY_CHANNEL_COUNT;
	mcw->mediaformat.KEY_CHANNEL_MASK =
		mcw_jni_mediaformat_KEY_CHANNEL_MASK;
	mcw->mediaformat.KEY_COLOR_FORMAT =
		mcw_jni_mediaformat_KEY_COLOR_FORMAT;
	mcw->mediaformat.KEY_DURATION =
		mcw_jni_mediaformat_KEY_DURATION;
	mcw->mediaformat.KEY_FLAC_COMPRESSION_LEVEL =
		mcw_jni_mediaformat_KEY_FLAC_COMPRESSION_LEVEL;
	mcw->mediaformat.KEY_FRAME_RATE =
		mcw_jni_mediaformat_KEY_FRAME_RATE;
	mcw->mediaformat.KEY_HEIGHT =
		mcw_jni_mediaformat_KEY_HEIGHT;
	mcw->mediaformat.KEY_IS_ADTS =
		mcw_jni_mediaformat_KEY_IS_ADTS;
	mcw->mediaformat.KEY_IS_AUTOSELECT =
		mcw_jni_mediaformat_KEY_IS_AUTOSELECT;
	mcw->mediaformat.KEY_IS_DEFAULT =
		mcw_jni_mediaformat_KEY_IS_DEFAULT;
	mcw->mediaformat.KEY_IS_FORCED_SUBTITLE =
		mcw_jni_mediaformat_KEY_IS_FORCED_SUBTITLE;
	mcw->mediaformat.KEY_I_FRAME_INTERVAL =
		mcw_jni_mediaformat_KEY_I_FRAME_INTERVAL;
	mcw->mediaformat.KEY_LANGUAGE =
		mcw_jni_mediaformat_KEY_LANGUAGE;
	mcw->mediaformat.KEY_MAX_HEIGHT =
		mcw_jni_mediaformat_KEY_MAX_HEIGHT;
	mcw->mediaformat.KEY_MAX_INPUT_SIZE =
		mcw_jni_mediaformat_KEY_MAX_INPUT_SIZE;
	mcw->mediaformat.KEY_MAX_WIDTH =
		mcw_jni_mediaformat_KEY_MAX_WIDTH;
	mcw->mediaformat.KEY_MIME =
		mcw_jni_mediaformat_KEY_MIME;
	mcw->mediaformat.KEY_PUSH_BLANK_BUFFERS_ON_STOP =
		mcw_jni_mediaformat_KEY_PUSH_BLANK_BUFFERS_ON_STOP;
	mcw->mediaformat.KEY_REPEAT_PREVIOUS_FRAME_AFTER =
		mcw_jni_mediaformat_KEY_REPEAT_PREVIOUS_FRAME_AFTER;
	mcw->mediaformat.KEY_SAMPLE_RATE =
		mcw_jni_mediaformat_KEY_SAMPLE_RATE;
	mcw->mediaformat.KEY_WIDTH =
		mcw_jni_mediaformat_KEY_WIDTH;
	mcw->mediaformat.KEY_STRIDE =
		mcw_jni_mediaformat_KEY_STRIDE;
	mcw->mediacodec.create_codec_by_name =
		mcw_jni_mediacodec_create_codec_by_name;
	mcw->mediacodec.create_decoder_by_type =
		mcw_jni_mediacodec_create_decoder_by_type;
	mcw->mediacodec.create_encoder_by_type =
		mcw_jni_mediacodec_create_encoder_by_type;
	mcw->mediacodec.ddelete = mcw_jni_mediacodec_delete;
	mcw->mediacodec.configure = mcw_jni_mediacodec_configure;
	mcw->mediacodec.start = mcw_jni_mediacodec_start;
	mcw->mediacodec.stop = mcw_jni_mediacodec_stop;
	mcw->mediacodec.flush = mcw_jni_mediacodec_flush;
	mcw->mediacodec.get_input_buffer =
		mcw_jni_mediacodec_get_input_buffer;
	mcw->mediacodec.get_output_buffer =
		mcw_jni_mediacodec_get_output_buffer;
	mcw->mediacodec.dequeue_input_buffer =
		mcw_jni_mediacodec_dequeue_input_buffer;
	mcw->mediacodec.queue_input_buffer =
		mcw_jni_mediacodec_queue_input_buffer;
	mcw->mediacodec.dequeue_output_buffer =
		mcw_jni_mediacodec_dequeue_output_buffer;
	mcw->mediacodec.get_output_format =
		mcw_jni_mediacodec_get_output_format;
	mcw->mediacodec.release_output_buffer =
		mcw_jni_mediacodec_release_output_buffer;
	mcw->mediacodec.release_output_buffer_at_time =
		mcw_jni_mediacodec_release_output_buffer_at_time;

	if (ret != 0) {
		for (i = 0; i < MCW_SIZEOF_ARRAY(mcw_jni_class); i++) {
			clazz = *(jclass *)((uint8_t *)&mcw_jni +
				mcw_jni_class[i].offset);
			if (clazz)
				(*env)->DeleteGlobalRef(env, clazz);
		}
	}

	pthread_mutex_unlock(&mutex);

	return ret;
}
