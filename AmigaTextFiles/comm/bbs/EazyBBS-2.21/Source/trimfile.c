#include <stdlib.h>
#include <stdio.h>
#include <string.h>

main (int argc, char *argv[])
{
	FILE *fp;
	char *p;
	char *x;
	long size;
	long lines;
	long maxlines = 200L;
	int arg = 1;

	if (sizeof (int) < 4) {
		fprintf (stderr, "sorry, program needs at least 4-byte-integers\n");
		exit (20);
	}

	if (argc < 2) {
		fprintf (stderr, "usage: trimfile [-l <maxlines>] <filename> ...\n");
		exit (5);
	}

	if (!strcmp (argv[1], "-l") && argc > 3) {
		maxlines = atol (argv[2]);
		arg += 2;
	}

	for (; arg < argc; arg++) {
		if (!(fp = fopen (argv[arg], "r"))) {
			fprintf (stderr, "trimfile: cannot open file %s for reading\n", argv[arg]);
			continue;
		}

		fseek (fp, 0L, 2);
		if (size = ftell (fp)) {
			if (!(p = malloc (size))) {
				fprintf (stderr, "trimfile: cannot allocate %ld bytes for %s\n", size, argv[arg]);
				fclose (fp);
				continue;
			}

			rewind (fp);
			if (fread (p, 1, size, fp) != size) {
				fprintf (stderr, "trimfile: cannot read %ld bytes from %s\n", size, argv[arg]);
				free (p);
				fclose (fp);
				continue;
			}
			fclose (fp);

			for (lines = 0L, x = p + size - 1; x >= p && lines <= maxlines; x--) {
				if (*x == '\n')
					lines++;
			}
			x++;

			if (*x == '\n' && x != p)
				x++;

			if (!(fp = fopen (argv[arg], "w"))) {
				fprintf (stderr, "trimfile: cannot open file %s for writing\n", argv[arg]);
				free (p);
				continue;
			}

			if (fwrite (x, 1, p + size - x, fp) != p + size - x) {
				fprintf (stderr, "trimfile: cannot read %ld bytes from %s\n", size, argv[arg]);
				free (p);
				fclose (fp);
				continue;
			}

			fclose (fp);
			free (p);
		}
		else
			fclose (fp);
	}
}
