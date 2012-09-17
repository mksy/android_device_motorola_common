#ifndef __OVERCLOCK_H__
#define __OVERCLOCK_H__

int get_overclock_value(char* name);

int set_overclock_value(char* name, int value);

int get_overclock_config(void);

int set_overclock_config(void);

int menu_overclock_status(int intl_value);

int menu_overclock_scaling(void);

int menu_overclock_clock(int mode);

int menu_overclock_scale(char* mode);

int show_menu_overclock(void);

int read_governors(void);

int check_for_old_config(void);

int update_old_config(void);

int read_governors(void);

int print_govs(void);

int add_gov(char* module, char* common);

int check_gov(char* module);

char* get_gov_common(char* name);

char* get_overclock_gov(char* name);

int set_overclock_gov(char* name, char* gov);

#endif
