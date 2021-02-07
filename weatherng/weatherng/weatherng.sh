#!/bin/sh
#
# weatherng.sh
#
# source: vdr-plugin weatherng
#

# To use this examples of urls , you must have the permission of owner to download and
# use them. Otherwise you must change the URLs. See README or README.DE.

# Change SETUPDIR if you need (Path to $VDRCONFIG [setup.con].
SETUPDIR=/VDR/etc

ID1=$1
ID2=$2
ID3=$3

## Put your key here !! example KEY="jhsbndmjk34xyz123"
KEY=""

LANGUAGE=( `grep -s ^OSDLanguage.*[0-9]$ "$SETUPDIR/setup.conf"` )

case "${LANGUAGE[2]}" in
	1) MESG='Download abgeschlossen und Radarmaps aktualisiert'
	   URLS=(\
                # Put here 6 urls for radarmaps
                #1: Tagestemperatur
		#2: Nachttemperatur
		#3: Regenradar
		#4: Pollenflug
		#5: Ozonwerte
		#6: Europawetter
		)
	;;
	7) MESG='Téléchargement des données Météo OK'
	   URLS=(\
		)
	;;
	8) MESG='Sääkuvat päivitetty'
	   URLS=(\
		)
	;;
	*) MESG='Weather pictures updated'
	   URLS=(\
		"" \
		"" \
		"" \
		"" \
		"" \
		)
	;;
esac

for i in $(seq 0 9) ; do
    eval ${URLS[$i]:+ wget -t 4 -T 20 "${URLS[$i]}" -O "$SETUPDIR/plugins/weatherng/pic${i}"}
done

##    convert "$SETUPDIR/plugins/weatherng/pic${i}" pic${i}.xpm

wget -t 4 -T 20 "http://xoap.weather.com/weather/local/$ID1?cc=*&unit=m&dayf=10&prod=xoap&par=1004124588&key=$KEY" -O "$SETUPDIR/plugins/weatherng/data1.xml"
wget -t 4 -T 20 "http://xoap.weather.com/weather/local/$ID2?cc=*&unit=m&dayf=10&prod=xoap&par=1004124588&key=$KEY" -O "$SETUPDIR/plugins/weatherng/data2.xml"
wget -t 4 -T 20 "http://xoap.weather.com/weather/local/$ID3?cc=*&unit=m&dayf=10&prod=xoap&par=1004124588&key=$KEY" -O "$SETUPDIR/plugins/weatherng/data3.xml"

echo $MESG
