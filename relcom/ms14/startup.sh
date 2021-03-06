:
#
# Start mail server daemon.
# Called from system /etc/rc startup file.
#

dir=/var/spool/mailserv
user=mailserv

echo

cd $dir && {
	find $dir/queue -type f -name q\* -print | sed "s:^.*/q::" |\
		sort -n | sed "s:^:$dir/queue/q:" > queueactive
	chown $user queueactive
        touch /var/adm/mslog
        chown $user /var/adm/mslog
	echo nice $dir/bin/mailservd | su $user
#	/usr/local/bin/marsd $dir/db
}

echo 'Mail Server daemon started'
echo 'Copyright (C) 1990-95 Serge Vakulenko'
