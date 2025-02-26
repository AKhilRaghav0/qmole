/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/* gkm-data-der.h - parsing and serializing of common crypto DER structures

   Copyright (C) 2007 Stefan Walter

   The Gnome Keyring Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Keyring Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gnome Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.

   Author: Stef Walter <stef@memberwebs.com>
*/

#ifndef GKRPKIXDER_H_
#define GKRPKIXDER_H_

#include <glib.h>
#include <gcrypt.h>

#include "gkm-data-types.h"

#include "egg/egg-asn1x.h"

/* -----------------------------------------------------------------------------
 * PRIVATE KEYS
 */

GkmDataResult      gkm_data_der_read_private_key_rsa         (gconstpointer data, gsize n_data,
                                                              gcry_sexp_t *s_key);

GkmDataResult      gkm_data_der_read_private_key_dsa         (gconstpointer data, gsize n_data,
                                                              gcry_sexp_t *s_key);

GkmDataResult      gkm_data_der_read_private_key_dsa_parts   (gconstpointer keydata, gsize n_keydata,
                                                              gconstpointer params, gsize n_params,
                                                              gcry_sexp_t *s_key);

GkmDataResult      gkm_data_der_read_private_key             (gconstpointer data, gsize n_data,
                                                              gcry_sexp_t *s_key);

GkmDataResult      gkm_data_der_read_private_pkcs8           (gconstpointer data, gsize n_data,
                                                              const gchar *password, gsize n_password,
                                                              gcry_sexp_t *s_key);

GkmDataResult      gkm_data_der_read_private_pkcs8_plain     (gconstpointer data, gsize n_data,
                                                              gcry_sexp_t *s_key);

GkmDataResult      gkm_data_der_read_private_pkcs8_crypted   (gconstpointer data, gsize n_data,
                                                              const gchar *password, gsize n_password,
                                                              gcry_sexp_t *s_key);

gpointer           gkm_data_der_write_private_key_rsa        (gcry_sexp_t s_key, gsize *n_data);

gpointer           gkm_data_der_write_private_key_dsa        (gcry_sexp_t s_key, gsize *len);

gpointer           gkm_data_der_write_private_key_dsa_part   (gcry_sexp_t skey, gsize *n_key);

gpointer           gkm_data_der_write_private_key_dsa_params (gcry_sexp_t skey, gsize *n_params);

gpointer           gkm_data_der_write_private_key            (gcry_sexp_t s_key, gsize *n_data);

gpointer           gkm_data_der_write_private_pkcs8_plain    (gcry_sexp_t skey, gsize *n_data);

gpointer           gkm_data_der_write_private_pkcs8_crypted  (gcry_sexp_t skey, const gchar *password,
                                                              gsize n_password, gsize *n_data);

/* -----------------------------------------------------------------------------
 * PUBLIC KEYS
 */

GkmDataResult      gkm_data_der_read_public_key_rsa        (gconstpointer data, gsize n_data,
                                                            gcry_sexp_t *s_key);

GkmDataResult      gkm_data_der_read_public_key_dsa        (gconstpointer data, gsize n_data,
                                                            gcry_sexp_t *s_key);

GkmDataResult      gkm_data_der_read_public_key_dsa_parts  (gconstpointer keydata, gsize n_keydata,
                                                            gconstpointer params, gsize n_params,
                                                            gcry_sexp_t *s_key);

GkmDataResult      gkm_data_der_read_public_key            (gconstpointer data, gsize n_data,
                                                            gcry_sexp_t *s_key);

GkmDataResult      gkm_data_der_read_public_key_info       (gconstpointer data, gsize n_data,
                                                            gcry_sexp_t *s_key);

gpointer           gkm_data_der_write_public_key_rsa       (gcry_sexp_t s_key, gsize *len);

gpointer           gkm_data_der_write_public_key_dsa       (gcry_sexp_t s_key, gsize *len);

gpointer           gkm_data_der_write_public_key           (gcry_sexp_t s_key, gsize *len);


/* -----------------------------------------------------------------------------
 * CERTIFICATES
 */

GkmDataResult      gkm_data_der_read_certificate           (gconstpointer data, gsize n_data,
                                                            GNode **asn1);

GkmDataResult      gkm_data_der_read_basic_constraints     (gconstpointer data, gsize n_data,
                                                            gboolean *is_ca, gint *path_len);

GkmDataResult      gkm_data_der_read_key_usage             (gconstpointer data,
                                                            gsize n_data,
                                                            gulong *key_usage);

GkmDataResult      gkm_data_der_read_enhanced_usage        (gconstpointer data, gsize n_data,
                                                            GQuark **oids);

gpointer           gkm_data_der_write_certificate          (GNode *asn1, gsize *n_data);

#endif /*GKRPKIXDER_H_*/
