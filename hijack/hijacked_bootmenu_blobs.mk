# Required tools and blobs for hijackedBootmenu

hijack_root = device/motorola/common/hijack/bootmenu

# binary
PRODUCT_COPY_FILES += \
   ${hijack_root}/binary/adbd:system/bootmenu/binary/adbd \
   ${hijack_root}/binary/adbd.root:system/bootmenu/binary/adbd.root \
   ${hijack_root}/binary/busybox:system/bootmenu/binary/busybox \
   ${hijack_root}/binary/lsof:system/bootmenu/binary/lsof \
   ${hijack_root}/binary/recovery:system/bootmenu/binary/recovery \

# config
PRODUCT_COPY_FILES += \
   ${hijack_root}/config/nvs_map_commands.txt:system/bootmenu/config/nvs_map_commands.txt \
   ${hijack_root}/config/overclock.conf:system/bootmenu/config/overclock.conf \

# images
PRODUCT_COPY_FILES += \
   ${hijack_root}/images/background.png:system/bootmenu/images/background.png \
   ${hijack_root}/images/indeterminate1.png:system/bootmenu/images/indeterminate1.png \
   ${hijack_root}/images/indeterminate2.png:system/bootmenu/images/indeterminate2.png \
   ${hijack_root}/images/indeterminate3.png:system/bootmenu/images/indeterminate3.png \
   ${hijack_root}/images/indeterminate4.png:system/bootmenu/images/indeterminate4.png \
   ${hijack_root}/images/indeterminate5.png:system/bootmenu/images/indeterminate5.png \
   ${hijack_root}/images/indeterminate6.png:system/bootmenu/images/indeterminate6.png \
   ${hijack_root}/images/progress_empty.png:system/bootmenu/images/progress_empty.png \
   ${hijack_root}/images/progress_fill.png:system/bootmenu/images/progress_fill.png \

# recovery
PRODUCT_COPY_FILES += \
   ${hijack_root}/recovery/cwm-recovery.zip:system/bootmenu/recovery/cwm-recovery.zip \
   ${hijack_root}/recovery/recovery.zip:system/bootmenu/recovery/recovery.zip \
   ${hijack_root}/recovery/rzr-recovery.zip:system/bootmenu/recovery/rzr-recovery.zip \
   ${hijack_root}/recovery/update-binary:system/bootmenu/recovery/update-binary \

# script
PRODUCT_COPY_FILES += \
   ${hijack_root}/script/adbd.sh:system/bootmenu/script/adbd.sh \
   ${hijack_root}/script/cdrom.sh:system/bootmenu/script/cdrom.sh \
   ${hijack_root}/script/_config.sh:system/bootmenu/script/_config.sh \
   ${hijack_root}/script/data.sh:system/bootmenu/script/data.sh \
   ${hijack_root}/script/fix_hijack_boot.sh:system/bootmenu/script/fix_hijack_boot.sh \
   ${hijack_root}/script/fix_recovery.sh:system/bootmenu/script/fix_recovery.sh \
   ${hijack_root}/script/nvs_map_creation.sh:system/bootmenu/script/nvs_map_creation.sh \
   ${hijack_root}/script/overclock_backup.sh:system/bootmenu/script/overclock_backup.sh \
   ${hijack_root}/script/overclock_restore.sh:system/bootmenu/script/overclock_restore.sh \
   ${hijack_root}/script/overclock.sh:system/bootmenu/script/overclock.sh \
   ${hijack_root}/script/pre_bootmenu.sh:system/bootmenu/script/pre_bootmenu.sh \
   ${hijack_root}/script/reboot_bootmenu.sh:system/bootmenu/script/reboot_bootmenu.sh \
   ${hijack_root}/script/reboot_cwr.sh:system/bootmenu/script/reboot_cwr.sh \
   ${hijack_root}/script/reboot_rzr.sh:system/bootmenu/script/reboot_rzr.sh \
   ${hijack_root}/script/recovery_cwr.sh:system/bootmenu/script/recovery_cwr.sh \
   ${hijack_root}/script/recovery_rzr.sh:system/bootmenu/script/recovery_rzr.sh \
   ${hijack_root}/script/sdcard.sh:system/bootmenu/script/sdcard.sh \
   ${hijack_root}/script/system.sh:system/bootmenu/script/system.sh \

# Fixes
PRODUCT_COPY_FILES += \
   ${hijack_root}/fixes/09multitouch-2pt:system/bootmenu/fixes/09multitouch-2pt \
   ${hijack_root}/fixes/09multitouch-8pt:system/bootmenu/fixes/09multitouch-8pt

