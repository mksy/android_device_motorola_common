#!/system/bootmenu/binary/busybox ash

wlan_cu -itiwlan0 -b < /system/bootmenu/config/nvs_map_commands.txt

cp /data/misc/wifi/nvs_map.bin /pds/wifi/
mv /data/misc/wifi/nvs_map.bin /system/etc/wifi/

