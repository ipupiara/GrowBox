#!c:/Perl/bin/perl.exe
##
##  printenv -- demo CGI program which just prints its environment
##
print "Content-type: text/plain; charset=iso-8859-1\n\n";

print "<HTML>";
print "<HEAD>\n";
print "<meta  http-equiv=\"Content-Type\" content=\"text/html; charset=windows-1252\">\n";
print "<meta  http-equiv=\"Refresh\" content=\"10;\">\n";
print "</HEAD><BODY>\n";
print "<br><p>\n";
system("GrowBoxSoapLines.exe");
print "</p>\n";
foreach $var (sort(keys(%ENV))) {
    $val = $ENV{$var};
    $val =~ s|\n|\\n|g;
    $val =~ s|"|\\"|g;
    print "${var}=\"${val}\"<br>\n";
}
print "</BODY>";
print "</HTML>\n";
