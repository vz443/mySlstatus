/* See LICENSE file for copyright and license details. */
#include <time.h>
/* interval between updates (in ms) */
const unsigned int interval = 1000;

/* text to show if no value can be retrieved */
static const char unknown_str[] = "n/a";

/* maximum output string length */
#define MAXLEN 2048


static time_t last_update = 0;

const char *
bitcoin_price(const char *unused)
{
    time_t current_time;
    time(&current_time);

    // Check if more than 3 minutes have passed since the last update
    if ((current_time - last_update) >= 180) { // 180 seconds = 3 minutes
        FILE *fp;
        char buf[128];

        fp = popen("curl -s 'https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd' | jq -r '.bitcoin.usd'", "r");
        if (fp == NULL)
            return NULL;

        if (fgets(buf, sizeof(buf), fp) == NULL) {
            pclose(fp);
            return NULL;
        }

        pclose(fp);

        // Update the last_update time
        last_update = current_time;
    }

    return bprintf(buf);
}
/*
 * function            description                     argument (example)
 *
 * battery_perc        battery percentage              battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_remaining   battery remaining HH:MM         battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_state       battery charging state          battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * cat                 read arbitrary file             path
 * cpu_freq            cpu frequency in MHz            NULL
 * cpu_perc            cpu usage in percent            NULL
 * datetime            date and time                   format string (%F %T)
 * disk_free           free disk space in GB           mountpoint path (/)
 * disk_perc           disk usage in percent           mountpoint path (/)
 * disk_total          total disk space in GB          mountpoint path (/)
 * disk_used           used disk space in GB           mountpoint path (/)
 * entropy             available entropy               NULL
 * gid                 GID of current user             NULL
 * hostname            hostname                        NULL
 * ipv4                IPv4 address                    interface name (eth0)
 * ipv6                IPv6 address                    interface name (eth0)
 * kernel_release      `uname -r`                      NULL
 * keyboard_indicators caps/num lock indicators        format string (c?n?)
 *                                                     see keyboard_indicators.c
 * keymap              layout (variant) of current     NULL
 *                     keymap
 * load_avg            load average                    NULL
 * netspeed_rx         receive network speed           interface name (wlan0)
 * netspeed_tx         transfer network speed          interface name (wlan0)
 * num_files           number of files in a directory  path
 *                                                     (/home/foo/Inbox/cur)
 * ram_free            free memory in GB               NULL
 * ram_perc            memory usage in percent         NULL
 * ram_total           total memory size in GB         NULL
 * ram_used            used memory in GB               NULL
 * run_command         custom shell command            command (echo foo)
 * swap_free           free swap in GB                 NULL
 * swap_perc           swap usage in percent           NULL
 * swap_total          total swap size in GB           NULL
 * swap_used           used swap in GB                 NULL
 * temp                temperature in degree celsius   sensor file
 *                                                     (/sys/class/thermal/...)
 *                                                     NULL on OpenBSD
 *                                                     thermal zone on FreeBSD
 *                                                     (tz0, tz1, etc.)
 * uid                 UID of current user             NULL
 * uptime              system uptime                   NULL
 * username            username of current user        NULL
 * vol_perc            OSS/ALSA volume in percent      mixer file (/dev/mixer)
 *                                                     NULL on OpenBSD/FreeBSD
 * wifi_essid          WiFi ESSID                      interface name (wlan0)
 * wifi_perc           WiFi signal in percent          interface name (wlan0)
 */
static const struct arg args[] = {
	/* function format          argument */
  { ipv4, "[嬨 %s 嬨]   ", "tun0"		},
  { bitcoin_price, "[BTC $%s]   ",  NULL },
  { battery_perc, "[BAT  %s%%]   ", "BAT0" },
  { run_command, "[BRI 󰃞 %s%%]   ", "cat /sys/class/backlight/amdgpu_bl1/brightness"},
  { run_command, "[VOL 墳 %s]   ", "pactl list sinks | grep 'Volume: front-left' | awk '{print $5}'" },
	{ ram_perc, "[RAM  %s%%]   ", NULL	      },
	{ datetime,		 "[ %s ]",           "%b %d %Y, %R" },
};
