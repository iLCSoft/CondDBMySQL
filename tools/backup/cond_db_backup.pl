#!/usr/bin/env perl

#############################################
#
# cond_db_backup.pl  -- script for backing up 
#                      mysql ConditiondDB databases
#
# Luis Pedro April.02
#
# Command line parameters:
#          - Database name
#          - Location (if the location starts
#                      by /castor/cern.ch the
#                      backup is moved to the
#                      specified location in
#                      castor)
#
#############################################   

unless (@ARGV == 4 || @ARGV == 3) {
    print "sintax: $0 database_name backup_location mysql_user [mysql_host]\n\n";
    print "Note: if the location starts by /castor/cern.ch\n";
    print "      the backup file is moved to the specified\n";
    print "      location at CERN's CASTOR\n\n";
    exit;
}

($database_name, $backup_location, $mysql_user, $mysql_host) = @ARGV;

if (defined $mysql_host) {$mysql_host = "-h ${mysql_host}";}
else{$mysql_host = "-h localhost";}

if ($backup_location =~ /^\/castor\/cern.ch\//) {
    $tmp_location = "/tmp";
}
else {
    $tmp_location = $backup_location;
}

if ((system "mysqldump  --add-drop-table --add-locks --all --quick --extended-insert --lock-tables --force ${mysql_host} -u ${mysql_user} -p $database_name > ${tmp_location}/tmp.sql") != 0) {
    die "Error: could not dump database...\n\n";	
}
else {

    open(FINAL, "> ${tmp_location}/${database_name}_backup.sql");
    open(DUMP,"< ${tmp_location}/tmp.sql");
    @dump_text=<DUMP>;
    print FINAL "create database if not exists ${database_name};\n";
    print FINAL "use ${database_name};\n\n";
    print FINAL @dump_text;
    close FINAL;
    close DUMP;
    system "rm ${tmp_location}/tmp.sql";

    if ($tmp_location eq "/tmp") {
        if ((system "rfcp ${tmp_location}/${database_name}_backup.sql $backup_location") != 0) {
            print "Error: could not move database to CASTOR...\n\n";
        }
        system "rm ${tmp_location}/${database_name}_backup.sql";
    }
}
















