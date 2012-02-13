#!/usr/bin/perl
#
#	Evil double-slashes (//) in C file is used for code comment.
#	However, ANSI C compiler does not support.
#	So this program will replace '//' with '/* ... */'.
#
#	Usage:
#		doubleSlashKiller.pl file [...]
#
#	With find:
#		find . -name "*.c" -exec tools/doubleSlashKiller.pl \{\} \;
#

while ( $file = shift )
{
	$orgfile = "$file.org";
	open( FP, "< $file" ) || die "open $file for read error: $!\n";
	unlink( $orgfile );
	rename( $file, $orgfile ) || die "rename $file to $orgfile error: $!\n";
	open( FP2, "> $file" ) || die "open $file for write error: $!\n";

	while( $line = <FP> )
	{
		chomp $line;
		$reminder = $line;
		$line = "";

		while( $reminder =~ /(.*?)"(.*?)"(.*)/ )
		{
			$one = $1; $two = $2; $three = $3;

			if ( $one =~ /(.*)\/\/(.*)/ )
			{
				$line .= $1."/*$2\"$two\"$three*/";
				$reminder = "";
				break;
			}

			$line .= "$one\"$two\"";
			$reminder = $three;
		}

		if ( $reminder =~ /(.*)\/\/(.*)/ )
		{
			$reminder = $1."/*$2 */";
			break;
		}

		$line .= $reminder;

		print FP2 "$line\n";
	}

	close( FP );
	close( FP2 );
}
