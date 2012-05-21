#!/system/bootmenu/binary/busybox ash

rm /init.mapphone_cdma.rc
mv /init.mapphone_cdma.rc.new /init.mapphone_cdma.rc
chmod 750 /init.mapphone_cdma.rc

rm /init.mapphone_umts.rc
mv /init.mapphone_umts.rc.new /init.mapphone_umts.rc
chmod 750 /init.mapphone_umts.rc

