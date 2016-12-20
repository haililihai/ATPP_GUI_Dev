#!/bin/bash

thefile="$1"

if [ ! -f "$thefile" ];then
	exit 0
fi

echo "$thefile"

folder=`dirname "$thefile"`

cd "$folder"



echo "#!/bin/bash" > ATPP
echo ". project" >> ATPP
echo ". widgets" >> ATPP
echo ". functions" >> ATPP
echo ". events" >> ATPP
echo "gtk \"gtk_server_exit\"" >> ATPP
chmod 755 ATPP


echo "function gtk() { GTK=\`gtk-server msg=\$\$,\"\$@\"\`; }" > project
echo "function define() { \$2 \"\$3\"; eval \$1=\$GTK; }" >> project
echo "gtk-server -cfg=gtk-server-glade.cfg -ipc=\$\$ &" >> project
echo "sleep 0.1" >> project
echo "gtk \"gtk_init NULL NULL\"" >> project
echo "gtk \"glade_init\"" >> project
echo "define XML gtk \"glade_xml_new ATPP.glade 0 0\"" >> project
echo "gtk \"glade_xml_signal_autoconnect, \$XML\"" >> project
echo "define window1 gtk \"glade_xml_get_widget \$XML window1\"" >> project
echo "gtk \"gtk_server_connect \$GTK delete-event window1\"" >> project
echo "gtk \"gtk_widget_show \$window1\" 1" >> project
echo "" >> project

echo "" > widgets


echo "until [[ \"\$E\" -eq \"window1\" ]];do" > events

#echo "echo \"Event: \$E\"" >> events

echo "    define E gtk \"gtk_server_callback wait\"" >> events


touch functions 2>/dev/null

name=""
cat ATPP.glade | while read a;do


	check=`echo "$a" | grep ' id='`
	if [ "$check" != "" ];then
		name=`echo "$check" | sed -e "s/.* id=\"//" -e "s/\".*//"`
		echo $name

		echo define $name gtk \"glade_xml_get_widget \$XML $name\" >> widgets

	fi
	check=`echo "$a" |grep ' handler='`
	if [ "$check" != "" ];then
		handler=`echo "$check" | sed -e "s/.* handler=\"//" -e "s/\".*//"`
		echo $handler
		event=`echo "$check" | sed -e "s/.* name=\"//" -e "s/\".*//"`
		echo $event

		echo define $name gtk \"glade_xml_get_widget \$XML $name\" >> widgets

		after=`echo "$a" |grep ' after=\"yes'`
		if [ "$after" != "" ];then
			echo gtk \"gtk_server_connect_after \$GTK $event ${name}_${event}\" >> widgets
		else

			echo gtk \"gtk_server_connect \$GTK $event ${name}_${event}\" >> widgets
		fi
		echo "	[[ \"\$E\" == \"${name}_${event}\" ]] && $handler" >> events
		fcheck=`grep "$handler () {" functions`
		#if [ "$fcheck" == "" ];then
		#	echo "$handler () {" >> functions
		#	echo "	echo $handler" >> functions
		#	echo "}" >> functions
		#fi
	fi
done

echo "done" >> events

