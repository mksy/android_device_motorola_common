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
#include <dirent.h>

#include "common.h"
#include "overclock.h"
#include "extendedcommands.h"
#include "minui/minui.h"
#include "bootmenu_ui.h"
#include "hijack.h"

#define MAX_NUM_GOVS 32
#define MAX_GOV_LONG_NAME 64
#define MAX_GOV_SHORT_NAME 32

#define USE_4_CLOCK_LEVELS

struct overclock_config
{
 const char *name; //configuration & variable name
 int value; //value, does not apply for scaling
 char gov[255]; //governor, ONLY applies to scaling
};

struct overclock_config overclock[] = {
   { "enable", 1, "" },
   { "scaling", 0, "cpufreq_smartassv2.ko" },
   { "clk1", 300, "" },
   { "clk2", 600, "" },
   { "clk3", 800, "" },
   { "clk4", 1000, "" },
   { "vsel1", 34, "" },
   { "vsel2", 48, "" },
   { "vsel3", 54, "" },
   { "vsel4", 66, "" },
   { NULL, NULL },
};

struct gov_config {
   char *gov_module;
   char *gov_common;
};

struct gov_config govs[31];

int total_gov_count=0;


int get_overclock_value(char* name) {
  struct overclock_config *config;

  for (config = overclock; config->name != NULL; ++config) {
      if (!strcmp(config->name, name)) {
        return config->value;
      }
    }
  return -1;
}

char* get_gov_common(char* name) {
   struct gov_config *config;

   for(config = govs; config->gov_module != NULL; ++config) {
      if (!strcmp(config->gov_module, name)) {
         return config->gov_common;
      }
   }
   return -1;
}

char* get_overclock_gov(char* name) {
   struct overclock_config *config;

   for (config=overclock; config->name != NULL; ++config) {
      if (!strcmp(config->name, name)) {
         return config->gov;
      }
   }
   return -1;
}


int set_overclock_value(char* name, int value) {
  struct overclock_config *config;

  for (config = overclock; config->name != NULL; ++config) {
      if (!strcmp(config->name, name)) {
        config->value = value;
        return config->value;
      }
    }
  return -1;
}

int set_overclock_gov(char* name, char* gov) {
   struct overclock_config *config;

   for (config = overclock; config->name != NULL; ++config) {
      if (!strcmp(config->name, name)) {
         strcpy(config->gov,gov);
         return 0;
      }
   }
   return -1;
}


int get_overclock_config(void) {
  FILE *fp;
  char name[255];
  char gov[255];
  struct overclock_config *config;

  if (check_for_old_config()) {
     update_old_config();
  }

  if ((fp = fopen(FILE_OVERCLOCK_CONF, "r")) == NULL) {
    return 1;
  }

  while((fscanf(fp, "%s", name)) != EOF) {
    for (config = overclock; config->name != NULL; ++config) {
      if (!strcmp(config->name, name)) {
        if (!strcmp(config->name, "scaling"))
           fscanf(fp, "%s", &config->gov);
        else
           fscanf(fp, "%d", &config->value);
      }
    }
  }
  fclose(fp);
  set_overclock_config();
  return 0;
}


int set_overclock_config(void) {
  FILE *fp;
  struct overclock_config *config;

  if ((fp = fopen(FILE_OVERCLOCK_CONF, "w")) == NULL) {
    return 1;
  }

  for (config = overclock; config->name != NULL; ++config) {
    fprintf(fp, "%s", config->name);
    if (!strcmp(config->name,"scaling"))
       fprintf(fp, " %s", config->gov);
    else
       fprintf(fp, " %d", config->value);
    fprintf(fp, "\n");
  }
  fclose(fp);
  return 0;
}


int menu_overclock_status(int intl_value) {

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

int menu_overclock_scaling(void) {

// Build a dynamic read of the available governors here
// instead of having them hard coded

  static char** title_headers = NULL;

  if (title_headers == NULL) {
    char* headers[] = {
      " #" MENU_SYSTEM MENU_OVERCLOCK " Scaling -->",
      "",
      NULL
    };
    title_headers = prepend_title((const char**)headers);
  }
  char * set_gov = get_overclock_gov("scaling");

  for (;;) {

    char* options[total_gov_count + 3];
    int i = 0;
    int mode = 0;

    struct gov_config *ptr;
    char *entry = "   [";
    char *entry2 = "  *[";
    char *end = "]";

    for (ptr = govs; ptr->gov_module != NULL; ptr++) {
       char *final = (char*)malloc(sizeof(char)*MAX_GOV_SHORT_NAME);
       if (ptr->gov_module != NULL) {
          if (!strcmp(set_gov, ptr->gov_module)) {
             final = strcpy(final, entry2);
             mode = i;
          } else
             final = strcpy(final, entry);
          final = strcat(final, ptr->gov_common);
          final = strcat(final, end);
          options[i] = final;
       }
//but not to here
       i++;
    }
          
    options[total_gov_count-1] = "";
    options[total_gov_count] = "   --Go Back.";
    options[total_gov_count + 1] = NULL;

    int chosen_item = get_menu_selection(title_headers, options, 1, mode);

    struct gov_config *ptr2;
    int lcv = 0;

/*
 * I think this needs to be replaced with a select statement to keep it from crashing in this menu
 *
 */
    for (ptr2 = govs, lcv=0; (ptr2->gov_module != NULL) && (lcv < total_gov_count); ++ptr2, lcv++) {
       if(lcv == chosen_item)
          break;
    }

    set_overclock_gov("scaling", ptr2->gov_module);

    return 0;
  }

  return 0;
}


int menu_set_value(char* name, int intl_value, int min_value, int max_value, int step) {

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


int show_menu_overclock(void) {

#define OVERCLOCK_STATUS                  0
#define OVERCLOCK_SCALING                 1
#define OVERCLOCK_CLOCK1                  2
#define OVERCLOCK_CLOCK2                  3
#define OVERCLOCK_CLOCK3                  4
#define OVERCLOCK_CLOCK4                  5
#define OVERCLOCK_VSEL1                   6
#define OVERCLOCK_VSEL2                   7
#define OVERCLOCK_VSEL3                   8
#define OVERCLOCK_VSEL4                   9
#define OVERCLOCK_DEFAULT                10
#define OVERCLOCK_BACKUP                 12
#define OVERCLOCK_RESTORE                13
#define OVERCLOCK_SAVE                   14
#define OVERCLOCK_GOBACK                 15

  static char** title_headers = NULL;
  int select = 0;

  if (title_headers == NULL) {
    char* headers[] = { " #" MENU_SYSTEM MENU_OVERCLOCK,
                        "",
                        NULL };
    title_headers = prepend_title((const char**)headers);
  }

  get_overclock_config();
  char* items[17];
    #define OC_MALLOC_FIRST 1
    items[1] = (char*)malloc(sizeof(char)*64);
    items[2] = (char*)malloc(sizeof(char)*64);
    items[3] = (char*)malloc(sizeof(char)*64);
    items[4] = (char*)malloc(sizeof(char)*64);
    items[5] = (char*)malloc(sizeof(char)*64);
    items[6] = (char*)malloc(sizeof(char)*64);
    items[7] = (char*)malloc(sizeof(char)*64);
    items[8] = (char*)malloc(sizeof(char)*64);
    items[9] = (char*)malloc(sizeof(char)*64);
    #define OC_MALLOC_LAST 9
    items[10] = "  [Set defaults(*req reboot/don't save!!)]";
    items[11] = "";
    items[12] = "  [Backup Settings]";
    items[13] = "  [Restore Settings]";
    items[14] = "  [Save]";
    items[15] = "  --Go Back";
    items[16] = NULL;

  for (;;) {

    switch (get_overclock_value("enable")) {
      case 0: items[0] = "  +Status: [Disable] -->"; break;
      case 1: items[0] = "  +Status: [Enable] -->"; break;

      default: items[0] = "  +Status: [Unknown] -->"; break;
    }

   struct gov_config *ptr;

   char *entry = "  +Scaling: [";
   char *end = "] -->";
   char *final = (char*)malloc(sizeof(char)*64);
   char *mode = get_overclock_gov("scaling");

   total_gov_count = 0;

   for (ptr = govs; ptr->gov_module != NULL; ++ptr) {
      if (!strcmp(mode, ptr->gov_module)) {
         final = strcpy(final, entry);
         final = strcat(final, ptr->gov_common);
         final = strcat(final, end);
      }
      total_gov_count++;
    }

    items[1] = final;
   
    //The following line causes a seg fault.  Is the gov not getting saved correctly in the struct?
    //sprintf(items[1], "  +Scaling: [ %s ] -->", get_gov_common( get_overclock_gov("scaling") ));

    sprintf(items[2], "  +Clk1: [%d] -->", get_overclock_value("clk1"));
    sprintf(items[3], "  +Clk2: [%d] -->", get_overclock_value("clk2"));
    sprintf(items[4], "  +Clk3: [%d] -->", get_overclock_value("clk3"));
#ifdef USE_4_CLOCK_LEVELS
    sprintf(items[5], "  +Clk4: [%d] --> (*req 2.3.3 kernel)", get_overclock_value("clk4"));
    sprintf(items[9], "  +Vsel4: [%d] --> (*req 2.3.3 kernel)", get_overclock_value("vsel4"));
#else
    strcpy(items[5], "  ----------------------");
    strcpy(items[9], "  ----------------------");
#endif
    sprintf(items[6], "  +Vsel1: [%d] -->", get_overclock_value("vsel1"));
    sprintf(items[7], "  +Vsel2: [%d] -->", get_overclock_value("vsel2"));
    sprintf(items[8], "  +Vsel3: [%d] -->", get_overclock_value("vsel3"));

    int chosen_item = get_menu_selection(title_headers, items, 1, select);

    switch (chosen_item) {
      case OVERCLOCK_STATUS:
        set_overclock_value("enable", menu_overclock_status(get_overclock_value("enable"))); break;

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
        get_overclock_config();
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


int check_for_old_config(void) {
   FILE *fp;

   if ((fp = fopen(FILE_OVERCLOCK_CONF, "r")) == NULL) {
      return 1;
   }

   char buffer[255];
   bool found = 0;

   while (fgets(buffer, 254, fp) != NULL) {
      if (strncasecmp("scaling cpufreq",buffer,15) == 0)
         found = 1;
   }

   fclose(fp);

   if (found)
      return 0;

   return 1;
}


int update_old_config(void) {
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
   int old_gov = get_overclock_value("scaling");

   switch (old_gov) {
      case 0:
         set_overclock_gov("scaling", "cpufreq_conservative.ko");
         break;
      case 1:
         set_overclock_gov("scaling", "cpufreq_interactive.ko");
         break;
      case 2:
         set_overclock_gov("scaling", "cpufreq_interactivex.ko");
         break;
      case 3:
         set_overclock_gov("scaling", "cpufreq_ondemand.ko");
         break;
      case 4:
         set_overclock_gov("scaling", "cpufreq_performance.ko");
         break;
      case 5:
         set_overclock_gov("scaling", "cpufreq_powersave.ko");
         break;
      case 6:
         set_overclock_gov("scaling", "cpufreq_smartass.ko");
         break;
      case 7:
         set_overclock_gov("scaling", "cpufreq_smartassv2.ko");
         break;
      case 8:
         set_overclock_gov("scaling", "cpufreq_BOOSTEDassV2.ko");
         break;
      case 9:
         set_overclock_gov("scaling", "cpufreq_userspace.ko");
         break;
      default:
         set_overclock_gov("scaling", "cpufreq_smartassv2.ko");
         break;
   }

   set_overclock_config();

   return 0;
}

int read_governors(void) {
   struct dirent *dp;

   const char *dir_path="/system/lib/modules";
   const char *base="cpufreq_";
   int count = 0; //Number of Governors found

   DIR *dir = opendir(dir_path);

   while ((dp=readdir(dir)) != NULL ) {
      if (strncasecmp(base, dp->d_name, 8) == 0) {
         count++;
      }
   }

   closedir(dir);

   struct gov_config temp[count];
   struct gov_config *ptr = temp;

   dir = opendir(dir_path);
   count = 0;

   while ((dp=readdir(dir)) != NULL) {
      if (strncasecmp(base, dp->d_name, 8) == 0) {
         int i = strlen(dp->d_name);
         char *cname = (char*)malloc(sizeof(char)*(i-10));
         int lcv1, lcv2;
         for (lcv1=8, lcv2=0; lcv1 < (i-3); lcv1++, lcv2++) {
            cname[lcv2] = dp->d_name[lcv1];
         }
         cname[i-11] = '\0';
         if (strcasecmp(cname, "stats") != 0) {
            ptr->gov_module = dp->d_name;
            ptr->gov_common = cname;
            ++ptr;
            count++;
         }
      }
   }

   ptr->gov_module = NULL;
   ptr->gov_common = NULL;

   int lcv, lcv3;
   int gov_count = 0;
   int gov_found = 0;
   int first_gov = 1;
   struct gov_config *ptr2 = temp;

   for (; (ptr2->gov_module != NULL); ++ptr2) {
      gov_found = check_gov(ptr2->gov_module);
      if (!gov_found) {
         add_gov(ptr2->gov_module, ptr2->gov_common);
      }
   }
   add_gov(NULL, NULL);
   print_govs();

   //Print the govs now
   struct gov_config *config;
   // **** Why is the above config defined???
   
   closedir(dir);
   return 0;
}

int print_govs(void) {
   struct gov_config *ptr;
   FILE *fp;

   if ((fp = fopen(FILE_OVERCLOCK_GOVS, "w")) == NULL) {
      return 1;
   }

   total_gov_count = 0;

   for (ptr = govs; ptr->gov_module != NULL; ++ptr) {
      fprintf(fp, "%s %s\n", ptr->gov_module, ptr->gov_common);
      total_gov_count++;
   }
   fclose(fp);
   return 0;
}

int add_gov(char *module, char *common) {
   struct gov_config *ptr;

   char *gov_long_name = calloc(MAX_GOV_LONG_NAME, sizeof(char));
   char *gov_short_name = calloc(MAX_GOV_SHORT_NAME, sizeof(char));

   printf("After calloc\n");

   if(module != NULL) {
      gov_long_name = strdup(module);
      gov_short_name = strdup(common);
   }

   for (ptr=govs; (ptr->gov_module != NULL); ++ptr) {
      //just finding the end
   }
   ptr->gov_module = gov_long_name;
   ptr->gov_common = gov_short_name;
   return 0;
}

int check_gov(char *module) {
   struct gov_config *ptr;

   for (ptr=govs; (ptr->gov_module != NULL); ++ptr) {
      if (strcasecmp(ptr->gov_module, module) == 0)
         return 1;
   }

   return 0;
}


