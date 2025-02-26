
/* Generated data (by glib-mkenums) */

#include <gio.h>

/* enumerations from "gioenums.h" */
GType
g_app_info_create_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_APP_INFO_CREATE_NONE, "G_APP_INFO_CREATE_NONE", "none" },
        { G_APP_INFO_CREATE_NEEDS_TERMINAL, "G_APP_INFO_CREATE_NEEDS_TERMINAL", "needs-terminal" },
        { G_APP_INFO_CREATE_SUPPORTS_URIS, "G_APP_INFO_CREATE_SUPPORTS_URIS", "supports-uris" },
        { G_APP_INFO_CREATE_SUPPORTS_STARTUP_NOTIFICATION, "G_APP_INFO_CREATE_SUPPORTS_STARTUP_NOTIFICATION", "supports-startup-notification" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GAppInfoCreateFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_converter_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_CONVERTER_NO_FLAGS, "G_CONVERTER_NO_FLAGS", "none" },
        { G_CONVERTER_INPUT_AT_END, "G_CONVERTER_INPUT_AT_END", "input-at-end" },
        { G_CONVERTER_FLUSH, "G_CONVERTER_FLUSH", "flush" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GConverterFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_converter_result_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_CONVERTER_ERROR, "G_CONVERTER_ERROR", "error" },
        { G_CONVERTER_CONVERTED, "G_CONVERTER_CONVERTED", "converted" },
        { G_CONVERTER_FINISHED, "G_CONVERTER_FINISHED", "finished" },
        { G_CONVERTER_FLUSHED, "G_CONVERTER_FLUSHED", "flushed" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GConverterResult"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_data_stream_byte_order_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_DATA_STREAM_BYTE_ORDER_BIG_ENDIAN, "G_DATA_STREAM_BYTE_ORDER_BIG_ENDIAN", "big-endian" },
        { G_DATA_STREAM_BYTE_ORDER_LITTLE_ENDIAN, "G_DATA_STREAM_BYTE_ORDER_LITTLE_ENDIAN", "little-endian" },
        { G_DATA_STREAM_BYTE_ORDER_HOST_ENDIAN, "G_DATA_STREAM_BYTE_ORDER_HOST_ENDIAN", "host-endian" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GDataStreamByteOrder"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_data_stream_newline_type_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_DATA_STREAM_NEWLINE_TYPE_LF, "G_DATA_STREAM_NEWLINE_TYPE_LF", "lf" },
        { G_DATA_STREAM_NEWLINE_TYPE_CR, "G_DATA_STREAM_NEWLINE_TYPE_CR", "cr" },
        { G_DATA_STREAM_NEWLINE_TYPE_CR_LF, "G_DATA_STREAM_NEWLINE_TYPE_CR_LF", "cr-lf" },
        { G_DATA_STREAM_NEWLINE_TYPE_ANY, "G_DATA_STREAM_NEWLINE_TYPE_ANY", "any" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GDataStreamNewlineType"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_file_attribute_type_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_FILE_ATTRIBUTE_TYPE_INVALID, "G_FILE_ATTRIBUTE_TYPE_INVALID", "invalid" },
        { G_FILE_ATTRIBUTE_TYPE_STRING, "G_FILE_ATTRIBUTE_TYPE_STRING", "string" },
        { G_FILE_ATTRIBUTE_TYPE_BYTE_STRING, "G_FILE_ATTRIBUTE_TYPE_BYTE_STRING", "byte-string" },
        { G_FILE_ATTRIBUTE_TYPE_BOOLEAN, "G_FILE_ATTRIBUTE_TYPE_BOOLEAN", "boolean" },
        { G_FILE_ATTRIBUTE_TYPE_UINT32, "G_FILE_ATTRIBUTE_TYPE_UINT32", "uint32" },
        { G_FILE_ATTRIBUTE_TYPE_INT32, "G_FILE_ATTRIBUTE_TYPE_INT32", "int32" },
        { G_FILE_ATTRIBUTE_TYPE_UINT64, "G_FILE_ATTRIBUTE_TYPE_UINT64", "uint64" },
        { G_FILE_ATTRIBUTE_TYPE_INT64, "G_FILE_ATTRIBUTE_TYPE_INT64", "int64" },
        { G_FILE_ATTRIBUTE_TYPE_OBJECT, "G_FILE_ATTRIBUTE_TYPE_OBJECT", "object" },
        { G_FILE_ATTRIBUTE_TYPE_STRINGV, "G_FILE_ATTRIBUTE_TYPE_STRINGV", "stringv" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GFileAttributeType"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_file_attribute_info_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_FILE_ATTRIBUTE_INFO_NONE, "G_FILE_ATTRIBUTE_INFO_NONE", "none" },
        { G_FILE_ATTRIBUTE_INFO_COPY_WITH_FILE, "G_FILE_ATTRIBUTE_INFO_COPY_WITH_FILE", "copy-with-file" },
        { G_FILE_ATTRIBUTE_INFO_COPY_WHEN_MOVED, "G_FILE_ATTRIBUTE_INFO_COPY_WHEN_MOVED", "copy-when-moved" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GFileAttributeInfoFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_file_attribute_status_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_FILE_ATTRIBUTE_STATUS_UNSET, "G_FILE_ATTRIBUTE_STATUS_UNSET", "unset" },
        { G_FILE_ATTRIBUTE_STATUS_SET, "G_FILE_ATTRIBUTE_STATUS_SET", "set" },
        { G_FILE_ATTRIBUTE_STATUS_ERROR_SETTING, "G_FILE_ATTRIBUTE_STATUS_ERROR_SETTING", "error-setting" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GFileAttributeStatus"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_file_query_info_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_FILE_QUERY_INFO_NONE, "G_FILE_QUERY_INFO_NONE", "none" },
        { G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, "G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS", "nofollow-symlinks" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GFileQueryInfoFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_file_create_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_FILE_CREATE_NONE, "G_FILE_CREATE_NONE", "none" },
        { G_FILE_CREATE_PRIVATE, "G_FILE_CREATE_PRIVATE", "private" },
        { G_FILE_CREATE_REPLACE_DESTINATION, "G_FILE_CREATE_REPLACE_DESTINATION", "replace-destination" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GFileCreateFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_mount_mount_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_MOUNT_MOUNT_NONE, "G_MOUNT_MOUNT_NONE", "none" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GMountMountFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_mount_unmount_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_MOUNT_UNMOUNT_NONE, "G_MOUNT_UNMOUNT_NONE", "none" },
        { G_MOUNT_UNMOUNT_FORCE, "G_MOUNT_UNMOUNT_FORCE", "force" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GMountUnmountFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_drive_start_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_DRIVE_START_NONE, "G_DRIVE_START_NONE", "none" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GDriveStartFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_drive_start_stop_type_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_DRIVE_START_STOP_TYPE_UNKNOWN, "G_DRIVE_START_STOP_TYPE_UNKNOWN", "unknown" },
        { G_DRIVE_START_STOP_TYPE_SHUTDOWN, "G_DRIVE_START_STOP_TYPE_SHUTDOWN", "shutdown" },
        { G_DRIVE_START_STOP_TYPE_NETWORK, "G_DRIVE_START_STOP_TYPE_NETWORK", "network" },
        { G_DRIVE_START_STOP_TYPE_MULTIDISK, "G_DRIVE_START_STOP_TYPE_MULTIDISK", "multidisk" },
        { G_DRIVE_START_STOP_TYPE_PASSWORD, "G_DRIVE_START_STOP_TYPE_PASSWORD", "password" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GDriveStartStopType"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_file_copy_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_FILE_COPY_NONE, "G_FILE_COPY_NONE", "none" },
        { G_FILE_COPY_OVERWRITE, "G_FILE_COPY_OVERWRITE", "overwrite" },
        { G_FILE_COPY_BACKUP, "G_FILE_COPY_BACKUP", "backup" },
        { G_FILE_COPY_NOFOLLOW_SYMLINKS, "G_FILE_COPY_NOFOLLOW_SYMLINKS", "nofollow-symlinks" },
        { G_FILE_COPY_ALL_METADATA, "G_FILE_COPY_ALL_METADATA", "all-metadata" },
        { G_FILE_COPY_NO_FALLBACK_FOR_MOVE, "G_FILE_COPY_NO_FALLBACK_FOR_MOVE", "no-fallback-for-move" },
        { G_FILE_COPY_TARGET_DEFAULT_PERMS, "G_FILE_COPY_TARGET_DEFAULT_PERMS", "target-default-perms" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GFileCopyFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_file_monitor_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_FILE_MONITOR_NONE, "G_FILE_MONITOR_NONE", "none" },
        { G_FILE_MONITOR_WATCH_MOUNTS, "G_FILE_MONITOR_WATCH_MOUNTS", "watch-mounts" },
        { G_FILE_MONITOR_SEND_MOVED, "G_FILE_MONITOR_SEND_MOVED", "send-moved" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GFileMonitorFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_file_type_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_FILE_TYPE_UNKNOWN, "G_FILE_TYPE_UNKNOWN", "unknown" },
        { G_FILE_TYPE_REGULAR, "G_FILE_TYPE_REGULAR", "regular" },
        { G_FILE_TYPE_DIRECTORY, "G_FILE_TYPE_DIRECTORY", "directory" },
        { G_FILE_TYPE_SYMBOLIC_LINK, "G_FILE_TYPE_SYMBOLIC_LINK", "symbolic-link" },
        { G_FILE_TYPE_SPECIAL, "G_FILE_TYPE_SPECIAL", "special" },
        { G_FILE_TYPE_SHORTCUT, "G_FILE_TYPE_SHORTCUT", "shortcut" },
        { G_FILE_TYPE_MOUNTABLE, "G_FILE_TYPE_MOUNTABLE", "mountable" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GFileType"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_filesystem_preview_type_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_FILESYSTEM_PREVIEW_TYPE_IF_ALWAYS, "G_FILESYSTEM_PREVIEW_TYPE_IF_ALWAYS", "if-always" },
        { G_FILESYSTEM_PREVIEW_TYPE_IF_LOCAL, "G_FILESYSTEM_PREVIEW_TYPE_IF_LOCAL", "if-local" },
        { G_FILESYSTEM_PREVIEW_TYPE_NEVER, "G_FILESYSTEM_PREVIEW_TYPE_NEVER", "never" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GFilesystemPreviewType"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_file_monitor_event_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_FILE_MONITOR_EVENT_CHANGED, "G_FILE_MONITOR_EVENT_CHANGED", "changed" },
        { G_FILE_MONITOR_EVENT_CHANGES_DONE_HINT, "G_FILE_MONITOR_EVENT_CHANGES_DONE_HINT", "changes-done-hint" },
        { G_FILE_MONITOR_EVENT_DELETED, "G_FILE_MONITOR_EVENT_DELETED", "deleted" },
        { G_FILE_MONITOR_EVENT_CREATED, "G_FILE_MONITOR_EVENT_CREATED", "created" },
        { G_FILE_MONITOR_EVENT_ATTRIBUTE_CHANGED, "G_FILE_MONITOR_EVENT_ATTRIBUTE_CHANGED", "attribute-changed" },
        { G_FILE_MONITOR_EVENT_PRE_UNMOUNT, "G_FILE_MONITOR_EVENT_PRE_UNMOUNT", "pre-unmount" },
        { G_FILE_MONITOR_EVENT_UNMOUNTED, "G_FILE_MONITOR_EVENT_UNMOUNTED", "unmounted" },
        { G_FILE_MONITOR_EVENT_MOVED, "G_FILE_MONITOR_EVENT_MOVED", "moved" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GFileMonitorEvent"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_io_error_enum_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_IO_ERROR_FAILED, "G_IO_ERROR_FAILED", "failed" },
        { G_IO_ERROR_NOT_FOUND, "G_IO_ERROR_NOT_FOUND", "not-found" },
        { G_IO_ERROR_EXISTS, "G_IO_ERROR_EXISTS", "exists" },
        { G_IO_ERROR_IS_DIRECTORY, "G_IO_ERROR_IS_DIRECTORY", "is-directory" },
        { G_IO_ERROR_NOT_DIRECTORY, "G_IO_ERROR_NOT_DIRECTORY", "not-directory" },
        { G_IO_ERROR_NOT_EMPTY, "G_IO_ERROR_NOT_EMPTY", "not-empty" },
        { G_IO_ERROR_NOT_REGULAR_FILE, "G_IO_ERROR_NOT_REGULAR_FILE", "not-regular-file" },
        { G_IO_ERROR_NOT_SYMBOLIC_LINK, "G_IO_ERROR_NOT_SYMBOLIC_LINK", "not-symbolic-link" },
        { G_IO_ERROR_NOT_MOUNTABLE_FILE, "G_IO_ERROR_NOT_MOUNTABLE_FILE", "not-mountable-file" },
        { G_IO_ERROR_FILENAME_TOO_LONG, "G_IO_ERROR_FILENAME_TOO_LONG", "filename-too-long" },
        { G_IO_ERROR_INVALID_FILENAME, "G_IO_ERROR_INVALID_FILENAME", "invalid-filename" },
        { G_IO_ERROR_TOO_MANY_LINKS, "G_IO_ERROR_TOO_MANY_LINKS", "too-many-links" },
        { G_IO_ERROR_NO_SPACE, "G_IO_ERROR_NO_SPACE", "no-space" },
        { G_IO_ERROR_INVALID_ARGUMENT, "G_IO_ERROR_INVALID_ARGUMENT", "invalid-argument" },
        { G_IO_ERROR_PERMISSION_DENIED, "G_IO_ERROR_PERMISSION_DENIED", "permission-denied" },
        { G_IO_ERROR_NOT_SUPPORTED, "G_IO_ERROR_NOT_SUPPORTED", "not-supported" },
        { G_IO_ERROR_NOT_MOUNTED, "G_IO_ERROR_NOT_MOUNTED", "not-mounted" },
        { G_IO_ERROR_ALREADY_MOUNTED, "G_IO_ERROR_ALREADY_MOUNTED", "already-mounted" },
        { G_IO_ERROR_CLOSED, "G_IO_ERROR_CLOSED", "closed" },
        { G_IO_ERROR_CANCELLED, "G_IO_ERROR_CANCELLED", "cancelled" },
        { G_IO_ERROR_PENDING, "G_IO_ERROR_PENDING", "pending" },
        { G_IO_ERROR_READ_ONLY, "G_IO_ERROR_READ_ONLY", "read-only" },
        { G_IO_ERROR_CANT_CREATE_BACKUP, "G_IO_ERROR_CANT_CREATE_BACKUP", "cant-create-backup" },
        { G_IO_ERROR_WRONG_ETAG, "G_IO_ERROR_WRONG_ETAG", "wrong-etag" },
        { G_IO_ERROR_TIMED_OUT, "G_IO_ERROR_TIMED_OUT", "timed-out" },
        { G_IO_ERROR_WOULD_RECURSE, "G_IO_ERROR_WOULD_RECURSE", "would-recurse" },
        { G_IO_ERROR_BUSY, "G_IO_ERROR_BUSY", "busy" },
        { G_IO_ERROR_WOULD_BLOCK, "G_IO_ERROR_WOULD_BLOCK", "would-block" },
        { G_IO_ERROR_HOST_NOT_FOUND, "G_IO_ERROR_HOST_NOT_FOUND", "host-not-found" },
        { G_IO_ERROR_WOULD_MERGE, "G_IO_ERROR_WOULD_MERGE", "would-merge" },
        { G_IO_ERROR_FAILED_HANDLED, "G_IO_ERROR_FAILED_HANDLED", "failed-handled" },
        { G_IO_ERROR_TOO_MANY_OPEN_FILES, "G_IO_ERROR_TOO_MANY_OPEN_FILES", "too-many-open-files" },
        { G_IO_ERROR_NOT_INITIALIZED, "G_IO_ERROR_NOT_INITIALIZED", "not-initialized" },
        { G_IO_ERROR_ADDRESS_IN_USE, "G_IO_ERROR_ADDRESS_IN_USE", "address-in-use" },
        { G_IO_ERROR_PARTIAL_INPUT, "G_IO_ERROR_PARTIAL_INPUT", "partial-input" },
        { G_IO_ERROR_INVALID_DATA, "G_IO_ERROR_INVALID_DATA", "invalid-data" },
        { G_IO_ERROR_DBUS_ERROR, "G_IO_ERROR_DBUS_ERROR", "dbus-error" },
        { G_IO_ERROR_HOST_UNREACHABLE, "G_IO_ERROR_HOST_UNREACHABLE", "host-unreachable" },
        { G_IO_ERROR_NETWORK_UNREACHABLE, "G_IO_ERROR_NETWORK_UNREACHABLE", "network-unreachable" },
        { G_IO_ERROR_CONNECTION_REFUSED, "G_IO_ERROR_CONNECTION_REFUSED", "connection-refused" },
        { G_IO_ERROR_PROXY_FAILED, "G_IO_ERROR_PROXY_FAILED", "proxy-failed" },
        { G_IO_ERROR_PROXY_AUTH_FAILED, "G_IO_ERROR_PROXY_AUTH_FAILED", "proxy-auth-failed" },
        { G_IO_ERROR_PROXY_NEED_AUTH, "G_IO_ERROR_PROXY_NEED_AUTH", "proxy-need-auth" },
        { G_IO_ERROR_PROXY_NOT_ALLOWED, "G_IO_ERROR_PROXY_NOT_ALLOWED", "proxy-not-allowed" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GIOErrorEnum"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_ask_password_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_ASK_PASSWORD_NEED_PASSWORD, "G_ASK_PASSWORD_NEED_PASSWORD", "need-password" },
        { G_ASK_PASSWORD_NEED_USERNAME, "G_ASK_PASSWORD_NEED_USERNAME", "need-username" },
        { G_ASK_PASSWORD_NEED_DOMAIN, "G_ASK_PASSWORD_NEED_DOMAIN", "need-domain" },
        { G_ASK_PASSWORD_SAVING_SUPPORTED, "G_ASK_PASSWORD_SAVING_SUPPORTED", "saving-supported" },
        { G_ASK_PASSWORD_ANONYMOUS_SUPPORTED, "G_ASK_PASSWORD_ANONYMOUS_SUPPORTED", "anonymous-supported" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GAskPasswordFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_password_save_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_PASSWORD_SAVE_NEVER, "G_PASSWORD_SAVE_NEVER", "never" },
        { G_PASSWORD_SAVE_FOR_SESSION, "G_PASSWORD_SAVE_FOR_SESSION", "for-session" },
        { G_PASSWORD_SAVE_PERMANENTLY, "G_PASSWORD_SAVE_PERMANENTLY", "permanently" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GPasswordSave"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_mount_operation_result_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_MOUNT_OPERATION_HANDLED, "G_MOUNT_OPERATION_HANDLED", "handled" },
        { G_MOUNT_OPERATION_ABORTED, "G_MOUNT_OPERATION_ABORTED", "aborted" },
        { G_MOUNT_OPERATION_UNHANDLED, "G_MOUNT_OPERATION_UNHANDLED", "unhandled" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GMountOperationResult"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_output_stream_splice_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_OUTPUT_STREAM_SPLICE_NONE, "G_OUTPUT_STREAM_SPLICE_NONE", "none" },
        { G_OUTPUT_STREAM_SPLICE_CLOSE_SOURCE, "G_OUTPUT_STREAM_SPLICE_CLOSE_SOURCE", "close-source" },
        { G_OUTPUT_STREAM_SPLICE_CLOSE_TARGET, "G_OUTPUT_STREAM_SPLICE_CLOSE_TARGET", "close-target" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GOutputStreamSpliceFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_io_stream_splice_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_IO_STREAM_SPLICE_NONE, "G_IO_STREAM_SPLICE_NONE", "none" },
        { G_IO_STREAM_SPLICE_CLOSE_STREAM1, "G_IO_STREAM_SPLICE_CLOSE_STREAM1", "close-stream1" },
        { G_IO_STREAM_SPLICE_CLOSE_STREAM2, "G_IO_STREAM_SPLICE_CLOSE_STREAM2", "close-stream2" },
        { G_IO_STREAM_SPLICE_WAIT_FOR_BOTH, "G_IO_STREAM_SPLICE_WAIT_FOR_BOTH", "wait-for-both" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GIOStreamSpliceFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_emblem_origin_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_EMBLEM_ORIGIN_UNKNOWN, "G_EMBLEM_ORIGIN_UNKNOWN", "unknown" },
        { G_EMBLEM_ORIGIN_DEVICE, "G_EMBLEM_ORIGIN_DEVICE", "device" },
        { G_EMBLEM_ORIGIN_LIVEMETADATA, "G_EMBLEM_ORIGIN_LIVEMETADATA", "livemetadata" },
        { G_EMBLEM_ORIGIN_TAG, "G_EMBLEM_ORIGIN_TAG", "tag" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GEmblemOrigin"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_resolver_error_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_RESOLVER_ERROR_NOT_FOUND, "G_RESOLVER_ERROR_NOT_FOUND", "not-found" },
        { G_RESOLVER_ERROR_TEMPORARY_FAILURE, "G_RESOLVER_ERROR_TEMPORARY_FAILURE", "temporary-failure" },
        { G_RESOLVER_ERROR_INTERNAL, "G_RESOLVER_ERROR_INTERNAL", "internal" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GResolverError"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_socket_family_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_SOCKET_FAMILY_INVALID, "G_SOCKET_FAMILY_INVALID", "invalid" },
        { G_SOCKET_FAMILY_UNIX, "G_SOCKET_FAMILY_UNIX", "unix" },
        { G_SOCKET_FAMILY_IPV4, "G_SOCKET_FAMILY_IPV4", "ipv4" },
        { G_SOCKET_FAMILY_IPV6, "G_SOCKET_FAMILY_IPV6", "ipv6" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GSocketFamily"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_socket_type_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_SOCKET_TYPE_INVALID, "G_SOCKET_TYPE_INVALID", "invalid" },
        { G_SOCKET_TYPE_STREAM, "G_SOCKET_TYPE_STREAM", "stream" },
        { G_SOCKET_TYPE_DATAGRAM, "G_SOCKET_TYPE_DATAGRAM", "datagram" },
        { G_SOCKET_TYPE_SEQPACKET, "G_SOCKET_TYPE_SEQPACKET", "seqpacket" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GSocketType"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_socket_msg_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_SOCKET_MSG_NONE, "G_SOCKET_MSG_NONE", "none" },
        { G_SOCKET_MSG_OOB, "G_SOCKET_MSG_OOB", "oob" },
        { G_SOCKET_MSG_PEEK, "G_SOCKET_MSG_PEEK", "peek" },
        { G_SOCKET_MSG_DONTROUTE, "G_SOCKET_MSG_DONTROUTE", "dontroute" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GSocketMsgFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_socket_protocol_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_SOCKET_PROTOCOL_UNKNOWN, "G_SOCKET_PROTOCOL_UNKNOWN", "unknown" },
        { G_SOCKET_PROTOCOL_DEFAULT, "G_SOCKET_PROTOCOL_DEFAULT", "default" },
        { G_SOCKET_PROTOCOL_TCP, "G_SOCKET_PROTOCOL_TCP", "tcp" },
        { G_SOCKET_PROTOCOL_UDP, "G_SOCKET_PROTOCOL_UDP", "udp" },
        { G_SOCKET_PROTOCOL_SCTP, "G_SOCKET_PROTOCOL_SCTP", "sctp" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GSocketProtocol"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_zlib_compressor_format_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_ZLIB_COMPRESSOR_FORMAT_ZLIB, "G_ZLIB_COMPRESSOR_FORMAT_ZLIB", "zlib" },
        { G_ZLIB_COMPRESSOR_FORMAT_GZIP, "G_ZLIB_COMPRESSOR_FORMAT_GZIP", "gzip" },
        { G_ZLIB_COMPRESSOR_FORMAT_RAW, "G_ZLIB_COMPRESSOR_FORMAT_RAW", "raw" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GZlibCompressorFormat"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_unix_socket_address_type_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_UNIX_SOCKET_ADDRESS_INVALID, "G_UNIX_SOCKET_ADDRESS_INVALID", "invalid" },
        { G_UNIX_SOCKET_ADDRESS_ANONYMOUS, "G_UNIX_SOCKET_ADDRESS_ANONYMOUS", "anonymous" },
        { G_UNIX_SOCKET_ADDRESS_PATH, "G_UNIX_SOCKET_ADDRESS_PATH", "path" },
        { G_UNIX_SOCKET_ADDRESS_ABSTRACT, "G_UNIX_SOCKET_ADDRESS_ABSTRACT", "abstract" },
        { G_UNIX_SOCKET_ADDRESS_ABSTRACT_PADDED, "G_UNIX_SOCKET_ADDRESS_ABSTRACT_PADDED", "abstract-padded" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GUnixSocketAddressType"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_bus_type_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_BUS_TYPE_STARTER, "G_BUS_TYPE_STARTER", "starter" },
        { G_BUS_TYPE_NONE, "G_BUS_TYPE_NONE", "none" },
        { G_BUS_TYPE_SYSTEM, "G_BUS_TYPE_SYSTEM", "system" },
        { G_BUS_TYPE_SESSION, "G_BUS_TYPE_SESSION", "session" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GBusType"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_bus_name_owner_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_BUS_NAME_OWNER_FLAGS_NONE, "G_BUS_NAME_OWNER_FLAGS_NONE", "none" },
        { G_BUS_NAME_OWNER_FLAGS_ALLOW_REPLACEMENT, "G_BUS_NAME_OWNER_FLAGS_ALLOW_REPLACEMENT", "allow-replacement" },
        { G_BUS_NAME_OWNER_FLAGS_REPLACE, "G_BUS_NAME_OWNER_FLAGS_REPLACE", "replace" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GBusNameOwnerFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_bus_name_watcher_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_BUS_NAME_WATCHER_FLAGS_NONE, "G_BUS_NAME_WATCHER_FLAGS_NONE", "none" },
        { G_BUS_NAME_WATCHER_FLAGS_AUTO_START, "G_BUS_NAME_WATCHER_FLAGS_AUTO_START", "auto-start" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GBusNameWatcherFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_dbus_proxy_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_DBUS_PROXY_FLAGS_NONE, "G_DBUS_PROXY_FLAGS_NONE", "none" },
        { G_DBUS_PROXY_FLAGS_DO_NOT_LOAD_PROPERTIES, "G_DBUS_PROXY_FLAGS_DO_NOT_LOAD_PROPERTIES", "do-not-load-properties" },
        { G_DBUS_PROXY_FLAGS_DO_NOT_CONNECT_SIGNALS, "G_DBUS_PROXY_FLAGS_DO_NOT_CONNECT_SIGNALS", "do-not-connect-signals" },
        { G_DBUS_PROXY_FLAGS_DO_NOT_AUTO_START, "G_DBUS_PROXY_FLAGS_DO_NOT_AUTO_START", "do-not-auto-start" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GDBusProxyFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_dbus_error_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_DBUS_ERROR_FAILED, "G_DBUS_ERROR_FAILED", "failed" },
        { G_DBUS_ERROR_NO_MEMORY, "G_DBUS_ERROR_NO_MEMORY", "no-memory" },
        { G_DBUS_ERROR_SERVICE_UNKNOWN, "G_DBUS_ERROR_SERVICE_UNKNOWN", "service-unknown" },
        { G_DBUS_ERROR_NAME_HAS_NO_OWNER, "G_DBUS_ERROR_NAME_HAS_NO_OWNER", "name-has-no-owner" },
        { G_DBUS_ERROR_NO_REPLY, "G_DBUS_ERROR_NO_REPLY", "no-reply" },
        { G_DBUS_ERROR_IO_ERROR, "G_DBUS_ERROR_IO_ERROR", "io-error" },
        { G_DBUS_ERROR_BAD_ADDRESS, "G_DBUS_ERROR_BAD_ADDRESS", "bad-address" },
        { G_DBUS_ERROR_NOT_SUPPORTED, "G_DBUS_ERROR_NOT_SUPPORTED", "not-supported" },
        { G_DBUS_ERROR_LIMITS_EXCEEDED, "G_DBUS_ERROR_LIMITS_EXCEEDED", "limits-exceeded" },
        { G_DBUS_ERROR_ACCESS_DENIED, "G_DBUS_ERROR_ACCESS_DENIED", "access-denied" },
        { G_DBUS_ERROR_AUTH_FAILED, "G_DBUS_ERROR_AUTH_FAILED", "auth-failed" },
        { G_DBUS_ERROR_NO_SERVER, "G_DBUS_ERROR_NO_SERVER", "no-server" },
        { G_DBUS_ERROR_TIMEOUT, "G_DBUS_ERROR_TIMEOUT", "timeout" },
        { G_DBUS_ERROR_NO_NETWORK, "G_DBUS_ERROR_NO_NETWORK", "no-network" },
        { G_DBUS_ERROR_ADDRESS_IN_USE, "G_DBUS_ERROR_ADDRESS_IN_USE", "address-in-use" },
        { G_DBUS_ERROR_DISCONNECTED, "G_DBUS_ERROR_DISCONNECTED", "disconnected" },
        { G_DBUS_ERROR_INVALID_ARGS, "G_DBUS_ERROR_INVALID_ARGS", "invalid-args" },
        { G_DBUS_ERROR_FILE_NOT_FOUND, "G_DBUS_ERROR_FILE_NOT_FOUND", "file-not-found" },
        { G_DBUS_ERROR_FILE_EXISTS, "G_DBUS_ERROR_FILE_EXISTS", "file-exists" },
        { G_DBUS_ERROR_UNKNOWN_METHOD, "G_DBUS_ERROR_UNKNOWN_METHOD", "unknown-method" },
        { G_DBUS_ERROR_TIMED_OUT, "G_DBUS_ERROR_TIMED_OUT", "timed-out" },
        { G_DBUS_ERROR_MATCH_RULE_NOT_FOUND, "G_DBUS_ERROR_MATCH_RULE_NOT_FOUND", "match-rule-not-found" },
        { G_DBUS_ERROR_MATCH_RULE_INVALID, "G_DBUS_ERROR_MATCH_RULE_INVALID", "match-rule-invalid" },
        { G_DBUS_ERROR_SPAWN_EXEC_FAILED, "G_DBUS_ERROR_SPAWN_EXEC_FAILED", "spawn-exec-failed" },
        { G_DBUS_ERROR_SPAWN_FORK_FAILED, "G_DBUS_ERROR_SPAWN_FORK_FAILED", "spawn-fork-failed" },
        { G_DBUS_ERROR_SPAWN_CHILD_EXITED, "G_DBUS_ERROR_SPAWN_CHILD_EXITED", "spawn-child-exited" },
        { G_DBUS_ERROR_SPAWN_CHILD_SIGNALED, "G_DBUS_ERROR_SPAWN_CHILD_SIGNALED", "spawn-child-signaled" },
        { G_DBUS_ERROR_SPAWN_FAILED, "G_DBUS_ERROR_SPAWN_FAILED", "spawn-failed" },
        { G_DBUS_ERROR_SPAWN_SETUP_FAILED, "G_DBUS_ERROR_SPAWN_SETUP_FAILED", "spawn-setup-failed" },
        { G_DBUS_ERROR_SPAWN_CONFIG_INVALID, "G_DBUS_ERROR_SPAWN_CONFIG_INVALID", "spawn-config-invalid" },
        { G_DBUS_ERROR_SPAWN_SERVICE_INVALID, "G_DBUS_ERROR_SPAWN_SERVICE_INVALID", "spawn-service-invalid" },
        { G_DBUS_ERROR_SPAWN_SERVICE_NOT_FOUND, "G_DBUS_ERROR_SPAWN_SERVICE_NOT_FOUND", "spawn-service-not-found" },
        { G_DBUS_ERROR_SPAWN_PERMISSIONS_INVALID, "G_DBUS_ERROR_SPAWN_PERMISSIONS_INVALID", "spawn-permissions-invalid" },
        { G_DBUS_ERROR_SPAWN_FILE_INVALID, "G_DBUS_ERROR_SPAWN_FILE_INVALID", "spawn-file-invalid" },
        { G_DBUS_ERROR_SPAWN_NO_MEMORY, "G_DBUS_ERROR_SPAWN_NO_MEMORY", "spawn-no-memory" },
        { G_DBUS_ERROR_UNIX_PROCESS_ID_UNKNOWN, "G_DBUS_ERROR_UNIX_PROCESS_ID_UNKNOWN", "unix-process-id-unknown" },
        { G_DBUS_ERROR_INVALID_SIGNATURE, "G_DBUS_ERROR_INVALID_SIGNATURE", "invalid-signature" },
        { G_DBUS_ERROR_INVALID_FILE_CONTENT, "G_DBUS_ERROR_INVALID_FILE_CONTENT", "invalid-file-content" },
        { G_DBUS_ERROR_SELINUX_SECURITY_CONTEXT_UNKNOWN, "G_DBUS_ERROR_SELINUX_SECURITY_CONTEXT_UNKNOWN", "selinux-security-context-unknown" },
        { G_DBUS_ERROR_ADT_AUDIT_DATA_UNKNOWN, "G_DBUS_ERROR_ADT_AUDIT_DATA_UNKNOWN", "adt-audit-data-unknown" },
        { G_DBUS_ERROR_OBJECT_PATH_IN_USE, "G_DBUS_ERROR_OBJECT_PATH_IN_USE", "object-path-in-use" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GDBusError"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_dbus_connection_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_DBUS_CONNECTION_FLAGS_NONE, "G_DBUS_CONNECTION_FLAGS_NONE", "none" },
        { G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT, "G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT", "authentication-client" },
        { G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_SERVER, "G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_SERVER", "authentication-server" },
        { G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_ALLOW_ANONYMOUS, "G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_ALLOW_ANONYMOUS", "authentication-allow-anonymous" },
        { G_DBUS_CONNECTION_FLAGS_MESSAGE_BUS_CONNECTION, "G_DBUS_CONNECTION_FLAGS_MESSAGE_BUS_CONNECTION", "message-bus-connection" },
        { G_DBUS_CONNECTION_FLAGS_DELAY_MESSAGE_PROCESSING, "G_DBUS_CONNECTION_FLAGS_DELAY_MESSAGE_PROCESSING", "delay-message-processing" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GDBusConnectionFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_dbus_capability_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_DBUS_CAPABILITY_FLAGS_NONE, "G_DBUS_CAPABILITY_FLAGS_NONE", "none" },
        { G_DBUS_CAPABILITY_FLAGS_UNIX_FD_PASSING, "G_DBUS_CAPABILITY_FLAGS_UNIX_FD_PASSING", "unix-fd-passing" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GDBusCapabilityFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_dbus_call_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_DBUS_CALL_FLAGS_NONE, "G_DBUS_CALL_FLAGS_NONE", "none" },
        { G_DBUS_CALL_FLAGS_NO_AUTO_START, "G_DBUS_CALL_FLAGS_NO_AUTO_START", "no-auto-start" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GDBusCallFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_dbus_message_type_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_DBUS_MESSAGE_TYPE_INVALID, "G_DBUS_MESSAGE_TYPE_INVALID", "invalid" },
        { G_DBUS_MESSAGE_TYPE_METHOD_CALL, "G_DBUS_MESSAGE_TYPE_METHOD_CALL", "method-call" },
        { G_DBUS_MESSAGE_TYPE_METHOD_RETURN, "G_DBUS_MESSAGE_TYPE_METHOD_RETURN", "method-return" },
        { G_DBUS_MESSAGE_TYPE_ERROR, "G_DBUS_MESSAGE_TYPE_ERROR", "error" },
        { G_DBUS_MESSAGE_TYPE_SIGNAL, "G_DBUS_MESSAGE_TYPE_SIGNAL", "signal" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GDBusMessageType"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_dbus_message_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_DBUS_MESSAGE_FLAGS_NONE, "G_DBUS_MESSAGE_FLAGS_NONE", "none" },
        { G_DBUS_MESSAGE_FLAGS_NO_REPLY_EXPECTED, "G_DBUS_MESSAGE_FLAGS_NO_REPLY_EXPECTED", "no-reply-expected" },
        { G_DBUS_MESSAGE_FLAGS_NO_AUTO_START, "G_DBUS_MESSAGE_FLAGS_NO_AUTO_START", "no-auto-start" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GDBusMessageFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_dbus_message_header_field_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_DBUS_MESSAGE_HEADER_FIELD_INVALID, "G_DBUS_MESSAGE_HEADER_FIELD_INVALID", "invalid" },
        { G_DBUS_MESSAGE_HEADER_FIELD_PATH, "G_DBUS_MESSAGE_HEADER_FIELD_PATH", "path" },
        { G_DBUS_MESSAGE_HEADER_FIELD_INTERFACE, "G_DBUS_MESSAGE_HEADER_FIELD_INTERFACE", "interface" },
        { G_DBUS_MESSAGE_HEADER_FIELD_MEMBER, "G_DBUS_MESSAGE_HEADER_FIELD_MEMBER", "member" },
        { G_DBUS_MESSAGE_HEADER_FIELD_ERROR_NAME, "G_DBUS_MESSAGE_HEADER_FIELD_ERROR_NAME", "error-name" },
        { G_DBUS_MESSAGE_HEADER_FIELD_REPLY_SERIAL, "G_DBUS_MESSAGE_HEADER_FIELD_REPLY_SERIAL", "reply-serial" },
        { G_DBUS_MESSAGE_HEADER_FIELD_DESTINATION, "G_DBUS_MESSAGE_HEADER_FIELD_DESTINATION", "destination" },
        { G_DBUS_MESSAGE_HEADER_FIELD_SENDER, "G_DBUS_MESSAGE_HEADER_FIELD_SENDER", "sender" },
        { G_DBUS_MESSAGE_HEADER_FIELD_SIGNATURE, "G_DBUS_MESSAGE_HEADER_FIELD_SIGNATURE", "signature" },
        { G_DBUS_MESSAGE_HEADER_FIELD_NUM_UNIX_FDS, "G_DBUS_MESSAGE_HEADER_FIELD_NUM_UNIX_FDS", "num-unix-fds" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GDBusMessageHeaderField"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_dbus_property_info_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_DBUS_PROPERTY_INFO_FLAGS_NONE, "G_DBUS_PROPERTY_INFO_FLAGS_NONE", "none" },
        { G_DBUS_PROPERTY_INFO_FLAGS_READABLE, "G_DBUS_PROPERTY_INFO_FLAGS_READABLE", "readable" },
        { G_DBUS_PROPERTY_INFO_FLAGS_WRITABLE, "G_DBUS_PROPERTY_INFO_FLAGS_WRITABLE", "writable" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GDBusPropertyInfoFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_dbus_subtree_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_DBUS_SUBTREE_FLAGS_NONE, "G_DBUS_SUBTREE_FLAGS_NONE", "none" },
        { G_DBUS_SUBTREE_FLAGS_DISPATCH_TO_UNENUMERATED_NODES, "G_DBUS_SUBTREE_FLAGS_DISPATCH_TO_UNENUMERATED_NODES", "dispatch-to-unenumerated-nodes" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GDBusSubtreeFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_dbus_server_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_DBUS_SERVER_FLAGS_NONE, "G_DBUS_SERVER_FLAGS_NONE", "none" },
        { G_DBUS_SERVER_FLAGS_RUN_IN_THREAD, "G_DBUS_SERVER_FLAGS_RUN_IN_THREAD", "run-in-thread" },
        { G_DBUS_SERVER_FLAGS_AUTHENTICATION_ALLOW_ANONYMOUS, "G_DBUS_SERVER_FLAGS_AUTHENTICATION_ALLOW_ANONYMOUS", "authentication-allow-anonymous" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GDBusServerFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_dbus_signal_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_DBUS_SIGNAL_FLAGS_NONE, "G_DBUS_SIGNAL_FLAGS_NONE", "none" },
        { G_DBUS_SIGNAL_FLAGS_NO_MATCH_RULE, "G_DBUS_SIGNAL_FLAGS_NO_MATCH_RULE", "no-match-rule" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GDBusSignalFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_dbus_send_message_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_DBUS_SEND_MESSAGE_FLAGS_NONE, "G_DBUS_SEND_MESSAGE_FLAGS_NONE", "none" },
        { G_DBUS_SEND_MESSAGE_FLAGS_PRESERVE_SERIAL, "G_DBUS_SEND_MESSAGE_FLAGS_PRESERVE_SERIAL", "preserve-serial" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GDBusSendMessageFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_credentials_type_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_CREDENTIALS_TYPE_INVALID, "G_CREDENTIALS_TYPE_INVALID", "invalid" },
        { G_CREDENTIALS_TYPE_LINUX_UCRED, "G_CREDENTIALS_TYPE_LINUX_UCRED", "linux-ucred" },
        { G_CREDENTIALS_TYPE_FREEBSD_CMSGCRED, "G_CREDENTIALS_TYPE_FREEBSD_CMSGCRED", "freebsd-cmsgcred" },
        { G_CREDENTIALS_TYPE_OPENBSD_SOCKPEERCRED, "G_CREDENTIALS_TYPE_OPENBSD_SOCKPEERCRED", "openbsd-sockpeercred" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GCredentialsType"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_dbus_message_byte_order_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_DBUS_MESSAGE_BYTE_ORDER_BIG_ENDIAN, "G_DBUS_MESSAGE_BYTE_ORDER_BIG_ENDIAN", "big-endian" },
        { G_DBUS_MESSAGE_BYTE_ORDER_LITTLE_ENDIAN, "G_DBUS_MESSAGE_BYTE_ORDER_LITTLE_ENDIAN", "little-endian" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GDBusMessageByteOrder"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_application_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_APPLICATION_FLAGS_NONE, "G_APPLICATION_FLAGS_NONE", "flags-none" },
        { G_APPLICATION_IS_SERVICE, "G_APPLICATION_IS_SERVICE", "is-service" },
        { G_APPLICATION_IS_LAUNCHER, "G_APPLICATION_IS_LAUNCHER", "is-launcher" },
        { G_APPLICATION_HANDLES_OPEN, "G_APPLICATION_HANDLES_OPEN", "handles-open" },
        { G_APPLICATION_HANDLES_COMMAND_LINE, "G_APPLICATION_HANDLES_COMMAND_LINE", "handles-command-line" },
        { G_APPLICATION_SEND_ENVIRONMENT, "G_APPLICATION_SEND_ENVIRONMENT", "send-environment" },
        { G_APPLICATION_NON_UNIQUE, "G_APPLICATION_NON_UNIQUE", "non-unique" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GApplicationFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_tls_error_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_TLS_ERROR_UNAVAILABLE, "G_TLS_ERROR_UNAVAILABLE", "unavailable" },
        { G_TLS_ERROR_MISC, "G_TLS_ERROR_MISC", "misc" },
        { G_TLS_ERROR_BAD_CERTIFICATE, "G_TLS_ERROR_BAD_CERTIFICATE", "bad-certificate" },
        { G_TLS_ERROR_NOT_TLS, "G_TLS_ERROR_NOT_TLS", "not-tls" },
        { G_TLS_ERROR_HANDSHAKE, "G_TLS_ERROR_HANDSHAKE", "handshake" },
        { G_TLS_ERROR_CERTIFICATE_REQUIRED, "G_TLS_ERROR_CERTIFICATE_REQUIRED", "certificate-required" },
        { G_TLS_ERROR_EOF, "G_TLS_ERROR_EOF", "eof" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GTlsError"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_tls_certificate_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_TLS_CERTIFICATE_UNKNOWN_CA, "G_TLS_CERTIFICATE_UNKNOWN_CA", "unknown-ca" },
        { G_TLS_CERTIFICATE_BAD_IDENTITY, "G_TLS_CERTIFICATE_BAD_IDENTITY", "bad-identity" },
        { G_TLS_CERTIFICATE_NOT_ACTIVATED, "G_TLS_CERTIFICATE_NOT_ACTIVATED", "not-activated" },
        { G_TLS_CERTIFICATE_EXPIRED, "G_TLS_CERTIFICATE_EXPIRED", "expired" },
        { G_TLS_CERTIFICATE_REVOKED, "G_TLS_CERTIFICATE_REVOKED", "revoked" },
        { G_TLS_CERTIFICATE_INSECURE, "G_TLS_CERTIFICATE_INSECURE", "insecure" },
        { G_TLS_CERTIFICATE_GENERIC_ERROR, "G_TLS_CERTIFICATE_GENERIC_ERROR", "generic-error" },
        { G_TLS_CERTIFICATE_VALIDATE_ALL, "G_TLS_CERTIFICATE_VALIDATE_ALL", "validate-all" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GTlsCertificateFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_tls_authentication_mode_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_TLS_AUTHENTICATION_NONE, "G_TLS_AUTHENTICATION_NONE", "none" },
        { G_TLS_AUTHENTICATION_REQUESTED, "G_TLS_AUTHENTICATION_REQUESTED", "requested" },
        { G_TLS_AUTHENTICATION_REQUIRED, "G_TLS_AUTHENTICATION_REQUIRED", "required" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GTlsAuthenticationMode"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_tls_rehandshake_mode_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { G_TLS_REHANDSHAKE_NEVER, "G_TLS_REHANDSHAKE_NEVER", "never" },
        { G_TLS_REHANDSHAKE_SAFELY, "G_TLS_REHANDSHAKE_SAFELY", "safely" },
        { G_TLS_REHANDSHAKE_UNSAFELY, "G_TLS_REHANDSHAKE_UNSAFELY", "unsafely" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("GTlsRehandshakeMode"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_dbus_interface_skeleton_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_DBUS_INTERFACE_SKELETON_FLAGS_NONE, "G_DBUS_INTERFACE_SKELETON_FLAGS_NONE", "none" },
        { G_DBUS_INTERFACE_SKELETON_FLAGS_HANDLE_METHOD_INVOCATIONS_IN_THREAD, "G_DBUS_INTERFACE_SKELETON_FLAGS_HANDLE_METHOD_INVOCATIONS_IN_THREAD", "handle-method-invocations-in-thread" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GDBusInterfaceSkeletonFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
g_dbus_object_manager_client_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_NONE, "G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_NONE", "none" },
        { G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_DO_NOT_AUTO_START, "G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_DO_NOT_AUTO_START", "do-not-auto-start" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GDBusObjectManagerClientFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

/* enumerations from "gsettings.h" */
GType
g_settings_bind_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { G_SETTINGS_BIND_DEFAULT, "G_SETTINGS_BIND_DEFAULT", "default" },
        { G_SETTINGS_BIND_GET, "G_SETTINGS_BIND_GET", "get" },
        { G_SETTINGS_BIND_SET, "G_SETTINGS_BIND_SET", "set" },
        { G_SETTINGS_BIND_NO_SENSITIVITY, "G_SETTINGS_BIND_NO_SENSITIVITY", "no-sensitivity" },
        { G_SETTINGS_BIND_GET_NO_CHANGES, "G_SETTINGS_BIND_GET_NO_CHANGES", "get-no-changes" },
        { G_SETTINGS_BIND_INVERT_BOOLEAN, "G_SETTINGS_BIND_INVERT_BOOLEAN", "invert-boolean" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("GSettingsBindFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}


/* Generated data ends here */

