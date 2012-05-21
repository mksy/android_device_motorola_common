#!/system/bootmenu/binary/busybox ash

cp /system/bootmenu/fixes/power_wake_off/cdma_shadow-keypad.kl /system/usr/keylayout/
chmod 644 /system/usr/keylayout/cdma_shadow-keypad.kl

cp /system/bootmenu/fixes/power_wake_off/qwerty.kl /system/usr/keylayout/
chmod 644 /system/usr/keylayout/qwerty.kl

cp /system/bootmenu/fixes/power_wake_off/sholes-keypad.kl /system/usr/keylayout/
chmod 644 /system/usr/keylayout/sholes-keypad.kl

