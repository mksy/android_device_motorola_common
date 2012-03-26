# Required tools and blobs for hijackedBootmenu

# binary
PRODUCT_COPY_FILES += \
   ./binary/adbd:system/bootmenu/binary/adbd \
   ./binary/adbd.root:system/bootmenu/binary/adbd.root \
   ./binary/busybox:system/bootmenu/binary/busybox \
   ./binary/lsof:system/bootmenu/binary/lsof \
   ./binary/recovery:system/bootmenu/binary/recovery \

# config
PRODUCT_COPY_FILES += \
   ./config/nvs_map_commands.txt:system/bootmenu/config/nvs_map_commands.txt \
   ./config/overclock.conf:system/bootmenu/config/overclock.conf \

# images
   ./images/background.png:system/bootmenu/images/background.png \
   ./images/indeterminate1.png:system/bootmenu/images/indeterminate1.png \
   ./images/indeterminate2.png:system/bootmenu/images/indeterminate2.png \
   ./images/indeterminate3.png:system/bootmenu/images/indeterminate3.png \
   ./images/indeterminate4.png:system/bootmenu/images/indeterminate4.png \
   ./images/indeterminate5.png:system/bootmenu/images/indeterminate5.png \
   ./images/indeterminate6.png:system/bootmenu/images/indeterminate6.png \
   ./images/progress_empty.png:system/bootmenu/images/progress_empty.png \
   ./images/progress_fill.png:system/bootmenu/images/progress_fill.png \

# recovery
   ./recovery/cwm-recovery.zip:system/bootmenu/recovery/cwm-recovery.zip \
   ./recovery/recovery.zip:system/bootmenu/recovery/recovery.zip \
   ./recovery/rzr-recovery.zip:system/bootmenu/recovery/rzr-recovery.zip \
   ./recovery/update-binary:system/bootmenu/recovery/update-binary \

# script
   ./script/adbd.sh:system/bootmenu/script/adbd.sh \
   ./script/cdrom.sh:system/bootmenu/script/cdrom.sh \
   ./script/_config.sh:system/bootmenu/script/_config.sh \
   ./script/data.sh:system/bootmenu/script/data.sh \
   ./script/fix_hijack_boot.sh:system/bootmenu/script/fix_hijack_boot.sh \
   ./script/fix_recovery.sh:system/bootmenu/script/fix_recovery.sh \
   ./script/nvs_map_creation.sh:system/bootmenu/script/nvs_map_creation.sh \
   ./script/overclock_backup.sh:system/bootmenu/script/overclock_backup.sh \
   ./script/overclock_restore.sh:system/bootmenu/script/overclock_restore.sh \
   ./script/overclock.sh:system/bootmenu/script/overclock.sh \
   ./script/pre_bootmenu.sh:system/bootmenu/script/pre_bootmenu.sh \
   ./script/reboot_bootmenu.sh:system/bootmenu/script/reboot_bootmenu.sh \
   ./script/reboot_cwr.sh:system/bootmenu/script/reboot_cwr.sh \
   ./script/reboot_rzr.sh:system/bootmenu/script/reboot_rzr.sh \
   ./script/recovery_cwr.sh:system/bootmenu/script/recovery_cwr.sh \
   ./script/recovery_rzr.sh:system/bootmenu/script/recovery_rzr.sh \
   ./script/sdcard.sh:system/bootmenu/script/sdcard.sh \
   ./script/system.sh:system/bootmenu/script/system.sh \


