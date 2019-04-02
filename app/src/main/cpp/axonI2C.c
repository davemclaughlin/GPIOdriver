#include <sys/ioctl.h>
#include <unistd.h>
#include "axonI2C.h"
#include <linux/i2c-dev.h>

#define  LOG_TAG    "axoni2c"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

//#define DEBUG_ON

//***************************************************************************
// Open the I2C device
//***************************************************************************

JNIEXPORT jint JNICALL Java_com_axoninstruments_gpio_axonI2C_open(JNIEnv *env, jobject obj, jstring file)
{
    char fileName[64];
    const char *str;
    
    str = (*env)->GetStringUTFChars(env, file, NULL);
    if (str == NULL) {
#ifdef DEBUG_ON
        LOGI("Can't get file name!");
#endif
        return -1;
    }
    sprintf(fileName, "%s", str);
#ifdef DEBUG_ON
    LOGI("will open i2c device node %s", fileName);
#endif
    (*env)->ReleaseStringUTFChars(env, file, str);
    return open(fileName, O_RDWR);
}

//***************************************************************************
// Read data from the I2C device
//***************************************************************************
  
JNIEXPORT jint JNICALL Java_com_axoninstruments_gpio_axonI2C_read(JNIEnv * env, jobject obj, jint fileHander, jint slaveAddr, jintArray bufArr, jint len)
{
    jint *bufInt;
    char *bufByte;
    int res = 0, i = 0, j = 0;
    
    if (len <= 0) {
#ifdef DEBUG_ON
	    LOGE("I2C: buf len <=0");
#endif
        return -1;
    }
      
    bufInt = (jint *) malloc(len * sizeof(int));
    if (bufInt == 0) {
#ifdef DEBUG_ON
        LOGE("I2C: nomem");
#endif
        return -1;
    }
    bufByte = (char*) malloc(len);
    if (bufByte == 0) {
#ifdef DEBUG_ON
        LOGE("I2C: nomem");
#endif
	    free(bufInt);
    	return -1;
    }
    
    (*env)->GetIntArrayRegion(env, bufArr, 0, len, bufInt);
    
    res = ioctl(fileHander, I2C_SLAVE, slaveAddr);
    if (res != 0) {
#ifdef DEBUG_ON
        LOGE("I2C: Can't set slave address");
#endif
	    free(bufByte);
	    free(bufInt);
	    return -1;
    }
    
    memset(bufByte, '\0', len);
    if ((j = read(fileHander, bufByte, len)) != len) {
#ifdef DEBUG_ON
        LOGE("read fail in i2c read jni i = %d buf 4", i);
#endif
	    free(bufByte);
	    free(bufInt);
        return -1;    
    } 
    else {
    for (i = 0; i < j ; i++)
        bufInt[i] = bufByte[i];
#ifdef DEBUG_ON
        LOGI("return %d %d %d %d in i2c read jni", bufByte[0], bufByte[1], bufByte[2], bufByte[3]);
#endif
        (*env)->SetIntArrayRegion(env, bufArr, 0, len, bufInt);
    }
    free(bufByte);
    free(bufInt);
    
    return j;
    
    err2:
    free(bufByte);
    err1:
    free(bufInt);
    err0:
    return -1;                                          
}

//***************************************************************************
// Write data to the I2C device
//***************************************************************************
  
JNIEXPORT jint JNICALL Java_com_axoninstruments_gpio_axonI2C_write(JNIEnv *env, jobject obj, jint fileHander,
                                           jint slaveAddr, jint mode, jintArray bufArr, jint len)
{
    jint *bufInt;
    char *bufByte;
    int res = 0, i = 0, j = 0;
    
    if (len <= 0) {
#ifdef DEBUG_ON
        LOGE("I2C: buf len <=0");
#endif
        return -1;
    }
    
    bufInt = (jint *) malloc(len * sizeof(int));
    if (bufInt == 0) {
#ifdef DEBUG_ON
        LOGE("I2C: nomem");
#endif
        return -1;
    }
    bufByte = (char*) malloc(len + 1);
    if (bufByte == 0) {
#ifdef DEBUG_ON
        LOGE("I2C: nomem");
#endif
    	free(bufInt);
	    return -1;     
    }
    
    (*env)->GetIntArrayRegion(env, bufArr, 0, len, bufInt);
    bufByte[0] = mode;
    for (i = 0; i < len; i++)
        bufByte[i] = bufInt[i];      
    
    res = ioctl(fileHander, I2C_SLAVE, slaveAddr);
    if (res != 0) {
#ifdef DEBUG_ON
        LOGE("I2C: Can't set slave address");
#endif
    	free(bufByte);
    	free(bufInt);
	    return -1;     
    }
    
    if ((j = write(fileHander, bufByte, len)) != len) {
#ifdef DEBUG_ON
        LOGE("write fail in i2c");
#endif
	    free(bufByte);
    	free(bufInt);
    	return -1;     
    }
    
#ifdef DEBUG_ON
    LOGI("I2C: write %d byte (%02x)", j, bufByte[0]);
#endif
    free(bufByte);
    free(bufInt);
    
    return j - 1;
}
  
//***************************************************************************
// Close the I2C device
//***************************************************************************

JNIEXPORT void JNICALL Java_com_axoninstruments_gpio_axonI2C_close(JNIEnv *env, jobject obj, jint fileHander)
{
    close(fileHander);
}

//***************************************************************************
// Sets the direction of the GPIO pin
// We expect only GPIO0, GPIO181 etc. No need for the full path as this is
// handled within the function
// Direction = 0 for input
// 			 = 1 for output
//           = 2 for output initially low
//           = 3 for output initially high
//***************************************************************************

JNIEXPORT jint JNICALL Java_com_axoninstruments_gpio_axonI2C_setdirectionGPIO(JNIEnv *env, jobject obj, jstring gpio, jint direction)
{
    char fileName[64];
    char bufByte[10];
    const char *str;
    int j, len;
    int fd;				// File handle

    str = (*env)->GetStringUTFChars(env, gpio, NULL);
    if (str == NULL) {
#ifdef GPIODEBUG_ON
        LOGI("Can't get file name!");
#endif
        return -1;
    }
    sprintf(fileName, "/sys/class/gpio/%s/direction", str);
#ifdef GPIODEBUG_ON
    LOGI("will open GPIO %s", fileName);
#endif
    (*env)->ReleaseStringUTFChars(env, gpio, str);

    fd = open(fileName, O_RDWR);

    if(fd == 0)					// Failed to open?
    {
#ifdef GPIODEBUG_ON
    	LOGI("Failed to open GPIO %s", fileName);
#endif
		return(-2);
	}
	if(direction == 0)			// Input
	{
		strcpy(bufByte, "in");
	}
	else if(direction == 1)		// Output
	{
		strcpy(bufByte, "out");
	}
	else if(direction == 2)		// Output initially LOW
	{
		strcpy(bufByte, "low");
	}
	else						// Output initially HIGH
	{
		strcpy(bufByte, "high");
	}
	len = strlen(bufByte);
    if ((j = write(fd, bufByte, len)) != len)
    {
#ifdef GPIODEBUG_ON
        LOGE("write fail in GPIO direction setting");
#endif
    }
    close(fd);
    return 1;	// Successful
}

//***************************************************************************
// Sets the state of the GPIO pin if this is an output
// We expect only GPIO0, GPIO181 etc. No need for the full path as this is
// handled within the function
//***************************************************************************

JNIEXPORT jint JNICALL Java_com_axoninstruments_gpio_axonI2C_setoutputGPIO(JNIEnv *env, jobject obj, jstring gpio, jint state)
{
    char fileName[64];
    char bufByte[10];
	const char *str;
	int j, len;
    int fd;				// File handle

    str = (*env)->GetStringUTFChars(env, gpio, NULL);
    if (str == NULL) {
#ifdef GPIODEBUG_ON
        LOGI("Can't get file name!");
#endif
        return -1;
    }
    sprintf(fileName, "/sys/class/gpio/%s/value", str);
#ifdef GPIODEBUG_ON
    LOGI("will open GPIO %s", fileName);
#endif
    (*env)->ReleaseStringUTFChars(env, gpio, str);

    fd = open(fileName, O_RDWR);

    if(fd == 0)					// Failed to open?
    {
#ifdef GPIODEBUG_ON
    	LOGI("Failed to open GPIO %s", fileName);
#endif
		return(-2);
	}

	if(state == 0)				// LOW
	{
		strcpy(bufByte, "0");
	}
	else						// HIGH
	{
		strcpy(bufByte, "1");
	}
	len = strlen(bufByte);
    if ((j = write(fd, bufByte, len)) != len)
    {
#ifdef GPIODEBUG_ON
        LOGE("write fail in GPIO output setting");
#endif
    }
    close(fd);
    return 1;	// Successful
}

//***************************************************************************
// Read the state of the GPIO pin if this is an input
// We expect only GPIO0, GPIO181 etc. No need for the full path as this is
// handled within the function
//***************************************************************************

JNIEXPORT jint JNICALL Java_com_axoninstruments_gpio_axonI2C_readGPIO(JNIEnv *env, jobject obj, jstring gpio)
{
    char fileName[64];
    char bufByte[10];
    const char *str;
    int j;
    int fd;				// File handle

    str = (*env)->GetStringUTFChars(env, gpio, NULL);
    if (str == NULL) {
#ifdef GPIODEBUG_ON
        LOGI("Can't get file name!");
#endif
        return -1;
    }
    sprintf(fileName, "/sys/class/gpio/%s/value", str);
#ifdef GPIODEBUG_ON
    LOGI("will open GPIO %s", fileName);
#endif
    (*env)->ReleaseStringUTFChars(env, gpio, str);

    fd = open(fileName, O_RDWR);

    if(fd == 0)					// Failed to open?
    {
#ifdef GPIODEBUG_ON
    	LOGI("Failed to open GPIO %s", fileName);
#endif
		return(-2);
	}
    if ((j = read(fd, bufByte, 9)) == 0)
    {
#ifdef GPIODEBUG_ON
        LOGE("read fail in GPIO input reading");
#endif
    }
    close(fd);
    if(bufByte[0] == '1')
	{
		return 1;				// HIGH
	}
    return 0;					// LOW
}

//***************************************************************************
// Creates the GPIO for us to control
//***************************************************************************

JNIEXPORT jint JNICALL Java_com_axoninstruments_gpio_axonI2C_exportGPIO(JNIEnv *env, jobject obj, jstring gpio)
{
    char fileName[64];
    char bufByte[10];
    const char *str;
    int j, len;
    int fd;				// File handle

    str = (*env)->GetStringUTFChars(env, gpio, NULL);
    if (str == NULL) {
#ifdef GPIODEBUG_ON
        LOGI("Can't get file name!");
#endif
        return -1;
    }
    sprintf(fileName, "/sys/class/gpio/export");
#ifdef GPIODEBUG_ON
    LOGI("will write to export");
#endif
    fd = open(fileName, O_RDWR);

    if(fd == 0)					// Failed to open?
    {
#ifdef GPIODEBUG_ON
        LOGI("Failed to write to export");
#endif
        return(-2);
    }
    strcpy(bufByte, str);

    len = strlen(bufByte);
    if ((j = write(fd, bufByte, len)) != len)
    {
#ifdef GPIODEBUG_ON
        LOGE("write fail in GPIO export setting");
#endif
    }
    close(fd);

    (*env)->ReleaseStringUTFChars(env, gpio, str);

    return 1;	// Successful
}
