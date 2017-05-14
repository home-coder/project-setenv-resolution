#include <stdio.h>
#include <getopt.h>

#define BUFSIZE    16

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

static const char *short_options = "d:r:";

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
	fprintf(stdout, "example 2, ./a.out -f lcd -r 1920x1080\n");

}

int main(int argc, char **argv)
{
	char *fbdev, *fbres;

	int arg;
	while ((arg = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
		switch (arg) {
			case 'd': fbdev = optarg; break;
			case 'r': fbres = optarg; break;
			case 'h': print_usage();  break;
			case '?':
					  perror("invalid command argument\n");
					  return -1;
		}
	}
	printf("fbdev:%s, resolution:%s\n", fbdev, fbres);

	return 0;
}
