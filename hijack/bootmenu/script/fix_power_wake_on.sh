#!/system/bootmenu/binary/busybox ash

cp /system/bootmenu/fixes/power_wake_on/cdma_shadow-keypad.kl /system/usr/keylayout/cdma_shadow-keypad.kl
chmod 644 /system/usr/keylayout/cdma_shadow-keypad.kl

cp /system/bootmenu/fixes/power_wake_on/qwerty.kl /system/usr/keylayout/qwerty.kl
chmod 644 /system/usr/keylayout/qwerty.kl

cp /system/bootmenu/fixes/power_wake_on/sholes-keypad.kl /system/usr/keylayout/sholes-keypad.kl
chmod 644 /system/usr/keylayout/sholes-keypad.kl

