         N   M      ��������'�eŲ0�\��9�~͉���@            uR= readme
T= $R.txt
H= $R.html

$T:	$H
	lynx -dump $? >$@

clean:
	-rm -f $T
