/*
 * Copyright (c) 1992, Brian Berliner and Jeff Polk
 * Copyright (c) 1989-1992, Brian Berliner
 * 
 * You may distribute under the terms of the GNU General Public License as
 * specified in the README file that comes with the CVS 1.4 kit.
 * 
 * Print Log Information
 * 
 * This line exists solely to test some pcl-cvs/ChangeLog stuff.  You
 * can delete it, if indeed it's still here when you read it.  -Karl
 *
 * Prints the RCS "log" (rlog) information for the specified files.  With no
 * argument, prints the log information for all the files in the directory
 * (recursive by default).
 */

#include "cvs.h"

/* This structure holds information parsed from the -r option.  */

struct option_revlist
{
    /* The next -r option.  */
    struct option_revlist *next;
    /* The first revision to print.  This is NULL if the range is
       :rev, or if no revision is given.  */
    char *first;
    /* The last revision to print.  This is NULL if the range is rev:,
       or if no revision is given.  If there is no colon, first and
       last are the same.  */
    char *last;
    /* Nonzero if there was a trailing `.', which means to print only
       the head revision of a branch.  */
    int branchhead;
};

/* This structure holds information derived from option_revlist given
   a particular RCS file.  */

struct revlist
{
    /* The next pair.  */
    struct revlist *next;
    /* The first numeric revision to print.  */
    char *first;
    /* The last numeric revision to print.  */
    char *last;
    /* The number of fields in these revisions (one more than
       numdots).  */
    int fields;
};

/* This structure holds information parsed from the -d option.  */

struct datelist
{
    /* The next date.  */
    struct datelist *next;
    /* The starting date.  */
    char *start;
    /* The ending date.  */
    char *end;
    /* Nonzero if the range is inclusive rather than exclusive.  */
    int inclusive;
};

/* This structure is used to pass information through start_recursion.  */
struct log_data
{
    /* Nonzero if the -R option was given, meaning that only the name
       of the RCS file should be printed.  */
    int nameonly;
    /* Nonzero if the -h option was given, meaning that only header
       information should be printed.  */
    int header;
    /* Nonzero if the -t option was given, meaning that only the
       header and the descriptive text should be printed.  */
    int long_header;
    /* Nonzero if the -N option was seen, meaning that tag information
       should not be printed.  */
    int notags;
    /* Nonzero if the -b option was seen, meaning that only revisions
       on the default branch should be printed.  */
    int default_branch;
    /* If not NULL, the value given for the -r option, which lists
       sets of revisions to be printed.  */
    struct option_revlist *revlist;
    /* If not NULL, the date pairs given for the -d option, which
       select date ranges to print.  */
    struct datelist *datelist;
    /* If not NULL, the single dates given for the -d option, which
       select specific revisions to print based on a date.  */
    struct datelist *singledatelist;
    /* If not NULL, the list of states given for the -s option, which
       only prints revisions of given states.  */
    List *statelist;
    /* If not NULL, the list of login names given for the -w option,
       which only prints revisions checked in by given users.  */
    List *authorlist;
};

/* This structure is used to pass information through walklist.  */
struct log_data_and_rcs
{
    struct log_data *log_data;
    struct revlist *revlist;
    RCSNode *rcs;
};

static Dtype log_dirproc PROTO ((void *callerdat, char *dir,
				 char *repository, char *update_dir,
				 List *entries));
static int log_fileproc PROTO ((void *callerdat, struct file_info *finfo));
static struct option_revlist *log_parse_revlist PROTO ((const char *));
static void log_parse_date PROTO ((struct log_data *, const char *));
static void log_parse_list PROTO ((List **, const char *));
static struct revlist *log_expand_revlist PROTO ((RCSNode *,
						  struct option_revlist *,
						  int));
static void log_free_revlist PROTO ((struct revlist *));
static int log_version_requested PROTO ((struct log_data *, struct revlist *,
					 RCSNode *, RCSVers *));
static int log_symbol PROTO ((Node *, void *));
static int log_count PROTO ((Node *, void *));
static int log_fix_singledate PROTO ((Node *, void *));
static int log_count_print PROTO ((Node *, void *));
static void log_tree PROTO ((struct log_data *, struct revlist *,
			     RCSNode *, const char *));
static void log_abranch PROTO ((struct log_data *, struct revlist *,
				RCSNode *, const char *));
static void log_version PROTO ((struct log_data *, struct revlist *,
				RCSNode *, RCSVers *, int));
static int log_branch PROTO ((Node *, void *));
static int version_compare PROTO ((const char *, const char *, int));

static const char *const log_usage[] =
{
    "Usage: %s %s [-lRhtNb] [-r[revisions]] [-d dates] [-s states]\n",
    "    [-w[logins]] [files...]\n",
    "\t-l\tLocal directory only, no recursion.\n",
    "\t-R\tOnly print name of RCS file.\n",
    "\t-h\tOnly print header.\n",
    "\t-t\tOnly print header and descriptive text.\n",
    "\t-N\tDo not list tags.\n",
    "\t-b\tOnly list revisions on the default branch.\n",
    "\t-r[revisions]\tSpecify revision(s)s to list.\n",
    "\t-d dates\tSpecify dates (D1<D2 for range, D for latest before).\n",
    "\t-s states\tOnly list revisions with specified states.\n",
    "\t-w[logins]\tOnly list revisions checked in by specified logins.\n",
    NULL
};

int
cvslog (argc, argv)
    int argc;
    char **argv;
{
    int c;
    int err = 0;
    int local = 0;
    struct log_data log_data;
    struct option_revlist *rl, **prl;

    if (argc == -1)
	usage (log_usage);

    memset (&log_data, 0, sizeof log_data);

    optind = 1;
    while ((c = getopt (argc, argv, "bd:hlNRr::s:tw::")) != -1)
    {
	switch (c)
	{
	    case 'b':
		log_data.default_branch = 1;
		break;
	    case 'd':
		log_parse_date (&log_data, optarg);
		break;
	    case 'h':
		log_data.header = 1;
		break;
	    case 'l':
		local = 1;
		break;
	    case 'N':
		log_data.notags = 1;
		break;
	    case 'R':
		log_data.nameonly = 1;
		break;
	    case 'r':
		rl = log_parse_revlist (optarg);
		for (prl = &log_data.revlist;
		     *prl != NULL;
		     prl = &(*prl)->next)
		    ;
		*prl = rl;
		break;
	    case 's':
		log_parse_list (&log_data.statelist, optarg);
		break;
	    case 't':
		log_data.long_header = 1;
		break;
	    case 'w':
		if (optarg != NULL)
		    log_parse_list (&log_data.authorlist, optarg);
		else
		    log_parse_list (&log_data.authorlist, getcaller ());
		break;
	    case '?':
	    default:
		usage (log_usage);
		break;
	}
    }

    wrap_setup ();

#ifdef CLIENT_SUPPORT
    if (client_active)
    {
	int i;

	/* We're the local client.  Fire up the remote server.  */
	start_server ();
	
	ign_setup ();

	for (i = 1; i < argc && argv[i][0] == '-'; i++)
	  send_arg (argv[i]);

	send_file_names (argc - i, argv + i, SEND_EXPAND_WILD);
/* FIXME:  We shouldn't have to send current files to get log entries, but it
   doesn't work yet and I haven't debugged it.  So send the files --
   it's slower but it works.  gnu@cygnus.com  Apr94  */
	send_files (argc - i, argv + i, local, 0, 0);

	send_to_server ("log\012", 0);
        err = get_responses_and_close ();
	return err;
    }
#endif

    err = start_recursion (log_fileproc, (FILESDONEPROC) NULL, log_dirproc,
			   (DIRLEAVEPROC) NULL, (void *) &log_data,
			   argc - optind, argv + optind, local,
			   W_LOCAL | W_REPOS | W_ATTIC, 0, 1,
			   (char *) NULL, 1);
    return (err);
}

/*
 * Parse a revision list specification.
 */

static struct option_revlist *
log_parse_revlist (argstring)
    const char *argstring;
{
    char *copy;
    struct option_revlist *ret, **pr;

    /* Unfortunately, rlog accepts -r without an argument to mean that
       latest revision on the default branch, so we must support that
       for compatibility.  */
    if (argstring == NULL)
    {
	ret = (struct option_revlist *) xmalloc (sizeof *ret);
	ret->first = NULL;
	ret->last = NULL;
	ret->next = NULL;
	ret->branchhead = 0;
	return ret;
    }

    ret = NULL;
    pr = &ret;

    /* Copy the argument into memory so that we can change it.  We
       don't want to change the argument because, at least as of this
       writing, we will use it if we send the arguments to the server.
       We never bother to free up our copy.  */
    copy = xstrdup (argstring);
    while (copy != NULL)
    {
	char *comma;
	char *cp;
	char *first, *last;
	struct option_revlist *r;

	comma = strchr (copy, ',');
	if (comma != NULL)
	    *comma++ = '\0';

	first = copy;
	cp = strchr (copy, ':');
	if (cp == NULL)
	    last = copy;
	else
	{
	    *cp++ = '\0';
	    last = cp;
	}

	if (*first == '\0')
	    first = NULL;
	if (*last == '\0')
	    last = NULL;

	r = (struct option_revlist *) xmalloc (sizeof *r);
	r->next = NULL;
	r->first = first;
	r->last = last;
	if (first != last
	    || first[strlen (first) - 1] != '.')
	{
	    r->branchhead = 0;
	}
	else
	{
	    r->branchhead = 1;
	    first[strlen (first) - 1] = '\0';
	}

	*pr = r;
	pr = &r->next;

	copy = comma;
    }

    return ret;
}

/*
 * Parse a date specification.
 */
static void
log_parse_date (log_data, argstring)
    struct log_data *log_data;
    const char *argstring;
{
    char *orig_copy, *copy;

    /* Copy the argument into memory so that we can change it.  We
       don't want to change the argument because, at least as of this
       writing, we will use it if we send the arguments to the server.  */
    copy = xstrdup (argstring);
    orig_copy = copy;
    while (copy != NULL)
    {
	struct datelist *nd, **pd;
	char *cpend, *cp, *ds, *de;

	nd = (struct datelist *) xmalloc (sizeof *nd);

	cpend = strchr (copy, ';');
	if (cpend != NULL)
	    *cpend++ = '\0';

	pd = &log_data->datelist;
	nd->inclusive = 0;

	if ((cp = strchr (copy, '>')) != NULL)
	{
	    *cp++ = '\0';
	    if (*cp == '=')
	    {
		++cp;
		nd->inclusive = 1;
	    }
	    ds = cp;
	    de = copy;
	}
	else if ((cp = strchr (copy, '<')) != NULL)
	{
	    *cp++ = '\0';
	    if (*cp == '=')
	    {
		++cp;
		nd->inclusive = 1;
	    }
	    ds = copy;
	    de = cp;
	}
	else
	{
	    ds = NULL;
	    de = copy;
	    pd = &log_data->singledatelist;
	}

	if (ds == NULL)
	    nd->start = NULL;
	else if (*ds != '\0')
	    nd->start = Make_Date (ds);
	else
	{
	  /* 1970 was the beginning of time, as far as get_date and
	     Make_Date are concerned.  FIXME: That is true only if time_t
	     is a POSIX-style time and there is nothing in ANSI that
	     mandates that.  It would be cleaner to set a flag saying
	     whether or not there is a start date.  */
	    nd->start = Make_Date ("1/1/1970 UTC");
	}

	if (*de != '\0')
	    nd->end = Make_Date (de);
	else
	{
	    /* We want to set the end date to some time sufficiently far
	       in the future to pick up all revisions that have been
	       created since the specified date and the time `cvs log'
	       completes.  FIXME: The date in question only makes sense
	       if time_t is a POSIX-style time and it is 32 bits
	       and signed.  We should instead be setting a flag saying
	       whether or not there is an end date.  Note that using
	       something like "next week" would break the testsuite (and,
	       perhaps less importantly, loses if the clock is set grossly
	       wrong).  */
	    nd->end = Make_Date ("2038-01-01");
	}

	nd->next = *pd;
	*pd = nd;

	copy = cpend;
    }

    free (orig_copy);
}

/*
 * Parse a comma separated list of items, and add each one to *PLIST.
 */
static void
log_parse_list (plist, argstring)
    List **plist;
    const char *argstring;
{
    while (1)
    {
	Node *p;
	char *cp;

	p = getnode ();

	cp = strchr (argstring, ',');
	if (cp == NULL)
	    p->key = xstrdup (argstring);
	else
	{
	    size_t len;

	    len = cp - argstring;
	    p->key = xmalloc (len + 1);
	    strncpy (p->key, argstring, len);
	    p->key[len + 1] = '\0';
	}

	if (*plist == NULL)
	    *plist = getlist ();
	if (addnode (*plist, p) != 0)
	    freenode (p);

	if (cp == NULL)
	    break;

	argstring = cp + 1;
    }
}

/*
 * Do an rlog on a file
 */
static int
log_fileproc (callerdat, finfo)
    void *callerdat;
    struct file_info *finfo;
{
    struct log_data *log_data = (struct log_data *) callerdat;
    Node *p;
    RCSNode *rcsfile;
    char buf[50];
    struct revlist *revlist;
    struct log_data_and_rcs log_data_and_rcs;

    if ((rcsfile = finfo->rcs) == NULL)
    {
	/* no rcs file.  What *do* we know about this file? */
	p = findnode (finfo->entries, finfo->file);
	if (p != NULL)
	{
	    Entnode *e;
	    
	    e = (Entnode *) p->data;
	    if (e->version[0] == '0' || e->version[1] == '\0')
	    {
		if (!really_quiet)
		    error (0, 0, "%s has been added, but not committed",
			   finfo->file);
		return(0);
	    }
	}
	
	if (!really_quiet)
	    error (0, 0, "nothing known about %s", finfo->file);
	
	return (1);
    }

    if (log_data->nameonly)
    {
	cvs_output (rcsfile->path, 0);
	cvs_output ("\n", 1);
	return 0;
    }

    /* We will need all the information in the RCS file.  */
    RCS_fully_parse (rcsfile);

    /* Turn any symbolic revisions in the revision list into numeric
       revisions.  */
    revlist = log_expand_revlist (rcsfile, log_data->revlist,
				  log_data->default_branch);

    /* The output here is intended to be exactly compatible with the
       output of rlog.  I'm not sure whether this code should be here
       or in rcs.c; I put it here because it is specific to the log
       function, even though it uses information gathered by the
       functions in rcs.c.  */

    cvs_output ("\n", 1);

    cvs_output ("RCS file: ", 0);
    cvs_output (rcsfile->path, 0);

    cvs_output ("\nWorking file: ", 0);
    if (finfo->update_dir[0] == '\0')
	cvs_output (finfo->file, 0);
    else
    {
	cvs_output (finfo->update_dir, 0);
	cvs_output ("/", 0);
	cvs_output (finfo->file, 0);

    }

    cvs_output ("\nhead:", 0);
    if (rcsfile->head != NULL)
    {
	cvs_output (" ", 1);
	cvs_output (rcsfile->head, 0);
    }

    cvs_output ("\nbranch:", 0);
    if (rcsfile->branch != NULL)
    {
	cvs_output (" ", 1);
	cvs_output (rcsfile->branch, 0);
    }

    cvs_output ("\nlocks:", 0);
    if (rcsfile->other != NULL)
    {
	p = findnode (rcsfile->other, "strict");
	if (p != NULL)
	    cvs_output (" strict", 0);
	p = findnode (rcsfile->other, "locks");
	if (p != NULL && p->data != NULL)
	{
	    char *f, *cp;

	    f = xstrdup (p->data);
	    cp = f;
	    while (*cp != '\0')
	    {
		char *cp2, *locker, *version;
		RCSVers *vnode;

		locker = cp;

		cp2 = strchr (cp, ':');
		if (cp2 == NULL)
		{
		    error (0, 0, "warning: bad locks field in RCS file `%s'",
			   finfo->fullname);
		    break;
		}

		*cp2 = '\0';

		cvs_output ("\n\t", 2);
		cvs_output (cp, cp2 - cp);
		cvs_output (": ", 2);

		cp = cp2 + 1;
		while (isspace (*cp) && *cp != '\0')
		    ++cp;

		version = cp;

		cp2 = cp;
		while (! isspace (*cp2) && *cp2 != '\0')
		    ++cp2;

		cvs_output (cp, cp2 - cp);

		if (*cp2 == '\0')
		    cp = cp2;
		else
		{
		    *cp2 = '\0';
		    cp = cp2 + 1;
		    while (isspace (*cp) && *cp != '\0')
			++cp;
		}

		p = findnode (rcsfile->versions, version);
		if (p == NULL)
		    error (0, 0,
			   "warning: lock for missing version `%s' in `%s'",
			   version, finfo->fullname);
		else
		{
		    vnode = (RCSVers *) p->data;
		    p = getnode ();
		    p->type = RCSFIELD;
		    p->key = xstrdup (";locker");
		    p->data = xstrdup (locker);
		    if (addnode (vnode->other, p) != 0)
		    {
			error (0, 0,
			       "warning: duplicate lock for `%s' in `%s'",
			       version, finfo->fullname);
			freenode (p);
		    }
		}
	    }

	    free (f);
	}
    }

    cvs_output ("\naccess list:", 0);
    if (rcsfile->other != NULL)
    {
	p = findnode (rcsfile->other, "access");
	if (p != NULL && p->data != NULL)
	{
	    const char *cp;

	    cp = p->data;
	    while (*cp != '\0')
	    {
		const char *cp2;

		cvs_output ("\n\t", 2);
		cp2 = cp;
		while (! isspace (*cp2) && *cp2 != '\0')
		    ++cp2;
		cvs_output (cp, cp2 - cp);
		cp = cp2;
		while (isspace (*cp) && *cp != '\0')
		    ++cp;
	    }
	}
    }

    if (! log_data->notags)
    {
	List *syms;

	cvs_output ("\nsymbolic names:", 0);
	syms = RCS_symbols (rcsfile);
	walklist (syms, log_symbol, NULL);
    }

    cvs_output ("\nkeyword substitution: ", 0);
    if (rcsfile->expand == NULL)
	cvs_output ("kv", 2);
    else
	cvs_output (rcsfile->expand, 0);

    cvs_output ("\ntotal revisions: ", 0);
    sprintf (buf, "%d", walklist (rcsfile->versions, log_count, NULL));
    cvs_output (buf, 0);

    if (! log_data->header && ! log_data->long_header)
    {
	cvs_output (";\tselected revisions: ", 0);

	log_data_and_rcs.log_data = log_data;
	log_data_and_rcs.revlist = revlist;
	log_data_and_rcs.rcs = rcsfile;

	/* If any single dates were specified, we need to identify the
	   revisions they select.  Each one selects the single
	   revision, which is otherwise selected, of that date or
	   earlier.  The log_fix_singledate routine will fill in the
	   start date for each specific revision.  */
	if (log_data->singledatelist != NULL)
	    walklist (rcsfile->versions, log_fix_singledate,
		      (void *) &log_data_and_rcs);

	sprintf (buf, "%d", walklist (rcsfile->versions, log_count_print,
				      (void *) &log_data_and_rcs));
	cvs_output (buf, 0);
    }

    cvs_output ("\n", 1);

    if (! log_data->header || log_data->long_header)
    {
	cvs_output ("description:\n", 0);
	if (rcsfile->other != NULL)
	{
	    p = findnode (rcsfile->other, "desc");
	    if (p != NULL && p->data != NULL)
		cvs_output (p->data, 0);
	}
    }

    if (! log_data->header && ! log_data->long_header && rcsfile->head != NULL)
    {
	p = findnode (rcsfile->versions, rcsfile->head);
	if (p == NULL)
	    error (1, 0, "can not find head revision in `%s'",
		   finfo->fullname);
	while (p != NULL)
	{
	    RCSVers *vers;

	    vers = (RCSVers *) p->data;
	    log_version (log_data, revlist, rcsfile, vers, 1);
	    if (vers->next == NULL)
		p = NULL;
	    else
	    {
		p = findnode (rcsfile->versions, vers->next);
		if (p == NULL)
		    error (1, 0, "can not find next revision `%s' in `%s'",
			   vers->next, finfo->fullname);
	    }
	}

	log_tree (log_data, revlist, rcsfile, rcsfile->head);
    }

    cvs_output("\
=============================================================================\n",
	       0);

    /* Free up the new revlist and restore the old one.  */
    log_free_revlist (revlist);

    /* If singledatelist is not NULL, free up the start dates we added
       to it.  */
    if (log_data->singledatelist != NULL)
    {
	struct datelist *d;

	for (d = log_data->singledatelist; d != NULL; d = d->next)
	{
	    if (d->start != NULL)
		free (d->start);
	    d->start = NULL;
	}
    }

    return 0;
}

/*
 * Fix up a revision list in order to compare it against versions.
 * Expand any symbolic revisions.
 */
static struct revlist *
log_expand_revlist (rcs, revlist, default_branch)
    RCSNode *rcs;
    struct option_revlist *revlist;
    int default_branch;
{
    struct option_revlist *r;
    struct revlist *ret, **pr;

    ret = NULL;
    pr = &ret;
    for (r = revlist; r != NULL; r = r->next)
    {
	struct revlist *nr;

	nr = (struct revlist *) xmalloc (sizeof *nr);

	if (r->first == NULL && r->last == NULL)
	{
	    /* If both first and last are NULL, it means that we want
	       just the head of the default branch, which is RCS_head.  */
	    nr->first = RCS_head (rcs);
	    nr->last = xstrdup (nr->first);
	    nr->fields = numdots (nr->first) + 1;
	}
	else if (r->branchhead)
	{
	    char *branch;

	    /* Print just the head of the branch.  */
	    if (isdigit (r->first[0]))
		nr->first = RCS_getbranch (rcs, r->first, 1);
	    else
	    {
		branch = RCS_whatbranch (rcs, r->first);
		if (branch == NULL)
		{
		    error (0, 0, "warning: `%s' is not a branch in `%s'",
			   r->first, rcs->path);
		    free (nr);
		    continue;
		}
		nr->first = RCS_getbranch (rcs, branch, 1);
		free (branch);
	    }
	    if (nr->first == NULL)
	    {
		error (0, 0, "warning: no revision `%s' in `%s'",
		       r->first, rcs->path);
		free (nr);
		continue;
	    }
	    nr->last = xstrdup (nr->first);
	    nr->fields = numdots (nr->first) + 1;
	}
	else
	{
	    if (r->first == NULL || isdigit (r->first[0]))
		nr->first = xstrdup (r->first);
	    else
	    {
		if (RCS_nodeisbranch (rcs, r->first))
		    nr->first = RCS_whatbranch (rcs, r->first);
		else
		    nr->first = RCS_gettag (rcs, r->first, 1, (int *) NULL);
		if (nr->first == NULL)
		{
		    error (0, 0, "warning: no revision `%s' in `%s'",
			   r->first, rcs->path);
		    free (nr);
		    continue;
		}
	    }

	    if (r->last == r->first)
		nr->last = xstrdup (nr->first);
	    else if (r->last == NULL || isdigit (r->last[0]))
		nr->last = xstrdup (r->last);
	    else
	    {
		if (RCS_nodeisbranch (rcs, r->last))
		    nr->last = RCS_whatbranch (rcs, r->last);
		else
		    nr->last = RCS_gettag (rcs, r->last, 1, (int *) NULL);
		if (nr->last == NULL)
		{
		    error (0, 0, "warning: no revision `%s' in `%s'",
			   r->last, rcs->path);
		    if (nr->first != NULL)
			free (nr->first);
		    free (nr);
		    continue;
		}
	    }

	    /* Process the revision numbers the same way that rlog
               does.  This code is a bit cryptic for my tastes, but
               keeping the same implementation as rlog ensures a
               certain degree of compatibility.  */
	    if (r->first == NULL)
	    {
		nr->fields = numdots (nr->last) + 1;
		if (nr->fields < 2)
		    nr->first = xstrdup (".0");
		else
		{
		    char *cp;

		    nr->first = xstrdup (nr->last);
		    cp = strrchr (nr->first, '.');
		    strcpy (cp, ".0");
		}
	    }
	    else if (r->last == NULL)
	    {
		nr->fields = numdots (nr->first) + 1;
		nr->last = xstrdup (nr->first);
		if (nr->fields < 2)
		    nr->last[0] = '\0';
		else
		{
		    char *cp;

		    cp = strrchr (nr->last, '.');
		    *cp = '\0';
		}
	    }
	    else
	    {
		nr->fields = numdots (nr->first) + 1;
		if (nr->fields != numdots (nr->last) + 1
		    || (nr->fields > 2
			&& version_compare (nr->first, nr->last,
					    nr->fields - 1) != 0))
		{
		    error (0, 0,
			   "invalid branch or revision pair %s:%s in `%s'",
			   r->first, r->last, rcs->path);
		    free (nr->first);
		    free (nr->last);
		    free (nr);
		    continue;
		}
		if (version_compare (nr->first, nr->last, nr->fields) > 0)
		{
		    char *tmp;

		    tmp = nr->first;
		    nr->first = nr->last;
		    nr->last = tmp;
		}
	    }
	}

	nr->next = NULL;
	*pr = nr;
	pr = &nr->next;
    }

    /* If the default branch was requested, add a revlist entry for
       it.  This is how rlog handles this option.  */
    if (default_branch
	&& (rcs->head != NULL || rcs->branch != NULL))
    {
	struct revlist *nr;

	nr = (struct revlist *) xmalloc (sizeof *nr);
	if (rcs->branch != NULL)
	    nr->first = xstrdup (rcs->branch);
	else
	{
	    char *cp;

	    nr->first = xstrdup (rcs->head);
	    cp = strrchr (nr->first, '.');
	    *cp = '\0';
	}
	nr->last = xstrdup (nr->first);
	nr->fields = numdots (nr->first) + 1;

	nr->next = NULL;
	*pr = nr;
    }

    return ret;
}

/*
 * Free a revlist created by log_expand_revlist.
 */
static void
log_free_revlist (revlist)
    struct revlist *revlist;
{
    struct revlist *r;

    r = revlist;
    while (r != NULL)
    {
	struct revlist *next;

	if (r->first != NULL)
	    free (r->first);
	if (r->last != NULL)
	    free (r->last);
	next = r->next;
	free (r);
	r = next;
    }
}

/*
 * Return nonzero if a revision should be printed, based on the
 * options provided.
 */
static int
log_version_requested (log_data, revlist, rcs, vnode)
    struct log_data *log_data;
    struct revlist *revlist;
    RCSNode *rcs;
    RCSVers *vnode;
{
    /* Handle the list of states from the -s option.  */
    if (log_data->statelist != NULL)
    {
	Node *p;

	p = findnode (vnode->other, "state");
	if (p != NULL
	    && findnode (log_data->statelist, p->data) == NULL)
	{
	    return 0;
	}
    }

    /* Handle the list of authors from the -w option.  */
    if (log_data->authorlist != NULL)
    {
	if (vnode->author != NULL
	    && findnode (log_data->authorlist, vnode->author) == NULL)
	{
	    return 0;
	}
    }

    /* rlog considers all the -d options together when it decides
       whether to print a revision, so we must be compatible.  */
    if (log_data->datelist != NULL || log_data->singledatelist != NULL)
    {
	struct datelist *d;

	for (d = log_data->datelist; d != NULL; d = d->next)
	{
	    int cmp;

	    cmp = RCS_datecmp (vnode->date, d->start);
	    if (cmp > 0 || (cmp == 0 && d->inclusive))
	    {
		cmp = RCS_datecmp (vnode->date, d->end);
		if (cmp < 0 || (cmp == 0 && d->inclusive))
		    break;
	    }
	}

	if (d == NULL)
	{
	    /* Look through the list of specific dates.  We want to
	       select the revision with the exact date found in the
	       start field.  The commit code ensures that it is
	       impossible to check in multiple revisions of a single
	       file in a single second, so checking the date this way
	       should never select more than one revision.  */
	    for (d = log_data->singledatelist; d != NULL; d = d->next)
	    {
		if (d->start != NULL
		    && RCS_datecmp (vnode->date, d->start) == 0)
		{
		    break;
		}
	    }

	    if (d == NULL)
		return 0;
	}
    }

    /* If the -r or -b options were used, REVLIST will be non NULL,
       and we print the union of the specified revisions.  */
    if (revlist != NULL)
    {
	char *v;
	int vfields;
	struct revlist *r;

	/* This code is taken from rlog.  */
	v = vnode->version;
	vfields = numdots (v) + 1;
	for (r = revlist; r != NULL; r = r->next)
	{
	    if (vfields == r->fields + (r->fields & 1)
		&& version_compare (v, r->first, r->fields) >= 0
		&& version_compare (v, r->last, r->fields) <= 0)
	    {
		return 1;
	    }
	}

	/* If we get here, then the -b and/or the -r option was used,
           but did not match this revision, so we reject it.  */

	return 0;
    }

    /* By default, we print all revisions.  */
    return 1;
}

/*
 * Output a single symbol.  This is called via walklist.
 */
/*ARGSUSED*/
static int
log_symbol (p, closure)
    Node *p;
    void *closure;
{
    cvs_output ("\n\t", 2);
    cvs_output (p->key, 0);
    cvs_output (": ", 2);
    cvs_output (p->data, 0);
    return 0;
}

/*
 * Count the number of entries on a list.  This is called via walklist.
 */
/*ARGSUSED*/
static int
log_count (p, closure)
    Node *p;
    void *closure;
{
    return 1;
}

/*
 * Sort out a single date specification by narrowing down the date
 * until we find the specific selected revision.
 */
static int
log_fix_singledate (p, closure)
    Node *p;
    void *closure;
{
    struct log_data_and_rcs *data = (struct log_data_and_rcs *) closure;
    Node *pv;
    RCSVers *vnode;
    struct datelist *holdsingle, *holddate;
    int requested;

    pv = findnode (data->rcs->versions, p->key);
    if (pv == NULL)
	error (1, 0, "missing version `%s' in RCS file `%s'",
	       p->key, data->rcs->path);
    vnode = (RCSVers *) pv->data;

    /* We are only interested if this revision passes any other tests.
       Temporarily clear log_data->singledatelist to avoid confusing
       log_version_requested.  We also clear log_data->datelist,
       because rlog considers all the -d options together.  We don't
       want to reject a revision because it does not match a date pair
       if we are going to select it on the basis of the singledate.  */
    holdsingle = data->log_data->singledatelist;
    data->log_data->singledatelist = NULL;
    holddate = data->log_data->datelist;
    data->log_data->datelist = NULL;
    requested = log_version_requested (data->log_data, data->revlist,
				       data->rcs, vnode);
    data->log_data->singledatelist = holdsingle;
    data->log_data->datelist = holddate;

    if (requested)
    {
	struct datelist *d;

	/* For each single date, if this revision is before the
	   specified date, but is closer than the previously selected
	   revision, select it instead.  */
	for (d = data->log_data->singledatelist; d != NULL; d = d->next)
	{
	    if (RCS_datecmp (vnode->date, d->end) <= 0
		&& (d->start == NULL
		    || RCS_datecmp (vnode->date, d->start) > 0))
	    {
		if (d->start != NULL)
		    free (d->start);
		d->start = xstrdup (vnode->date);
	    }
	}
    }

    return 0;
}

/*
 * Count the number of revisions we are going to print.
 */
static int
log_count_print (p, closure)
    Node *p;
    void *closure;
{
    struct log_data_and_rcs *data = (struct log_data_and_rcs *) closure;
    Node *pv;

    pv = findnode (data->rcs->versions, p->key);
    if (pv == NULL)
	error (1, 0, "missing version `%s' in RCS file `%s'",
	       p->key, data->rcs->path);
    if (log_version_requested (data->log_data, data->revlist, data->rcs,
			       (RCSVers *) pv->data))
	return 1;
    else
	return 0;
}

/*
 * Print the list of changes, not including the trunk, in reverse
 * order for each branch.
 */
static void
log_tree (log_data, revlist, rcs, ver)
    struct log_data *log_data;
    struct revlist *revlist;
    RCSNode *rcs;
    const char *ver;
{
    Node *p;
    RCSVers *vnode;

    p = findnode (rcs->versions, ver);
    if (p == NULL)
	error (1, 0, "missing version `%s' in RCS file `%s'",
	       ver, rcs->path);
    vnode = (RCSVers *) p->data;
    if (vnode->next != NULL)
	log_tree (log_data, revlist, rcs, vnode->next);
    if (vnode->branches != NULL)
    {
	Node *head, *branch;

	/* We need to do the branches in reverse order.  This breaks
           the List abstraction, but so does most of the branch
           manipulation in rcs.c.  */
	head = vnode->branches->list;
	for (branch = head->prev; branch != head; branch = branch->prev)
	{
	    log_abranch (log_data, revlist, rcs, branch->key);
	    log_tree (log_data, revlist, rcs, branch->key);
	}
    }
}

/*
 * Log the changes for a branch, in reverse order.
 */
static void
log_abranch (log_data, revlist, rcs, ver)
    struct log_data *log_data;
    struct revlist *revlist;
    RCSNode *rcs;
    const char *ver;
{
    Node *p;
    RCSVers *vnode;

    p = findnode (rcs->versions, ver);
    if (p == NULL)
	error (1, 0, "missing version `%s' in RCS file `%s'",
	       ver, rcs->path);
    vnode = (RCSVers *) p->data;
    if (vnode->next != NULL)
	log_abranch (log_data, revlist, rcs, vnode->next);
    log_version (log_data, revlist, rcs, vnode, 0);
}

/*
 * Print the log output for a single version.
 */
static void
log_version (log_data, revlist, rcs, ver, trunk)
    struct log_data *log_data;
    struct revlist *revlist;
    RCSNode *rcs;
    RCSVers *ver;
    int trunk;
{
    Node *p;
    int year, mon, mday, hour, min, sec;
    char buf[100];
    Node *padd, *pdel;

    if (! log_version_requested (log_data, revlist, rcs, ver))
	return;

    cvs_output ("----------------------------\nrevision ", 0);
    cvs_output (ver->version, 0);

    p = findnode (ver->other, ";locker");
    if (p != NULL)
    {
	cvs_output ("\tlocked by: ", 0);
	cvs_output (p->data, 0);
	cvs_output (";", 1);
    }

    cvs_output ("\ndate: ", 0);
    (void) sscanf (ver->date, SDATEFORM, &year, &mon, &mday, &hour, &min,
		   &sec);
    if (year < 1900)
	year += 1900;
    sprintf (buf, "%04d/%02d/%02d %02d:%02d:%02d", year, mon, mday,
	     hour, min, sec);
    cvs_output (buf, 0);

    cvs_output (";  author: ", 0);
    cvs_output (ver->author, 0);

    cvs_output (";  state: ", 0);
    p = findnode (ver->other, "state");
    cvs_output (p->data, 0);
    cvs_output (";", 1);

    if (! trunk)
    {
	padd = findnode (ver->other, ";add");
	pdel = findnode (ver->other, ";delete");
    }
    else if (ver->next == NULL)
    {
	padd = NULL;
	pdel = NULL;
    }
    else
    {
	Node *nextp;
	RCSVers *nextver;

	nextp = findnode (rcs->versions, ver->next);
	if (nextp == NULL)
	    error (1, 0, "missing version `%s' in `%s'", ver->next,
		   rcs->path);
	nextver = (RCSVers *) nextp->data;
	pdel = findnode (nextver->other, ";add");
	padd = findnode (nextver->other, ";delete");
    }

    if (padd != NULL)
    {
	cvs_output ("  lines: +", 0);
	cvs_output (padd->data, 0);
	cvs_output (" -", 2);
	cvs_output (pdel->data, 0);
    }

    if (ver->branches != NULL)
    {
	cvs_output ("\nbranches:", 0);
	walklist (ver->branches, log_branch, (void *) NULL);
    }

    cvs_output ("\n", 1);

    p = findnode (ver->other, "log");
    if (p == NULL || p->data[0] == '\0')
	cvs_output ("*** empty log message ***\n", 0);
    else
    {
	/* FIXME: Technically, the log message could contain a null
           byte.  */
	cvs_output (p->data, 0);
	if (p->data[strlen (p->data) - 1] != '\n')
	    cvs_output ("\n", 1);
    }
}

/*
 * Output a branch version.  This is called via walklist.
 */
/*ARGSUSED*/
static int
log_branch (p, closure)
    Node *p;
    void *closure;
{
    cvs_output ("  ", 2);
    if ((numdots (p->key) & 1) == 0)
	cvs_output (p->key, 0);
    else
    {
	char *f, *cp;

	f = xstrdup (p->key);
	cp = strrchr (f, '.');
	*cp = '\0';
	cvs_output (f, 0);
	free (f);
    }
    cvs_output (";", 1);
    return 0;
}

/*
 * Print a warm fuzzy message
 */
/* ARGSUSED */
static Dtype
log_dirproc (callerdat, dir, repository, update_dir, entries)
    void *callerdat;
    char *dir;
    char *repository;
    char *update_dir;
    List *entries;
{
    if (!isdir (dir))
	return (R_SKIP_ALL);

    if (!quiet)
	error (0, 0, "Logging %s", update_dir);
    return (R_PROCESS);
}

/*
 * Compare versions.  This is taken from RCS compartial.
 */
static int
version_compare (v1, v2, len)
    const char *v1;
    const char *v2;
    int len;
{
    while (1)
    {
	int d1, d2, r;

	if (*v1 == '\0')
	    return 1;
	if (*v2 == '\0')
	    return -1;

	while (*v1 == '0')
	    ++v1;
	for (d1 = 0; isdigit (v1[d1]); ++d1)
	    ;

	while (*v2 == '0')
	    ++v2;
	for (d2 = 0; isdigit (v2[d2]); ++d2)
	    ;

	if (d1 != d2)
	    return d1 < d2 ? -1 : 1;

	r = memcmp (v1, v2, d1);
	if (r != 0)
	    return r;

	--len;
	if (len == 0)
	    return 0;

	v1 += d1;
	v2 += d1;

	if (*v1 == '.')
	    ++v1;
	if (*v2 == '.')
	    ++v2;
    }
}
