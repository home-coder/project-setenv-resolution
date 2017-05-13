#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define ENV_CONFIG_HEAD \
"ipaddr=192.168.1.103\n"\
"serverip=192.168.1.101\n"\
"netmask=255.255.255.0\n"\
"loadaddr=0x10800000\n"\
"rd_loadaddr=0x11000000\n"\
"netdev=eth0\n"\
"ethprime=FEC0\n"\
"splashimage=0x30000000\n"\
"splashpos=m,m\n"\
"lvds_num=1\n"\
"bootfile=uImage\n"

#define BOOTARGS_SPL_CONST "bootargs=console=ttymxc0,115200 androidboot.console=ttymxc0 vmalloc=400M init=/init "

char* mxcfb0_hdmi_1920_1080 = "video=mxcfb0:dev=hdmi,1920x1080M@60,if=RGB24,bpp=32 ";
char* mxcfb1_off = "video=mxcfb1:off video=mxcfb2:off video=mxcfb3:off fbmem=28M  androidboot.hardware=freescale\n";

#define ENV_CONFIG_TAIL \
"bootcmd=booti mmc3\n"\
"ethact=FEC0"

int main()
{
	char bootargs[256];
	printf("%s\n", ENV_CONFIG_HEAD);
	sprintf(bootargs, "%s%s%s", BOOTARGS_SPL_CONST, mxcfb0_hdmi_1920_1080, mxcfb1_off);
	printf("%s\n", bootargs);
	printf("%s\n", ENV_CONFIG_TAIL);

	int fd = open("boot.ini", O_CREAT | O_RDWR);
	if (fd < 0) {
		printf("===>error!\n");
		return -1;
	}
	int boot_len = sizeof(ENV_CONFIG_HEAD) + sizeof(BOOTARGS_SPL_CONST) + strlen(mxcfb0_hdmi_1920_1080) + strlen(mxcfb1_off) + sizeof(ENV_CONFIG_TAIL) + 64;
	char *boot_buf = malloc(boot_len);
	memset(boot_buf, 0, boot_len);
	memcpy(boot_buf, ENV_CONFIG_HEAD, strlen(ENV_CONFIG_HEAD));
	memcpy(boot_buf+sizeof(ENV_CONFIG_HEAD)-1, BOOTARGS_SPL_CONST, sizeof(BOOTARGS_SPL_CONST));
	memcpy(boot_buf+sizeof(ENV_CONFIG_HEAD)+sizeof(BOOTARGS_SPL_CONST)-2, mxcfb0_hdmi_1920_1080, strlen(mxcfb0_hdmi_1920_1080));
		
	write(fd, boot_buf, boot_len);
	
	return 0;
}
