Marvell "doimage" flash image builder
=====================================

Marvell "doimage" utility is used for contructing flash images suitable for
a boot media installation in format that accepted by Marvell SoC BootROM.
The flash image format is compatible with the following Marvell SoC product
lines:

- ARMADA 7K (88F70xx)
- ARMADA 8K (88F80xx)
- OCTEON TX2 (CN913x)

The "doimage" supports both trusted and untrusted flash image formats.
Untrusted image is normally created by standalone self-contained "doimage"
executable.
Trusted image requres production of digital signatures for various flash image
components.

Each signature is created using dedicated RSA-2048 private key and later
verified by the BootROM using RSA-2048 public keys included in the trusted
boot image security extension.

Some usage cases require separation between the build hosts that compile the
flash image components and create the final bootable flash image and host that
is able to create digital signatures using RSA private keys.
This use case prevents the private keys from sharing across all hosts able to
build flash images.

The above reqirement is covered by client-server split-execution doimage model.

:WARNING:
       The client-server model of the image formatter is supplied in a form of
       application example. The example only covers  digital signatures
       creation, but does not support image encryption on server side.
       There are many ways this sample code might be improved, for instance:
       - use mutual client-server authentication instead of currently provided
       server-only authentication
       - add bootloader image encryption support
       - add support for multiple key groups bound to a various products
       (identified, for instance by the BoxID and FlashID).


Client-Server mode of operation
===============================

The difference between standalone and client-serevr doimage variant lays in
digital signatures production. The rest of the code is similar across these
two doimage variants.
When the image builder (client) needs to create a digital signature for a block
of data, it calculates SHA256 digest of the data buffer and sends it to the
signage server along with the required key index.
Indexes from 0 to 15 requires usage of the appropriate CSK0 - CSK15 private
keys for signatures, while index 16 calls for KAK private key usage.
The server encrypts the incoming SHA256 digest using the requested private key
and sends it back to the client, where the resulting signature verified using
appropriate public key and inserted into flash image security extension.


Client and Server system components
===================================

The client and server system setups contain the following components:

**Image building host**

+-----------------------------------+-----------------------------------------+
|| Component                        || Description                            |
+===================================+=========================================+
|| **doimage_client**               || The "doimage" executable to be used    |
||                                  || for image formatting                   |
+-----------------------------------+-----------------------------------------+
|| **client configuration file**    || Client configuration file defining all |
|| See example file *client.cfg* in || parameters required for successful     |
|| tools/marvell/doimage/secure/    || image build.                           |
+-----------------------------------+-----------------------------------------+
|| **RSA-2048 Public keys**         || Public keys files in PEM format for    |
|| See example key files            || use in trusted image security extension|
|| *csk_pub_pem0.key*,              || These keys should match private keys   |
|| *csk_pub_pem1.key*,              || used on server side.                   |
|| *csk_pub_pem2.key*, ...          ||                                        |
|| *kak_pub_pem.key*  in            ||                                        |
|| tools/marvell/doimage/secure/    ||                                        |
+-----------------------------------+-----------------------------------------+
|| **CA certificate**               || CA cerfificate for server connection   |
|| See example file *rootCA.crt* in || authentication. The server cerificate  |
|| tools/marvell/doimage/secure/    || should be signed by the same CA        |
+-----------------------------------+-----------------------------------------+


**Digital signage server**

+-----------------------------------+-----------------------------------------+
|| Component                        || Description                            |
+===================================+=========================================+
|| **doimage_server**               || The server executable to be used for   |
||                                  || responding to client signatire         |
||                                  || creation requests.                     |
+-----------------------------------+-----------------------------------------+
|| **server configuration file**    || Server configuration file defining all |
|| See example file *server.cfg* in || parameters required for a normal       |
|| tools/marvell/doimage/secure/    || serverrun.                             |
+-----------------------------------+-----------------------------------------+
|| **RSA-2048 Private keys**        || Private keys files in PEM format used  |
|| See example key files            || by the signage server for creating     |
|| *csk_priv_pem0.key*,             || digital signatures embedded into flash |
|| *csk_priv_pem1.key*,             || image security extension by the client |
|| *csk_priv_pem2.key*, ...         || These keys should match public keys    |
|| *kak_priv_pem.key* in            || used on client side                    |
|| tools/marvell/doimage/secure/    ||                                        |
+-----------------------------------+-----------------------------------------+
|| **CA certificate**               || CA cerfificate used for creation of    |
|| See example file *rootCA.crt* in || server certificate(s).                 |
|| tools/marvell/doimage/secure/    ||                                        |
+-----------------------------------+-----------------------------------------+
|| **CA private key**               || CA private key used for creation of    |
|| See example file *rootCA.key* in || server certificate(s). This file is    |
|| tools/marvell/doimage/secure/    || not used for the TLS connection setup. |
+-----------------------------------+-----------------------------------------+
|| **server certificate**           || Server cerfificate presented to the    |
|| See example file *localhost.crt* || client upon TLS connection setup.      |
|| in tools/marvell/doimage/secure/ ||                                        |
+-----------------------------------+-----------------------------------------+
|| **server private key**           || Server key used for TLS connection     |
|| See example file *localhost.key* || setup                                  |
|| in tools/marvell/doimage/secure/ ||                                        |
+-----------------------------------+-----------------------------------------+


Usage example
=============

This example uses the same computer for client-server connection
(localhost:4433).

1. Build client and server binaries:

   .. code:: shell

      cd tools/marvell/doimage
      make server

#. Open two terminal sessions - one for the server and one for the client.

#. On server terminal start the server:

   .. code:: shell

      doimage_server -c <path-to>>/server.cfg

   Expected output:

   .. code:: shell

       Starting Marvell(C) doimage signage server version 1.0

#. On client terminal issue the flash image build command:

   .. code:: shell

      doimage_client -l 0x4100000 -e 0x4100000 -b <path-to>/ble.bin -t SLC -n 256 \
      <path-to>/boot-image.bin -c <path-to>/client.cfg <path-to>/flash-image-signed.bin

   Expected output:

   .. code:: shell

      Request signature using key ID 16
      Request signature using key ID 3
      Request signature using key ID 3

#. Server terminal traces every client request:

   .. code:: shell

      Create signature for key index 16 (KAK)
      Create signature for key index 3 (CSK)
      Create signature for key index 3 (CSK)

#. The flash image could be verified using the client binary in the same way
   as it is done with stanalone utility:

   .. code:: shell

      doimage_client -p -k 3 <path-to>>/flash-image-signed.bin
      ################### Prolog Start ######################

      ########### Header ##############
      [0x00000 : 0x00003]  magic                     0xb105b002
      [0x00004 : 0x00007]  prolog_size               393216
      [0x00008 : 0x0000b]  prolog_checksum           0xb294c11f
      [0x0000c : 0x0000f]  boot_image_size           1840048
      [0x00010 : 0x00013]  boot_image_checksum       0x69d7279a
      [0x00014 : 0x00017]  rsrvd0                    0x0
      [0x00018 : 0x0001b]  load_addr                 0x4100000
      [0x0001c : 0x0001f]  exec_addr                 0x4100000
      [0x00020 : 0x00020]  uart_cfg                  0x0
      [0x00021 : 0x00021]  baudrate                  0x0
      [0x00022 : 0x00022]  ext_count                 2
      [0x00023 : 0x00023]  aux_flags                 0x0
      [0x00024 : 0x00027]  io_arg_0                  0x4
      [0x00028 : 0x0002b]  io_arg_1                  0x0
      [0x0002c : 0x0002f]  io_arg_2                  0x0
      [0x00030 : 0x00033]  io_arg_3                  0x0
      [0x00034 : 0x00037]  rsrvd1                    0x0
      [0x00038 : 0x0003b]  rsrvd2                    0x0
      [0x0003c : 0x0003f]  rsrvd3                    0x0

      ########### Binary extension ###########
      [0x00040 : 0x00040]  type                      0x2
      [0x00041 : 0x00041]  offset                    0x0
      [0x00042 : 0x00043]  reserved                  0x0
      [0x00044 : 0x00047]  size                      376760
      [0x00048 : 0x5bfff]  binary image

      ########### Secure extension ###########
      [0x5c000 : 0x5c000]  type                      0x1
      [0x5c001 : 0x5c001]  offset                    0x0
      [0x5c002 : 0x5c003]  reserved                  0x0
      [0x5c004 : 0x5c007]  size                      9956
      [0x5c008 : 0x5c213]  KAK key
      [0x5c20c : 0x5c20f]  jtag_delay                20
      [0x5c210 : 0x5c213]  box_id                    0xdeadbeef
      [0x5c214 : 0x5c217]  flash_id                  0xbaddf00d
      [0x5c21c : 0x5c21f]  encrypt_en                0
      [0x5c220 : 0x5c223]  efuse_dis                 0
      [0x5c22c : 0x5c32b]  header signature
      [0x5c32c : 0x5c42b]  image signature
      [0x5c42c : 0x5e4eb]  CSK keys
      [0x5e4ec : 0x5e5eb]  CSK block signature
      [0x5e5ec : 0x5e62b]  control
      [0x5e6ec : 0x5ffff]  padding

      ################### Prolog End ######################

      ################### Boot image ######################
      [0x60000 : 0x2213ab]  boot image
      ################### Image end ########################

      Checking values:
      Headers magic:    OK!
      Headers checksum: OK!
      Image checksum:   OK!

      Check RSA Signatures
      #########################
      CSK Block Signature: OK
      Image Signature:     OK
      Header Signature:    OK
      >>>>>>>>>> KAK KEY HASH >>>>>>>>>>
      SHA256: 8B3732C8F03D0C407F34200C206CA36B0701E47C9074034EDE97FBFF5BA6D778
      <<<<<<<<< KAK KEY HASH <<<<<<<<<


How to create certificates
==========================

The example application uses custom CA (Marvell) and server located on the same
machine as the client (localhost). This configuration is only useful for basic
tests.
In order to deploy real application, new CA and server cerfificates should be
created and propagated to client and server configration files.
The CA root key and certificate are created only once even multiple signage
servers exist.
Every new server certificate is created using the same CA attributes.
Below instructions are using OpenSSl command line tools and Quitters, Inc.
company name as an example.

1. Create custom Certification Authority root key (password-protected):

   .. code:: shell

      openssl genrsa -des3 -out QuittersCA.key 4096

#. Create and self-sign the root CA cerificate using previously generated
   root CA key (SHA1 is retired now, so SHA2 should be used):

   .. code:: shell

      openssl req -x509 -new -nodes -key QuittersCA.key -sha256 -days 1825 -out QuittersCA.crt

#. Create server certificate key for "quitters.com" (each server requires one)

   .. code:: shell

      openssl genrsa -out quitters.com.key 2048

#. Create server signing request for "quitters.com" host

   .. code:: shell

      openssl req -new -sha256 -key quitters.com.key -subj "/C=US/ST=NY/L=Brooklyn\
      /O=Quitters, Inc./CN=quitters.com" -out quitters.com.csr

#. Generate server certificate for "quitters.com" host signed by Quitters CA

   .. code:: shell

      openssl x509 -req -in quitters.com.csr -CA QuittersCA.crt -CAkey QuittersCA.key \
      -CAcreateserial -out quitters.com.crt -days 730 -sha256

#. Verify the certificate content and signature

   .. code:: shell

      openssl x509 -in quitters.com.crt -text -noout
      openssl verify -verbose -CAfile QuittersCA.crt quitters.com.crt

#. Replace at least the following entries in *client.cfg* and *server.cfg*:

   .. code:: shell

      # *tools/marvell/doimage/secure/client.cfg*
      #
      # Signage server name and port
      server_name = "quitters.com";
      # CA certificate
      ca_cert_file = "<path-to>>/QuittersCA.crt";

      # *tools/marvell/doimage/secure/server.cfg*
      #
      # SSL ceritifates and server key
      server_key_file = "<path-to>>/quitters.com.key";
      server_cert_file = "<path-to>>/quitters.com.crt";
      ca_cert_file = "<path-to>>/QuittersCA.crt";


Useful references
=================

`Self-signed certificate with custom CA
<https://gist.github.com/fntlnz/cf14feb5a46b2eda428e000157447309>`_

`OpenSSL Essentials: Working with SSL Certificates, Private Keys and CSRs
<https://www.digitalocean.com/community/tutorials/openssl-essentials-working-with-ssl-certificates-private-keys-and-csrs>`_

`mbedTLS Tutorial
<https://tls.mbed.org/kb/how-to/mbedtls-tutorial>`_

