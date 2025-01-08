# Draw Rectangles and Elipses via Osc or sACN
a small fullscreen app which draw 2 rectangles and 2 elipses (and a pixmap) in RGB via OSC or sACN
in sACN, it responde to univers 7, channels :  
First rectangle :
100 : Master
101 : R
-102 : G
-103 : B
-104 105 : 16bit PAN
-106 107 : 16Bit TILT
-108 109 : 16Bit width
-110 111 : 16Bit height
-112 113 : 16Bit thickness
-114 115 : 16Bit 360° rotation

120 - 135 : Rectangle 2
140 - 155 : Elipse 1
160 - 175 : Elipse 2

Pimax
-180 : Master
-184 185 : 16bit PAN
-186 187 : 16Bit TILT
-188 189 : 16Bit width
-190 191 : 16Bit height
-194 195 : 16Bit 360° rotation
-196 : load image : 11 ranges : 1 : no image, 2 : image1 .... 11 : image10

in OSC respond to port 7003, initialy conceived for DLight lighting software
