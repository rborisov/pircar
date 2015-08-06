/*
 * Copyright (C) 2010-2014 Max Kellermann <max@duempel.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * FOUNDATION OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef JAVA_FILE_HXX
#define JAVA_FILE_HXX

#include "Object.hxx"

#include <jni.h>

class AllocatedPath;

namespace Java {
	/**
	 * Wrapper for a java.io.File object.
	 */
	class File : public LocalObject {
		static jmethodID getAbsolutePath_method;

	public:
		gcc_nonnull_all
		static void Initialise(JNIEnv *env);

		gcc_nonnull_all
		static jstring getAbsolutePath(JNIEnv *env, jobject file) {
			return (jstring)env->CallObjectMethod(file,
							      getAbsolutePath_method);
		}

		/**
		 * Invoke File.getAbsolutePath() and release the
		 * specified File reference.
		 */
		gcc_pure gcc_nonnull_all
		static AllocatedPath ToAbsolutePath(JNIEnv *env,
						    jobject file);
	};
}

#endif
