a small fullscreen app which draw 2 rectangles and 2 elipses (and a pixmap) in RGB via OSC or sACN in sACN, it responde to univers 7, channels :
First rectangle :
-100 : Master
-101 : R
-102 : G
-103 : B
-104 : thickness
-105 : 360° rotation
-106 107 : 16bit PAN
-108 109 : 16Bit TILT
-110 111 : 16Bit width
-112 113 : 16Bit height

114 - 127 : Rectangle 2
128 - 141 : Elipse 1
142 - 155 : Elipse 2
Pimax
-156 : Master
-161 : 360° rotation -162 163 : 16bit PAN
-164 165 : 16Bit TILT
-166 167 : 16Bit width
-168 169 : 16Bit height

-170 : load image : 11 ranges : 1 : no image, 2 : image1 .... 11 : image10
in OSC respond to port 7003, initialy conceived for DLight lighting software
