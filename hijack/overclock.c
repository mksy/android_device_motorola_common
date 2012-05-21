/*
 * Copyright (C) 2007-2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "overclock.h"
#include "extendedcommands.h"
#include "minui/minui.h"
#include "bootmenu_ui.h"
#include "hijack.h"

#define USE_4_CLOCK_LEVELS

struct overclock_config
{
 const char *name;
 int value;
};

struct overclock_config overclock[] = {
  { "enable", 1 },
  { "load_all", 0 },
  { "scaling", 3 },
  { "clk1", 300 },
  { "clk2", 600 },
  { "clk3", 800 },
  { "clk4", 1000 },
  { "vsel1", 34 },
  { "vsel2", 48 },
  { "vsel3", 54 },
  { "vsel4", 66 },
  { NULL, 0 },
};


int
get_overclock_value(char* name) {
  struct overclock_config *config;

  for (config = overclock; config->name != NULL; ++config) {
      if (!strcmp(config->name, name)) {
        return config->value;
      }
    }
  return -1;
}

int
set_overclock_value(char* name, int value) {
  struct overclock_config *config;

  for (config = overclock; config->name != NULL; ++config) {
      if (!strcmp(config->name, name)) {
        config->value = value;
        return config->value;
      }
    }
  return -1;
}

int
get_overclock_config(void) {
  FILE *fp;
  char name[255];
  struct overclock_config *config;

  if ((fp = fopen(FILE_OVERCLOCK_CONF, "r")) == NULL) {
    return 1;
  }

  while((fscanf(fp, "%s", name)) != EOF) {

    for (config = overclock; config->name != NULL; ++config) {
      if (!strcmp(config->name, name)) {
        fscanf(fp, "%d", &config->value);
      }
    }
  }
  fclose(fp);
  return 0;
}

int
set_overclock_config(void) {
  FILE *fp;
  struct overclock_config *config;

  if ((fp = fopen(FILE_OVERCLOCK_CONF, "w")) == NULL) {
    return 1;
  }

  for (config = overclock; config->name != NULL; ++config) {
    fprintf(fp, "%s", config->name);
    fprintf(fp, " %d\n", config->value);
  }
  fclose(fp);
  return 0;
}


int
menu_overclock_status(int intl_value) {

#define OVERCLOCK_STATUS_DISABLE      0
#define OVERCLOCK_STATUS_ENABLE       1

  static char** title_headers = NULL;

  if (title_headers == NULL) {
    char* headers[] = { " # --> Set Enable/Disable -->",
                        "",
                        NULL };
    title_headers = prepend_title((const char**)headers);
  }

  char* items[2][2] =  {
                         { "  *[Disable]", "   [Disable]" },
                         { "  *[Enable]", "   [Enable]" },
                       };

  int mode = intl_value;

  for (;;) {
    char* options[4];
    int i;

    for (i = 0; i < 2; ++i) {
      if (mode == i)
        options[i] = items[i][0];
      else
        options[i] = items[i][1];
    }

    options[2] = "   --Go Back.";
    options[3] = NULL;

    int chosen_item = get_menu_selection(title_headers, options, 1, mode);

    switch (chosen_item) {
      case OVERCLOCK_STATUS_ENABLE:
        mode = 1;
        break;

      case OVERCLOCK_STATUS_DISABLE:
        mode = 0;
        break;

      default:
        return mode;
    }
  }

  return mode;
}

#if STOCK_VERSION
# define MENU_SYSTEM " System -->"
# define MENU_OVERCLOCK " Overclock -->"
#else
# define MENU_SYSTEM ""
# define MENU_OVERCLOCK " CPU settings -->"
#endif

int
menu_overclock_scaling(void) {

// Build a dynamic read of the available governors here
// instead of having them hard coded

#define OVERCLOCK_SCALING_Conservative   0
#define OVERCLOCK_SCALING_Interactive    1
#define OVERCLOCK_SCALING_InteractiveX   2
#define OVERCLOCK_SCALING_Ondemand       3
#define OVERCLOCK_SCALING_Performance    4
#define OVERCLOCK_SCALING_Powersave      5
#define OVERCLOCK_SCALING_Smartass       6
#define OVERCLOCK_SCALING_Smartass2      7
#define OVERCLOCK_SCALING_BOOSTEDass2    8
#define OVERCLOCK_SCALING_Userspace      9

  static char** title_headers = NULL;

  if (title_headers == NULL) {
    char* headers[] = {
      " #" MENU_SYSTEM MENU_OVERCLOCK " Scaling -->",
      "",
      NULL
    };
    title_headers = prepend_title((const char**)headers);
  }

  char* items[10][2] = {
    { "  *[Conservative]", "   [Conservative]" },
    { "  *[Interactive]",  "   [Interactive]" },
    { "  *[InteractiveX]", "   [InteractiveX]" },
    { "  *[Ondemand]",     "   [Ondemand]" },
    { "  *[Performance]",  "   [Performance]" },
    { "  *[Powersave]",    "   [Powersave]" },
    { "  *[Smartass]",     "   [Smartass]" },
    { "  *[SmartassV2]",   "   [SmartassV2]" },
    { "  *[BOOSTEDassV2]", "   [BOOSTEDassV2]" },
    { "  *[Userspace]",    "   [Userspace]" },
  };

  for (;;) {

    char* options[13];
    int i;
    int mode = get_overclock_value("scaling");

    for (i = 0; i < 10; ++i) {
      if (mode == i)
        options[i] = items[i][0];
      else
        options[i] = items[i][1];
    }
    options[10] = "";
    options[11] = "   --Go Back.";
    options[12] = NULL;

    int chosen_item = get_menu_selection(title_headers, options, 1, mode);

    switch (chosen_item) {
      case OVERCLOCK_SCALING_Conservative:
        set_overclock_value("scaling", 0);
        ui_print("Governer set to Conservative.\n");
        break;

      case OVERCLOCK_SCALING_Interactive:
        set_overclock_value("scaling", 1);
        ui_print("Governer set to Interactive.\n");
        break;

      case OVERCLOCK_SCALING_InteractiveX:
        set_overclock_value("scaling", 2);
        ui_print("Governer set to InteractiveX.\n");
        break;

      case OVERCLOCK_SCALING_Ondemand:
        set_overclock_value("scaling", 3);
        ui_print("Governer set to Ondemand.\n");
        break;

      case OVERCLOCK_SCALING_Performance:
        set_overclock_value("scaling", 4);
        ui_print("Governer set to Performance.\n");
        break;

      case OVERCLOCK_SCALING_Powersave:
        set_overclock_value("scaling", 5);
        ui_print("Governer set to Powersave.\n");
        break;

      case OVERCLOCK_SCALING_Smartass:
        set_overclock_value("scaling", 6);
        ui_print("Governer set to Smartass.\n");
        break;

      case OVERCLOCK_SCALING_Smartass2:
        set_overclock_value("scaling", 7);
        ui_print("Governer set to SmartassV2.\n");
        break;

      case OVERCLOCK_SCALING_BOOSTEDass2:
        set_overclock_value("scaling", 8);
        ui_print("Governer set to BOOSTEDassV2.\n");
        break;

      case OVERCLOCK_SCALING_Userspace:
        set_overclock_value("scaling", 9);
        ui_print("Governer set to Userspace.\n");
        break;

      default:
        return 0;
    }
  }

  return 0;
}


int
menu_set_value(char* name, int intl_value, int min_value, int max_value, int step) {

#define SETVALUE_TITLE     0
#define SETVALUE_SEP       1
#define SETVALUE_ADD       2
#define SETVALUE_SUB       3
#define SETVALUE_BACK      4

  static char** title_headers = NULL;
  int select = 0;

  if (title_headers == NULL) {
    char* headers[] = { " # --> Set Value -->",
                        "",
                        NULL };
    title_headers = prepend_title((const char**)headers);
  }

  char* items[6];
    items[0] = (char*)malloc(sizeof(char)*64);
    items[1] = "  ----------------------";
    items[2] = (char*)malloc(sizeof(char)*64);
    items[3] = (char*)malloc(sizeof(char)*64);
    items[4] = "  --Go Back";
    items[5] = NULL;

  int value = intl_value;

  for (;;) {
    if (value < min_value) value = min_value;
    if (value > max_value) value = max_value;

    sprintf(items[0], "  %s: [%d]", name, value);
    sprintf(items[2], "  [+%d %s]", step, name);
    sprintf(items[3], "  [-%d %s]", step, name);

    int chosen_item = get_menu_selection(title_headers, items, 1, select);

    switch (chosen_item) {
      case SETVALUE_ADD:
        value += step; break;
      case SETVALUE_SUB:
        value -= step; break;

      case SETVALUE_BACK:
        free(items[0]); free(items[2]); free(items[3]);
        return value;

      default:
        break;
    }
    select = chosen_item;
  }

  free(items[0]); free(items[2]); free(items[3]);
  return value;
}

int
show_menu_overclock(void) {

#define OVERCLOCK_STATUS                  0
#define OVERCLOCK_LOAD_ALL                1
#define OVERCLOCK_SCALING                 2
#define OVERCLOCK_CLOCK1                  3
#define OVERCLOCK_CLOCK2                  4
#define OVERCLOCK_CLOCK3                  5
#define OVERCLOCK_CLOCK4                  6
#define OVERCLOCK_VSEL1                   7
#define OVERCLOCK_VSEL2                   8
#define OVERCLOCK_VSEL3                   9
#define OVERCLOCK_VSEL4                  10
#define OVERCLOCK_DEFAULT                11
#define OVERCLOCK_BACKUP                 13
#define OVERCLOCK_RESTORE                14
#define OVERCLOCK_SAVE                   15
#define OVERCLOCK_GOBACK                 16

  static char** title_headers = NULL;
  int select = 0;

  if (title_headers == NULL) {
    char* headers[] = { " #" MENU_SYSTEM MENU_OVERCLOCK,
                        "",
                        NULL };
    title_headers = prepend_title((const char**)headers);
  }

  get_overclock_config();
  char* items[18];
    #define OC_MALLOC_FIRST 3
    items[3] = (char*)malloc(sizeof(char)*64);
    items[4] = (char*)malloc(sizeof(char)*64);
    items[5] = (char*)malloc(sizeof(char)*64);
    items[6] = (char*)malloc(sizeof(char)*64);
    items[7] = (char*)malloc(sizeof(char)*64);
    items[8] = (char*)malloc(sizeof(char)*64);
    items[9] = (char*)malloc(sizeof(char)*64);
    items[10] = (char*)malloc(sizeof(char)*64);
    #define OC_MALLOC_LAST 10
    items[11] = "  [Set defaults(*req reboot/don't save!!)]";
    items[12] = "";
    items[13] = "  [Backup Settings]";
    items[14] = "  [Restore Settings]";
    items[15] = "  [Save]";
    items[16] = "  --Go Back";
    items[17] = NULL;

  for (;;) {

    switch (get_overclock_value("enable")) {
      case 0: items[0] = "  +Status: [Disable] -->"; break;
      case 1: items[0] = "  +Status: [Enable] -->"; break;

      default: items[0] = "  +Status: [Unknown] -->"; break;
    }

    switch (get_overclock_value("load_all")) {
      case 0: items[1] = "  +Load all modules: [Disable] -->"; break;
      case 1: items[1] = "  +Load all modules: [Enable] -->"; break;

      default: items[1] = "  +Load all modules: [Unknown] -->"; break;
    }

    switch (get_overclock_value("scaling")) {
      case 0: items[2] = "  +Scaling: [Conservative] -->"; break;
      case 1: items[2] = "  +Scaling: [Interactive] -->"; break;
      case 2: items[2] = "  +Scaling: [InteractiveX] -->"; break;
      case 3: items[2] = "  +Scaling: [Ondemand] -->"; break;
      case 4: items[2] = "  +Scaling: [Performance] -->"; break;
      case 5: items[2] = "  +Scaling: [Powersave] -->"; break;
      case 6: items[2] = "  +Scaling: [Smartass] -->"; break;
      case 7: items[2] = "  +Scaling: [SmartassV2] -->"; break;
      case 8: items[2] = "  +Scaling: [BOOSTEDassV2] -->"; break;
      case 9: items[2] = "  +Scaling: [Userspace] -->"; break;

      default: items[2] = "  +Scaling: [Unknown] -->"; break;
    }
    
    sprintf(items[3], "  +Clk1: [%d] -->", get_overclock_value("clk1"));
    sprintf(items[4], "  +Clk2: [%d] -->", get_overclock_value("clk2"));
    sprintf(items[5], "  +Clk3: [%d] -->", get_overclock_value("clk3"));
#ifdef USE_4_CLOCK_LEVELS
    sprintf(items[6], "  +Clk4: [%d] --> (*req 2.3.3 kernel)", get_overclock_value("clk4"));
    sprintf(items[10], "  +Vsel4: [%d] --> (*req 2.3.3 kernel)", get_overclock_value("vsel4"));
#else
    strcpy(items[6], "  ----------------------");
    strcpy(items[10], "  ----------------------");
#endif
    sprintf(items[7], "  +Vsel1: [%d] -->", get_overclock_value("vsel1"));
    sprintf(items[8], "  +Vsel2: [%d] -->", get_overclock_value("vsel2"));
    sprintf(items[9], "  +Vsel3: [%d] -->", get_overclock_value("vsel3"));

    int chosen_item = get_menu_selection(title_headers, items, 1, select);

    switch (chosen_item) {
      case OVERCLOCK_STATUS:
        set_overclock_value("enable", menu_overclock_status(get_overclock_value("enable"))); break;

      case OVERCLOCK_LOAD_ALL:
        set_overclock_value("load_all", menu_overclock_status(get_overclock_value("load_all"))); break;

      case OVERCLOCK_SCALING:
        menu_overclock_scaling(); break;

      case OVERCLOCK_CLOCK1:
        set_overclock_value("clk1", menu_set_value("Clk1", get_overclock_value("clk1"), 200, 2000, 10)); break;

      case OVERCLOCK_CLOCK2:
        set_overclock_value("clk2", menu_set_value("Clk2", get_overclock_value("clk2"), 200, 2000, 10)); break;

      case OVERCLOCK_CLOCK3:
        set_overclock_value("clk3", menu_set_value("Clk3", get_overclock_value("clk3"), 200, 2000, 10)); break;

#ifdef USE_4_CLOCK_LEVELS
      case OVERCLOCK_CLOCK4:
        set_overclock_value("clk4", menu_set_value("Clk4", get_overclock_value("clk4"), 200, 2000, 10)); break;

      case OVERCLOCK_VSEL4:
        set_overclock_value("vsel4", menu_set_value("Vsel4", get_overclock_value("vsel4"), 10, 100, 1)); break;
#endif

      case OVERCLOCK_VSEL1:
        set_overclock_value("vsel1", menu_set_value("Vsel1", get_overclock_value("vsel1"), 10, 100, 1)); break;

      case OVERCLOCK_VSEL2:
        set_overclock_value("vsel2", menu_set_value("Vsel2", get_overclock_value("vsel2"), 10, 100, 1)); break;

      case OVERCLOCK_VSEL3:
        set_overclock_value("vsel3", menu_set_value("Vsel3", get_overclock_value("vsel3"), 10, 100, 1)); break;

      case OVERCLOCK_BACKUP:
        ui_print("Saving config.... ");
        exec_script("/system/bin/hijack", FILE_OVERCLOCK_BACKUP);
        ui_print("Done.\n");
        break;

      case OVERCLOCK_RESTORE:
        ui_print("Restoring config.... ");
        set_overclock_config();
        exec_script("/system/bin/hijack", FILE_OVERCLOCK_RESTORE);
        ui_print("Done.\n");
        break;

      case OVERCLOCK_SAVE:
        ui_print("Saving.... ");
        set_overclock_config();
        exec_script("/system/bin/hijack", FILE_OVERCLOCK);
        ui_print("Done.\n");
        break;

      case OVERCLOCK_DEFAULT:
        ui_print("Set defaults...");
        remove(FILE_OVERCLOCK_CONF);
        ui_print("Done. Please reboot.\n");
        break;

      default:
        return 0;
    }
    select = chosen_item;
  }

  //release mallocs
  for (select=OC_MALLOC_FIRST; select<=OC_MALLOC_LAST; select++) {
    free(items[select]);
  }

  return 0;
}
