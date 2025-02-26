/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2008 Red Hat, Inc.
 */

#ifndef SOUP_LOGGER_H
#define SOUP_LOGGER_H 1

#include <libsoup/soup-types.h>

#define SOUP_TYPE_LOGGER            (soup_logger_get_type ())
#define SOUP_LOGGER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOUP_TYPE_LOGGER, SoupLogger))
#define SOUP_LOGGER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SOUP_TYPE_LOGGER, SoupLoggerClass))
#define SOUP_IS_LOGGER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOUP_TYPE_LOGGER))
#define SOUP_IS_LOGGER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), SOUP_TYPE_LOGGER))
#define SOUP_LOGGER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SOUP_TYPE_LOGGER, SoupLoggerClass))

typedef enum {
	SOUP_LOGGER_LOG_NONE,
	SOUP_LOGGER_LOG_MINIMAL,
	SOUP_LOGGER_LOG_HEADERS,
	SOUP_LOGGER_LOG_BODY
} SoupLoggerLogLevel;

typedef struct {
	GObject parent;

} SoupLogger;

typedef struct {
	GObjectClass parent_class;

	/* Padding for future expansion */
	void (*_libsoup_reserved1) (void);
	void (*_libsoup_reserved2) (void);
	void (*_libsoup_reserved3) (void);
	void (*_libsoup_reserved4) (void);
} SoupLoggerClass;

typedef SoupLoggerLogLevel (*SoupLoggerFilter)  (SoupLogger         *logger,
						 SoupMessage        *msg,
						 gpointer            user_data);

typedef void               (*SoupLoggerPrinter) (SoupLogger         *logger,
						 SoupLoggerLogLevel  level,
						 char                direction,
						 const char         *data,
						 gpointer            user_data);

GType       soup_logger_get_type    (void);

SoupLogger *soup_logger_new         (SoupLoggerLogLevel  level,
				     int                 max_body_size);
void        soup_logger_attach      (SoupLogger         *logger,
				     SoupSession        *session);
void        soup_logger_detach      (SoupLogger         *logger,
				     SoupSession        *session);

void        soup_logger_set_request_filter  (SoupLogger        *logger,
					     SoupLoggerFilter   request_filter,
					     gpointer           filter_data,
					     GDestroyNotify     destroy);
void        soup_logger_set_response_filter (SoupLogger        *logger,
					     SoupLoggerFilter   response_filter,
					     gpointer           filter_data,
					     GDestroyNotify     destroy);

void        soup_logger_set_printer         (SoupLogger        *logger,
					     SoupLoggerPrinter  printer,
					     gpointer           printer_data,
					     GDestroyNotify     destroy);

#endif /* SOUP_LOGGER_H */
