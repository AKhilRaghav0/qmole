/*
 * Copyright (C) 2010 Free Software Foundation, Inc.
 * Author: Nikos Mavrogiannopoulos
 *
 * This file is part of GnuTLS.
 *
 * GnuTLS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GnuTLS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <config.h>

#include <getpass.h>

#include <gnutls/gnutls.h>
#include <gnutls/extra.h>
#include <gnutls/pkcs11.h>
#include <gnutls/abstract.h>
#include <stdio.h>
#include <stdlib.h>
#include "p11tool.h"
#include "certtool-cfg.h"
#include "certtool-common.h"
#include <unistd.h>
#include <string.h>
#include <p11common.h>

void
pkcs11_delete (FILE * outfile, const char *url, int batch, unsigned int login,
               common_info_st * info)
{
  int ret;
  unsigned int obj_flags = 0;

  if (login)
    obj_flags = GNUTLS_PKCS11_OBJ_FLAG_LOGIN;

  if (!batch)
    {
      pkcs11_list (outfile, url, PKCS11_TYPE_ALL, login,
                   GNUTLS_PKCS11_URL_LIB, info);
      ret =
        read_yesno ("Are you sure you want to delete those objects? (y/N): ");
      if (ret == 0)
        {
          exit (1);
        }
    }

  ret = gnutls_pkcs11_delete_url (url, obj_flags);
  if (ret < 0)
    {
      fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
               gnutls_strerror (ret));
      exit (1);
    }

  fprintf (outfile, "\n%d objects deleted\n", ret);

  return;
}

/* lists certificates from a token
 */
void
pkcs11_list (FILE * outfile, const char *url, int type, unsigned int login,
             unsigned int detailed, common_info_st * info)
{
  gnutls_pkcs11_obj_t *crt_list;
  gnutls_x509_crt_t xcrt;
  unsigned int crt_list_size = 0;
  int ret;
  char *output;
  int i, attrs;
  unsigned int obj_flags = 0;

  if (login)
    obj_flags = GNUTLS_PKCS11_OBJ_FLAG_LOGIN;

  pkcs11_common ();

  if (url == NULL)
    url = "pkcs11:";

  if (type == PKCS11_TYPE_TRUSTED)
    {
      attrs = GNUTLS_PKCS11_OBJ_ATTR_CRT_TRUSTED;
    }
  else if (type == PKCS11_TYPE_PK)
    {
      attrs = GNUTLS_PKCS11_OBJ_ATTR_CRT_WITH_PRIVKEY;
    }
  else if (type == PKCS11_TYPE_CRT_ALL)
    {
      attrs = GNUTLS_PKCS11_OBJ_ATTR_CRT_ALL;
    }
  else if (type == PKCS11_TYPE_PRIVKEY)
    {
      attrs = GNUTLS_PKCS11_OBJ_ATTR_PRIVKEY;
    }
  else
    {
      attrs = GNUTLS_PKCS11_OBJ_ATTR_ALL;
    }

  /* give some initial value to avoid asking for the pkcs11 pin twice.
   */
  crt_list_size = 128;
  crt_list = malloc (sizeof (*crt_list) * crt_list_size);
  if (crt_list == NULL)
    {
      fprintf (stderr, "Memory error\n");
      exit (1);
    }

  ret = gnutls_pkcs11_obj_list_import_url (crt_list, &crt_list_size, url,
                                           attrs, obj_flags);
  if (ret < 0 && ret != GNUTLS_E_SHORT_MEMORY_BUFFER)
    {
      fprintf (stderr, "Error in crt_list_import (1): %s\n",
               gnutls_strerror (ret));
      exit (1);
    }

  if (crt_list_size == 0)
    {
      fprintf (stderr, "No matching objects found\n");
      exit (0);
    }

  if (ret == GNUTLS_E_SHORT_MEMORY_BUFFER)
    {
      crt_list = realloc (crt_list, sizeof (*crt_list) * crt_list_size);
      if (crt_list == NULL)
        {
          fprintf (stderr, "Memory error\n");
          exit (1);
        }

      ret =
        gnutls_pkcs11_obj_list_import_url (crt_list, &crt_list_size, url,
                                           attrs, obj_flags);
      if (ret < 0)
        {
          fprintf (stderr, "Error in crt_list_import: %s\n",
                   gnutls_strerror (ret));
          exit (1);
        }
    }

  for (i = 0; i < crt_list_size; i++)
    {
      char buf[128];
      size_t size;

      ret = gnutls_pkcs11_obj_export_url (crt_list[i], detailed, &output);
      if (ret < 0)
        {
          fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                   gnutls_strerror (ret));
          exit (1);
        }

      fprintf (outfile, "Object %d:\n\tURL: %s\n", i, output);

      fprintf (outfile, "\tType: %s\n",
               gnutls_pkcs11_type_get_name (gnutls_pkcs11_obj_get_type
                                            (crt_list[i])));

      size = sizeof (buf);
      ret =
        gnutls_pkcs11_obj_get_info (crt_list[i], GNUTLS_PKCS11_OBJ_LABEL, buf,
                                    &size);
      if (ret < 0)
        {
          fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                   gnutls_strerror (ret));
          exit (1);
        }
      fprintf (outfile, "\tLabel: %s\n", buf);

      size = sizeof (buf);
      ret =
        gnutls_pkcs11_obj_get_info (crt_list[i], GNUTLS_PKCS11_OBJ_ID_HEX,
                                    buf, &size);
      if (ret < 0)
        {
          fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                   gnutls_strerror (ret));
          exit (1);
        }
      fprintf (outfile, "\tID: %s\n\n", buf);



      if (attrs == GNUTLS_PKCS11_OBJ_ATTR_ALL
          || attrs == GNUTLS_PKCS11_OBJ_ATTR_PRIVKEY)
        continue;

      ret = gnutls_x509_crt_init (&xcrt);
      if (ret < 0)
        {
          fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                   gnutls_strerror (ret));
          exit (1);
        }

      ret = gnutls_x509_crt_import_pkcs11 (xcrt, crt_list[i]);
      if (ret < 0)
        {
          fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                   gnutls_strerror (ret));
          exit (1);
        }

#if 0
      size = buffer_size;
      ret = gnutls_x509_crt_export (xcrt, GNUTLS_X509_FMT_PEM, buffer, &size);
      if (ret < 0)
        {
          fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                   gnutls_strerror (ret));
          exit (1);
        }

      fwrite (buffer, 1, size, outfile);
      fputs ("\n\n", outfile);
#endif

      gnutls_x509_crt_deinit (xcrt);


    }

  return;
}

void
pkcs11_export (FILE * outfile, const char *url, unsigned int login,
               common_info_st * info)
{
  gnutls_pkcs11_obj_t crt;
  gnutls_x509_crt_t xcrt;
  gnutls_pubkey_t pubkey;
  int ret;
  size_t size;
  unsigned int obj_flags = 0;

  if (login)
    obj_flags = GNUTLS_PKCS11_OBJ_FLAG_LOGIN;

  pkcs11_common ();

  if (url == NULL)
    url = "pkcs11:";

  ret = gnutls_pkcs11_obj_init (&crt);
  if (ret < 0)
    {
      fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
               gnutls_strerror (ret));
      exit (1);
    }

  ret = gnutls_pkcs11_obj_import_url (crt, url, obj_flags);
  if (ret < 0)
    {
      fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
               gnutls_strerror (ret));
      exit (1);
    }

  switch (gnutls_pkcs11_obj_get_type (crt))
    {
    case GNUTLS_PKCS11_OBJ_X509_CRT:
      ret = gnutls_x509_crt_init (&xcrt);
      if (ret < 0)
        {
          fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                   gnutls_strerror (ret));
          exit (1);
        }

      ret = gnutls_x509_crt_import_pkcs11 (xcrt, crt);
      if (ret < 0)
        {
          fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                   gnutls_strerror (ret));
          exit (1);
        }

      size = buffer_size;
      ret = gnutls_x509_crt_export (xcrt, GNUTLS_X509_FMT_PEM, buffer, &size);
      if (ret < 0)
        {
          fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                   gnutls_strerror (ret));
          exit (1);
        }
      fwrite (buffer, 1, size, outfile);

      gnutls_x509_crt_deinit (xcrt);
      break;
    case GNUTLS_PKCS11_OBJ_PUBKEY:
      ret = gnutls_pubkey_init (&pubkey);
      if (ret < 0)
        {
          fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                   gnutls_strerror (ret));
          exit (1);
        }

      ret = gnutls_pubkey_import_pkcs11 (pubkey, crt, 0);
      if (ret < 0)
        {
          fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                   gnutls_strerror (ret));
          exit (1);
        }

      size = buffer_size;
      ret = gnutls_pubkey_export (pubkey, GNUTLS_X509_FMT_PEM, buffer, &size);
      if (ret < 0)
        {
          fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                   gnutls_strerror (ret));
          exit (1);
        }
      fwrite (buffer, 1, size, outfile);

      gnutls_pubkey_deinit (pubkey);
      break;
    default:
      {
        gnutls_datum data, enc;

        size = buffer_size;
        ret = gnutls_pkcs11_obj_export (crt, buffer, &size);
        if (ret < 0)
          {
            break;
          }

        data.data = buffer;
        data.size = size;

        ret = gnutls_pem_base64_encode_alloc ("DATA", &data, &enc);
        if (ret < 0)
          {
            fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                     gnutls_strerror (ret));
            exit (1);
          }

        fwrite (enc.data, 1, enc.size, outfile);

        gnutls_free (enc.data);
        break;
      }
    }
  fputs ("\n\n", outfile);


  gnutls_pkcs11_obj_deinit (crt);

  return;

}

void
pkcs11_token_list (FILE * outfile, unsigned int detailed,
                   common_info_st * info)
{
  int ret;
  int i;
  char *url;
  char buf[128];
  size_t size;

  pkcs11_common ();

  for (i = 0;; i++)
    {
      ret = gnutls_pkcs11_token_get_url (i, detailed, &url);
      if (ret == GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE)
        break;

      if (ret < 0)
        {
          fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                   gnutls_strerror (ret));
          exit (1);
        }

      fprintf (outfile, "Token %d:\n\tURL: %s\n", i, url);

      size = sizeof (buf);
      ret =
        gnutls_pkcs11_token_get_info (url, GNUTLS_PKCS11_TOKEN_LABEL, buf,
                                      &size);
      if (ret < 0)
        {
          fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                   gnutls_strerror (ret));
          exit (1);
        }

      fprintf (outfile, "\tLabel: %s\n", buf);

      size = sizeof (buf);
      ret =
        gnutls_pkcs11_token_get_info (url, GNUTLS_PKCS11_TOKEN_MANUFACTURER,
                                      buf, &size);
      if (ret < 0)
        {
          fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                   gnutls_strerror (ret));
          exit (1);
        }

      fprintf (outfile, "\tManufacturer: %s\n", buf);

      size = sizeof (buf);
      ret =
        gnutls_pkcs11_token_get_info (url, GNUTLS_PKCS11_TOKEN_MODEL, buf,
                                      &size);
      if (ret < 0)
        {
          fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                   gnutls_strerror (ret));
          exit (1);
        }

      fprintf (outfile, "\tModel: %s\n", buf);

      size = sizeof (buf);
      ret =
        gnutls_pkcs11_token_get_info (url, GNUTLS_PKCS11_TOKEN_SERIAL, buf,
                                      &size);
      if (ret < 0)
        {
          fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                   gnutls_strerror (ret));
          exit (1);
        }

      fprintf (outfile, "\tSerial: %s\n", buf);
      fprintf (outfile, "\n\n");

      gnutls_free (url);

    }

  return;
}

void
pkcs11_write (FILE * outfile, const char *url, const char *label, int trusted,
              unsigned int login, common_info_st * info)
{
  gnutls_x509_crt_t xcrt;
  gnutls_x509_privkey_t xkey;
  int ret;
  unsigned int flags = 0;
  unsigned int key_usage = 0;
  gnutls_datum_t *secret_key;

  if (login)
    flags = GNUTLS_PKCS11_OBJ_FLAG_LOGIN;

  pkcs11_common ();

  if (url == NULL)
    url = "pkcs11:";

  secret_key = load_secret_key (0, info);
  if (secret_key != NULL)
    {
      ret =
        gnutls_pkcs11_copy_secret_key (url, secret_key, label, key_usage,
                                       flags |
                                       GNUTLS_PKCS11_OBJ_FLAG_MARK_SENSITIVE);
      if (ret < 0)
        {
          fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                   gnutls_strerror (ret));
          exit (1);
        }
    }

  xcrt = load_cert (0, info);
  if (xcrt != NULL)
    {
      if (trusted)
        flags |= GNUTLS_PKCS11_OBJ_FLAG_MARK_TRUSTED|GNUTLS_PKCS11_OBJ_FLAG_LOGIN_SO;

      ret = gnutls_pkcs11_copy_x509_crt (url, xcrt, label, flags);
      if (ret < 0)
        {
          fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                   gnutls_strerror (ret));
          exit (1);
        }

      gnutls_x509_crt_get_key_usage (xcrt, &key_usage, NULL);
    }

  xkey = load_x509_private_key (0, info);
  if (xkey != NULL)
    {
      ret =
        gnutls_pkcs11_copy_x509_privkey (url, xkey, label, key_usage,
                                         flags |
                                         GNUTLS_PKCS11_OBJ_FLAG_MARK_SENSITIVE);
      if (ret < 0)
        {
          fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
                   gnutls_strerror (ret));
          exit (1);
        }
    }

  if (xkey == NULL && xcrt == NULL && secret_key == NULL)
    {
      fprintf (stderr,
               "You must use --load-privkey, --load-certificate or --secret-key to load the file to be copied\n");
      exit (1);
    }

  return;
}

void
pkcs11_init (FILE * outfile, const char *url, const char *label,
             common_info_st * info)
{
  int ret;
  char *pin;
  char so_pin[32];

  pkcs11_common ();

  if (url == NULL)
    {
      fprintf (stderr, "No token URL given to initialize!\n");
      exit (1);
    }

  pin = getpass ("Enter Security Officer's PIN: ");
  if (pin == NULL)
    exit (1);

  if (strlen(pin) >= sizeof(so_pin))
    exit (1);

  strcpy (so_pin, pin);

  pin = getpass ("Enter new User's PIN: ");
  if (pin == NULL)
    exit (1);

  ret = gnutls_pkcs11_token_init (url, so_pin, label);
  if (ret < 0)
    {
      fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
               gnutls_strerror (ret));
      exit (1);
    }

  ret = gnutls_pkcs11_token_set_pin (url, NULL, pin, GNUTLS_PKCS11_PIN_USER);
  if (ret < 0)
    {
      fprintf (stderr, "Error in %s:%d: %s\n", __func__, __LINE__,
               gnutls_strerror (ret));
      exit (1);
    }

  return;
}

const char *mech_list[] = {
  [0] = "CKM_RSA_PKCS_KEY_PAIR_GEN",
  [1] = "CKM_RSA_PKCS",
  [2] = "CKM_RSA_9796",
  [3] = "CKM_RSA_X_509",
  [4] = "CKM_MD2_RSA_PKCS",
  [5] = "CKM_MD5_RSA_PKCS",
  [6] = "CKM_SHA1_RSA_PKCS",
  [7] = "CKM_RIPEMD128_RSA_PKCS",
  [8] = "CKM_RIPEMD160_RSA_PKCS",
  [9] = "CKM_RSA_PKCS_OAEP",
  [0xa] = "CKM_RSA_X9_31_KEY_PAIR_GEN",
  [0xb] = "CKM_RSA_X9_31",
  [0xc] = "CKM_SHA1_RSA_X9_31",
  [0xd] = "CKM_RSA_PKCS_PSS",
  [0xe] = "CKM_SHA1_RSA_PKCS_PSS",
  [0x10] = "CKM_DSA_KEY_PAIR_GEN",
  [0x11] = "CKM_DSA",
  [0x12] = "CKM_DSA_SHA1",
  [0x20] = "CKM_DH_PKCS_KEY_PAIR_GEN",
  [0x21] = "CKM_DH_PKCS_DERIVE",
  [0x30] = "CKM_X9_42_DH_KEY_PAIR_GEN",
  [0x31] = "CKM_X9_42_DH_DERIVE",
  [0x32] = "CKM_X9_42_DH_HYBRID_DERIVE",
  [0x33] = "CKM_X9_42_MQV_DERIVE",
  [0x40] = "CKM_SHA256_RSA_PKCS",
  [0x41] = "CKM_SHA384_RSA_PKCS",
  [0x42] = "CKM_SHA512_RSA_PKCS",
  [0x43] = "CKM_SHA256_RSA_PKCS_PSS",
  [0x44] = "CKM_SHA384_RSA_PKCS_PSS",
  [0x45] = "CKM_SHA512_RSA_PKCS_PSS",
  [0x100] = "CKM_RC2_KEY_GEN",
  [0x101] = "CKM_RC2_ECB",
  [0x102] = "CKM_RC2_CBC",
  [0x103] = "CKM_RC2_MAC",
  [0x104] = "CKM_RC2_MAC_GENERAL",
  [0x105] = "CKM_RC2_CBC_PAD",
  [0x110] = "CKM_RC4_KEY_GEN",
  [0x111] = "CKM_RC4",
  [0x120] = "CKM_DES_KEY_GEN",
  [0x121] = "CKM_DES_ECB",
  [0x122] = "CKM_DES_CBC",
  [0x123] = "CKM_DES_MAC",
  [0x124] = "CKM_DES_MAC_GENERAL",
  [0x125] = "CKM_DES_CBC_PAD",
  [0x130] = "CKM_DES2_KEY_GEN",
  [0x131] = "CKM_DES3_KEY_GEN",
  [0x132] = "CKM_DES3_ECB",
  [0x133] = "CKM_DES3_CBC",
  [0x134] = "CKM_DES3_MAC",
  [0x135] = "CKM_DES3_MAC_GENERAL",
  [0x136] = "CKM_DES3_CBC_PAD",
  [0x140] = "CKM_CDMF_KEY_GEN",
  [0x141] = "CKM_CDMF_ECB",
  [0x142] = "CKM_CDMF_CBC",
  [0x143] = "CKM_CDMF_MAC",
  [0x144] = "CKM_CDMF_MAC_GENERAL",
  [0x145] = "CKM_CDMF_CBC_PAD",
  [0x200] = "CKM_MD2",
  [0x201] = "CKM_MD2_HMAC",
  [0x202] = "CKM_MD2_HMAC_GENERAL",
  [0x210] = "CKM_MD5",
  [0x211] = "CKM_MD5_HMAC",
  [0x212] = "CKM_MD5_HMAC_GENERAL",
  [0x220] = "CKM_SHA_1",
  [0x221] = "CKM_SHA_1_HMAC",
  [0x222] = "CKM_SHA_1_HMAC_GENERAL",
  [0x230] = "CKM_RIPEMD128",
  [0x231] = "CKM_RIPEMD128_HMAC",
  [0x232] = "CKM_RIPEMD128_HMAC_GENERAL",
  [0x240] = "CKM_RIPEMD160",
  [0x241] = "CKM_RIPEMD160_HMAC",
  [0x242] = "CKM_RIPEMD160_HMAC_GENERAL",
  [0x250] = "CKM_SHA256",
  [0x251] = "CKM_SHA256_HMAC",
  [0x252] = "CKM_SHA256_HMAC_GENERAL",
  [0x260] = "CKM_SHA384",
  [0x261] = "CKM_SHA384_HMAC",
  [0x262] = "CKM_SHA384_HMAC_GENERAL",
  [0x270] = "CKM_SHA512",
  [0x271] = "CKM_SHA512_HMAC",
  [0x272] = "CKM_SHA512_HMAC_GENERAL",
  [0x300] = "CKM_CAST_KEY_GEN",
  [0x301] = "CKM_CAST_ECB",
  [0x302] = "CKM_CAST_CBC",
  [0x303] = "CKM_CAST_MAC",
  [0x304] = "CKM_CAST_MAC_GENERAL",
  [0x305] = "CKM_CAST_CBC_PAD",
  [0x310] = "CKM_CAST3_KEY_GEN",
  [0x311] = "CKM_CAST3_ECB",
  [0x312] = "CKM_CAST3_CBC",
  [0x313] = "CKM_CAST3_MAC",
  [0x314] = "CKM_CAST3_MAC_GENERAL",
  [0x315] = "CKM_CAST3_CBC_PAD",
  [0x320] = "CKM_CAST128_KEY_GEN",
  [0x321] = "CKM_CAST128_ECB",
  [0x322] = "CKM_CAST128_CBC",
  [0x323] = "CKM_CAST128_MAC",
  [0x324] = "CKM_CAST128_MAC_GENERAL",
  [0x325] = "CKM_CAST128_CBC_PAD",
  [0x330] = "CKM_RC5_KEY_GEN",
  [0x331] = "CKM_RC5_ECB",
  [0x332] = "CKM_RC5_CBC",
  [0x333] = "CKM_RC5_MAC",
  [0x334] = "CKM_RC5_MAC_GENERAL",
  [0x335] = "CKM_RC5_CBC_PAD",
  [0x340] = "CKM_IDEA_KEY_GEN",
  [0x341] = "CKM_IDEA_ECB",
  [0x342] = "CKM_IDEA_CBC",
  [0x343] = "CKM_IDEA_MAC",
  [0x344] = "CKM_IDEA_MAC_GENERAL",
  [0x345] = "CKM_IDEA_CBC_PAD",
  [0x350] = "CKM_GENERIC_SECRET_KEY_GEN",
  [0x360] = "CKM_CONCATENATE_BASE_AND_KEY",
  [0x362] = "CKM_CONCATENATE_BASE_AND_DATA",
  [0x363] = "CKM_CONCATENATE_DATA_AND_BASE",
  [0x364] = "CKM_XOR_BASE_AND_DATA",
  [0x365] = "CKM_EXTRACT_KEY_FROM_KEY",
  [0x370] = "CKM_SSL3_PRE_MASTER_KEY_GEN",
  [0x371] = "CKM_SSL3_MASTER_KEY_DERIVE",
  [0x372] = "CKM_SSL3_KEY_AND_MAC_DERIVE",
  [0x373] = "CKM_SSL3_MASTER_KEY_DERIVE_DH",
  [0x374] = "CKM_TLS_PRE_MASTER_KEY_GEN",
  [0x375] = "CKM_TLS_MASTER_KEY_DERIVE",
  [0x376] = "CKM_TLS_KEY_AND_MAC_DERIVE",
  [0x377] = "CKM_TLS_MASTER_KEY_DERIVE_DH",
  [0x380] = "CKM_SSL3_MD5_MAC",
  [0x381] = "CKM_SSL3_SHA1_MAC",
  [0x390] = "CKM_MD5_KEY_DERIVATION",
  [0x391] = "CKM_MD2_KEY_DERIVATION",
  [0x392] = "CKM_SHA1_KEY_DERIVATION",
  [0x3a0] = "CKM_PBE_MD2_DES_CBC",
  [0x3a1] = "CKM_PBE_MD5_DES_CBC",
  [0x3a2] = "CKM_PBE_MD5_CAST_CBC",
  [0x3a3] = "CKM_PBE_MD5_CAST3_CBC",
  [0x3a4] = "CKM_PBE_MD5_CAST128_CBC",
  [0x3a5] = "CKM_PBE_SHA1_CAST128_CBC",
  [0x3a6] = "CKM_PBE_SHA1_RC4_128",
  [0x3a7] = "CKM_PBE_SHA1_RC4_40",
  [0x3a8] = "CKM_PBE_SHA1_DES3_EDE_CBC",
  [0x3a9] = "CKM_PBE_SHA1_DES2_EDE_CBC",
  [0x3aa] = "CKM_PBE_SHA1_RC2_128_CBC",
  [0x3ab] = "CKM_PBE_SHA1_RC2_40_CBC",
  [0x3b0] = "CKM_PKCS5_PBKD2",
  [0x3c0] = "CKM_PBA_SHA1_WITH_SHA1_HMAC",
  [0x400] = "CKM_KEY_WRAP_LYNKS",
  [0x401] = "CKM_KEY_WRAP_SET_OAEP",
  [0x1000] = "CKM_SKIPJACK_KEY_GEN",
  [0x1001] = "CKM_SKIPJACK_ECB64",
  [0x1002] = "CKM_SKIPJACK_CBC64",
  [0x1003] = "CKM_SKIPJACK_OFB64",
  [0x1004] = "CKM_SKIPJACK_CFB64",
  [0x1005] = "CKM_SKIPJACK_CFB32",
  [0x1006] = "CKM_SKIPJACK_CFB16",
  [0x1007] = "CKM_SKIPJACK_CFB8",
  [0x1008] = "CKM_SKIPJACK_WRAP",
  [0x1009] = "CKM_SKIPJACK_PRIVATE_WRAP",
  [0x100a] = "CKM_SKIPJACK_RELAYX",
  [0x1010] = "CKM_KEA_KEY_PAIR_GEN",
  [0x1011] = "CKM_KEA_KEY_DERIVE",
  [0x1020] = "CKM_FORTEZZA_TIMESTAMP",
  [0x1030] = "CKM_BATON_KEY_GEN",
  [0x1031] = "CKM_BATON_ECB128",
  [0x1032] = "CKM_BATON_ECB96",
  [0x1033] = "CKM_BATON_CBC128",
  [0x1034] = "CKM_BATON_COUNTER",
  [0x1035] = "CKM_BATON_SHUFFLE",
  [0x1036] = "CKM_BATON_WRAP",
  [0x1040] = "CKM_ECDSA_KEY_PAIR_GEN",
  [0x1041] = "CKM_ECDSA",
  [0x1042] = "CKM_ECDSA_SHA1",
  [0x1050] = "CKM_ECDH1_DERIVE",
  [0x1051] = "CKM_ECDH1_COFACTOR_DERIVE",
  [0x1052] = "CKM_ECMQV_DERIVE",
  [0x1060] = "CKM_JUNIPER_KEY_GEN",
  [0x1061] = "CKM_JUNIPER_ECB128",
  [0x1062] = "CKM_JUNIPER_CBC128",
  [0x1063] = "CKM_JUNIPER_COUNTER",
  [0x1064] = "CKM_JUNIPER_SHUFFLE",
  [0x1065] = "CKM_JUNIPER_WRAP",
  [0x1070] = "CKM_FASTHASH",
  [0x1080] = "CKM_AES_KEY_GEN",
  [0x1081] = "CKM_AES_ECB",
  [0x1082] = "CKM_AES_CBC",
  [0x1083] = "CKM_AES_MAC",
  [0x1084] = "CKM_AES_MAC_GENERAL",
  [0x1085] = "CKM_AES_CBC_PAD",
  [0x2000] = "CKM_DSA_PARAMETER_GEN",
  [0x2001] = "CKM_DH_PKCS_PARAMETER_GEN",
  [0x2002] = "CKM_X9_42_DH_PARAMETER_GEN",
  [0x255] = "CKM_SHA224",
  [0x256] = "CKM_SHA224_HMAC",
  [0x257] = "CKM_SHA224_HMAC_GENERAL",
  [0x46] = "CKM_SHA224_RSA_PKCS",
  [0x47] = "CKM_SHA224_RSA_PKCS_PSS",
  [0x396] = "CKM_SHA224_KEY_DERIVATION",
  [0x550] = "CKM_CAMELLIA_KEY_GEN",
  [0x551] = "CKM_CAMELLIA_ECB",
  [0x552] = "CKM_CAMELLIA_CBC",
  [0x553] = "CKM_CAMELLIA_MAC",
  [0x554] = "CKM_CAMELLIA_MAC_GENERAL",
  [0x555] = "CKM_CAMELLIA_CBC_PAD",
  [0x556] = "CKM_CAMELLIA_ECB_ENCRYPT_DATA",
  [0x557] = "CKM_CAMELLIA_CBC_ENCRYPT_DATA"
};

void
pkcs11_mechanism_list (FILE * outfile, const char *url, unsigned int login,
                       common_info_st * info)
{
  int ret;
  int idx;
  unsigned int obj_flags = 0;
  unsigned long mechanism;
  const char *str;

  if (login)
    obj_flags = GNUTLS_PKCS11_OBJ_FLAG_LOGIN;

  pkcs11_common ();

  if (url == NULL)
    url = "pkcs11:";

  idx = 0;
  do
    {
      ret = gnutls_pkcs11_token_get_mechanism (url, idx++, &mechanism);
      if (ret >= 0)
        {
          str = NULL;
          if (mechanism <= sizeof (mech_list) / sizeof (mech_list[0]))
            str = mech_list[mechanism];
          if (str == NULL)
            str = "UNKNOWN";

          fprintf (outfile, "[0x%.4lx] %s\n", mechanism, str);
        }
    }
  while (ret >= 0);


  return;
}
