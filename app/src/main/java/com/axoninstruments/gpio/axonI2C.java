package com.axoninstruments.gpio;
/**
 * This is an I2C operation class
 */
public class axonI2C {
    /**
     * @param nodeName
     *            node path name
     * @return return file hander else return <0 on fail
     */
    public native int open(String nodeName);

    /**
     * @param fileHander
     * @param i2c_adr
     *            slave addr
     * @param buf
     * @param Length
     *            of buf
     * @return read length
     */
    public native int read(int fileHander, int i2c_adr, int buf[], int Length);

    /**
     * @param fileHander
     * @param i2c_adr
     *            slave addr
     * @param sub_adr
     *            sub addr
     * @param buf
     * @param Length
     *            of buf
     * @return write length
     */
    public native int write(int fileHander, int i2c_adr, int sub_adr,
            int buf[], int Length);

    public native void close(int fileHander);

    //
    // GPIO handler
    //
    /**
     * @param nodeName
     *            node path name
     * @param State
     * 			  state of output 0,1,2 or 3
     * @return return file hander else return <0 on fail
     */
    public native int setdirectionGPIO(String nodeName, int State);

    /**
     * @param nodeName
     * @return read GPIO state
     */
    public native int readGPIO(String nodeName);

    /**
     * @param nodeName
     *            node path name
     * @param State
     * 			  state of output 0,1 for LOW or HIGH
     * @return 1 for OK, 0 or -neg for fail
     */
    public native int setoutputGPIO(String nodeName, int State);

    static {
        System.loadLibrary("axoni2c");
    }

    //
    // GPIO handler
    //
    /**
     * @param nodeName
     *            node path name
     * @return return file hander else return <0 on fail
     */
    public native int exportGPIO(String nodeName);
}
