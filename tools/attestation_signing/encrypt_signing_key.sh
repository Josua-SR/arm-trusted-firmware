#/bin/bash

##############################################################################
# This script is used to create the encrypted signing key datafile used
# by the ATF Software Attestation module.
#
# Three components are required:
#   a private signing key (in DER format)
#   a signing certificate (in DER format)
#   a secret pass phrase
#
# First, the secret pass phrase is input to the Key Derivation Function,
# which produces an encryption key (Key) and Initialization Vector (IV).
#
# Next, the private signing key is encrypted using the encryption Key.
# The encrypted data is saved to the datafile.
#
# Finally, the signing certificate is appended to the datafile, for use
# by the Software Attestation feature.
#
# Note: the output datafile is in a 'C' file format, suitable for inclusion by
# the Software Attestation module.
#
# Execute the script without any parameters to view the usage help.

##############################################################################
# variables
#
# This is the base filename of this script
scriptname=`basename $0`

# This is the encrypting cipher.
cipher_spec=-aes-256-ctr

# This is pseudo-random data from which the secret encrypting key is generated.
passphrase=

# This contains the encrypted signing key (intermediate file)
ciphertext_keyfile=

# This contains the encrypted signing key (used for decryption verification).
vfy_ciphertext_file=

# This contains the encrypted signing key + IV (intermediate file)
ciphertext_datafile=

# This contains the encrypted signing key + IV for inclusion in a 'C' module.
ciphertext_c_file=attestation_signing_key.h

# This is the [unencrypted] private signing key (in DER format).
plaintext_signing_key=

# This is the public signing key (derived from private signing key); this is
# used for verification of the private signing key.
public_signing_keyfile=

# This is the attestation certificate file.
signing_certfile=

# This variable prevents encrypted intermediate files from being erased.
keep_cipher_files=0
##############################################################################

# This removes all temporary files and exits to calling process.
# The ciphertext files are first overwritten with zero (0) prior to being
# erased so as not to leak any data.
cleanup()
{
	local _bytes
	# overwrite cipher data with 0
	if [ -f ${ciphertext_keyfile} ] && [ ${keep_cipher_files} -eq 0 ]; then
		_bytes=`wc -c ${ciphertext_keyfile} | cut -d ' ' -f 1`
		dd if=/dev/zero bs=1 count=${_bytes} of=${ciphertext_keyfile} \
		  2>/dev/null
		rm -f ${ciphertext_keyfile}
	elif [ -s ${ciphertext_keyfile} ]; then
		echo "Keeping ciphertext key file '${ciphertext_keyfile}'."
	else
		rm -f ${ciphertext_keyfile}
	fi
	if [ -f ${vfy_ciphertext_file} ]; then
		_bytes=`wc -c ${vfy_ciphertext_file} | cut -d ' ' -f 1`
		dd if=/dev/zero bs=1 count=${_bytes} of=${vfy_ciphertext_file} \
		  2>/dev/null
		rm -f ${vfy_ciphertext_file}
	fi
	if [ -f ${ciphertext_datafile} ] && [ ${keep_cipher_files} -eq 0 ]; then
		_bytes=`wc -c ${ciphertext_datafile} | cut -d ' ' -f 1`
		dd if=/dev/zero bs=1 count=${_bytes} of=${ciphertext_datafile} \
		  2>/dev/null
		rm -f ${ciphertext_datafile}
	elif [ -s ${ciphertext_datafile} ]; then
		echo "Keeping ciphertext data file '${ciphertext_datafile}'."
	else
		rm -f ${ciphertext_datafile}
	fi
	if [ -f ${public_signing_keyfile} ]; then
		rm -f ${public_signing_keyfile}
	fi
	exit $1
}

# main entry point
if [ $# -lt 2 ]; then
	echo "Error: please specify key and certificate."
	echo "Usage: ${scriptname} <keyfile> <certfile> [passphrase]"
	echo "where:"
	echo "  <keyfile>:    private key used to sign attestation info"
	echo "                (this must be in DER format)"
	echo "  <certfile>:   certificate provided with attestation info"
	echo "                (this must be in DER format)"
	echo "  <passphrase>:  secret used for encryption key generation"
	echo "                 (if not specified, a prompt will be provided)"
	exit 1
else
	plaintext_signing_key=$1; shift
	signing_certfile=$1; shift
	if [ $# -gt 0 ]; then
		passphrase=$1; shift
	fi
fi

# create temporary files
ciphertext_keyfile=`mktemp --tmpdir ciphertext.key.XXX 2> /dev/null`
ciphertext_datafile=`mktemp --tmpdir ciphertext.data.XXX 2> /dev/null`
public_signing_keyfile=`mktemp --tmpdir pub.key.XXX 2> /dev/null`
vfy_ciphertext_file=`mktemp --tmpdir vfy.ciphertext.key.XXX 2> /dev/null`
if [ "${ciphertext_keyfile}" == "" ] || [ "${ciphertext_datafile}" == "" ] || \
   [ "${public_signing_keyfile}" == "" ] ||
   [ "${vfy_ciphertext_file}" == "" ] || \
   [ ! -f ${ciphertext_keyfile} ] || [ ! -f ${ciphertext_datafile} ] || \
   [ ! -f ${public_signing_keyfile} ] || [ ! -f ${vfy_ciphertext_file} ] ; then
	echo "Error creating output files."
	exit 1
fi

# Ensure we don't overwrite an existing output file.
if [ -f ${ciphertext_c_file} ] ; then
	echo "Error: cannot overwrite existing file '${ciphertext_c_file}'"
	cleanup 1
fi

# Verify the [plaintext] signing private key by extracting public key from it.
openssl pkey -in ${plaintext_signing_key} -inform der -pubout 2>/dev/null > \
 ${public_signing_keyfile}
if [ $? -ne 0 ]; then
	echo -n "Invalid [private] signing key specified; "
	echo "unable to extract public key.  Ensure it is using DER format."
	cleanup 1
fi

# Verify the signing certificate.
openssl x509 -in ${signing_certfile} -inform der -noout 2>/dev/null
if [ $? -ne 0 ]; then
	echo -n "Invalid signing certificate specified; "
	echo "ensure it is using DER format."
	cleanup 1
fi

# Generate the secret key data used for encrypting the signing key.
# This consists of a [32 B] key and a [16 B] Initialization Vector (IV).
if [ "${passphrase}" == "" ]; then
	res=`openssl enc -e ${cipher_spec} -P 2>/dev/null`
else
	res=`openssl enc -e ${cipher_spec} -P -pass pass:${passphrase} \
	  2>/dev/null`
fi
if [ $? -ne 0 ]; then
	echo "OpenSSL error generating encryption secret data."
	cleanup 1
fi
#echo "res: '${res}'"

# retrieve the [generated] secret key & IV
key=`echo ${res} | sed "s/.*key=\([0-9,a-f,A-F]*\)\(.*\)/\L\1\E/"`
iv=`echo ${res} | sed "s/.*iv =\([0-9,a-f,A-F]*\)\(.*\)/\L\1\E/"`
#echo "key: '${key}'"
#echo "iv: '${iv}'"

# encrypt the [signing] key file
res=`openssl enc -e ${cipher_spec} -K ${key} -iv ${iv} \
 -in ${plaintext_signing_key} -out ${ciphertext_keyfile} -p 2>/dev/null`
if [ $? -ne 0 ]; then
	echo "OpenSSL error encrypting input key file."
	cleanup 1
fi
#echo "res: '${res}'"

# Verify the secret key & IV values used for encryption
# (the values output during encryption should match the input values specified).
out_key=`echo ${res} | sed "s/.*key=\([0-9,a-f,A-F]*\)\(.*\)/\L\1\E/"`
out_iv=`echo ${res} | sed "s/.*iv =\([0-9,a-f,A-F]*\)\(.*\)/\L\1\E/"`
if [ "${out_key}" != "${key}" ] || [ "${out_iv}" != "${iv}" ]; then
	echo "OpenSSL error encrypting input key file."
	cleanup 1
fi

# Create ciphertext data file containing the encrypted key and IV.
# The format is:
#
#        00 .. 15              Initialization Vector
#        16 .. 31              AES Encrypted Block #1
#           ..
#      N*16 .. (N+1)*16 - 1    AES Encrypted Block #N
#
echo 0x${out_iv} | xxd -r > ${ciphertext_datafile}
cat ${ciphertext_keyfile} >> ${ciphertext_datafile}

# Verify the ciphertext data file by decrypting it and
# comparing to original [plaintext] signing key.
# First, extract the IV from the ciphertext data file.
vfy_iv=`dd if=${ciphertext_datafile} bs=1 count=16 2>/dev/null | \
 xxd -g1|sed "s/0000000: \([0-9,a-f,A-F, ]\{48\}\).*/\1/g" | sed "s/ //g"`

# Next, extract the encrypted data from the ciphertext data file.
dd if=${ciphertext_datafile} bs=1 skip=16 2>/dev/null \
 of=${vfy_ciphertext_file}

# Finally, decrypt the data and compare it to the original plaintext key file.
openssl enc -d ${cipher_spec} -K ${key} -iv ${vfy_iv} \
 -in ${vfy_ciphertext_file} | diff - ${plaintext_signing_key}
if [ $? -ne 0 ]; then
	echo "Internal Error: unable to decrypt ciphertext data file."
	cleanup 1
fi

# Finally, create a 'C' data file from the ciphertext data file and
# the signing certificate.
ifndef_name=__`echo ${ciphertext_c_file} | tr [a-z] [A-Z] | tr [.] [_] `__
cat > ${ciphertext_c_file} << EOF
/*
 * Copyright (C) 2016-2019 Marvell International Ltd.
 *
 * This program is provided "as is" without any warranty of any kind,
 * and is distributed under the applicable Marvell proprietary limited use
 * license agreement.
 *
 * This file is machine-generated by the following script:
 *   ${scriptname}
 */

#ifndef ${ifndef_name}
#define ${ifndef_name}

/*
 * This is the signing key in DER format that has been encrypted.
 *
 * The encrypted data is structured as follows:
 *
 *        00 .. 15              Initialization Vector
 *        16 .. 31              AES Encrypted Block #1
 *           ..
 *      N*16 .. (N+1)*16 - 1    AES Encrypted Block #N
 *
 * This can be decrypted using the following secret key,
 * which must be programmed into FUSF_SW(2..5).
 *
 * ${key}
 * FUSF_SW2: 0x${key:0:16}
 * FUSF_SW3: 0x${key:16:16}
 * FUSF_SW4: 0x${key:32:16}
 * FUSF_SW5: 0x${key:48:16}
 */
EOF
c_var_name=`echo ${ciphertext_datafile} | sed "s/\//_/g"`
xxd -i ${ciphertext_datafile} 2>/dev/null | \
  sed "s/.*${c_var_name}_len.*//g" | \
  sed "s/${c_var_name}/encrypted_sign_key/g" | \
  sed "s/^  0x/0x/g" >> ${ciphertext_c_file}
if [ $? -ne 0 ]; then
	echo "Error creating attestation signing ciphertext datafile."
	# we can't remove it in 'cleanup', remove it here
	rm -f ${ciphertext_c_file}
	cleanup 1
fi
cat >> ${ciphertext_c_file} << EOF
/*
 * This is the signing certificate in DER format.
 */
EOF
c_var_name=`echo ${signing_certfile} | sed "s/\//_/g"`
xxd -i ${signing_certfile} 2>/dev/null | \
  sed "s/.*${c_var_name}_len.*//g" | \
  sed "s/${c_var_name}/sign_cert_der/g" | \
  sed "s/^  0x/0x/g" >> ${ciphertext_c_file}
if [ $? -ne 0 ]; then
	echo -n "Error writing certificate to attestation signing "
	echo "ciphertext datafile."
	# we can't remove it in 'cleanup', remove it here
	rm -f ${ciphertext_c_file}
	cleanup 1
fi
cat >> ${ciphertext_c_file} << EOF
#endif /* ${ifndef_name} */
EOF

echo "Created attestation signing ciphertext data file '${ciphertext_c_file}'."
cleanup 0

