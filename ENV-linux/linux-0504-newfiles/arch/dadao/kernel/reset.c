#include <linux/reboot.h>
#include <linux/pm.h>

void (*pm_power_off)(void) = NULL;
EXPORT_SYMBOL(pm_power_off);

void machine_restart(char *cmd)
{
	do_kernel_restart(cmd);
	while (1);
}

void machine_halt(void)
{
	while (1);
}

void machine_power_off(void)
{
	while (1);
}
