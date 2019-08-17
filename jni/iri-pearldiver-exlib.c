/*
 * Copyright (C) 2018-2019 BiiLabs Co., Ltd. and Contributors
 * All Rights Reserved.
 * This is free software; you can redistribute it and/or modify it under the
 * terms of the MIT license. A copy of the license can be found in the file
 * "LICENSE" at the root of this distribution.
 */

#include "iri-pearldiver-exlib.h"
#include <jni.h>
#include <stdint.h>
#include "../src/dcurl.h"
#include "../src/trinary.h"

#include "../src/common.h"
#include "../src/constants.h"

JNIEXPORT jboolean JNICALL
Java_com_iota_iri_crypto_PearlDiver_exlibInit(JNIEnv *env, jclass clazz)
{
    if (!dcurl_init())
        return JNI_FALSE;
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_com_iota_iri_crypto_PearlDiver_exlibSearch(JNIEnv *env,
                                                jclass clazz,
                                                jbyteArray trits,
                                                jint mwm,
                                                jint threads)
{
    jboolean ret = JNI_TRUE;

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    // dcurl start
    /*********** Get the Byte array from Java byte Array *************/
    jbyte *c_trits = (*env)->GetByteArrayElements(env, trits, NULL);

    Trits_t *arg_trits = initTrits((int8_t *) c_trits, 8019);
    Trytes_t *arg_trytes = trytes_from_trits(arg_trits);
    if (!arg_trytes) {
        ret = JNI_FALSE;
        goto fail_input;
    }
    /****************************************************************/

    int8_t *result = dcurl_entry(arg_trytes->data, mwm, threads);

    /************ Write result back Java byte array *****************/
    Trytes_t *ret_trytes = initTrytes(result, 2673);
    Trits_t *ret_trits = trits_from_trytes(ret_trytes);
    if (!ret_trits) {
        ret = JNI_FALSE;
        goto fail_output;
    }
    (*env)->SetByteArrayRegion(env, trits, 0, 8019, ret_trits->data);
    /****************************************************************/
    // dcurl end
    clock_gettime(CLOCK_REALTIME, &end);
    printf("bundleHash %.*s ", BundleTrinarySize       / 3, (char *)arg_trytes->data + (BundleTrinaryOffset / 3));
    printf("currentIdx %.*s ", CurrentIndexTrinarySize / 3, (char *)arg_trytes->data + (CurrentIndexTrinaryOffset / 3));
    printf("time %u\n", diff_in_nanosecond(start, end));
    fflush(stdout);

fail_output:
    free(result);
    freeTrobject(ret_trits);
    freeTrobject(ret_trytes);
fail_input:
    freeTrobject(arg_trits);
    freeTrobject(arg_trytes);

    return ret;
}

JNIEXPORT void JNICALL
Java_com_iota_iri_crypto_PearlDiver_exlibCancel(JNIEnv *env, jclass clazz)
{
    /* Do nothing */
}

JNIEXPORT void JNICALL
Java_com_iota_iri_crypto_PearlDiver_exlibDestroy(JNIEnv *env, jclass clazz)
{
    dcurl_destroy();
}
