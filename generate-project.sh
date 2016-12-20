#!/bin/bash

thefile="$1"

if [ ! -f "$thefile" ];then
	exit 0
fi

echo "$thefile"

folder=`dirname "$thefile"`

cd "$folder"



echo "#!/bin/bash" > myproject
echo ". project.inc" >> myproject
echo ". widgets.inc" >> myproject
echo ". functions.inc" >> myproject
echo ". events.inc" >> myproject
echo "gtk \"gtk_server_exit\"" >> myproject
chmod 755 myproject


echo "function gtk() { GTK=\`gtk-server msg=\$\$,\"\$@\"\`; }" > project.inc
echo "function define() { \$2 \"\$3\"; eval \$1=\$GTK; }" >> project.inc
echo "gtk-server -cfg=gtk-server-glade.cfg -ipc=\$\$ &" >> project.inc
echo "sleep 0.1" >> project.inc
echo "gtk \"gtk_init NULL NULL\"" >> project.inc
echo "gtk \"glade_init\"" >> project.inc
echo "define XML gtk \"glade_xml_new project.glade 0 0\"" >> project.inc
echo "gtk \"glade_xml_signal_autoconnect, \$XML\"" >> project.inc
echo "define window1 gtk \"glade_xml_get_widget \$XML window1\"" >> project.inc
echo "gtk \"gtk_server_connect \$GTK delete-event window1\"" >> project.inc
echo "gtk \"gtk_widget_show \$window1\" 1" >> project.inc
echo "" >> project.inc

echo "" > widgets.inc


echo "until [[ \"\$E\" -eq \"window1\" ]];do" > events.inc

#echo "echo \"Event: \$E\"" >> events.inc

echo "    define E gtk \"gtk_server_callback wait\"" >> events.inc


touch functions.inc 2>/dev/null

name=""
cat project.glade | while read a;do


	check=`echo "$a" | grep ' id='`
	if [ "$check" != "" ];then
		name=`echo "$check" | sed -e "s/.* id=\"//" -e "s/\".*//"`
		echo $name

		echo define $name gtk \"glade_xml_get_widget \$XML $name\" >> widgets.inc

	fi
	check=`echo "$a" |grep ' handler='`
	if [ "$check" != "" ];then
		handler=`echo "$check" | sed -e "s/.* handler=\"//" -e "s/\".*//"`
		echo $handler
		event=`echo "$check" | sed -e "s/.* name=\"//" -e "s/\".*//"`
		echo $event

		echo define $name gtk \"glade_xml_get_widget \$XML $name\" >> widgets.inc

		after=`echo "$a" |grep ' after=\"yes'`
		if [ "$after" != "" ];then
			echo gtk \"gtk_server_connect_after \$GTK $event ${name}_${event}\" >> widgets.inc
		else

			echo gtk \"gtk_server_connect \$GTK $event ${name}_${event}\" >> widgets.inc
		fi
		echo "	[[ \"\$E\" == \"${name}_${event}\" ]] && $handler" >> events.inc
		fcheck=`grep "$handler () {" functions.inc`
		if [ "$fcheck" == "" ];then
			echo "$handler () {" >> functions.inc
			echo "	echo $handler" >> functions.inc
			echo "}" >> functions.inc
		fi
	fi
done

echo "done" >> events.inc

