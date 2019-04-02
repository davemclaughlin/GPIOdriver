#include <jni.h>
/* Header for class axonI2C */
#include <stdio.h>
#include <android/log.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <memory.h>
#include <malloc.h>

#ifndef _Included_axon_I2C
#define _Included_axon_I2C
#ifdef __cplusplus
  extern "C" {
#endif
/*
 * Class:     axon_I2C
 * Method:    open
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_axoninstruments_gpio_axonI2C_open
  (JNIEnv *, jobject, jstring);

/*
 * Class:     axon_I2C
 * Method:    read
 * Signature: (II[II)I
 */
JNIEXPORT jint JNICALL Java_com_axoninstruments_gpio_axonI2C_read
  (JNIEnv *, jobject, jint, jint, jintArray, jint);

/*
 * Class:     axon_I2C
 * Method:    write
 * Signature: (III[II)I
 */
JNIEXPORT jint JNICALL Java_com_axoninstruments_gpio_axonI2C_write
  (JNIEnv *, jobject, jint, jint, jint, jintArray, jint);

/*
 * Class:     axon_I2c
 * Method:    close
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_axoninstruments_gpio_axonI2C_close
  (JNIEnv *, jobject, jint);

/*
 * GPIO handler
 *
 */

/*
 * Class:     axon_io
 * Method:    setdirectionGPIO
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_axoninstruments_gpio_axonI2C_setdirectionGPIO
  (JNIEnv *env, jobject obj, jstring gpio, jint state);

/*
 * Class:     axon_io
 * Method:    readGPIO
 * Signature: (II[II)I
 */
JNIEXPORT jint JNICALL Java_com_axoninstruments_gpio_axonI2C_readGPIO
  (JNIEnv *env, jobject obj, jstring gpio);

/*
 * Class:     axon_io
 * Method:    setoutputGPIO
 * Signature: (III[II)I
 */
JNIEXPORT jint JNICALL Java_com_axoninstruments_gpio_axonI2C_setoutputGPIO
  (JNIEnv *env, jobject obj, jstring gpio, jint state);

#ifdef __cplusplus
}
#endif
#endif
