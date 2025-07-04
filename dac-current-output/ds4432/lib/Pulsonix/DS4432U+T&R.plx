PULSONIX_LIBRARY_ASCII "SamacSys ECAD Model"
//1190762/454976/2.50/8/2/Integrated Circuit

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "r142.5_45"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 0.45) (shapeHeight 1.425))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(textStyleDef "Normal"
		(font
			(fontType Stroke)
			(fontFace "Helvetica")
			(fontHeight 1.27)
			(strokeWidth 0.127)
		)
	)
	(patternDef "SOP65P490X110-8N" (originalName "SOP65P490X110-8N")
		(multiLayer
			(pad (padNum 1) (padStyleRef r142.5_45) (pt -2.212, 0.975) (rotation 90))
			(pad (padNum 2) (padStyleRef r142.5_45) (pt -2.212, 0.325) (rotation 90))
			(pad (padNum 3) (padStyleRef r142.5_45) (pt -2.212, -0.325) (rotation 90))
			(pad (padNum 4) (padStyleRef r142.5_45) (pt -2.212, -0.975) (rotation 90))
			(pad (padNum 5) (padStyleRef r142.5_45) (pt 2.212, -0.975) (rotation 90))
			(pad (padNum 6) (padStyleRef r142.5_45) (pt 2.212, -0.325) (rotation 90))
			(pad (padNum 7) (padStyleRef r142.5_45) (pt 2.212, 0.325) (rotation 90))
			(pad (padNum 8) (padStyleRef r142.5_45) (pt 2.212, 0.975) (rotation 90))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt 0, 0) (textStyleRef "Normal") (isVisible True))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -3.175 1.8) (pt 3.175 1.8) (width 0.05))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 3.175 1.8) (pt 3.175 -1.8) (width 0.05))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 3.175 -1.8) (pt -3.175 -1.8) (width 0.05))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -3.175 -1.8) (pt -3.175 1.8) (width 0.05))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.5 1.5) (pt 1.5 1.5) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 1.5 1.5) (pt 1.5 -1.5) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 1.5 -1.5) (pt -1.5 -1.5) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.5 -1.5) (pt -1.5 1.5) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.5 0.85) (pt -0.85 1.5) (width 0.025))
		)
		(layerContents (layerNumRef 18)
			(line (pt -1.15 1.5) (pt 1.15 1.5) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt 1.15 1.5) (pt 1.15 -1.5) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt 1.15 -1.5) (pt -1.15 -1.5) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -1.15 -1.5) (pt -1.15 1.5) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -2.925 1.55) (pt -1.5 1.55) (width 0.2))
		)
	)
	(symbolDef "DS4432U+T&R" (originalName "DS4432U+T&R")

		(pin (pinNum 1) (pt 0 mils 0 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -25 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 2) (pt 0 mils -100 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -125 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 3) (pt 0 mils -200 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -225 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 4) (pt 0 mils -300 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -325 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 5) (pt 1100 mils 0 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 870 mils -25 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(pin (pinNum 6) (pt 1100 mils -100 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 870 mils -125 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(pin (pinNum 7) (pt 1100 mils -200 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 870 mils -225 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(pin (pinNum 8) (pt 1100 mils -300 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 870 mils -325 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(line (pt 200 mils 100 mils) (pt 900 mils 100 mils) (width 6 mils))
		(line (pt 900 mils 100 mils) (pt 900 mils -400 mils) (width 6 mils))
		(line (pt 900 mils -400 mils) (pt 200 mils -400 mils) (width 6 mils))
		(line (pt 200 mils -400 mils) (pt 200 mils 100 mils) (width 6 mils))
		(attr "RefDes" "RefDes" (pt 950 mils 300 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))
		(attr "Type" "Type" (pt 950 mils 200 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))

	)
	(compDef "DS4432U+T&R" (originalName "DS4432U+T&R") (compHeader (numPins 8) (numParts 1) (refDesPrefix IC)
		)
		(compPin "1" (pinName "SDA") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "2" (pinName "SCL") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "3" (pinName "FS1") (partNum 1) (symPinNum 3) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "4" (pinName "GND") (partNum 1) (symPinNum 4) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "8" (pinName "VCC") (partNum 1) (symPinNum 5) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "7" (pinName "OUT1") (partNum 1) (symPinNum 6) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "6" (pinName "OUT0") (partNum 1) (symPinNum 7) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "5" (pinName "FS0") (partNum 1) (symPinNum 8) (gateEq 0) (pinEq 0) (pinType Unknown))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "DS4432U+T&R"))
		(attachedPattern (patternNum 1) (patternName "SOP65P490X110-8N")
			(numPads 8)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "2")
				(padNum 3) (compPinRef "3")
				(padNum 4) (compPinRef "4")
				(padNum 5) (compPinRef "5")
				(padNum 6) (compPinRef "6")
				(padNum 7) (compPinRef "7")
				(padNum 8) (compPinRef "8")
			)
		)
		(attr "Mouser Part Number" "700-DS4432UT&R")
		(attr "Mouser Price/Stock" "https://www.mouser.co.uk/ProductDetail/Maxim-Integrated/DS4432U%2bTR?qs=wTZ%2FFzl837aX6EFuE2M5uA%3D%3D")
		(attr "Manufacturer_Name" "Analog Devices")
		(attr "Manufacturer_Part_Number" "DS4432U+T&R")
		(attr "Description" "MAXIM INTEGRATED PRODUCTS - DS4432U+T&R - Digital to Analogue Converter, 7 bit, I2C, Serial, 2.7V to 5.5V, SOP, 8 Pins")
		(attr "<Hyperlink>" "https://pdfserv.maximintegrated.com/en/ds/DS4432.pdf")
		(attr "<Component Height>" "1.1")
		(attr "<STEP Filename>" "DS4432U+T&R.stp")
		(attr "<STEP Offsets>" "X=0;Y=0.05;Z=0.63")
		(attr "<STEP Rotation>" "X=90;Y=0;Z=0")
	)

)
