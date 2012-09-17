#ifndef __EXTENDEDCOMMANDS_H__
#define __EXTENDEDCOMMANDS_H__

#define BM_ROOTDIR "/system/bootmenu"

#ifndef BOOTMODE_CONFIG_FILE
#define BOOTMODE_CONFIG_FILE "/data/.bootmenu"
#endif

static const char *FILE_PRE_MENU  = BM_ROOTDIR "/script/pre_bootmenu.sh";
static const char *FILE_REPLACE_2ND_INIT   = BM_ROOTDIR "/script/file_replace_2nd_init.sh";
static const char *FILE_REPLACE_STOCK      = BM_ROOTDIR "/script/file_replace_stock.sh";

static const char *FILE_ADBD      = BM_ROOTDIR "/script/adbd.sh";
static const char *FILE_SDCARD    = BM_ROOTDIR "/script/sdcard.sh";
static const char *FILE_CDROM     = BM_ROOTDIR "/script/cdrom.sh";
static const char *FILE_SYSTEM    = BM_ROOTDIR "/script/system.sh";
static const char *FILE_DATA      = BM_ROOTDIR "/script/data.sh";

static const char *FILE_OVERCLOCK       = BM_ROOTDIR "/script/overclock.sh";
static const char *FILE_OVERCLOCK_CONF  = BM_ROOTDIR "/config/overclock.conf";
static const char *FILE_OVERCLOCK_RESTORE = BM_ROOTDIR "/script/overclock_restore.sh";
static const char *FILE_OVERCLOCK_BACKUP  = BM_ROOTDIR "/script/overclock_backup.sh";
static const char *FILE_OVERCLOCK_GOVS    = BM_ROOTDIR "/config/governors.conf";

static const char *FILE_CWR_RECOVERY  = BM_ROOTDIR "/script/recovery_cwr.sh";
static const char *FILE_RZR_RECOVERY  = BM_ROOTDIR "/script/recovery_rzr.sh";
static const char *FILE_BOOTMODE_CLEAN  = BM_ROOTDIR "/script/bootmode_clean.sh";

static const char *FILE_FIX_HIJACK_BOOT  = BM_ROOTDIR "/script/fix_hijack_boot.sh";
static const char *FILE_FIX_RECOVERY     = BM_ROOTDIR "/script/fix_recovery.sh";
static const char *FILE_FIX_WIFI_NVS     = BM_ROOTDIR "/script/nvs_map_creation.sh";
static const char *FILE_FIX_MT_2         = BM_ROOTDIR "/script/fix_touch_2pt.sh";
static const char *FILE_FIX_MT_8         = BM_ROOTDIR "/script/fix_touch_8pt.sh";
static const char *FILE_FIX_PWR_WAKE_ON  = BM_ROOTDIR "/script/fix_power_wake_on.sh";
static const char *FILE_FIX_PWR_WAKE_OFF = BM_ROOTDIR "/script/fix_power_wake_off.sh";
static const char *FILE_FIX_KEYLIGHT_ON  = BM_ROOTDIR "/script/fix_key_lights_on.sh";
static const char *FILE_FIX_KEYLIGHT_OFF = BM_ROOTDIR "/script/fix_key_lights_off.sh";

static const char *FILE_DEFAULTBOOTMODE = BM_ROOTDIR "/config/default_bootmode.conf";
static const char *FILE_BOOTMODE        = BOOTMODE_CONFIG_FILE;
static const char *FILE_BYPASS          = "/data/.bootmenu_bypass";
static const char *FILE_BOOTMENU_CONF   = BM_ROOTDIR "/config/bootmenu.conf";

static const char *SYS_POWER_CONNECTED  = "/sys/class/power_supply/ac/online";
static const char *SYS_USB_CONNECTED    = "/sys/class/power_supply/usb/online";
static const char *SYS_BATTERY_LEVEL    = "/sys/class/power_supply/battery/charge_counter"; // content: 0 to 100

int get_bootmenu_value(char* name);
int set_bootmenu_value(char* name, int value);
int get_bootmenu_config(void);
int set_bootmenu_config(void);

int show_menu_overclock(void);
int show_menu_tools(void);
int show_menu_recovery(void);
int show_menu_fixes(void);
int show_menu_toggle_multitouch(int initial);
int show_menu_toggle_power_wake(int initial);
int show_menu_toggle_key_lights(int initial);

int usb_connected(void);
int adb_started(void);

int set_usb_device_mode(const char *mode);
int mount_usb_storage(const char *part);

int get_bootmode(int clean);
int int_mode(char* mode);

#endif
