#!/bin/bash
#
# weatherng.sh
#
# source: vdr-plugin weatherng
#
# To use this examples of urls , you must have the permission of owner to download and
# use them. Otherwise you must change the URLs. See README or README.DE.

# Change SETUPDIR if you need (Path to $VDRCONFIG [setup.conf]).
SETUPDIR=/etc/vdr

# Wget Options.
WGETOPT="-t 4 -T 20"
Y=$( date +%Y )
D=$( date +%d )
M=$( date +%m )
#LD=$(($D-1))
case $LANG in
	de_*)
	   MESG='Download abgeschlossen und Radarmaps aktualisiert'
	   IMGS=(
		http://www.dwd.de/bvbw/generator/Sites/DWDWWW/Content/Oeffentlichkeit/WV/WV11/Wetterkarten/Wetter__Deutschland__teaser__Bild,property=default.jpg
		http://www.zdf.de/CMO/frontend/subsystem_we/WeShowPicture/0,6008,337,00.jpg
		http://www.zdf.de/CMO/frontend/subsystem_we/WeShowPicture/0,6008,261,00.jpg
		http://www.zdf.de/CMO/frontend/subsystem_we/WeShowPicture/0,6008,258,00.jpg
		http://www.zdf.de/CMO/frontend/subsystem_we/WeShowPicture/0,6008,346,00.jpg
		http://www.zdf.de/CMO/frontend/subsystem_we/WeShowPicture/0,6008,346,00.jpg
		)
	;;
	fr_*)
	   MESG='Týlýchargement des donnýes Mýtýo OK'
	   IMGS=(
		""
		""
		""
		""
		""
		""
		)
	;;
	fi_*)
	   MESG='Sýýkuvat pýivitetty'
	   IMGS=(
		""
		""
		""
		""
		""
		""
		)
	;;
	ru_*)
	   MESG='Погодные картинки обновлены'
	   IMGS=(
		"http://sirocco.accuweather.com/sat_mosaic_234x175_public/ir/isasia.gif"
		"http://weather.is.kochi-u.ac.jp/FE/00Latest.jpg"
		"http://st5.gismeteo.ru/images/maps/ru/"$Y"/"$M"/"$D"/"$Y$M$D"000000.RFERU.TTWP.png"
#		"http://st5.gisstatic.ru/images/maps/"$Y"/"$M"/"$D"/"$Y$M$D"030000.RFERU.TTWP.png"
		"http://st5.gismeteo.ru/images/maps/ru/"$Y"/"$M"/"$D"/"$Y$M$D"060000.RFERU.TTWP.png"
#		"http://st5.gisstatic.ru/images/maps/"$Y"/"$M"/"$D"/"$Y$M$D"090000.RFERU.TTWP.png"
		"http://st5.gismeteo.ru/images/maps/ru/"$Y"/"$M"/"$D"/"$Y$M$D"120000.RFERU.TTWP.png"
#		"http://st5.gisstatic.ru/images/maps/"$Y"/"$M"/"$D"/"$Y$M$D"150000.RFERU.TTWP.png"
		"http://st5.gismeteo.ru/images/maps/ru/"$Y"/"$M"/"$D"/"$Y$M$D"200000.RFERU.TTWP.png"
#		"http://st5.gisstatic.ru/images/maps/"$Y"/"$M"/"$D"/"$Y$M$D"230000.RFERU.TTWP.png"
		)
	;;
	*)
	   MESG='Weather pictures updated'
	   IMGS=(
		""
		""
		""
		""
		""
		""
		)
	;;

esac

for i in ${!IMGS[*]} ; do
echo "$LANG----${IMGS[$i]}---" >> /var/log/syslog
    wget $WGETOPT "${IMGS[$i]}" -O "$SETUPDIR/plugins/weatherng/pic${i}"
done

l=`echo $LANG|cut -f 1 -d '_'`
c=0
for i in $@ ; do
    c=$((c+1))
    case $i in
	 *|*) wget $WGETOPT "http://samsungmobile.accu-weather.com/widget/samsungmobile/weather-data.asp?location=$i&metric=1&language=$l" \
		-O "$SETUPDIR/plugins/weatherng/data$c.xml"
	      xsltproc \
		-o "$SETUPDIR/plugins/weatherng/"{data$c.xml,accu.xslt,data$c.xml}
	;;
	 *) wget $WGETOPT "http://xoap.weather.com/weather/local/$i?cc=*&unit=m&dayf=10" \
		-O "$SETUPDIR/plugins/weatherng/data$c.xml"
	;;
    esac
done

echo $MESG

