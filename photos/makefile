#Makefile for annotation with convert


pub: clean
		for img in $$(ls *.jpg | cut -d \. -f 1); do make pub/$$img.jpg; make pub/$${img}_small.jpg; done

pub/%.jpg: %.jpg
		convert $< -gravity SouthWest  -pointsize 15 -annotate +10+10 'AS5043 encoder by Sebastian Schumb sebastians-site.de' $@

pub/%_small.jpg: pub/%.jpg
		convert $<  -resize 400x300 $@

clean:
		-rm pub/*.jpg

all: pub


.SILENT: pub
