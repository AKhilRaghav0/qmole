/* main.c

Copyright (C) 1999-2003 Tom Gilbert.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies of the Software and its documentation and acknowledgment shall be
given in the documentation and software packages that this Software was
used.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include "feh.h"
#include "filelist.h"
#include "winwidget.h"
#include "timers.h"
#include "options.h"
#include "events.h"
#include "support.h"

char **cmdargv = NULL;
int cmdargc = 0;
int call_level = 0;
char *mode = NULL;

int
main(int argc, char **argv)
{
   D_ENTER(4);
   atexit(feh_clean_exit);

   init_parse_options(argc, argv);

   init_x_and_imlib();

   feh_event_init();

   if (opt.index)
      init_index_mode();
   else if (opt.collage)
      init_collage_mode();
   else if (opt.multiwindow)
      init_multiwindow_mode();
   else if (opt.list || opt.customlist)
      init_list_mode();
   else if (opt.loadables)
      init_loadables_mode();
   else if (opt.unloadables)
      init_unloadables_mode();
   else if (opt.thumbs)
      init_thumbnail_mode();
   else if(opt.bgmode)
   {
      feh_wm_set_bg_file(opt.output_file, opt.bgmode);
      exit(0);
   }
/*   else if (opt.fmmode)
   {
      fmmode();
      opt.slideshow = 1;
      init_slideshow_mode(); 
   }
 */
   else
   {
      /* Slideshow mode is the default. Because it's spiffy */
      opt.slideshow = 1;
      init_slideshow_mode();
   }

   /* main event loop */
   while (feh_main_iteration(1));

   D_RETURN(4,0);
}


/* Return 0 to stop iterating, 1 if ok to continue. */
int
feh_main_iteration(int block)
{
   static int first = 1;
   static int xfd = 0;
   static int fdsize = 0;
   static double pt = 0.0;
   XEvent ev;
   struct timeval tval;
   fd_set fdset;
   int count = 0;
   double t1 = 0.0, t2 = 0.0;
   fehtimer ft;

   D_ENTER(5);

   if (window_num == 0)
      D_RETURN(5,0);

   if (first)
   {
      /* Only need to set these up the first time */
      xfd = ConnectionNumber(disp);
      fdsize = xfd + 1;
      pt = feh_get_time();
      first = 0;
   }

   /* Timers */
   t1 = feh_get_time();
   t2 = t1 - pt;
   pt = t1;
   while (XPending(disp))
   {
      XNextEvent(disp, &ev);
      if (ev_handler[ev.type])
         (*(ev_handler[ev.type])) (&ev);

      if (window_num == 0)
         D_RETURN(5,0);
   }
   XFlush(disp);

   feh_redraw_menus();

   FD_ZERO(&fdset);
   FD_SET(xfd, &fdset);

   /* Timers */
   ft = first_timer;
   /* Don't do timers if we're zooming/panning/etc or if we are paused*/
   if (ft && (opt.mode == MODE_NORMAL) && !opt.paused)
   {
      D(5,("There are timers in the queue\n"));
      if (ft->just_added)
      {
         D(5,("The first timer has just been added\n"));
         D(5,("ft->in = %f\n", ft->in));
         ft->just_added = 0;
         t1 = ft->in;
      }
      else
      {
         D(5,("The first timer was not just added\n"));
         t1 = ft->in - t2;
         if (t1 < 0.0)
            t1 = 0.0;
         ft->in = t1;
      }
      
      XSync(disp, False);
      D(5,("I next need to action a timer in %f seconds\n", t1));
      /* Only do a blocking select if there's a timer due, or no events
         waiting */
      if (t1 == 0.0 || (block && !XPending(disp)))
      {
         tval.tv_sec = (long) t1;
         tval.tv_usec = (long) ((t1 - ((double) tval.tv_sec)) * 1000000);
         if (tval.tv_sec < 0)
            tval.tv_sec = 0;
         if (tval.tv_usec <= 1000)
            tval.tv_usec = 1000;
         errno = 0;
         D(5,("Performing blocking select - waiting for timer or event\n"));
         count = select(fdsize, &fdset, NULL, NULL, &tval);
         if ((count < 0)
             && ((errno == ENOMEM) || (errno == EINVAL) || (errno == EBADF)))
            eprintf("Connection to X display lost");
         if ((ft) && (count == 0))
         {
            /* This means the timer is due to be executed. If count was > 0,
               that would mean an X event had woken us, we're not interested
               in that */
            feh_handle_timer();
         }
      }
   }
   else
   {
      /* Don't block if there are events in the queue. That's a bit rude ;-) */
      if (block && !XPending(disp))
      {
         errno = 0;
         D(5,("Performing blocking select - no timers, or zooming\n"));
         count = select(fdsize, &fdset, NULL, NULL, NULL);
         if ((count < 0)
             && ((errno == ENOMEM) || (errno == EINVAL) || (errno == EBADF)))
            eprintf("Connection to X display lost");
      }
   }
   if (window_num == 0)
      D_RETURN(5,0);
   D_RETURN(5,1);
}


void
feh_clean_exit(void)
{
   D_ENTER(4);

   delete_rm_files();

   if (opt.filelistfile)
      feh_write_filelist(filelist, opt.filelistfile);

   D_RETURN_(4);
}
