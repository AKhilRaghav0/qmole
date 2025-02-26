#if HAVE_CONFIG_H
# include "config.h"
#endif

#include <libtasn1.h>

const ASN1_ARRAY_TYPE pk_asn1_tab[] = {
  { "PK", 536872976, NULL },
  { NULL, 1073741836, NULL },
  { "RSAPublicKey", 1610612741, NULL },
  { "modulus", 1073741827, NULL },
  { "publicExponent", 3, NULL },
  { "RSAPrivateKey", 1610612741, NULL },
  { "version", 1073741826, "Version"},
  { "modulus", 1073741827, NULL },
  { "publicExponent", 1073741827, NULL },
  { "privateExponent", 1073741827, NULL },
  { "prime1", 1073741827, NULL },
  { "prime2", 1073741827, NULL },
  { "exponent1", 1073741827, NULL },
  { "exponent2", 1073741827, NULL },
  { "coefficient", 1073741827, NULL },
  { "otherPrimeInfos", 16386, "OtherPrimeInfos"},
  { "Version", 1610874883, NULL },
  { "two-prime", 1073741825, "0"},
  { "multi", 1, "1"},
  { "RSAParameters", 1073741844, NULL },
  { "OtherPrimeInfos", 1612709899, NULL },
  { "MAX", 1074266122, "1"},
  { NULL, 2, "OtherPrimeInfo"},
  { "OtherPrimeInfo", 1610612741, NULL },
  { "prime", 1073741827, NULL },
  { "exponent", 1073741827, NULL },
  { "coefficient", 3, NULL },
  { "AlgorithmIdentifier", 1610612741, NULL },
  { "algorithm", 1073741836, NULL },
  { "parameters", 541081613, NULL },
  { "algorithm", 1, NULL },
  { "DigestInfo", 1610612741, NULL },
  { "digestAlgorithm", 1073741826, "DigestAlgorithmIdentifier"},
  { "digest", 2, "Digest"},
  { "DigestAlgorithmIdentifier", 1073741826, "AlgorithmIdentifier"},
  { "Digest", 1073741831, NULL },
  { "DSAPublicPart", 1073741827, NULL },
  { "DSAPublicKey", 1610612741, NULL },
  { "version", 1073741827, NULL },
  { "p", 1073741827, NULL },
  { "q", 1073741827, NULL },
  { "g", 1073741827, NULL },
  { "Y", 3, NULL },
  { "DSAParameters", 1610612741, NULL },
  { "p", 1073741827, NULL },
  { "q", 1073741827, NULL },
  { "g", 3, NULL },
  { "DSASignatureValue", 1610612741, NULL },
  { "r", 1073741827, NULL },
  { "s", 3, NULL },
  { "DSAPrivatePart", 1073741827, NULL },
  { "DSAPrivateKey", 1610612741, NULL },
  { "version", 1073741827, NULL },
  { "p", 1073741827, NULL },
  { "q", 1073741827, NULL },
  { "g", 1073741827, NULL },
  { "Y", 1073741827, NULL },
  { "priv", 3, NULL },
  { "DHParameter", 536870917, NULL },
  { "prime", 1073741827, NULL },
  { "base", 1073741827, NULL },
  { "privateValueLength", 16387, NULL },
  { NULL, 0, NULL }
};
