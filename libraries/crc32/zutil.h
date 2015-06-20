//
// Adaptor for the zLib CRC routines into the Arduino environment.
//

#define local static
#define FAR
#define ZEXPORT
#define OF(x)   x
#define Z_NULL (0)

typedef unsigned long  z_crc_t;
typedef long z_off_t;
typedef long z_off64_t;

typedef unsigned long uLong;
typedef unsigned short uInt;

/*
     Update a running CRC-32 with the bytes buf[0..len-1] and return the
   updated CRC-32.  If buf is Z_NULL, this function returns the required
   initial value for the crc.  Pre- and post-conditioning (one's complement) is
   performed within this function so it shouldn't be done by the application.

   Usage example:

     uLong crc = crc32(0L, Z_NULL, 0);

     while (read_buffer(buffer, length) != EOF) {
       crc = crc32(crc, buffer, length);
     }
     if (crc != original_crc) error();
*/
#ifdef __cplusplus
        extern "C"{
#endif


unsigned long ZEXPORT crc32(
    unsigned long crc,
    const unsigned char FAR *buf,
    uInt len
);

#ifdef __cplusplus
} // extern "C"
#endif

