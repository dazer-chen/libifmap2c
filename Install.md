# Building and Installing libifmap2c #

Development and testing of the library is done using
  * Slackware Linux 13.37. (32 bit and 64 bit)

Other systems on which the library was compile-tested at least:
  * Ubuntu Server 11.04 (64 bit)
  * Arch Linux
  * FreeBSD 8.2 (32 bit)
  * OpenBSD 4.9 (64 bit)
  * Mac OS X

If you have problems building the library on those systems, please let me know.

At the moment I'm not doing development using Windows.

## Prerequisites ##
You need the following software to be able to successful compile libifmap2c.
Make sure you have the development packages for libxml2 and libcurl.

  * cmake (tested 2.8)
  * libxml2 (tested > 2.7.7)
  * libcurl (min 7.19.1)

**Note:** If using Ubuntu, make sure to use the OpenSSL package, i.e. **libcurl4-openssl-dev**, as there are problems with the gnutls package verifying the server's certificate.

## Checking out the source / Extracting the archive ##
  * Switch to a directory where you want to check out the source
  * `svn checkout http://libifmap2c.googlecode.com/svn/trunk/ libifmap2c`
  * `cd libifmap2c/libifmap2c`
Alternatively you can download the archive and extract it:
  * `tar xvf libifmap2c-<version>.tar.gz`
  * `cd libifmap2c-<version>`

## Building the Library ##
  * `cd build`
  * `cmake ../`
  * `make`

## Installing the Library ##
If you want to install the library and header files into system directories, e.g. `/usr/local/[lib/include]`, run `make install` as root. Don't forget to run `ldconfig` afterwards in order to make use of the newly installed library.