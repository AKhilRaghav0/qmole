/* Automatically generated by gen_defs.  Do not edit! */

#ifdef HELP_C
const char *HelpStrings[] = {
	N_("null operation"),
	N_("end of conditional execution (noop)"),
	N_("force viewing of attachment using mailcap"),
	N_("view attachment as text"),
	N_("Toggle display of subparts"),
	N_("move to the bottom of the page"),
	N_("remail a message to another user"),
	N_("select a new file in this directory"),
	N_("view file"),
	N_("display the currently selected file's name"),
	N_("subscribe to current mailbox (IMAP only)"),
	N_("unsubscribe from current mailbox (IMAP only)"),
	N_("toggle view all/subscribed mailboxes (IMAP only)"),
	N_("list mailboxes with new mail"),
	N_("change directories"),
	N_("check mailboxes for new mail"),
	N_("attach file(s) to this message"),
	N_("attach message(s) to this message"),
	N_("edit the BCC list"),
	N_("edit the CC list"),
	N_("edit attachment description"),
	N_("edit attachment transfer-encoding"),
	N_("enter a file to save a copy of this message in"),
	N_("edit the file to be attached"),
	N_("edit the from field"),
	N_("edit the message with headers"),
	N_("edit the message"),
	N_("edit attachment using mailcap entry"),
	N_("edit the Reply-To field"),
	N_("edit the subject of this message"),
	N_("edit the TO list"),
	N_("create a new mailbox (IMAP only)"),
	N_("edit attachment content type"),
	N_("get a temporary copy of an attachment"),
	N_("run ispell on the message"),
	N_("compose new attachment using mailcap entry"),
	N_("toggle recoding of this attachment"),
	N_("save this message to send later"),
	N_("rename/move an attached file"),
	N_("send the message"),
	N_("toggle disposition between inline/attachment"),
	N_("toggle whether to delete file after sending it"),
	N_("update an attachment's encoding info"),
	N_("write the message to a folder"),
	N_("copy a message to a file/mailbox"),
	N_("create an alias from a message sender"),
	N_("move entry to bottom of screen"),
	N_("move entry to middle of screen"),
	N_("move entry to top of screen"),
	N_("make decoded (text/plain) copy"),
	N_("make decoded copy (text/plain) and delete"),
	N_("delete the current entry"),
	N_("delete the current mailbox (IMAP only)"),
	N_("delete all messages in subthread"),
	N_("delete all messages in thread"),
	N_("display full address of sender"),
	N_("display message and toggle header weeding"),
	N_("display a message"),
	N_("edit the raw message"),
	N_("delete the char in front of the cursor"),
	N_("move the cursor one character to the left"),
	N_("move the cursor to the beginning of the word"),
	N_("jump to the beginning of the line"),
	N_("cycle among incoming mailboxes"),
	N_("complete filename or alias"),
	N_("complete address with query"),
	N_("delete the char under the cursor"),
	N_("jump to the end of the line"),
	N_("move the cursor one character to the right"),
	N_("move the cursor to the end of the word"),
	N_("scroll down through the history list"),
	N_("scroll up through the history list"),
	N_("delete chars from cursor to end of line"),
	N_("delete chars from the cursor to the end of the word"),
	N_("delete all chars on the line"),
	N_("delete the word in front of the cursor"),
	N_("quote the next typed key"),
	N_("transpose character under cursor with previous"),
	N_("capitalize the word"),
	N_("convert the word to lower case"),
	N_("convert the word to upper case"),
	N_("enter a muttrc command"),
	N_("enter a file mask"),
	N_("exit this menu"),
	N_("filter attachment through a shell command"),
	N_("move to the first entry"),
	N_("toggle a message's 'important' flag"),
	N_("forward a message with comments"),
	N_("select the current entry"),
	N_("reply to all recipients"),
	N_("scroll down 1/2 page"),
	N_("scroll up 1/2 page"),
	N_("this screen"),
	N_("jump to an index number"),
	N_("move to the last entry"),
	N_("reply to specified mailing list"),
	N_("execute a macro"),
	N_("compose a new mail message"),
	N_("break the thread in two"),
	N_("open a different folder"),
	N_("open a different folder in read only mode"),
	N_("clear a status flag from a message"),
	N_("delete messages matching a pattern"),
	N_("force retrieval of mail from IMAP server"),
	N_("logout from all IMAP servers"),
	N_("retrieve mail from POP server"),
	N_("move to the first message"),
	N_("move to the last message"),
	N_("show only messages matching a pattern"),
	N_("link tagged message to the current one"),
	N_("open next mailbox with new mail"),
	N_("jump to the next new message"),
	N_("jump to the next new or unread message"),
	N_("jump to the next subthread"),
	N_("jump to the next thread"),
	N_("move to the next undeleted message"),
	N_("jump to the next unread message"),
	N_("jump to parent message in thread"),
	N_("jump to previous thread"),
	N_("jump to previous subthread"),
	N_("move to the previous undeleted message"),
	N_("jump to the previous new message"),
	N_("jump to the previous new or unread message"),
	N_("jump to the previous unread message"),
	N_("mark the current thread as read"),
	N_("mark the current subthread as read"),
	N_("set a status flag on a message"),
	N_("save changes to mailbox"),
	N_("tag messages matching a pattern"),
	N_("undelete messages matching a pattern"),
	N_("untag messages matching a pattern"),
	N_("move to the middle of the page"),
	N_("move to the next entry"),
	N_("scroll down one line"),
	N_("move to the next page"),
	N_("jump to the bottom of the message"),
	N_("toggle display of quoted text"),
	N_("skip beyond quoted text"),
	N_("jump to the top of the message"),
	N_("pipe message/attachment to a shell command"),
	N_("move to the previous entry"),
	N_("scroll up one line"),
	N_("move to the previous page"),
	N_("print the current entry"),
	N_("query external program for addresses"),
	N_("append new query results to current results"),
	N_("save changes to mailbox and quit"),
	N_("recall a postponed message"),
	N_("clear and redraw the screen"),
	N_("{internal}"),
	N_("rename the current mailbox (IMAP only)"),
	N_("reply to a message"),
	N_("use the current message as a template for a new one"),
	N_("save message/attachment to a mailbox/file"),
	N_("search for a regular expression"),
	N_("search backwards for a regular expression"),
	N_("search for next match"),
	N_("search for next match in opposite direction"),
	N_("toggle search pattern coloring"),
	N_("invoke a command in a subshell"),
	N_("sort messages"),
	N_("sort messages in reverse order"),
	N_("tag the current entry"),
	N_("apply next function to tagged messages"),
	N_("apply next function ONLY to tagged messages"),
	N_("tag the current subthread"),
	N_("tag the current thread"),
	N_("toggle a message's 'new' flag"),
	N_("toggle whether the mailbox will be rewritten"),
	N_("toggle whether to browse mailboxes or all files"),
	N_("move to the top of the page"),
	N_("undelete the current entry"),
	N_("undelete all messages in thread"),
	N_("undelete all messages in subthread"),
	N_("show the Mutt version number and date"),
	N_("view attachment using mailcap entry if necessary"),
	N_("show MIME attachments"),
	N_("display the keycode for a key press"),
	N_("show currently active limit pattern"),
	N_("collapse/uncollapse current thread"),
	N_("collapse/uncollapse all threads"),
	N_("attach a PGP public key"),
	N_("show PGP options"),
	N_("mail a PGP public key"),
	N_("verify a PGP public key"),
	N_("view the key's user id"),
	N_("check for classic PGP"),
	N_("show S/MIME options"),
	N_("make decrypted copy and delete"),
	N_("make decrypted copy"),
	N_("wipe passphrase(s) from memory"),
	N_("extract supported public keys"),
	NULL
};
#endif /* MAIN_C */

enum {
	OP_NULL,
	OP_END_COND,
	OP_ATTACH_VIEW_MAILCAP,
	OP_ATTACH_VIEW_TEXT,
	OP_ATTACH_COLLAPSE,
	OP_BOTTOM_PAGE,
	OP_BOUNCE_MESSAGE,
	OP_BROWSER_NEW_FILE,
	OP_BROWSER_VIEW_FILE,
	OP_BROWSER_TELL,
	OP_BROWSER_SUBSCRIBE,
	OP_BROWSER_UNSUBSCRIBE,
	OP_BROWSER_TOGGLE_LSUB,
	OP_BUFFY_LIST,
	OP_CHANGE_DIRECTORY,
	OP_CHECK_NEW,
	OP_COMPOSE_ATTACH_FILE,
	OP_COMPOSE_ATTACH_MESSAGE,
	OP_COMPOSE_EDIT_BCC,
	OP_COMPOSE_EDIT_CC,
	OP_COMPOSE_EDIT_DESCRIPTION,
	OP_COMPOSE_EDIT_ENCODING,
	OP_COMPOSE_EDIT_FCC,
	OP_COMPOSE_EDIT_FILE,
	OP_COMPOSE_EDIT_FROM,
	OP_COMPOSE_EDIT_HEADERS,
	OP_COMPOSE_EDIT_MESSAGE,
	OP_COMPOSE_EDIT_MIME,
	OP_COMPOSE_EDIT_REPLY_TO,
	OP_COMPOSE_EDIT_SUBJECT,
	OP_COMPOSE_EDIT_TO,
	OP_CREATE_MAILBOX,
	OP_EDIT_TYPE,
	OP_COMPOSE_GET_ATTACHMENT,
	OP_COMPOSE_ISPELL,
	OP_COMPOSE_NEW_MIME,
	OP_COMPOSE_TOGGLE_RECODE,
	OP_COMPOSE_POSTPONE_MESSAGE,
	OP_COMPOSE_RENAME_FILE,
	OP_COMPOSE_SEND_MESSAGE,
	OP_COMPOSE_TOGGLE_DISPOSITION,
	OP_COMPOSE_TOGGLE_UNLINK,
	OP_COMPOSE_UPDATE_ENCODING,
	OP_COMPOSE_WRITE_MESSAGE,
	OP_COPY_MESSAGE,
	OP_CREATE_ALIAS,
	OP_CURRENT_BOTTOM,
	OP_CURRENT_MIDDLE,
	OP_CURRENT_TOP,
	OP_DECODE_COPY,
	OP_DECODE_SAVE,
	OP_DELETE,
	OP_DELETE_MAILBOX,
	OP_DELETE_SUBTHREAD,
	OP_DELETE_THREAD,
	OP_DISPLAY_ADDRESS,
	OP_DISPLAY_HEADERS,
	OP_DISPLAY_MESSAGE,
	OP_EDIT_MESSAGE,
	OP_EDITOR_BACKSPACE,
	OP_EDITOR_BACKWARD_CHAR,
	OP_EDITOR_BACKWARD_WORD,
	OP_EDITOR_BOL,
	OP_EDITOR_BUFFY_CYCLE,
	OP_EDITOR_COMPLETE,
	OP_EDITOR_COMPLETE_QUERY,
	OP_EDITOR_DELETE_CHAR,
	OP_EDITOR_EOL,
	OP_EDITOR_FORWARD_CHAR,
	OP_EDITOR_FORWARD_WORD,
	OP_EDITOR_HISTORY_DOWN,
	OP_EDITOR_HISTORY_UP,
	OP_EDITOR_KILL_EOL,
	OP_EDITOR_KILL_EOW,
	OP_EDITOR_KILL_LINE,
	OP_EDITOR_KILL_WORD,
	OP_EDITOR_QUOTE_CHAR,
	OP_EDITOR_TRANSPOSE_CHARS,
	OP_EDITOR_CAPITALIZE_WORD,
	OP_EDITOR_DOWNCASE_WORD,
	OP_EDITOR_UPCASE_WORD,
	OP_ENTER_COMMAND,
	OP_ENTER_MASK,
	OP_EXIT,
	OP_FILTER,
	OP_FIRST_ENTRY,
	OP_FLAG_MESSAGE,
	OP_FORWARD_MESSAGE,
	OP_GENERIC_SELECT_ENTRY,
	OP_GROUP_REPLY,
	OP_HALF_DOWN,
	OP_HALF_UP,
	OP_HELP,
	OP_JUMP,
	OP_LAST_ENTRY,
	OP_LIST_REPLY,
	OP_MACRO,
	OP_MAIL,
	OP_MAIN_BREAK_THREAD,
	OP_MAIN_CHANGE_FOLDER,
	OP_MAIN_CHANGE_FOLDER_READONLY,
	OP_MAIN_CLEAR_FLAG,
	OP_MAIN_DELETE_PATTERN,
	OP_MAIN_IMAP_FETCH,
	OP_MAIN_IMAP_LOGOUT_ALL,
	OP_MAIN_FETCH_MAIL,
	OP_MAIN_FIRST_MESSAGE,
	OP_MAIN_LAST_MESSAGE,
	OP_MAIN_LIMIT,
	OP_MAIN_LINK_THREADS,
	OP_MAIN_NEXT_UNREAD_MAILBOX,
	OP_MAIN_NEXT_NEW,
	OP_MAIN_NEXT_NEW_THEN_UNREAD,
	OP_MAIN_NEXT_SUBTHREAD,
	OP_MAIN_NEXT_THREAD,
	OP_MAIN_NEXT_UNDELETED,
	OP_MAIN_NEXT_UNREAD,
	OP_MAIN_PARENT_MESSAGE,
	OP_MAIN_PREV_THREAD,
	OP_MAIN_PREV_SUBTHREAD,
	OP_MAIN_PREV_UNDELETED,
	OP_MAIN_PREV_NEW,
	OP_MAIN_PREV_NEW_THEN_UNREAD,
	OP_MAIN_PREV_UNREAD,
	OP_MAIN_READ_THREAD,
	OP_MAIN_READ_SUBTHREAD,
	OP_MAIN_SET_FLAG,
	OP_MAIN_SYNC_FOLDER,
	OP_MAIN_TAG_PATTERN,
	OP_MAIN_UNDELETE_PATTERN,
	OP_MAIN_UNTAG_PATTERN,
	OP_MIDDLE_PAGE,
	OP_NEXT_ENTRY,
	OP_NEXT_LINE,
	OP_NEXT_PAGE,
	OP_PAGER_BOTTOM,
	OP_PAGER_HIDE_QUOTED,
	OP_PAGER_SKIP_QUOTED,
	OP_PAGER_TOP,
	OP_PIPE,
	OP_PREV_ENTRY,
	OP_PREV_LINE,
	OP_PREV_PAGE,
	OP_PRINT,
	OP_QUERY,
	OP_QUERY_APPEND,
	OP_QUIT,
	OP_RECALL_MESSAGE,
	OP_REDRAW,
	OP_REFORMAT_WINCH,
	OP_RENAME_MAILBOX,
	OP_REPLY,
	OP_RESEND,
	OP_SAVE,
	OP_SEARCH,
	OP_SEARCH_REVERSE,
	OP_SEARCH_NEXT,
	OP_SEARCH_OPPOSITE,
	OP_SEARCH_TOGGLE,
	OP_SHELL_ESCAPE,
	OP_SORT,
	OP_SORT_REVERSE,
	OP_TAG,
	OP_TAG_PREFIX,
	OP_TAG_PREFIX_COND,
	OP_TAG_SUBTHREAD,
	OP_TAG_THREAD,
	OP_TOGGLE_NEW,
	OP_TOGGLE_WRITE,
	OP_TOGGLE_MAILBOXES,
	OP_TOP_PAGE,
	OP_UNDELETE,
	OP_UNDELETE_THREAD,
	OP_UNDELETE_SUBTHREAD,
	OP_VERSION,
	OP_VIEW_ATTACH,
	OP_VIEW_ATTACHMENTS,
	OP_WHAT_KEY,
	OP_MAIN_SHOW_LIMIT,
	OP_MAIN_COLLAPSE_THREAD,
	OP_MAIN_COLLAPSE_ALL,
	OP_COMPOSE_ATTACH_KEY,
	OP_COMPOSE_PGP_MENU,
	OP_MAIL_KEY,
	OP_VERIFY_KEY,
	OP_VIEW_ID,
	OP_CHECK_TRADITIONAL,
	OP_COMPOSE_SMIME_MENU,
	OP_DECRYPT_SAVE,
	OP_DECRYPT_COPY,
	OP_FORGET_PASSPHRASE,
	OP_EXTRACT_KEYS,
	OP_MAX
};
