libfreelica
===========

libfreelica is a C++ library to access FeliCa NFC smart cards by Sony. It uses libnfc for NFC communications.

Requirements
------------

* libnfc >= 1.7.0 (http://nfc-tools.org/index.php?title=Libnfc)
* cmake >= 2.6 (http://www.cmake.org/, for building)

Building & Installing
---------------------

To build and install libfreelica, perform the following steps:

1. mkdir build && cd build
2. cmake ../
3. make
4. sudo make install

To build the examples, perform the following steps in the examples/ folder:

1. mkdir build && cd build
2. cmake ../
3. make

Usage
-----

For hints on how to use the library, have a look at the examples in examples/.
No API reference is available yet, sorry for that.

The following features are implemented so far:

* All FeliCa commands for DES FeliCa cards that don't require authentication or NDA-locked documentation. (Except Write without Encryption, since I don't have any cards to test it on.)
* Parsing for Authentication 1 Command/Response and Authentication 2 Command/Response. Since I don't have any information about the actual authentication process or cryptographic procedures, I cannot implement authentication, though.

Licence & Copyright
-------------------
libfreelica is licensed under the MIT license. For details, see the LICENSE file.

(c) 2014 Maximilian Wolter (Prof. MAAD)
