# gpio

GPIO JNI driver for Android

To use this driver will need a little work from yoru side

In theory, it should work by simply copying the directory structure to your Android app and the include the following:

import com.axoninstruments.gpio.axonI2C;

In you functions use the following to control the GPIO. Eg, these lines of code control the GPIOC24 on the Nano PC T3 Plus

    axonI2C gpio = new axonI2C();

    gpio.exportGPIO("88");
    gpio.setdirectionGPIO("gpio88", 0);

    State = gpio.readGPIO("gpio88");

See the code for further details.