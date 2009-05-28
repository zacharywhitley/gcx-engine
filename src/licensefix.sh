for file in `ls *.h *.cpp *.l *.y`;
do
	./licensefix.pl $file
	mv $file.fixed $file
done
