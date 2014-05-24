#######################
# Auxiliary Functions #
#######################

import struct, binascii

def FP32toFP16(float32):
    F16_EXPONENT_BITS = 0x1F
    F16_EXPONENT_SHIFT = 10
    F16_EXPONENT_BIAS = 15
    F16_MANTISSA_BITS = 0x3ff
    F16_MANTISSA_SHIFT =  (23 - F16_EXPONENT_SHIFT)
    F16_MAX_EXPONENT =  (F16_EXPONENT_BITS << F16_EXPONENT_SHIFT)

    a = struct.pack('>f',float32)
    b = binascii.hexlify(a)

    f32 = int(b,16)
    f16 = 0
    sign = (f32 >> 16) & 0x8000
    exponent = ((f32 >> 23) & 0xff) - 127
    mantissa = f32 & 0x007fffff

    if exponent == 128:
        f16 = sign | F16_MAX_EXPONENT
        if mantissa:
            f16 |= (mantissa & F16_MANTISSA_BITS)
    elif exponent > 15:
        f16 = sign | F16_MAX_EXPONENT
    elif exponent > -15:
        exponent += F16_EXPONENT_BIAS
        mantissa >>= F16_MANTISSA_SHIFT
        f16 = sign | exponent << F16_EXPONENT_SHIFT | mantissa
    else:
        f16 = sign
    return f16


########
# Main #
########

from PIL import Image

img = Image.open("Texture_Original.png")
pix = img.load()


#A1R5G5B5
w = open("Texture-A1R5G5B5.raw", "wb")
for y in range(img.size[0])[::-1]:
    for x in range(img.size[1]):
        ARGB  = (pix[x,y][3] & 0x80) << 8
        ARGB |= (pix[x,y][0] & 0xF8) << 7
        ARGB |= (pix[x,y][1] & 0xF8) << 2
        ARGB |= (pix[x,y][2] & 0xF8) >> 3
        w.write(chr((ARGB >>  8) & 0xFF))
        w.write(chr((ARGB >>  0) & 0xFF))
w.close()


#A4R4G4B4
w = open("Texture-A4R4G4B4.raw", "wb")
for y in range(img.size[0])[::-1]:
    for x in range(img.size[1]):
        AR = (pix[x,y][3] & 0xF0) | (pix[x,y][0] >> 4)
        GB = (pix[x,y][1] & 0xF0) | (pix[x,y][2] >> 4)
        w.write(chr(AR))
        w.write(chr(GB))
w.close()


#A8R8G8B8
w = open("Texture-A8R8G8B8.raw", "wb")
for y in range(img.size[0])[::-1]:
    for x in range(img.size[1]):
        w.write(chr(pix[x,y][3]))
        w.write(chr(pix[x,y][0]))
        w.write(chr(pix[x,y][1]))
        w.write(chr(pix[x,y][2]))
w.close()


#B8
w = open("Texture-B8.raw", "wb")
for y in range(img.size[0])[::-1]:
    for x in range(img.size[1]):
        w.write(chr(pix[x,y][2]))
w.close()


#COMPRESSED_B8R8_G8R8
w = open("Texture-COMPRESSED_B8R8_G8R8.raw", "wb")
for y in range(img.size[0])[::-1]:
    for x in range(0, img.size[1], 2):
        b = (pix[x + 0, y][2] + pix[x + 1, y][2]) / 2
        g = (pix[x + 0, y][1] + pix[x + 1, y][1]) / 2
        w.write(chr(b))
        w.write(chr(pix[x + 1, y][0]))
        w.write(chr(g))
        w.write(chr(pix[x + 0, y][0]))
w.close()


#COMPRESSED_R8B8_R8G8
w = open("Texture-COMPRESSED_R8B8_R8G8.raw", "wb")
for y in range(img.size[0])[::-1]:
    for x in range(0, img.size[1], 2):
        b = (pix[x + 0, y][2] + pix[x + 1, y][2]) / 2
        g = (pix[x + 0, y][1] + pix[x + 1, y][1]) / 2
        w.write(chr(pix[x + 1, y][0]))
        w.write(chr(b))
        w.write(chr(pix[x + 0, y][0]))
        w.write(chr(g))
w.close()


#COMPRESSED_HILO8
w = open("Texture-COMPRESSED_HILO8.raw", "wb")
for y in range(img.size[0])[::-1]:
    for x in range(0, img.size[1]):
        w.write(chr(255-y))
        w.write(chr(x))
w.close()


#COMPRESSED_HILO_S8
w = open("Texture-COMPRESSED_HILO_S8.raw", "wb")
for y in range(img.size[0])[::-1]:
    for x in range(0, img.size[1]):
        w.write(chr(255-y))
        w.write(chr(x))
w.close()


#D1R5G5B5
w = open("Texture-D1R5G5B5.raw", "wb")
for y in range(img.size[0])[::-1]:
    for x in range(img.size[1]):
        ARGB  = (0x00        & 0x80) << 8
        ARGB |= (pix[x,y][0] & 0xF8) << 7
        ARGB |= (pix[x,y][1] & 0xF8) << 2
        ARGB |= (pix[x,y][2] & 0xF8) >> 3
        w.write(chr((ARGB >>  8) & 0xFF))
        w.write(chr((ARGB >>  0) & 0xFF))
w.close()


#D8R8G8B8
w = open("Texture-D8R8G8B8.raw", "wb")
for y in range(img.size[0])[::-1]:
    for x in range(img.size[1]):
        w.write(chr(0x00))
        w.write(chr(pix[x,y][0]))
        w.write(chr(pix[x,y][1]))
        w.write(chr(pix[x,y][2]))
w.close()


#G8B8
w = open("Texture-G8B8.raw", "wb")
for y in range(img.size[0])[::-1]:
    for x in range(img.size[1]):
        w.write(chr(pix[x,y][1]))
        w.write(chr(pix[x,y][2]))
w.close()


#R5G5B5A1
w = open("Texture-R5G5B5A1.raw", "wb")
for y in range(img.size[0])[::-1]:
    for x in range(img.size[1]):
        ARGB  = (pix[x,y][0] & 0xF8) << 8
        ARGB |= (pix[x,y][1] & 0xF8) << 3
        ARGB |= (pix[x,y][2] & 0xF8) >> 2
        ARGB |= (pix[x,y][3] & 0x80) >> 7
        w.write(chr((ARGB >>  8) & 0xFF))
        w.write(chr((ARGB >>  0) & 0xFF))
w.close()


#R5G6B5
w = open("Texture-R5G6B5.raw", "wb")
for y in range(img.size[0])[::-1]:
    for x in range(img.size[1]):
        ARGB  = (pix[x,y][0] & 0xF8) << 8
        ARGB |= (pix[x,y][1] & 0xFC) << 3
        ARGB |= (pix[x,y][2] & 0xF8) >> 3
        w.write(chr((ARGB >>  8) & 0xFF))
        w.write(chr((ARGB >>  0) & 0xFF))
w.close()


#R6G5B5
w = open("Texture-R6G5B5.raw", "wb")
for y in range(img.size[0])[::-1]:
    for x in range(img.size[1]):
        ARGB  = (pix[x,y][0] & 0xFC) << 8
        ARGB |= (pix[x,y][1] & 0xF8) << 2
        ARGB |= (pix[x,y][2] & 0xF8) >> 3
        w.write(chr((ARGB >>  8) & 0xFF))
        w.write(chr((ARGB >>  0) & 0xFF))
w.close()


#W16_Z16_Y16_X16_FLOAT
w = open("Texture-W16_Z16_Y16_X16_FLOAT.raw", "wb")
for y in range(img.size[0])[::-1]:
    for x in range(img.size[1]):
        W = FP32toFP16(float(pix[x,y][0]/255.0))
        Z = FP32toFP16(float(pix[x,y][1]/255.0))
        Y = FP32toFP16(float(pix[x,y][2]/255.0))
        X = FP32toFP16(float(pix[x,y][3]/255.0))
        w.write(chr(W >> 8))
        w.write(chr(W & 0xFF))
        w.write(chr(Z >> 8))
        w.write(chr(Z & 0xFF))
        w.write(chr(Y >> 8))
        w.write(chr(Y & 0xFF))
        w.write(chr(X >> 8))
        w.write(chr(X & 0xFF))
w.close()


#X16
w = open("Texture-X16.raw", "wb")
for y in range(img.size[0])[::-1]:
    for x in range(img.size[1]):
        w.write(chr(pix[x,y][3]))
        w.write('\x00')
w.close()


#Y16_X16
w = open("Texture-Y16_X16.raw", "wb")
for y in range(img.size[0])[::-1]:
    for x in range(img.size[1]):
        w.write(chr(pix[x,y][2]))
        w.write('\x00')
        w.write(chr(pix[x,y][3]))
        w.write('\x00')
w.close()


#Y16_X16_FLOAT
w = open("Texture-Y16_X16_FLOAT.raw", "wb")
for y in range(img.size[0])[::-1]:
    for x in range(img.size[1]):
        Y = FP32toFP16(float(pix[x,y][2]/255.0))
        X = FP32toFP16(float(pix[x,y][3]/255.0))
        w.write(chr(Y >> 8))
        w.write(chr(Y & 0xFF))
        w.write(chr(X >> 8))
        w.write(chr(X & 0xFF))
w.close()
