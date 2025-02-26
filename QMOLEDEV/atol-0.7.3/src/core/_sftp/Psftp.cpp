////////////////////////////////////////////////////////////////////////////
// Atol file manager project <http://atol.sf.net>
//
// This code is licensed under BSD license.See "license.txt" for more details.
//
// File: TOFIX
////////////////////////////////////////////////////////////////////////////

/*
 * psftp.c: front end for PSFTP.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <limits.h>

#define PUTTY_DO_GLOBALS
#include "putty.h"
#include "storage.h"
#include "ssh.h"
#include "sftp.h"
#include "int64.h"

/*
 * Since SFTP is a request-response oriented protocol, it requires
 * no buffer management: when we send data, we stop and wait for an
 * acknowledgement _anyway_, and so we can't possibly overfill our
 * send buffer.
 */

static int psftp_connect(CSshSession &session, char *userhost, char *user, int portnumber);
static int do_sftp_init(CSshSession &session);

/* ----------------------------------------------------------------------
 * Higher-level helper functions used in commands.
 */

/*
 * Attempt to canonify a pathname starting from the pwd. If
 * canonification fails, at least fall back to returning a _valid_
 * pathname (though it may be ugly, eg /home/simon/../foobar).
 */
char *canonify(CSshSession &session, char *name)
{
    char *fullname, *canonname;

	if(!session.pwd)
		return NULL;

    if (name[0] == '/') {
		fullname = dupstr(name);
    } else {
		char *slash;
		if (session.pwd[strlen(session.pwd) - 1] == '/')
			slash = "";
		else
			slash = "/";
		fullname = dupcat(session.pwd, slash, name, NULL);
    }

    canonname = fxp_realpath(session, fullname);

    if (canonname) {
		sfree(fullname);
		return canonname;
    } else {
	/*
	 * Attempt number 2. Some FXP_REALPATH implementations
	 * (glibc-based ones, in particular) require the _whole_
	 * path to point to something that exists, whereas others
	 * (BSD-based) only require all but the last component to
	 * exist. So if the first call failed, we should strip off
	 * everything from the last slash onwards and try again,
	 * then put the final component back on.
	 * 
	 * Special cases:
	 * 
	 *  - if the last component is "/." or "/..", then we don't
	 *    bother trying this because there's no way it can work.
	 * 
	 *  - if the thing actually ends with a "/", we remove it
	 *    before we start. Except if the string is "/" itself
	 *    (although I can't see why we'd have got here if so,
	 *    because surely "/" would have worked the first
	 *    time?), in which case we don't bother.
	 * 
	 *  - if there's no slash in the string at all, give up in
	 *    confusion (we expect at least one because of the way
	 *    we constructed the string).
	 */

	int i;
	char *returnname;

	i = strlen(fullname);
	if (i > 2 && fullname[i - 1] == '/')
	    fullname[--i] = '\0';      /* strip trailing / unless at pos 0 */
	while (i > 0 && fullname[--i] != '/');

	/*
	 * Give up on special cases.
	 */
	if (fullname[i] != '/' ||      /* no slash at all */
	    !strcmp(fullname + i, "/.") ||	/* ends in /. */
	    !strcmp(fullname + i, "/..") ||	/* ends in /.. */
	    !strcmp(fullname, "/")) {
	    return fullname;
	}

	/*
	 * Now i points at the slash. Deal with the final special
	 * case i==0 (ie the whole path was "/nonexistentfile").
	 */
	fullname[i] = '\0';	       /* separate the string */
	if (i == 0) {
	    canonname = fxp_realpath(session, "/");
	} else {
	    canonname = fxp_realpath(session, fullname);
	}

	if (!canonname)
	    return fullname;	       /* even that failed; give up */

	/*
	 * We have a canonical name for all but the last path
	 * component. Concatenate the last component and return.
	 */
	returnname = dupcat(canonname,
			    canonname[strlen(canonname) - 1] ==
			    '/' ? "" : "/", fullname + i + 1, NULL);
	sfree(fullname);
	sfree(canonname);
	return returnname;
    }
}

/*
 * Return a pointer to the portion of str that comes after the last
 * slash (or backslash or colon, if `local' is TRUE).
 */
static char *stripslashes(char *str, int local)
{
    char *p;

    if (local) {
        p = strchr(str, ':');
        if (p) str = p+1;
    }

    p = strrchr(str, '/');
    if (p) str = p+1;

    if (local) {
	p = strrchr(str, '\\');
	if (p) str = p+1;
    }

    return str;
}

/* ----------------------------------------------------------------------
 * Actual sftp commands.
 */
struct sftp_command {
    char **words;
    int nwords, wordssize;
    int (*obey) (CSshSession &session, struct sftp_command *);	/* returns <0 to quit */
};

int sftp_cmd_null(CSshSession &session, struct sftp_command *cmd)
{
    return 1;			       /* success */
}

int sftp_cmd_unknown(CSshSession &session, struct sftp_command *cmd)
{
    printf("psftp: unknown command \"%s\"\n", cmd->words[0]);
    return 0;			       /* failure */
}

int sftp_cmd_quit(CSshSession &session, struct sftp_command *cmd)
{
	//MIRO
	if (session.back != NULL && session.back->socket(session) != NULL) {
			char ch;
			session.back->special(session, TS_EOF);
			sftp_recvdata(session, &ch, 1);
	}
	
	//TOFIX cleanup everything from ssh connection
	random_save_seed();
    sk_cleanup(session);
	session.back = NULL;	//TOFIX we must know somehow that we disconencted

    return -1;
}

/*
 * List a directory. If no arguments are given, list pwd; otherwise
 * list the directory given in words[1].
 */
static int sftp_ls_compare(const void *av, const void *bv)
{
    const struct fxp_name *const *a = (const struct fxp_name *const *) av;
    const struct fxp_name *const *b = (const struct fxp_name *const *) bv;
    return strcmp((*a)->filename, (*b)->filename);
}

int sftp_cmd_ls(CSshSession &session, struct sftp_command *cmd)
{
    struct fxp_handle *dirh;
    struct fxp_names *names, *result;
    struct fxp_name **ournames;
    int nnames, namesize;
    char *dir, *cdir;
    int i;

    if (session.back == NULL) {
		printf("psftp: not connected to a host; use \"open host.name\"\n");
		return 0;
    }

    if (cmd->nwords < 2)
		dir = ".";
    else
		dir = cmd->words[1];

    cdir = canonify(session, dir);
    if (!cdir) {
		printf("%s: %s\n", dir, fxp_error());
		return 0;
    }

    printf("Listing directory %s\n", cdir);//TOFIX logeventf

    dirh = fxp_opendir(session, cdir);
    if (dirh == NULL) {
		printf("Unable to open %s: %s\n", dir, fxp_error());
    } else {
		nnames = namesize = 0;
		ournames = NULL;

		while (1) {

			names = fxp_readdir(session, dirh);
			if (names == NULL) {
				if (fxp_error_type() == SSH_FX_EOF)
					break;
				printf("Reading directory %s: %s\n", dir, fxp_error());
				break;
			}
			if (names->nnames == 0) {
				fxp_free_names(names);
				break;
			}

			if (nnames + names->nnames >= namesize) {
				namesize += names->nnames + 128;
				ournames =
					(struct fxp_name **)srealloc(ournames, namesize * sizeof(*ournames));
			}

			for (i = 0; i < names->nnames; i++)
				ournames[nnames++] = fxp_dup_name(&names->names[i]);

			fxp_free_names(names);
		}
		fxp_close(session, dirh);

		/*
		 * Now we have our filenames. Sort them by actual file
		 * name, and then output the longname parts.
		 */
		qsort(ournames, nnames, sizeof(*ournames), sftp_ls_compare);

		/*
		 * And print them.
		 */
		//for (i = 0; i < nnames; i++) {
		//	printf("%s\n", ournames[i]->longname);
			//fxp_free_name(ournames[i]);
		//}
		//sfree(ournames);
    }

    sfree(cdir);

	//return entire list
	result = (struct fxp_names *)smalloc(sizeof(struct fxp_names));
	result->nnames = nnames;
	result->names  = (struct fxp_name *)ournames;

    //return 1; //
	return (int)result;
}

/*
 * Change directories. We do this by canonifying the new name, then
 * trying to OPENDIR it. Only if that succeeds do we set the new pwd.
 */
int sftp_cmd_cd(CSshSession &session, struct sftp_command *cmd)
{
    struct fxp_handle *dirh;
    char *dir;

    if (session.back == NULL) {
		printf("psftp: not connected to a host; use \"open host.name\"\n");
		return 0;
    }

    if (cmd->nwords < 2)
		dir = dupstr(session.homedir);
    else
		dir = canonify(session, cmd->words[1]);

    if (!dir) {
		printf("%s: %s\n", dir, fxp_error());
		return 0;
    }

    dirh = fxp_opendir(session, dir);
    if (!dirh) {
		printf("Directory %s: %s\n", dir, fxp_error());
		sfree(dir);
		return 0;
    }

    fxp_close(session, dirh);

    sfree(session.pwd);
    session.pwd = dir;
    printf("Remote directory is now %s\n", session.pwd);

    return 1;
}

/*
 * Print current directory. Easy as pie.
 */
int sftp_cmd_pwd(CSshSession &session, struct sftp_command *cmd)
{
    if (session.back == NULL) {
		printf("psftp: not connected to a host; use \"open host.name\"\n");
		return 0;
    }

    printf("Remote directory is %s\n", session.pwd);
    return 1;
}

/*
 * Get a file and save it at the local end. We have two very
 * similar commands here: `get' and `reget', which differ in that
 * `reget' checks for the existence of the destination file and
 * starts from where a previous aborted transfer left off.
 */
int sftp_general_get(CSshSession &session, struct sftp_command *cmd, int restart)
{
    struct fxp_handle *fh;
    char *fname, *outfname;
    uint64 offset;
    FILE *fp;
    int ret;

    if (session.back == NULL) {
		printf("psftp: not connected to a host; use \"open host.name\"\n");
		return 0;
    }

    if (cmd->nwords < 2) {
		printf("get: expects a filename\n");
		return 0;
    }

    fname = canonify(session, cmd->words[1]);
    if (!fname) {
		printf("%s: %s\n", cmd->words[1], fxp_error());
		return 0;
    }
    outfname = (cmd->nwords == 2 ?
		stripslashes(cmd->words[1], 0) : cmd->words[2]);

    fh = fxp_open(session, fname, SSH_FXF_READ);
    if (!fh) {
		printf("SSH: %s: %s\n", fname, fxp_error());
		sfree(fname);	//TOFIX
		return 0;
    }

    if (restart) {
		fp = fopen(outfname, "rb+");
    } else {
		fp = fopen(outfname, "wb");
    }

    if (!fp) {
		printf("local: unable to open %s\n", outfname);
		fxp_close(session, fh);
		sfree(fname);	//TOFIX
		return 0;
    }

    if (restart) {
		long posn;
		fseek(fp, 0L, SEEK_END);
		posn = ftell(fp);
		printf("reget: restarting at file position %ld\n", posn);
		offset = uint64_make(0, posn);
    } else {
		offset = uint64_make(0, 0);
    }

    printf("remote:%s => local:%s\n", fname, outfname);

    /*
     * FIXME: we can use FXP_FSTAT here to get the file size, and
     * thus put up a progress bar.
     */
	
	__int64 lSize = 0;
	struct fxp_attrs attrs;
	if(fxp_stat(session, fname, &attrs))
		lSize = attrs.size.lo + attrs.size.hi * UINT_MAX;

	if(session.m_pfnProgress)
		session.m_pfnProgress(0, lSize, session.m_data);

    ret = 1;
    while (1) {
		char buffer[4096];
		int len;
		int wpos, wlen;

		len = fxp_read(session, fh, buffer, offset, sizeof(buffer));
		if ((len == -1 && fxp_error_type() == SSH_FX_EOF) || len == 0)
			break;
		if (len == -1) {
			printf("error while reading: %s\n", fxp_error());
			ret = 0;
			break;
		}

		wpos = 0;
		while (wpos < len) {
			wlen = fwrite(buffer, 1, len - wpos, fp);
			if (wlen <= 0) {
			printf("error while writing local file\n");
			ret = 0;
			break;
			}
			wpos += wlen;
		}
		if (wpos < len) {	       /* we had an error */
			ret = 0;
			break;
		}
		offset = uint64_add32(offset, len);

		//progress report
		if(session.m_pfnProgress)
		{
			__int64 offset2 = offset.lo+offset.hi*UINT_MAX;
			if(!session.m_pfnProgress(offset2, lSize, session.m_data))
				break;
		}
    }

    fclose(fp);
    fxp_close(session, fh);
    sfree(fname);

    return ret;
}
int sftp_cmd_get(CSshSession &session, struct sftp_command *cmd)
{
    return sftp_general_get(session, cmd, 0);
}
int sftp_cmd_reget(CSshSession &session, struct sftp_command *cmd)
{
    return sftp_general_get(session, cmd, 1);
}

/*
 * Send a file and store it at the remote end. We have two very
 * similar commands here: `put' and `reput', which differ in that
 * `reput' checks for the existence of the destination file and
 * starts from where a previous aborted transfer left off.
 */
int sftp_general_put(CSshSession &session, struct sftp_command *cmd, int restart)
{
    struct fxp_handle *fh;
    char *fname, *origoutfname, *outfname;
    uint64 offset;
    FILE *fp;
    int ret;

    if (session.back == NULL) {
		printf("psftp: not connected to a host; use \"open host.name\"\n");
		return 0;
    }

    if (cmd->nwords < 2) {
		printf("put: expects a filename\n");
		return 0;
    }

    fname = cmd->words[1];
    origoutfname = (cmd->nwords == 2 ?
		    stripslashes(cmd->words[1], 1) : cmd->words[2]);
    outfname = canonify(session, origoutfname);
    if (!outfname) {
		printf("%s: %s\n", origoutfname, fxp_error());
		return 0;
    }

    fp = fopen(fname, "rb");
    if (!fp) {
		printf("local: unable to open %s\n", fname);
		sfree(outfname);
		return 0;
    }
    if (restart) {
		fh = fxp_open(session, outfname,
			      SSH_FXF_WRITE);
    } else {
		fh = fxp_open(session, outfname,
			      SSH_FXF_WRITE | SSH_FXF_CREAT | SSH_FXF_TRUNC);
    }
    if (!fh) {
		printf("%s: %s\n", outfname, fxp_error());
		sfree(outfname);
		return 0;
    }

    if (restart) {
		char decbuf[30];
		struct fxp_attrs attrs;
		if (!fxp_fstat(session, fh, &attrs)) {
			printf("read size of %s: %s\n", outfname, fxp_error());
		    fxp_close(session, fh);	//MIRO
			sfree(outfname);
			return 0;
		}
		if (!(attrs.flags & SSH_FILEXFER_ATTR_SIZE)) {
			printf("read size of %s: size was not given\n", outfname);
			fxp_close(session, fh);	//MIRO
			sfree(outfname);
			return 0;
		}
		offset = attrs.size;
		uint64_decimal(offset, decbuf);
		printf("reput: restarting at file position %s\n", decbuf);
		if (uint64_compare(offset, uint64_make(0, LONG_MAX)) > 0) {
			printf("reput: remote file is larger than we can deal with\n");
			sfree(outfname);
			fxp_close(session, fh);	//MIRO
			return 0;
		}
		if (fseek(fp, offset.lo, SEEK_SET) != 0)
			fseek(fp, 0, SEEK_END);    /* *shrug* */
    } else {
		offset = uint64_make(0, 0);
    }

    printf("local:%s => remote:%s\n", fname, outfname);

    /*
     * FIXME: we can use FXP_FSTAT here to get the file size, and
     * thus put up a progress bar.
     */

	//calculate local file size
	fpos_t lPos = 0;
	fgetpos(fp, &lPos);
	fseek(fp, 0, SEEK_END);
	fpos_t lSize = 0;
	fgetpos(fp, &lSize);
	fsetpos(fp, &lPos);

	if(session.m_pfnProgress)
		session.m_pfnProgress(0, lSize, session.m_data);

    ret = 1;
    while (1) {
		char buffer[4096];
		int len;

		len = fread(buffer, 1, sizeof(buffer), fp);
		if (len == -1) {
			printf("error while reading local file\n");
			ret = 0;
			break;
		} else if (len == 0) {
			break;
		}
		if (!fxp_write(session, fh, buffer, offset, len)) {
			printf("error while writing: %s\n", fxp_error());
			ret = 0;
			break;
		}
		offset = uint64_add32(offset, len);

		if(session.m_pfnProgress)
		{
			__int64 offset2 = offset.lo+offset.hi*UINT_MAX;
			if(!session.m_pfnProgress(offset2, lSize, session.m_data))
				break;
		}
    }

    fxp_close(session, fh);
    fclose(fp);
    sfree(outfname);

    return ret;
}
int sftp_cmd_put(CSshSession &session, struct sftp_command *cmd)
{
    return sftp_general_put(session, cmd, 0);
}
int sftp_cmd_reput(CSshSession &session, struct sftp_command *cmd)
{
    return sftp_general_put(session, cmd, 1);
}

int sftp_cmd_mkdir(CSshSession &session, struct sftp_command *cmd)
{
    char *dir;
    int result;

    if (session.back == NULL) {
		printf("psftp: not connected to a host; use \"open host.name\"\n");
		return 0;
    }

    if (cmd->nwords < 2) {
	printf("mkdir: expects a directory\n");
	return 0;
    }

    dir = canonify(session, cmd->words[1]);
    if (!dir) {
		printf("%s: %s\n", dir, fxp_error());
		return 0;
    }

    result = fxp_mkdir(session, dir);
    if (!result) {
		printf("mkdir %s: %s\n", dir, fxp_error());
		sfree(dir);
		return 0;
    }

    sfree(dir);
    return 1;
}

int sftp_cmd_rmdir(CSshSession &session, struct sftp_command *cmd)
{
    char *dir;
    int result;

    if (session.back == NULL) {
		printf("psftp: not connected to a host; use \"open host.name\"\n");
		return 0;
    }

    if (cmd->nwords < 2) {
	printf("rmdir: expects a directory\n");
	return 0;
    }

    dir = canonify(session, cmd->words[1]);
    if (!dir) {
		printf("%s: %s\n", dir, fxp_error());
		return 0;
    }

    result = fxp_rmdir(session, dir);
    if (!result) {
		printf("rmdir %s: %s\n", dir, fxp_error());
		sfree(dir);
		return 0;
    }

    sfree(dir);
    return 1;
}

int sftp_cmd_rm(CSshSession &session, struct sftp_command *cmd)
{
    char *fname;
    int result;

    if (session.back == NULL) {
		printf("psftp: not connected to a host; use \"open host.name\"\n");
		return 0;
    }

    if (cmd->nwords < 2) {
	printf("rm: expects a filename\n");
	return 0;
    }

    fname = canonify(session, cmd->words[1]);
    if (!fname) {
		printf("%s: %s\n", fname, fxp_error());
		return 0;
    }

    result = fxp_remove(session, fname);
    if (!result) {
		printf("rm %s: %s\n", fname, fxp_error());
		sfree(fname);
		return 0;
    }

    sfree(fname);
    return 1;
}

int sftp_cmd_mv(CSshSession &session, struct sftp_command *cmd)
{
    char *srcfname, *dstfname;
    int result;

    if (session.back == NULL) {
		printf("psftp: not connected to a host; use \"open host.name\"\n");
		return 0;
    }

    if (cmd->nwords < 3) {
		printf("mv: expects two filenames\n");
		return 0;
    }
    srcfname = canonify(session, cmd->words[1]);
    if (!srcfname) {
		printf("%s: %s\n", srcfname, fxp_error());
		return 0;
    }

    dstfname = canonify(session, cmd->words[2]);
    if (!dstfname) {
		//printf("%s: %s\n", dstfname, fxp_error());
	    sfree(srcfname);
		return 0;
    }

    result = fxp_rename(session, srcfname, dstfname);
    if (!result) {
		char const *error = fxp_error();
		struct fxp_attrs attrs;

	/*
	 * The move might have failed because dstfname pointed at a
	 * directory. We check this possibility now: if dstfname
	 * _is_ a directory, we re-attempt the move by appending
	 * the basename of srcfname to dstfname.
	 */
	result = fxp_stat(session, dstfname, &attrs);
	if (result &&
	    (attrs.flags & SSH_FILEXFER_ATTR_PERMISSIONS) &&
	    (attrs.permissions & 0040000)) {
	    char *p;
	    char *newname, *newcanon;
	    printf("(destination %s is a directory)\n", dstfname);
	    p = srcfname + strlen(srcfname);
	    while (p > srcfname && p[-1] != '/') p--;
	    newname = dupcat(dstfname, "/", p, NULL);
	    newcanon = canonify(session, newname);
	    sfree(newname);
	    if (newcanon) {
			sfree(dstfname);
			dstfname = newcanon;
			result = fxp_rename(session, srcfname, dstfname);
			error = result ? NULL : fxp_error();
	    }
	}
	if (error) {
	    printf("mv %s %s: %s\n", srcfname, dstfname, error);
	    sfree(srcfname);
	    sfree(dstfname);
	    return 0;
	}
    }
    printf("%s -> %s\n", srcfname, dstfname);

    sfree(srcfname);
    sfree(dstfname);
    return 1;
}

int sftp_cmd_chmod(CSshSession &session, struct sftp_command *cmd)
{
    char *fname, *mode;
    int result;
    struct fxp_attrs attrs;
    unsigned attrs_clr, attrs_xor, oldperms, newperms;

    if (session.back == NULL) {
		printf("psftp: not connected to a host; use \"open host.name\"\n");
		return 0;
    }

    if (cmd->nwords < 3) {
	printf("chmod: expects a mode specifier and a filename\n");
	return 0;
    }

    /*
     * Attempt to parse the mode specifier in cmd->words[1]. We
     * don't support the full horror of Unix chmod; instead we
     * support a much simpler syntax in which the user can either
     * specify an octal number, or a comma-separated sequence of
     * [ugoa]*[-+=][rwxst]+. (The initial [ugoa] sequence may
     * _only_ be omitted if the only attribute mentioned is t,
     * since all others require a user/group/other specification.
     * Additionally, the s attribute may not be specified for any
     * [ugoa] specifications other than exactly u or exactly g.
     */
    attrs_clr = attrs_xor = 0;
    mode = cmd->words[1];
    if (mode[0] >= '0' && mode[0] <= '9') {
	if (mode[strspn(mode, "01234567")]) {
	    printf("chmod: numeric file modes should"
		   " contain digits 0-7 only\n");
	    return 0;
	}
	attrs_clr = 07777;
	sscanf(mode, "%o", &attrs_xor);
	attrs_xor &= attrs_clr;
    } else {
	while (*mode) {
	    char *modebegin = mode;
	    unsigned subset, perms;
	    int action;

	    subset = 0;
	    while (*mode && *mode != ',' &&
		   *mode != '+' && *mode != '-' && *mode != '=') {
		switch (*mode) {
		  case 'u': subset |= 04700; break; /* setuid, user perms */
		  case 'g': subset |= 02070; break; /* setgid, group perms */
		  case 'o': subset |= 00007; break; /* just other perms */
		  case 'a': subset |= 06777; break; /* all of the above */
		  default:
		    printf("chmod: file mode '%.*s' contains unrecognised"
			   " user/group/other specifier '%c'\n",
			   strcspn(modebegin, ","), modebegin, *mode);
		    return 0;
		}
		mode++;
	    }
	    if (!*mode || *mode == ',') {
		printf("chmod: file mode '%.*s' is incomplete\n",
		       strcspn(modebegin, ","), modebegin);
		return 0;
	    }
	    action = *mode++;
	    if (!*mode || *mode == ',') {
		printf("chmod: file mode '%.*s' is incomplete\n",
		       strcspn(modebegin, ","), modebegin);
		return 0;
	    }
	    perms = 0;
	    while (*mode && *mode != ',') {
		switch (*mode) {
		  case 'r': perms |= 00444; break;
		  case 'w': perms |= 00222; break;
		  case 'x': perms |= 00111; break;
		  case 't': perms |= 01000; subset |= 01000; break;
		  case 's':
		    if ((subset & 06777) != 04700 &&
			(subset & 06777) != 02070) {
			printf("chmod: file mode '%.*s': set[ug]id bit should"
			       " be used with exactly one of u or g only\n",
			       strcspn(modebegin, ","), modebegin);
			return 0;
		    }
		    perms |= 06000;
		    break;
		  default:
		    printf("chmod: file mode '%.*s' contains unrecognised"
			   " permission specifier '%c'\n",
			   strcspn(modebegin, ","), modebegin, *mode);
		    return 0;
		}
		mode++;
	    }
	    if (!(subset & 06777) && (perms &~ subset)) {
		printf("chmod: file mode '%.*s' contains no user/group/other"
		       " specifier and permissions other than 't' \n",
		       strcspn(modebegin, ","), modebegin, *mode);
		return 0;
	    }
	    perms &= subset;
	    switch (action) {
	      case '+':
		attrs_clr |= perms;
		attrs_xor |= perms;
		break;
	      case '-':
		attrs_clr |= perms;
		attrs_xor &= ~perms;
		break;
	      case '=':
		attrs_clr |= subset;
		attrs_xor |= perms;
		break;
	    }
	    if (*mode) mode++;	       /* eat comma */
	}
    }

    fname = canonify(session, cmd->words[2]);
    if (!fname) {
		printf("%s: %s\n", fname, fxp_error());
		return 0;
    }

    result = fxp_stat(session, fname, &attrs);
    if (!result || !(attrs.flags & SSH_FILEXFER_ATTR_PERMISSIONS)) {
		printf("get attrs for %s: %s\n", fname,
			   result ? "file permissions not provided" : fxp_error());
	sfree(fname);
	return 0;
    }

    attrs.flags = SSH_FILEXFER_ATTR_PERMISSIONS;   /* perms _only_ */
    oldperms = attrs.permissions & 07777;
    attrs.permissions &= ~attrs_clr;
    attrs.permissions ^= attrs_xor;
    newperms = attrs.permissions & 07777;

    result = fxp_setstat(session, fname, attrs);

    if (!result) {
	printf("set attrs for %s: %s\n", fname, fxp_error());
	sfree(fname);
	return 0;
    }

    printf("%s: %04o -> %04o\n", fname, oldperms, newperms);

    sfree(fname);
    return 1;
}

static int sftp_cmd_open(CSshSession &session, struct sftp_command *cmd)
{
    if (session.back != NULL) {
		printf("psftp: already connected\n");
		return 0;
    }

    if (cmd->nwords < 2) {
		printf("open: expects a host name\n");
		return 0;
    }

    if (psftp_connect(session, cmd->words[1], NULL, 0)) {
		session.back = NULL;	       /* connection is already closed */
		return -1;		       /* this is fatal */
    }
    do_sftp_init(session);
    return 1;
}

static int sftp_cmd_lcd(CSshSession &session, struct sftp_command *cmd)
{
    char *currdir;
    int len;

    if (cmd->nwords < 2) {
		printf("lcd: expects a local directory name\n");
		return 0;
    }

    if (!SetCurrentDirectory(cmd->words[1])) {
		LPVOID message;
		int i;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				  FORMAT_MESSAGE_FROM_SYSTEM |
				  FORMAT_MESSAGE_IGNORE_INSERTS,
				  NULL, GetLastError(),
				  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				  (LPTSTR)&message, 0, NULL);
		i = strcspn((char *)message, "\n");
		printf("lcd: unable to change directory: %.*s\n", i, (LPCTSTR)message);
		LocalFree(message);
		return 0;
    }

    currdir = (char *)smalloc(256);
    len = GetCurrentDirectory(256, currdir);
    if (len > 256)
		currdir = (char *)srealloc(currdir, len);
    GetCurrentDirectory(len, currdir);
    printf("New local directory is %s\n", currdir);
    sfree(currdir);

    return 1;
}

static int sftp_cmd_lpwd(CSshSession &session, struct sftp_command *cmd)
{
    char *currdir;
    int len;

    currdir = (char *)smalloc(256);
    len = GetCurrentDirectory(256, currdir);
    if (len > 256)
		currdir = (char *)srealloc(currdir, len);
    GetCurrentDirectory(len, currdir);
    printf("Current local directory is %s\n", currdir);
    sfree(currdir);

    return 1;
}

static int sftp_cmd_pling(CSshSession &session, struct sftp_command *cmd)
{
    int exitcode;

    exitcode = system(cmd->words[1]);
    return (exitcode == 0);
}

static int sftp_cmd_help(CSshSession &session, struct sftp_command *cmd);

static struct sftp_cmd_lookup {
    char *name;
    /*
     * For help purposes, there are two kinds of command:
     * 
     *  - primary commands, in which `longhelp' is non-NULL. In
     *    this case `shorthelp' is descriptive text, and `longhelp'
     *    is longer descriptive text intended to be printed after
     *    the command name.
     * 
     *  - alias commands, in which `longhelp' is NULL. In this case
     *    `shorthelp' is the name of a primary command, which
     *    contains the help that should double up for this command.
     */
    int listed;			       /* do we list this in primary help? */
    char *shorthelp;
    char *longhelp;
    int (*obey) (CSshSession &session, struct sftp_command *);
} sftp_lookup[] = {
    /*
     * List of sftp commands. This is binary-searched so it MUST be
     * in ASCII order.
     */
    {
	"!", TRUE, "run a local Windows command",
	    "<command>\n"
	    "  Runs a local Windows command. For example, \"!del myfile\".\n",
	    sftp_cmd_pling
    },
    {
	"bye", TRUE, "finish your SFTP session",
	    "\n"
	    "  Terminates your SFTP session and quits the PSFTP program.\n",
	    sftp_cmd_quit
    },
    {
	"cd", TRUE, "change your remote working directory",
	    " [ <New working directory> ]\n"
	    "  Change the remote working directory for your SFTP session.\n"
	    "  If a new working directory is not supplied, you will be\n"
	    "  returned to your home directory.\n",
	    sftp_cmd_cd
    },
    {
	"chmod", TRUE, "change file permissions and modes",
	    " ( <octal-digits> | <modifiers> ) <filename>\n"
	    "  Change the file permissions on a file or directory.\n"
	    "  <octal-digits> can be any octal Unix permission specifier.\n"
	    "  Alternatively, <modifiers> can include:\n"
	    "    u+r     make file readable by owning user\n"
	    "    u+w     make file writable by owning user\n"
	    "    u+x     make file executable by owning user\n"
	    "    u-r     make file not readable by owning user\n"
	    "    [also u-w, u-x]\n"
	    "    g+r     make file readable by members of owning group\n"
	    "    [also g+w, g+x, g-r, g-w, g-x]\n"
	    "    o+r     make file readable by all other users\n"
	    "    [also o+w, o+x, o-r, o-w, o-x]\n"
	    "    a+r     make file readable by absolutely everybody\n"
	    "    [also a+w, a+x, a-r, a-w, a-x]\n"
	    "    u+s     enable the Unix set-user-ID bit\n"
	    "    u-s     disable the Unix set-user-ID bit\n"
	    "    g+s     enable the Unix set-group-ID bit\n"
	    "    g-s     disable the Unix set-group-ID bit\n"
	    "    +t      enable the Unix \"sticky bit\"\n"
	    "  You can give more than one modifier for the same user (\"g-rwx\"), and\n"
	    "  more than one user for the same modifier (\"ug+w\"). You can\n"
	    "  use commas to separate different modifiers (\"u+rwx,g+s\").\n",
	    sftp_cmd_chmod
    },
    {
	"del", TRUE, "delete a file",
	    " <filename>\n"
	    "  Delete a file.\n",
	    sftp_cmd_rm
    },
    {
	"delete", FALSE, "del", NULL, sftp_cmd_rm
    },
    {
	"dir", TRUE, "list contents of a remote directory",
	    " [ <directory-name> ]\n"
	    "  List the contents of a specified directory on the server.\n"
	    "  If <directory-name> is not given, the current working directory\n"
	    "  will be listed.\n",
	    sftp_cmd_ls
    },
    {
	"exit", TRUE, "bye", NULL, sftp_cmd_quit
    },
    {
	"get", TRUE, "download a file from the server to your local machine",
	    " <filename> [ <local-filename> ]\n"
	    "  Downloads a file on the server and stores it locally under\n"
	    "  the same name, or under a different one if you supply the\n"
	    "  argument <local-filename>.\n",
	    sftp_cmd_get
    },
    {
	"help", TRUE, "give help",
	    " [ <command> [ <command> ... ] ]\n"
	    "  Give general help if no commands are specified.\n"
	    "  If one or more commands are specified, give specific help on\n"
	    "  those particular commands.\n",
	    sftp_cmd_help
    },
    {
	"lcd", TRUE, "change local working directory",
	    " <local-directory-name>\n"
	    "  Change the local working directory of the PSFTP program (the\n"
	    "  default location where the \"get\" command will save files).\n",
	    sftp_cmd_lcd
    },
    {
	"lpwd", TRUE, "print local working directory",
	    "\n"
	    "  Print the local working directory of the PSFTP program (the\n"
	    "  default location where the \"get\" command will save files).\n",
	    sftp_cmd_lpwd
    },
    {
	"ls", TRUE, "dir", NULL,
	    sftp_cmd_ls
    },
    {
	"mkdir", TRUE, "create a directory on the remote server",
	    " <directory-name>\n"
	    "  Creates a directory with the given name on the server.\n",
	    sftp_cmd_mkdir
    },
    {
	"mv", TRUE, "move or rename a file on the remote server",
	    " <source-filename> <destination-filename>\n"
	    "  Moves or renames the file <source-filename> on the server,\n"
	    "  so that it is accessible under the name <destination-filename>.\n",
	    sftp_cmd_mv
    },
    {
	"open", TRUE, "connect to a host",
	    " [<user>@]<hostname>\n"
	    "  Establishes an SFTP connection to a given host. Only usable\n"
	    "  when you did not already specify a host name on the command\n"
	    "  line.\n",
	    sftp_cmd_open
    },
    {
	"put", TRUE, "upload a file from your local machine to the server",
	    " <filename> [ <remote-filename> ]\n"
	    "  Uploads a file to the server and stores it there under\n"
	    "  the same name, or under a different one if you supply the\n"
	    "  argument <remote-filename>.\n",
	    sftp_cmd_put
    },
    {
	"pwd", TRUE, "print your remote working directory",
	    "\n"
	    "  Print the current remote working directory for your SFTP session.\n",
	    sftp_cmd_pwd
    },
    {
	"quit", TRUE, "bye", NULL,
	    sftp_cmd_quit
    },
    {
	"reget", TRUE, "continue downloading a file",
	    " <filename> [ <local-filename> ]\n"
	    "  Works exactly like the \"get\" command, but the local file\n"
	    "  must already exist. The download will begin at the end of the\n"
	    "  file. This is for resuming a download that was interrupted.\n",
	    sftp_cmd_reget
    },
    {
	"ren", TRUE, "mv", NULL,
	    sftp_cmd_mv
    },
    {
	"rename", FALSE, "mv", NULL,
	    sftp_cmd_mv
    },
    {
	"reput", TRUE, "continue uploading a file",
	    " <filename> [ <remote-filename> ]\n"
	    "  Works exactly like the \"put\" command, but the remote file\n"
	    "  must already exist. The upload will begin at the end of the\n"
	    "  file. This is for resuming an upload that was interrupted.\n",
	    sftp_cmd_reput
    },
    {
	"rm", TRUE, "del", NULL,
	    sftp_cmd_rm
    },
    {
	"rmdir", TRUE, "remove a directory on the remote server",
	    " <directory-name>\n"
	    "  Removes the directory with the given name on the server.\n"
	    "  The directory will not be removed unless it is empty.\n",
	    sftp_cmd_rmdir
    }
};

const struct sftp_cmd_lookup *lookup_command(char *name)
{
    int i, j, k, cmp;

    i = -1;
    j = sizeof(sftp_lookup) / sizeof(*sftp_lookup);
    while (j - i > 1) {
	k = (j + i) / 2;
	cmp = strcmp(name, sftp_lookup[k].name);
	if (cmp < 0)
	    j = k;
	else if (cmp > 0)
	    i = k;
	else {
	    return &sftp_lookup[k];
	}
    }
    return NULL;
}

static int sftp_cmd_help(CSshSession &session, struct sftp_command *cmd)
{
    int i;
    if (cmd->nwords == 1) {
	/*
	 * Give short help on each command.
	 */
	int maxlen;
	maxlen = 0;
	for (i = 0; i < sizeof(sftp_lookup) / sizeof(*sftp_lookup); i++) {
	    int len;
	    if (!sftp_lookup[i].listed)
		continue;
	    len = strlen(sftp_lookup[i].name);
	    if (maxlen < len)
		maxlen = len;
	}
	for (i = 0; i < sizeof(sftp_lookup) / sizeof(*sftp_lookup); i++) {
	    const struct sftp_cmd_lookup *lookup;
	    if (!sftp_lookup[i].listed)
		continue;
	    lookup = &sftp_lookup[i];
	    printf("%-*s", maxlen+2, lookup->name);
	    if (lookup->longhelp == NULL)
		lookup = lookup_command(lookup->shorthelp);
	    printf("%s\n", lookup->shorthelp);
	}
    } else {
	/*
	 * Give long help on specific commands.
	 */
	for (i = 1; i < cmd->nwords; i++) {
	    const struct sftp_cmd_lookup *lookup;
	    lookup = lookup_command(cmd->words[i]);
	    if (!lookup) {
		printf("help: %s: command not found\n", cmd->words[i]);
	    } else {
		printf("%s", lookup->name);
		if (lookup->longhelp == NULL)
		    lookup = lookup_command(lookup->shorthelp);
		printf("%s", lookup->longhelp);
	    }
	}
    }
    return 1;
}

/* ----------------------------------------------------------------------
 * Command line reading and parsing.
 */
struct sftp_command *sftp_getcmd(FILE *fp, int mode, int modeflags)
{
    char *line;
    int linelen, linesize;
    struct sftp_command *cmd;
    char *p, *q, *r;
    int quoting;

    if ((mode == 0) || (modeflags & 1)) {
	printf("psftp> ");
    }
    fflush(stdout);

    cmd = (struct sftp_command *)smalloc(sizeof(struct sftp_command));
    cmd->words = NULL;
    cmd->nwords = 0;
    cmd->wordssize = 0;

    line = NULL;
    linesize = linelen = 0;
    while (1) {
	int len;
	char *ret;

	linesize += 512;
	line = (char *)srealloc(line, linesize);
	ret = fgets(line + linelen, linesize - linelen, fp);

	if (!ret || (linelen == 0 && line[0] == '\0')) {
	    cmd->obey = sftp_cmd_quit;
	    if ((mode == 0) || (modeflags & 1))
		printf("quit\n");
	    return cmd;		       /* eof */
	}
	len = linelen + strlen(line + linelen);
	linelen += len;
	if (line[linelen - 1] == '\n') {
	    linelen--;
	    line[linelen] = '\0';
	    break;
	}
    }
    if (modeflags & 1) {
	printf("%s\n", line);
    }

    p = line;
    while (*p && (*p == ' ' || *p == '\t'))
	p++;

    if (*p == '!') {
	/*
	 * Special case: the ! command. This is always parsed as
	 * exactly two words: one containing the !, and the second
	 * containing everything else on the line.
	 */
	cmd->nwords = cmd->wordssize = 2;
	cmd->words = (char **)srealloc(cmd->words, cmd->wordssize * sizeof(char *));
	cmd->words[0] = "!";
	cmd->words[1] = p+1;
    } else {

	/*
	 * Parse the command line into words. The syntax is:
	 *  - double quotes are removed, but cause spaces within to be
	 *    treated as non-separating.
	 *  - a double-doublequote pair is a literal double quote, inside
	 *    _or_ outside quotes. Like this:
	 *
	 *      firstword "second word" "this has ""quotes"" in" and""this""
	 *
	 * becomes
	 *
	 *      >firstword<
	 *      >second word<
	 *      >this has "quotes" in<
	 *      >and"this"<
	 */
	while (*p) {
	    /* skip whitespace */
	    while (*p && (*p == ' ' || *p == '\t'))
		p++;
	    /* mark start of word */
	    q = r = p;		       /* q sits at start, r writes word */
	    quoting = 0;
	    while (*p) {
		if (!quoting && (*p == ' ' || *p == '\t'))
		    break;		       /* reached end of word */
		else if (*p == '"' && p[1] == '"')
		    p += 2, *r++ = '"';    /* a literal quote */
		else if (*p == '"')
		    p++, quoting = !quoting;
		else
		    *r++ = *p++;
	    }
	    if (*p)
		p++;		       /* skip over the whitespace */
	    *r = '\0';
	    if (cmd->nwords >= cmd->wordssize) {
		cmd->wordssize = cmd->nwords + 16;
		cmd->words =
		   (char **) srealloc(cmd->words, cmd->wordssize * sizeof(char *));
	    }
	    cmd->words[cmd->nwords++] = q;
	}
    }

    /*
     * Now parse the first word and assign a function.
     */

    if (cmd->nwords == 0)
		cmd->obey = sftp_cmd_null;
    else {
	const struct sftp_cmd_lookup *lookup;
	lookup = lookup_command(cmd->words[0]);
	if (!lookup)
	    cmd->obey = sftp_cmd_unknown;
	else
	    cmd->obey = lookup->obey;
    }

	sfree(line); //MIRO

    return cmd;
}

static int do_sftp_init(CSshSession &session)
{
    /*
     * Do protocol initialisation. 
     */
    if (!fxp_init(session)) {
		//TOFIX fprintf(stderr,
		//TOFIX		"Fatal: unable to initialise SFTP: %s\n", fxp_error());
		return 1;		       /* failure */
    }

    /*
     * Find out where our home directory is.
     */
    session.homedir = fxp_realpath(session, ".");
    if (!session.homedir) {
		//TOFIX fprintf(stderr,
		//TOFIX "Warning: failed to resolve home directory: %s\n",
		//TOFIX fxp_error());
		session.homedir = dupstr(".");
    } else {
		printf("Remote working directory is %s\n", session.homedir);
    }
    session.pwd = dupstr(session.homedir);
    return 0;
}

void do_sftp(CSshSession &session, int mode, int modeflags, char *batchfile)
{
    FILE *fp;
    int ret;

    /*
     * Batch mode?
     */
    if (mode == 0) {

        /* ------------------------------------------------------------------
         * Now we're ready to do Real Stuff.
         */
        while (1) {
			struct sftp_command *cmd;
			cmd = sftp_getcmd(stdin, 0, 0);
			if (!cmd)
				break;
			if (cmd->obey(session, cmd) < 0)
				break;
		}
    } else {
        fp = fopen(batchfile, "r");
        if (!fp) {
			printf("Fatal: unable to open %s\n", batchfile);
			return;
        }
        while (1) {
			struct sftp_command *cmd;
			cmd = sftp_getcmd(fp, mode, modeflags);
			if (!cmd)
				break;
			ret = cmd->obey(session, cmd);
			if (ret < 0)
				break;
			if (ret == 0) {
				if (!(modeflags & 2))
					break;
			}
        }
		fclose(fp);
    }
}

/* ----------------------------------------------------------------------
 * Dirty bits: integration with PuTTY.
 */

static int verbose = 0;

/*
 *  Print an error message and perform a fatal exit.
 */
void fatalbox(CSshSession &session, char *fmt, ...)
{
    char str[0x100];		       /* Make the size big enough */
    va_list ap;
    va_start(ap, fmt);
    strcpy(str, "Fatal:");
    vsprintf(str + strlen(str), fmt, ap);
    va_end(ap);
    strcat(str, "\n");
    
	//fputs(str, stderr);
	if(session.m_fnError)
		session.m_fnError(str, session.m_fnErrData);

    cleanup_exit(1);
}

void connection_fatal(CSshSession &session, char *fmt, ...)
{
    char str[0x100];		       /* Make the size big enough */
    va_list ap;
    va_start(ap, fmt);
    strcpy(str, "Fatal:");
    vsprintf(str + strlen(str), fmt, ap);
    va_end(ap);
    strcat(str, "\n");

    //TOFIX fputs(str, stderr);
	session.m_dbgFn(str, session.m_dwDbgData);

    cleanup_exit(1);
}

void ldisc_send(char *buf, int len, int interactive)
{
    /*
     * This is only here because of the calls to ldisc_send(NULL,
     * 0) in ssh.c. Nothing in PSFTP actually needs to use the
     * ldisc as an ldisc. So if we get called with any real data, I
     * want to know about it.
     */
    assert(len == 0);
}

/*
 * Be told what socket we're supposed to be using.
 */
char *do_select(CSshSession &session, SOCKET skt, int startup)
{
    if (startup)
		session.sftp_ssh_socket = skt;
    else
		session.sftp_ssh_socket = INVALID_SOCKET;
    return NULL;
}
int select_result(CSshSession &, WPARAM, LPARAM);

/*
 * Receive a block of data from the SSH link. Block until all data
 * is available.
 *
 * To do this, we repeatedly call the SSH protocol module, with our
 * own trap in from_backend() to catch the data that comes back. We
 * do this until we have enough data.
 */

static unsigned char *outptr;	       /* where to put the data */
static unsigned outlen;		       /* how much data required */
static unsigned char *pending = NULL;  /* any spare data */
static unsigned pendlen = 0, pendsize = 0;	/* length and phys. size of buffer */

int from_backend(int is_stderr, char *data, int datalen)
{
    unsigned char *p = (unsigned char *) data;
    unsigned len = (unsigned) datalen;

    assert(len > 0);

    /*
     * stderr data is just spouted to local stderr and otherwise
     * ignored.
     */
    if (is_stderr) {
	//TOFIX fwrite(data, 1, len, stderr);
		return 0;
    }

    /*
     * If this is before the real session begins, just return.
     */
    if (!outptr)
	return 0;

    if (outlen > 0) {
	unsigned used = outlen;
	if (used > len)
	    used = len;
	memcpy(outptr, p, used);
	outptr += used;
	outlen -= used;
	p += used;
	len -= used;
    }

    if (len > 0) {
	if (pendsize < pendlen + len) {
	    pendsize = pendlen + len + 4096;
	    pending = (pending ? (unsigned char *)srealloc(pending, pendsize) :
		       (unsigned char *)smalloc(pendsize));
	    //if (!pending)
		//	fatalbox(session, "Out of memory");
	}
	memcpy(pending + pendlen, p, len);
	pendlen += len;
    }

    return 0;
}

int sftp_recvdata(CSshSession &session, char *buf, int len)
{
    outptr = (unsigned char *) buf;
    outlen = len;

    /*
     * See if the pending-input block contains some of what we
     * need.
     */
    if (pendlen > 0) {
	unsigned pendused = pendlen;
	if (pendused > outlen)
	    pendused = outlen;
	memcpy(outptr, pending, pendused);
	memmove(pending, pending + pendused, pendlen - pendused);
	outptr += pendused;
	outlen -= pendused;
	pendlen -= pendused;
	if (pendlen == 0) {
	    pendsize = 0;
	    sfree(pending);
	    pending = NULL;
	}
	if (outlen == 0)
	    return 1;
    }

    while (outlen > 0) {
	fd_set readfds;

	FD_ZERO(&readfds);
	FD_SET(session.sftp_ssh_socket, &readfds);
	if (select(1, &readfds, NULL, NULL, NULL) < 0)
	    return 0;		       /* doom */
	select_result(session, (WPARAM) session.sftp_ssh_socket, (LPARAM) FD_READ);
    }

    return 1;
}
int sftp_senddata(CSshSession &session, char *buf, int len)
{
    session.back->send(session, (char *)buf, len);
    return 1;
}

/*
 * Loop through the ssh connection and authentication process.
 */
static void ssh_sftp_init(CSshSession &session)
{
    if (session.sftp_ssh_socket == INVALID_SOCKET)
		return;
    
	while (!session.back->sendok(session)) {
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(session.sftp_ssh_socket, &readfds);
		if (select(1, &readfds, NULL, NULL, NULL) < 0)
			return;		       /* doom */
		select_result(session, (WPARAM) session.sftp_ssh_socket, (LPARAM) FD_READ);
    }
}

/*
 *  Initialize the Win$ock driver.
 */
static void init_winsock(void)
{
    WORD winsock_ver;
    WSADATA wsadata;

    winsock_ver = MAKEWORD(1, 1);
    if (WSAStartup(winsock_ver, &wsadata)) {
		//TOFIX fprintf(stderr, "Unable to initialise WinSock");
		cleanup_exit(1);
    }
    if (LOBYTE(wsadata.wVersion) != 1 || HIBYTE(wsadata.wVersion) != 1) {
		//TOFIX fprintf(stderr, "WinSock version is incompatible with 1.1");
		cleanup_exit(1);
    }
}

/*
 *  Short description of parameters.
 */
static void usage(void)
{
    printf("PuTTY Secure File Transfer (SFTP) client\n");
    printf("%s\n", ver);
    printf("Usage: psftp [options] user@host\n");
    printf("Options:\n");
    printf("  -b file   use specified batchfile\n");
    printf("  -bc       output batchfile commands\n");
    printf("  -be       don't stop batchfile processing if errors\n");
    printf("  -v        show verbose messages\n");
    printf("  -P port   connect to specified port\n");
    printf("  -pw passw login with specified password\n");
    cleanup_exit(1);
}

/*
 * Connect to a host.
 */
static int psftp_connect(CSshSession &session, char *userhost, char *user, int portnumber)
{
    char *host, *realhost;
    char *err;

	realhost = NULL;

    /* Separate host and username */
    host = userhost;
    host = strrchr(host, '@');
    if (host == NULL) {
	host = userhost;
    } else {
	*host++ = '\0';
	if (user) {
	    printf("psftp: multiple usernames specified; using \"%s\"\n",
		   user);
	} else
	    user = userhost;
    }

    /* Try to load settings for this host */
    do_defaults(host, &cfg);
    if (cfg.host[0] == '\0') {
		/* No settings for this host; use defaults */
		do_defaults(NULL, &cfg);
		strncpy(cfg.host, host, sizeof(cfg.host) - 1);
		cfg.host[sizeof(cfg.host) - 1] = '\0';
		cfg.port = 22;
    }

    /*
     * Trim leading whitespace off the hostname if it's there.
     */
    {
	int space = strspn(cfg.host, " \t");
	memmove(cfg.host, cfg.host+space, 1+strlen(cfg.host)-space);
    }

    /* See if host is of the form user@host */
    if (cfg.host[0] != '\0') {
	char *atsign = strchr(cfg.host, '@');
	/* Make sure we're not overflowing the user field */
	if (atsign) {
	    if (atsign - cfg.host < sizeof cfg.username) {
		strncpy(cfg.username, cfg.host, atsign - cfg.host);
		cfg.username[atsign - cfg.host] = '\0';
	    }
	    memmove(cfg.host, atsign + 1, 1 + strlen(atsign + 1));
	}
    }

    /*
     * Trim a colon suffix off the hostname if it's there.
     */
    cfg.host[strcspn(cfg.host, ":")] = '\0';

    /* Set username */
    if (user != NULL && user[0] != '\0') {
	strncpy(cfg.username, user, sizeof(cfg.username) - 1);
	cfg.username[sizeof(cfg.username) - 1] = '\0';
    }
    if (!cfg.username[0]) {
	printf("login as: ");
	if (!fgets(cfg.username, sizeof(cfg.username), stdin)) {
			logevent(session, "psftp: aborting\n");
			cleanup_exit(1);
	} else {
	    int len = strlen(cfg.username);
	    if (cfg.username[len - 1] == '\n')
		cfg.username[len - 1] = '\0';
	}
    }

    if (cfg.protocol != PROT_SSH)
		cfg.port = 22;

    if (portnumber)
		cfg.port = portnumber;

    /* SFTP uses SSH2 by default always */
    cfg.sshprot = 2;

    /*
     * Disable scary things which shouldn't be enabled for simple
     * things like SCP and SFTP: agent forwarding, port forwarding,
     * X forwarding.
     */
    cfg.x11_forward = 0;
    cfg.agentfwd = 0;
    cfg.portfwd[0] = cfg.portfwd[1] = '\0';

    /* Set up subsystem name. */
    strcpy(cfg.remote_cmd, "sftp");
    cfg.ssh_subsys = TRUE;
    cfg.nopty = TRUE;

    /*
     * Set up fallback option, for SSH1 servers or servers with the
     * sftp subsystem not enabled but the server binary installed
     * in the usual place. We only support fallback on Unix
     * systems, and we use a kludgy piece of shellery which should
     * try to find sftp-server in various places (the obvious
     * systemwide spots /usr/lib and /usr/local/lib, and then the
     * user's PATH) and finally give up.
     * 
     *   test -x /usr/lib/sftp-server && exec /usr/lib/sftp-server
     *   test -x /usr/local/lib/sftp-server && exec /usr/local/lib/sftp-server
     *   exec sftp-server
     * 
     * the idea being that this will attempt to use either of the
     * obvious pathnames and then give up, and when it does give up
     * it will print the preferred pathname in the error messages.
     */
    cfg.remote_cmd_ptr2 =
	"test -x /usr/lib/sftp-server && exec /usr/lib/sftp-server\n"
	"test -x /usr/local/lib/sftp-server && exec /usr/local/lib/sftp-server\n"
	"exec sftp-server";
    cfg.ssh_subsys2 = FALSE;

    session.back = &ssh_backend;

    err = session.back->init(session, cfg.host, cfg.port, &realhost, 0);
    if (err != NULL) {
		char szErr[256];
		sprintf(szErr, "ssh_init: %s\n", err);
		logevent(session, szErr);
		return 1;
    }
    ssh_sftp_init(session);
    if (verbose && realhost != NULL)
		printf("Connected to %s\n", realhost);

	//MIRO
	if(realhost)
		sfree(realhost);

    return 0;
}

/*
* Main function
*/

int sftp_main(CSshSession &session, char *host, int port, char *user, const char *password)
{
	int portnumber = 0;
	char *userhost;
	int mode = 0;
	int modeflags = 0;
	char *batchfile = NULL;
	
	flags = FLAG_INTERACTIVE;
	session.ssh_get_line = &console_get_line;
	sk_init(session);

	strcpy(session.m_password, password);
	userhost = host;
	portnumber = port;
	session.back = NULL;
	
	//
	// If a user@host string has already been provided, connect to
	// it now.
	//
	if (userhost) {

		if (psftp_connect(session, userhost, user, portnumber))
			return 1;
		do_sftp_init(session);
		
	} else {
		logevent(session, "psftp: no hostname specified; use \"open host.name\""
			" to connect\n");
	}
	
	return 0;
}

//TOFIX move to CSshSession
int sftp_debug_fn(CSshSession &session, void (*pfn)(const char *, unsigned long), unsigned long data)
{
	session.m_dbgFn		= pfn;
	session.m_dwDbgData = data;

	return 1;
}

//TOFIX move to CSshSession
void sftp_progress_fn(CSshSession &session, int (* m_pProgress)(__int64, __int64, unsigned long), unsigned long data)
{
	session.m_pfnProgress = m_pProgress;
	session.m_data		  = data;
}

//TOFIX move to CSshSession
bool sftp_connected(CSshSession &session)
{
	if(session.back != NULL && session.s != NULL)
		return true;
	return false;
}