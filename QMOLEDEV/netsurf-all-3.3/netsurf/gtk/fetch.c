/*
 * Copyright 2014 vincent Sanders <vince@netsurf-browser.org>
 *
 * This file is part of NetSurf, http://www.netsurf-browser.org/
 *
 * NetSurf is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * NetSurf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>

#include "utils/hashtable.h"
#include "utils/log.h"
#include "utils/filepath.h"
#include "utils/file.h"
#include "desktop/gui_fetch.h"

#include "gtk/gui.h"
#include "gtk/fetch.h"

static struct hash_table *mime_hash = NULL;

void gtk_fetch_filetype_init(const char *mimefile)
{
	struct stat statbuf;
	FILE *fh = NULL;

	mime_hash = hash_create(117);

	/* first, check to see if /etc/mime.types in preference */

	if ((stat("/etc/mime.types", &statbuf) == 0) &&
	    S_ISREG(statbuf.st_mode)) {
		mimefile = "/etc/mime.types";
	}

	fh = fopen(mimefile, "r");

	/* Some OSes (mentioning no Solarises) have a worthlessly tiny
	 * /etc/mime.types that don't include essential things, so we
	 * pre-seed our hash with the essentials.  These will get
	 * over-ridden if they are mentioned in the mime.types file.
	 */

	hash_add(mime_hash, "css", "text/css");
	hash_add(mime_hash, "htm", "text/html");
	hash_add(mime_hash, "html", "text/html");
	hash_add(mime_hash, "jpg", "image/jpeg");
	hash_add(mime_hash, "jpeg", "image/jpeg");
	hash_add(mime_hash, "gif", "image/gif");
	hash_add(mime_hash, "png", "image/png");
	hash_add(mime_hash, "jng", "image/jng");
	hash_add(mime_hash, "mng", "image/mng");
	hash_add(mime_hash, "webp", "image/webp");
	hash_add(mime_hash, "spr", "image/x-riscos-sprite");

	if (fh == NULL) {
		LOG(("Unable to open a mime.types file, so using a minimal one for you."));
		return;
	}

	while (!feof(fh)) {
		char line[256], *ptr, *type, *ext;

		if (fgets(line, 256, fh) == NULL)
			break;

		if (!feof(fh) && line[0] != '#') {
			ptr = line;

			/* search for the first non-whitespace character */
			while (isspace(*ptr)) {
				ptr++;
			}

			/* is this line empty other than leading whitespace? */
			if (*ptr == '\n' || *ptr == '\0') {
				continue;
			}

			type = ptr;

			/* search for the first non-whitespace char or NUL or
			 * NL */
			while (*ptr && (!isspace(*ptr)) && *ptr != '\n') {
				ptr++;
			}

			if (*ptr == '\0' || *ptr == '\n') {
				/* this mimetype has no extensions - read next
				 * line.
				 */
				continue;
			}

			*ptr++ = '\0';

			/* search for the first non-whitespace character which
			 * will be the first filename extenion */
			while (isspace(*ptr)) {
				ptr++;
			}

			while(true) {
				ext = ptr;

				/* search for the first whitespace char or
				 * NUL or NL which is the end of the ext.
				 */
				while (*ptr &&
				       (!isspace(*ptr)) &&
				       *ptr != '\n') {
					ptr++;
				}

				if (*ptr == '\0' || *ptr == '\n') {
					/* special case for last extension on
					 * the line
					 */
					*ptr = '\0';
					hash_add(mime_hash, ext, type);
					break;
				}

				*ptr++ = '\0';
				hash_add(mime_hash, ext, type);

				/* search for the first non-whitespace char or
				 * NUL or NL, to find start of next ext.
				 */
				while (*ptr &&
				       (isspace(*ptr)) &&
				       *ptr != '\n') {
					ptr++;
				}
			}
		}
	}

	fclose(fh);
}

void gtk_fetch_filetype_fin(void)
{
	hash_destroy(mime_hash);
}

const char *fetch_filetype(const char *unix_path)
{
	struct stat statbuf;
	char *ext;
	const char *ptr;
	char *lowerchar;
	const char *type;
	int l;

	if (stat(unix_path, &statbuf) != 0) {
		/* stat failed */
		return "text/plain";
	}

	if (S_ISDIR(statbuf.st_mode)) {
		return "application/x-netsurf-directory";
	}

	l = strlen(unix_path);

	/* Hacky RISC OS compatibility */
	if ((3 < l) && (strcasecmp(unix_path + l - 4, ",f79") == 0)) {
		return "text/css";
	} else if ((3 < l) && (strcasecmp(unix_path + l - 4, ",faf") == 0)) {
		return "text/html";
	} else if ((3 < l) && (strcasecmp(unix_path + l - 4, ",b60") == 0)) {
		return "image/png";
	} else if ((3 < l) && (strcasecmp(unix_path + l - 4, ",ff9") == 0)) {
		return "image/x-riscos-sprite";
	}

	if (strchr(unix_path, '.') == NULL) {
		/* no extension anywhere! */
		return "text/plain";
	}

	ptr = unix_path + strlen(unix_path);
	while (*ptr != '.' && *ptr != '/') {
		ptr--;
	}

	if (*ptr != '.') {
		return "text/plain";
	}

	ext = strdup(ptr + 1);	/* skip the . */

	/* the hash table only contains lower-case versions - make sure this
	 * copy is lower case too.
	 */
	lowerchar = ext;
	while (*lowerchar) {
		*lowerchar = tolower(*lowerchar);
		lowerchar++;
	}

	type = hash_get(mime_hash, ext);
	free(ext);

	if (type == NULL) {
		type = "text/plain";
	}

	return type;
}


static nsurl *gui_get_resource_url(const char *path)
{
	char buf[PATH_MAX];
	nsurl *url = NULL;

	/* default.css -> gtkdefault.css */
	if (strcmp(path, "default.css") == 0) {
		path = "gtkdefault.css";
	}

	/* favicon.ico -> favicon.png */
	if (strcmp(path, "favicon.ico") == 0) {
		path = "favicon.png";
	}

	netsurf_path_to_nsurl(filepath_sfind(respaths, buf, path), &url);

	return url;
}

static struct gui_fetch_table fetch_table = {
	.filetype = fetch_filetype,

	.get_resource_url = gui_get_resource_url,
};

struct gui_fetch_table *nsgtk_fetch_table = &fetch_table;
