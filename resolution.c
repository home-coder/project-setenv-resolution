#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE    16
#define BOOTINI    "./boot.ini"
#define LINE_SIZE  512

enum para{VAL1 = 0, VAL2, VAL3, VAL_MAX};

typedef struct _resolution {
	char mode[BUFSIZE];
	char *res;
}resolution;

resolution lcdfbres[] = {
	{"1920x1080", "lcd,1920x1080M@60,if=RGB24,bpp=32"},
	{"1024x768",  "lcd,1024x768M@60,if=RGB24,bpp=32"},
};

resolution hdmifbres[] = {
	{"1920x1080", "hdmi,1920x1080M@60,bpp=32"},
	{"1024x768",  "hdmi,1024x768M@60,bpp=32"},
};

typedef struct _chosen_res {
	resolution *fbdevres;
	int size;
}chosen_res;

static const char *short_options = "d:r:h";

static const struct option long_options[] = {
	{"fbdev", required_argument, NULL, 'd'},
	{"fbres", required_argument, NULL, 'r'},
	{"help",  no_argument,       NULL, 'h'},
	{ NULL, 0, NULL, 0 },
};

static void print_usage()
{
	fprintf(stdout, "Usage:\n");
	fprintf(stdout, "example 1, ./a.out --fbdev lcd --fbres 1920x1080\n");
	fprintf(stdout, "example 2, ./a.out -d lcd -r 1920x1080\n");

}

static int set_res(const char *fbres, chosen_res *chres, char **resval)
{
	int i;
	for (i = 0; i < chres->size; i++) {
		if (!strcmp(fbres, chres->fbdevres[i].mode)) {
			*resval = chres->fbdevres[i].res;
			return 0;
		}
	}

	return -1;
}

static int fbdev_setres(const char *fbdev, const char *fbres, char **resval)
{
	chosen_res *chres = (chosen_res *)malloc(sizeof(chosen_res));

	if (!strcmp(fbdev, "lcd")) {
		chres->fbdevres = lcdfbres;
		chres->size = sizeof(lcdfbres)/sizeof(lcdfbres[0]);

	} else if (!strcmp(fbdev, "hdmi")) {
		chres->fbdevres = hdmifbres;
		chres->size = sizeof(hdmifbres)/sizeof(hdmifbres[0]);
	} else {
		return -1;
	}

	return set_res(fbres, chres, resval);
}

static void set_res_bootenv(char *fbdev, char *resval)
{
	FILE *fp;
	char linebuf[LINE_SIZE];
	char *key[VAL_MAX];
	char *mid, *end;
	int i;

	if (!(fp = fopen(BOOTINI, "r+"))) {
		fprintf(stderr, "open boot env error\n");
		exit(-1);
	}

	for (i = 0; i < VAL_MAX; i++) {
		key[i] = malloc(sizeof(char) * 128);
	}

	while (fgets(linebuf, LINE_SIZE, fp)) {
		if (!strncmp(linebuf, "bootargs", 8)) {
			if ((mid = strstr(linebuf, fbdev)) != NULL) {
				if ((end = strstr(mid, " ")) != NULL) {
					memcpy(key[VAL3], end, strlen(end) + 1);
					end[1] = '\0';
				}
				memcpy(key[VAL2], mid, strlen(mid));
				mid[0] = '\0';
				key[VAL1] = linebuf;
				printf("%s%s%s\n", key[VAL1], key[VAL2], key[VAL3]);
			}else {
				break;
			}
		}
	}
}

int main(int argc, char **argv)
{
	char *fbdev, *fbres;

	if (argc < 2) {
		print_usage();
		exit(-1);
	}
	int arg;
	while ((arg = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
		switch (arg) {
			case 'd': fbdev = optarg; break;
			case 'r': fbres = optarg; break;
			case 'h': print_usage();  exit(0);
			case '?':
					  //fprintf(stderr, "invalid command argument\n");默认也也也打印出错信息
					  return -1;
		}
	}
	if (fbdev == NULL || fbres == NULL) {
		fprintf(stderr, "fbdev or resolution is null, please do ./a.out --help\n");
		exit(-1);
	} else {
		printf("fbdev:%s, resolution:%s\n", fbdev, fbres);
	}

	char *resval;
	int ret = 0;
	ret = fbdev_setres(fbdev, fbres, &resval);
	if (ret < 0) {
		fprintf(stderr, "fbdev_setres failed\n");
	}
	printf("%s\n", resval);

	/* now, set resval to boot.ini  */
	set_res_bootenv(fbdev, resval);
	printf("over\n");

	return 0;
}
