#!/bin/sh
ignore="icon.png|menu|background.png|banka-p.png|barely-poz.png|bathroom-p.png|batyskaf-p.png|bludiste-p.png|bottles-p.png|bunker-p.png|chodba-p2.png|dela-p.png|deutsche-pozadi.png|diry-p.png|disketa-p-opr.png|drakar1-p.png|drakar-p.png|fdto-p.png|gral-pozadi.png|jednicky-p.png|jeskyne-p.png|kajuta1p.png|kajuta2p.png|kankan-p.png|koraly-p.png|koste-p.png|kuchyne-pozadi.png|kufrik-p.png|letadlo-p.png|mapa-p.png|mikro-p.png|motor-pozadi.png|ncp-p.png|noground-p.png|odpadky-p.png|paprsky-pozadi.png|party1-p.png|pocitac-pozadi.png|pohon-p.png|potopena-pozadi.png|pozadi.png|pozadi2.png|pravidla-p.png|prvni-p.png|puclik-p.png|reaktor-p.png|recycled-p.png|redhat1-p.png|schody-p.png|score-pozadi.png|secret-p.png|sloupy-p.png|smetak-p.png|spunt-p.png|steel-pozadi0.png|steel-pozadi1.png|steel-pozadi2.png|svatba-pozadi.png|tetris-p.png|truhla-p.png|trup-p1.png|ufo-pozadi-1.png|utes-p.png|ves-p.png|vitejte1-p.png|vladova-p.png|vrak-pozadi.png|warcr2-p1.png|wc-p.png|win-p.png|zaval-p.png|zdviz1-p.png|zdviz2-p.png|zelva-p.png|zrc-p.png|zx-pozadi.png"

find images -name "*.png" | while read line ;
do {
    rm $line &
    #echo $line &
}; done

wait

find images-baseline -name "*.png" | grep -E "${ignore}" | cut -c 16- | while read line ; 
do {
    magick images-baseline$line -strip -set option:png:exclude-chunk tIME -transparent green -background black -alpha background png32:images$line & 
    #echo images$line & 
}; done

find images-baseline -name "*.png" | grep -vE "${ignore}" | grep -v "fishes/" | cut -c 16- | while read line ; 
do {
    magick images-baseline$line -strip -set option:png:exclude-chunk tIME -transparent green -background black -alpha background -bordercolor none -border 5 -channel A -gaussian-blur 0x1 png32:images$line &
    #echo images$line & 
}; done

find images-baseline -name "*.png" | grep 'fishes/' | cut -c 16- | while read line ; 
do {
    magick images-baseline$line -strip -set option:png:exclude-chunk tIME -transparent green -background black -alpha background -bordercolor none -border 5 -channel A -gaussian-blur 0x1 png32:images$line &
    #echo images$line &
}; done

wait

find images-baseline -name "*.png" | grep 'fishes/big/right' | grep -vE '_t'   | grep -v skeleton | cut -c 16- | while read line ; do { magick images-baseline$line -bordercolor none -border 5 -channel A -gaussian-blur 0x1 -strip -set option:png:exclude-chunk tIME png32:images$line & } done &
find images-baseline -name "*.png" | grep 'fishes/big/left' | grep -vE '_t'    | grep -v skeleton | cut -c 16- | while read line ; do { magick images-baseline$line -bordercolor none -border 5 -channel A -gaussian-blur 0x1 -strip -set option:png:exclude-chunk tIME png32:images$line & } done &
find images-baseline -name "*.png" | grep 'fishes/small/right' | grep -vE '_t' | grep -v skeleton | cut -c 16- | while read line ; do { magick images-baseline$line -bordercolor none -border 5 -channel A -gaussian-blur 0x1 -strip -set option:png:exclude-chunk tIME png32:images$line & } done &
find images-baseline -name "*.png" | grep 'fishes/small/left' | grep -vE '_t'  | grep -v skeleton | cut -c 16- | while read line ; do { magick images-baseline$line -bordercolor none -border 5 -channel A -gaussian-blur 0x1 -strip -set option:png:exclude-chunk tIME png32:images$line & } done &

find images-baseline -name "*.png" | grep 'fishes/big/heads/right'             | grep -v skeleton | grep -vE 'head_dark_00|head_dark_01' | cut -c 16- | while read line ; do { magick images-baseline$line -bordercolor none -border 5 png32:images$line ; magick images$line -channel A -gaussian-blur 0x1 -channel Alpha -separate -fill black -draw "rectangle 0,0  48,39" png32:images$line-alpha ; magick images$line images$line-alpha -strip -set option:png:exclude-chunk tIME -alpha off -compose CopyOpacity -composite -channel A -threshold 95% +channel png32:images$line & } done &
find images-baseline -name "*.png" | grep 'fishes/big/heads/left'              | grep -v skeleton | grep -vE 'head_dark_00|head_dark_01' | cut -c 16- | while read line ; do { magick images-baseline$line -bordercolor none -border 5 png32:images$line ; magick images$line -channel A -gaussian-blur 0x1 -channel Alpha -separate -fill black -draw "rectangle 21,0 69,39" png32:images$line-alpha ; magick images$line images$line-alpha -strip -set option:png:exclude-chunk tIME -alpha off -compose CopyOpacity -composite -channel A -threshold 95% +channel png32:images$line & } done &
find images-baseline -name "*.png" | grep 'fishes/small/heads/right'           | grep -v skeleton | grep -vE 'head_dark_00|head_dark_01' | cut -c 16- | while read line ; do { magick images-baseline$line -bordercolor none -border 5 png32:images$line ; magick images$line -channel A -gaussian-blur 0x1 -channel Alpha -separate -fill black -draw "rectangle 0,0  38,24" png32:images$line-alpha ; magick images$line images$line-alpha -strip -set option:png:exclude-chunk tIME -alpha off -compose CopyOpacity -composite -channel A -threshold 95% +channel png32:images$line & } done &
find images-baseline -name "*.png" | grep 'fishes/small/heads/left'            | grep -v skeleton | grep -vE 'head_dark_00|head_dark_01' | cut -c 16- | while read line ; do { magick images-baseline$line -bordercolor none -border 5 png32:images$line ; magick images$line -channel A -gaussian-blur 0x1 -channel Alpha -separate -fill black -draw "rectangle 16,0 54,24" png32:images$line-alpha ; magick images$line images$line-alpha -strip -set option:png:exclude-chunk tIME -alpha off -compose CopyOpacity -composite -channel A -threshold 95% +channel png32:images$line & } done &

find images-baseline -name "*.png" | grep 'fishes/' | grep -E 'head_dark_00|head_dark_01' | cut -c 16- | while read line ; do { magick images-baseline$line -bordercolor none -border 5 -channel A -gaussian-blur 0x1 -strip -set option:png:exclude-chunk tIME png32:images$line & } done &

wait


find images -name "*.png-alpha" | while read line ;
do {
    rm $line &
    #echo $line &
}; done

wait