#!/usr/bin/python3
from PIL import Image
# gimp 8bit grayscale , picture width = font width,
# picture height = font height * font count
result = [] #for debug
themeColor = int(input('ThemeColor(sa:0xffffff):'),16); 
w = Image.open(input('Image path:'))
font_height = int(input("font height:"))
fontName = input("font name:")
C_Array = "const uint16_t %s[][%d] = {"%(fontName + 'Font',w.width * font_height)

#def rgb565(r,g,b):
#    return ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3)
def rgb565(red, green, blue):
    # take in the red, green and blue values (0-255) as 8 bit values and then combine
    # and shift them to make them a 16 bit hex value in 565 format. 
   return ((int(red / 255 * 31) << 11) | (int(green / 255 * 63) << 5) | (int(blue / 255 * 31)))

tr = themeColor >> 16
tg = (themeColor >> 8 )& 0xff
tb = themeColor & 0xff

print("red:%d,green:%d,blue:%d"%(tr,tg,tb))
for i in range(w.height):
    for j in range(w.width):
        pixel = w.getpixel((j,i))[0]
        factor = pixel / 255;
        red,green,blue = tr * factor,tg * factor, tb * factor
        pixel = rgb565(red,green ,blue)
        new_pixel = ((pixel & 0xff) << 8) | ((pixel & 0xff00) >> 8)
        result.append(new_pixel)
        C_Array = C_Array + str(new_pixel) + ','
        pass
    pass
C_Array = C_Array[:-1] + "};\r\n"
C_Index = "const uint16_t* %sFontIndex[] = {"%(fontName)
for i in range(int((w.height + 1) / font_height )):
    C_Index += " %s[%d],"%(fontName+'Font',i)
C_Index = C_Index[:-1] + "};\r\n"
C_struct = "const font_t %s = {%d, %d, %d, (uintptr_t)%sFont, %sFontIndex};\r\n"%(fontName, font_height, w.width, font_height * w.width * 2, fontName,fontName)

w = open(fontName+".h", 'w')
w.write('#include "font_common.h"\r\n'+C_Array + C_Index + C_struct)
w.close()
