# Install script for directory: /usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/lib/sdk/esp-idf/tools/tools/xtensa-esp-elf/esp-13.2.0_20230928/xtensa-esp-elf/bin/xtensa-esp32s3-elf-objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mbedtls" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/aes.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/aria.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/asn1.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/asn1write.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/base64.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/bignum.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/build_info.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/camellia.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/ccm.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/chacha20.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/chachapoly.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/check_config.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/cipher.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/cmac.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/compat-2.x.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_legacy_crypto.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_legacy_from_psa.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_psa_from_legacy.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_psa_superset_legacy.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_ssl.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_x509.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/config_psa.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/constant_time.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/ctr_drbg.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/debug.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/des.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/dhm.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/ecdh.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/ecdsa.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/ecjpake.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/ecp.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/entropy.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/error.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/gcm.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/hkdf.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/hmac_drbg.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/lms.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/mbedtls_config.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/md.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/md5.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/memory_buffer_alloc.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/net_sockets.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/nist_kw.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/oid.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/pem.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/pk.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/pkcs12.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/pkcs5.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/pkcs7.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/platform.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/platform_time.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/platform_util.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/poly1305.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/private_access.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/psa_util.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/ripemd160.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/rsa.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/sha1.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/sha256.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/sha3.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/sha512.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/ssl.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cache.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ciphersuites.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cookie.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ticket.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/threading.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/timing.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/version.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/x509.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/x509_crl.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/x509_crt.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/mbedtls/x509_csr.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/psa" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/build_info.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_auto_enabled.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_config_key_pair_types.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_config_synonyms.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_composites.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_key_derivation.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_primitives.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto_compat.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto_config.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto_driver_common.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_composites.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_key_derivation.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_primitives.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto_extra.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto_legacy.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto_platform.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto_se_driver.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto_sizes.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto_struct.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto_types.h"
    "/usr/lib/sdk/esp-idf/idf/components/mbedtls/mbedtls/include/psa/crypto_values.h"
    )
endif()

